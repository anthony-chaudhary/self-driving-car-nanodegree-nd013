// ******************** FlopCpp **********************************************
// File: MP_index.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _MP_index_hpp_
#define _MP_index_hpp_

#include "MP_utilities.hpp"
#include "MP_constant.hpp"

namespace flopc {

    class MP_index;
    class MP_domain;
    class MP_set;

    /** @brief Internal representation of a index
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
    */
    class MP_index_base {
	friend class Handle<MP_index_base*>;
	friend class MP_index_exp;
    public:
	virtual int evaluate() const = 0;
	virtual MP_index* getIndex() const = 0;
	virtual MP_domain getDomain(MP_set* s) const = 0;
	virtual void display()const;
    protected:
	MP_index_base() : count(0) {}
	virtual ~MP_index_base() {}
    private:
	int count;
    };

    /** @brief Representation of an index.
        @ingroup PublicInterface
        This is one of the main public interface classes.  
        It is used to iterate through, or index into an MP_domain.
        It is also used to share the 'current' index offsets between
        expressions which share an index.
        @li these can be built stand-alone
        @li these are sometime constructed as needed.
        @li there is a special "empty" which is a unique constant. \
        This constant is used when defaulting passed parameters for \
        extra dimensions which are unused.
    */
    class MP_index : public MP_index_base {
    public:
        /// Default constructor. 
	MP_index() : index(0), instantiated(false) {}
	int evaluate() const { 
	    return index; 
	}
    /** interrogate state of instatiation of data.
        @todo should this be private?
    */
	bool isInstantiated() const { 
	    return instantiated; 
	}
    /** Setter for the index.
        @todo should this be private?
        @todo should this assert "instatiated"?
    */
	void assign(int i) { 
	    index = i;
	}
    /** unsetter for instatiated.
        @todo should this be private?
    */
	void unInstantiate() {
	    instantiated = false; 
	}
    /** setter for instatiated.
        @todo should this be private?
    */
	void instantiate() {
	    instantiated = true; 
	}
    /** getter for MP_index * data type.  
        @todo should this be private?
        @todo should this be virtual?
    */
	MP_index* getIndex() const {
	    return const_cast<MP_index*>(this);
	}
    /// Getter for domain over which this index is applied.
	virtual MP_domain getDomain(MP_set* s) const;
    /** @todo should this be private?
        @todo doc purpose.
    */
	static MP_index& Any;
    /// returns a reference to the distinct "empty" index.
	static MP_index &getEmpty();

    private:
	static MP_index& Empty;
	int index;
	bool instantiated;
    };


    /// returns a Constant as a result of addition of two MP_index values.
    Constant operator+(MP_index& a, MP_index& b);
    /// returns a Constant as a result of a difference of two MP_index values.
    Constant operator-(MP_index& a, MP_index& b);

    /** returns an index expression from a difference between an MP_index
        and an integer.  (i-5)
    */
    MP_index_exp operator-(MP_index& i,const int& j);
    /** returns an index expression from a sum between an MP_index
        and an integer.  (i+5)
    */
    MP_index_exp operator+(MP_index& i,const int& j);
    /** returns an index expression from a sum between an MP_index
        and a Constant.  
    */
    MP_index_exp operator+(MP_index& i,const Constant& j);
    /** returns an index expression from a product between an MP_index
        and a Constant.  
    */
    MP_index_exp operator*(MP_index& i,const Constant& j);

    class SUBSETREF;

    /** @brief Representation of an expression involving an index.
        @ingroup PublicInterface
        This is one of the main public interface classes.  
        It is used to create complex arrangements of index values.
        <br> Index expressions can involve:
        @li constants
        @li other indexes
        @li subset references
        @li other index expressions.
        <br>
        There is a unique 'empty' version for use in defaulting extra
        dimensions.
    */
    class MP_index_exp : public Handle<MP_index_base*> {
    public:
    /// For internal use.
	MP_index_exp(MP_index_base* r) : Handle<MP_index_base*>(r) {} 
    /// create an index expression from a constant integer.
	MP_index_exp(int i=0); 
    /// create an index expression from a Constant
	MP_index_exp(const Constant& c);
    /// create an index expression from an MP_index.
	MP_index_exp(MP_index& i);
    /** create an index expression from a SUBSETREF
            @todo internal? or explain?
    */
	MP_index_exp(const SUBSETREF& d);
    /// copy constructor from another MP_index_exp
	MP_index_exp(const MP_index_exp& other);
	virtual ~MP_index_exp() {}
    /// Return the unique empty expression.
	static const MP_index_exp &getEmpty();
    private:
	static MP_index_exp Empty;
    };

    /** @brief Internal representation of an index expression
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
        @see operator*(MP_index& i, const Constant & j);
    */
    class MP_index_mult : public MP_index_base {
	friend MP_index_exp operator*(MP_index& i,const Constant& j);
    private:
	MP_index_mult(MP_index& i, const Constant& j) : left(i), right(j) {}
    
	int evaluate() const {
	    return left->evaluate()*int(right->evaluate()); 
	}
	MP_index* getIndex() const {
	    return left->getIndex();
	}
	virtual MP_domain getDomain(MP_set* s) const;
	MP_index_exp left;
	Constant right;
    };

    /** @brief Internal representation of an index expression
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
        @see operator+(MP_index& i, const Constant & j);
    */
    class MP_index_sum : public MP_index_base {
	friend MP_index_exp operator+(MP_index& i,const Constant& j);
	friend MP_index_exp operator+(MP_index& i,const int& j);
    private:
	MP_index_sum(MP_index& i, const Constant& j) : left(i), right(j) {}
    
	int evaluate() const {
	    return left->evaluate()+int(right->evaluate()); 
	}
	MP_index* getIndex() const {
	    return left->getIndex();
	}
	virtual MP_domain getDomain(MP_set* s) const;
	MP_index_exp left;
	Constant right;
    };

    /** @brief Internal representation of an index expression
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
        @see operator+(MP_index& i, const Constant & j);
    */
    class MP_index_dif : public MP_index_base {
	friend MP_index_exp operator-(MP_index& i,const Constant& j);
	friend MP_index_exp operator-(MP_index& i,const int& j);
    private:
	MP_index_dif(MP_index& i, const Constant& j) : left(i), right(j) {}
   
	int evaluate() const {
	    return left->evaluate()-int(right->evaluate()); 
	}
	MP_index* getIndex() const {
	    return left->getIndex();
	}
	virtual MP_domain getDomain(MP_set* s) const;
	MP_index_exp left;
	Constant right;
    };

} // End of namespace flopc
#endif
