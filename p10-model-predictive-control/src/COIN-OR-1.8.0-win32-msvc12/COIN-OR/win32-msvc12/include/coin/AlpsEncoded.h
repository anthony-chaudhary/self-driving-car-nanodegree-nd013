#ifndef AlpsEncoded_h
#define AlpsEncoded_h

#include <cstring>
#include <string>
#include <memory>
#include <vector>

#include "CoinError.hpp"

#include "Alps.h"

// AlpsEncoded is modified from BCP_buffer and CoinEncoded

//#############################################################################
/** This data structure is to contain the packed form of an encodable
 *    knowledge. It servers two purposes:
 *  <ul>
 *  <li> used as a buffer when passing messages
 *  <li> allow Alps to manipulate the user derived knowledge 
 *  </ul>
 */
//#############################################################################

class AlpsEncoded {  

 private:

    /** Disable copy constructor and assignment operator */
    ///@{
    AlpsEncoded(const AlpsEncoded&);
    AlpsEncoded& operator=(const AlpsEncoded&);
    ///@}
    
 private:
    
    /** The next read/write position in the representation. */
    size_t pos_;

    /** The amount of memory allocated for the representation. */
    size_t maxSize_;

    /** Represent the type of the object. We might use RTTI
	to point this into the static data of the executable :-)
	Can only be initialized during constructing. 
        User take care of memory.*/
    //std::string type_;   

    int type_;

    /** The size of the packed representation. */
    int size_;

    /** The encoded/compressed representation of the object. */
    // const char* representation_;   //why const ??? XY
    char* representation_;
    
 public:
    
    /**@name Constructors and destructor */
    /*@{*/

    /** The default constructor creates a buffer of size 16 Kbytes with no
	message in it. */
    AlpsEncoded() 
	: 
	pos_(0), 
	maxSize_(0x4000/*16K*/), 
	type_(0), 
	size_(0), 
	representation_(new char[maxSize_]) 
	{}
    
    /** Useful constructor. */
    AlpsEncoded(int t) 
	: 
	pos_(0),
	maxSize_(0), 
	type_(t), 
	size_(0), 
	representation_(0) 
	{}

    /** Useful constructor.  Take over ownership of r. */
    AlpsEncoded(int t, int s, char*& r) 
	: 
	pos_(0), 
	maxSize_(s + 4),
	type_(t),  
	size_(s),
	representation_(r) 
	{ r = NULL; }                    
    
    /** Destructor. */
    ~AlpsEncoded() {
	if (representation_) { 
	    delete [] representation_; 
	    representation_ = 0; 
	}
    }
    /*@}*/
    
    /**@name Query methods */
    ///@{
    int type() const { return type_; }
    int size() const { return size_; }
    const char* representation() const { return representation_; }
    ///@}

    inline void setPosition(const int pos) {
	if (pos < 0 || pos >= size()) {
	    //     const char msg [100] = "Incorrest position setting.";
	    //throw AlpsException(__FILE__, __LINE__, msg);
	    throw CoinError("Incorrest position setting.", "setPosition",
			    "AlpsEncoded");
	}
	pos_ = pos;
    }
  
    inline void setRepresentation(char*& buf) {
        //> 0x4000/*16K*/ ? (strlen(buf)+1) : 0x4000;
	maxSize_ = strlen(buf) + 1;
	representation_ = buf;
	buf = 0; 
    }

  /** Reallocate the size of encoded if necessary so that at least
      <code>addsize_</code> number of additional bytes will fit into the
      encoded. */
    inline void make_fit(const int addSize){
	assert(addSize > 0);
	size_t addSize1 = static_cast<size_t>(addSize);
	
	if (maxSize_ < size_ + addSize1){
	    maxSize_ = 4 * (size_ + addSize1 + 0x1000/*4K*/);
	    char* newRep = new char[maxSize_];
	    if (size_)
		memcpy(newRep, representation_, size_);
	    delete[] representation_;
	    representation_ = newRep;
	}
    }

    /** Completely clear the encoded. Delete and zero out <code>type_, size_,
	pos_</code>. */
    inline void clear(){
	size_ = 0;
	pos_ = 0;
        type_ = 0;
	if (representation_ != 0) { 
	    delete  representation_; 
	    representation_ = 0; 
	}
    }
    
    //------------------------------------------------------
    // Following functiosn are used in parallel code only.
    //------------------------------------------------------

    /** Write a single object of type <code>T</code> in <code>repsentation_
	</code>. Copies <code>sizeof(T)</code> bytes from 
	the address of the object. */
    template <class T> AlpsEncoded& writeRep(const T& value) {
	make_fit( sizeof(T) );
	memcpy(representation_ + size_, &value, sizeof(T));
	size_ += static_cast<int>(sizeof(T));
	return *this;
    }

    /** Read a single object of type <code>T</code> from <code>repsentation_
	</code>. Copies <code>sizeof(T)</code> bytes to the address of 
	the object. */
    template <class T> AlpsEncoded& readRep(T& value){
#ifdef PARANOID
	if (pos_ + sizeof(T) > size_) {
	    throw CoinError("Reading over the end of buffer.", 
			    "readRep(const T& value)", "AlpsEncoded");
	}
#endif
	memcpy(&value, representation_ + pos_, sizeof(T));
	pos_ += sizeof(T);
	return *this;
    }


    /** Write a C style array of objects of type <code>T</code> in 
	<code>repsentation_</code>. First write the length, 
	then write the content of the array */
    template <class T> AlpsEncoded& writeRep(const T* const values,
					     const int length){
       make_fit( static_cast<int>(sizeof(int)) +
		 static_cast<int>(sizeof(T)) * length );
	memcpy(representation_ + size_, &length, sizeof(int));
	size_ += static_cast<int>(sizeof(int));
	if (length > 0){
	    memcpy(representation_ + size_, values,
		   static_cast<int>(sizeof(T)) * length);
	    size_ += static_cast<int>(sizeof(T)) * length;
	}
	return *this;
    }

    /** Read an array of objects of type <code>T</code> from <code>
	repsentation_</code>, where T <strong>must</strong> be a 
	built-in type (ar at least something that can be copied with memcpy).
	
	If the third argument is true then memory is allocated for the array
	and the array pointer and the length of the array are returned in the
	arguments.

	If the third argument is false then the arriving array's length is
	compared to <code>length</code> and an exception is thrown if they are
	not the same. Also, the array passed as the first argument will be
	filled with the arriving array.
    */
    template <class T> AlpsEncoded& readRep(T*& values, 
					    int& length,
					    bool needAllocateMemory = true)
       {
        
	if (needAllocateMemory) {
	    // Need allocate memeory for arrary "values".
            
#ifdef PARANOID
	    if (pos_ + sizeof(int) > size_) {
		throw CoinError("Reading over the end of buffer.", 
				"readRep(T*& values, int& length,...", 
				"AlpsEncoded");
	    }
#endif
	    memcpy(&length, representation_ + pos_, sizeof(int));
	    pos_ += sizeof(int);
	    if (length > 0){
#ifdef PARANOID
		if (pos_ + sizeof(T)*length > size_) {
		    throw CoinError("Reading over the end of buffer.", 
				    "readRep(T*& values, int& length,...",
				    "AlpsEncoded");
		}
#endif
		values = new T[length];
		memcpy(values, representation_ + pos_, sizeof(T)*length);
		pos_ += sizeof(T) * length;
	    }
	    
	} 
	else { /* values has been allocated memory. */

	    int l;
#ifdef PARANOID
	    if (pos_ + sizeof(int) > size_) {
		throw CoinError("Reading over the end of buffer.", 
				"readRep(T*& values, int& length,...",
				"AlpsEncoded");
	    }
#endif
	    memcpy(&l, representation_ + pos_, sizeof(int));
	    pos_ += sizeof(int);
	    if (l != length) {
		throw CoinError("Reading over the end of buffer.", 
				"readRep(T*& values, int& length,...", 
				"AlpsEncoded");
	    }
	    if (length > 0){
#ifdef PARANOID
		if (pos_ + sizeof(T)*length > size_) {
		    throw CoinError("Reading over the end of buffer.", 
				    "readRep(T*& values, int& length,...",
				    "AlpsEncoded");
		}
#endif
		memcpy(values, representation_ + pos_, sizeof(T)*length);
		pos_ += sizeof(T) * length;
	    }       
	}
	
	return *this;
    }

    /** Read a <code>std::string</code> in <code>repsentation_ </code>. */
    AlpsEncoded& writeRep(std::string& value){
	// must define here, 'cos in *_message.C we have only templated members
	const int len = static_cast<const int> (value.length());
	make_fit( static_cast<int>(sizeof(int)) + len );
	memcpy(representation_ + size_, &len, static_cast<int>(sizeof(int)));
	size_ += static_cast<int>(sizeof(int));
	if (len > 0){
	    memcpy(representation_ + size_, value.c_str(), len);
	    size_ += len;
	}
	return *this;
    }

    /** Read a <code>std::string</code> from <code>repsentation_ </code>. */
    AlpsEncoded& readRep(std::string& value){
	int len;
	readRep(len);
	value.assign(representation_ + pos_, len);
	pos_ += len;
	return *this;
    }
    
    /** Write a <code>std::vector</code> into <code>repsentation_ </code>. */
    template <class T> AlpsEncoded& writeRep(const std::vector<T>& vec) {
	int objnum = vec.size();
	int new_bytes = objnum * sizeof(T);
	make_fit( sizeof(int) + new_bytes );
	memcpy(representation_ + size_, &objnum, sizeof(int));
	size_ += sizeof(int);
	if (objnum > 0){
	    memcpy(representation_ + size_, &vec[0], new_bytes);
	    size_ += new_bytes;
	}
	return *this;
   }
    
   /** Read a <code>std::vector</code> from <code>repsentation_ </code>. */
   template <class T> AlpsEncoded& readRep(std::vector<T>& vec) {
       int objnum;
#ifdef PARANOID
       if (pos_ + sizeof(int) > size_)
	   throw CoinError("Reading over the end of buffer.", 
			   "AlpsEncoded", "readRep(std::vector<T>& vec");
#endif
       memcpy(&objnum, representation_ + pos_, sizeof(int));
       pos_ += sizeof(int);
       vec.clear();
       if (objnum > 0){
#ifdef PARANOID
	   if (pos_ + sizeof(T)*objnum > size_)
	       throw CoinError("Reading over the end of buffer.", 
			       "AlpsEncoded", "readRep(std::vector<T>& vec");
#endif
	   vec.insert(vec.end(), objnum, T());
	   memcpy(&vec[0], representation_ + pos_, objnum * sizeof(T));
	   pos_ += objnum * sizeof(T);
       }
       return *this;
   }
   /*@}*/

};

#endif
