/*===========================================================================*
 * This file is part of the Abstract Library for Parallel Search (ALPS).     *
 *                                                                           *
 * ALPS is distributed under the Eclipse Public License as part of the       *
 * COIN-OR repository (http://www.coin-or.org).                              *
 *                                                                           *
 * Authors:                                                                  *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *                                                                           *
 * Conceptual Design:                                                        *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *          Laszlo Ladanyi, IBM T.J. Watson Research Center                  *
 *          Matthew Saltzman, Clemson University                             *
 *                                                                           * 
 *                                                                           *
 * Copyright (C) 2001-2013, Lehigh University, Yan Xu, and Ted Ralphs.       *
 *===========================================================================*/

#ifndef AlpsParameters_h
#define AlpsParameters_h

//----------------------------------------------------------
// AlpsParameterBase.h is modified from BCP_parameters.hpp
//----------------------------------------------------------

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>          // for 'pair'
#include <vector>

#include "CoinError.hpp"

#if defined(__GNUC__) && (__GNUC__ >= 3)
#    include <sstream>
#    include <locale>
#    define ALPS_STRINGSTREAM std::istringstream
#else
#    include <strstream>
#    include <cctype>
#    define ALPS_STRINGSTREAM std::istrstream
#endif

class AlpsEncoded;

//##############################################################################

/** This enumerative constant describes the possible parameter types. */
enum AlpsParameterT{
    /** The type is not yet specified. Used only in the default constructor of
	a Alps parameter. */
    AlpsNoPar,
    /** Bool parameter. */
    AlpsBoolPar,
    /** Integer parameter. */
    AlpsIntPar,
    /** Double parameter. */
    AlpsDoublePar,
    /** String parameter (E.g., data file name.). */
    AlpsStringPar,
    /** The parameter is an array of strings. N
	Cause possible memory leak. Not used any more (10/24/06, Yan).*/
    AlpsStringArrayPar
};

//##############################################################################

/** This parameter indeintifies a single parameter entry. */

class AlpsParameter {

 private:
    /**@name Data members */
    /*@{*/
    /** The type of the parameter (e.g., AlpsIntPar). */
    AlpsParameterT type_;
    /** The index of this parameter within all parameters of the same type. */
    int index_;
    /*@}*/
    
 public:
    // default copy constructor and assignment operator are fine 
    /**@name Constructors / Destructor */
    /*@{*/
    /** The default constructor creates a phony parameter. */
    AlpsParameter() : type_(AlpsNoPar), index_(0) {}
    /** Constructor where members are specified. */
    AlpsParameter(const AlpsParameterT t, const int i) :
	type_(t), index_(i) {}
    /** The destructor. */
    ~AlpsParameter() {}
    /*@}*/

    /**@name Query methods */
    /*@{*/
    /** Return the type of the parameter. */
    AlpsParameterT type() const { return type_; }
    /** Return the index of the parameter within all parameters of the same
	type. */
    int index() const            { return index_; }
    /*@}*/
};

//##############################################################################

/** This is the class serves as a holder for a set of parameters. 
    For example, Alps stores has a parameter set for each process. Of course,
    the user can use this class for her own parameters. To use this class the
    user must 
    <ul>
    <li> first derive a subclass with the names of the parameters (see,
    e.g., AlpsParams.)
    <li> then define the member functions
    <code>createKeywordList()</code> and
    <code>setDefaultEntries()</code>.
    For an example look at the file
    <code>AlpsParams.cpp</code>. Essentially, the first
    method defines what keywords should be looked for in the parameter
    file, and if one is found which parameter should take the
    corresponding value; the other method specifies the default values
    for each parameter.
    </ul>
    
    After this the user can read in the parameters from a file, she can
    set/access the parameters in the parameter set.
*/
class AlpsParameterSet {
 protected:
    /**@name Data members. All of them are protected. */
    /*@{*/
    /** The keyword, parameter pairs. Used when the parameter file is read in.
     */
    std::vector< std::pair<std::string, AlpsParameter> > keys_;
    
    /** list of obsolete keywords. If any of these is encountered a warning is
	printed. */
    std::vector<std::string> obsoleteKeys_;
    
    /** The bool parameters. */
    bool*                bpar_;
    
    /** The integer parameters. */
    int*                 ipar_;

    /** The double parameters. */
    double*              dpar_;
    
    /** The string (actually, std::string) parameters. */
    std::string*          spar_;

    /** The "vector of string" parameters. */
    int numSa_;
    std::vector<std::string>* sapar_;
  
  /*@}*/
  //---------------------------------------------------------------------------

 public:
    /**@name Pure virtual functions that must be defined for each parameter set.
       If the user creates a new parameter set, she must define these two
       methods for the class. */
    /*@{*/
    /** Method for creating the list of keyword looked for in the parameter
	file. */
    virtual void createKeywordList() = 0;
    
    /** Method for setting the default values for the parameters. */
    virtual void setDefaultEntries() = 0;
    /*@}*/

    /**@name Pack and unpack 
     */
    //@{
    /** Pack the parameter set into the buffer. */
    virtual void pack(AlpsEncoded& buf) {
	throw CoinError("can't call pack()", "pack", " AlpsParameterSet");
    }

    /** Unpack the parameter set from the buffer. */
    virtual void unpack(AlpsEncoded& buf){
	throw CoinError("can't call unpack()", "unpack", " AlpsParameterSet");
    }
    //@}

    //---------------------------------------------------------------------------
    
 public:

    /**First, there is the assignment operator that sets the whole parameter
       set at once.
       Individual members of the parameter set can be set for using the
       overloaded setEntry() method. Using the example in the
       class documentation the user can set a parameter with the
       "<code>param.setEntry(USER_par::parameter_name, param_value)</code>"
       expression. */
  
    // This the one used in readFromStream()
    void setEntry(const AlpsParameter key, const char * val) {
	switch (key.type()){
	case AlpsNoPar: break;
	case AlpsBoolPar:        bpar_ [key.index()] = atoi(val) ? true : false;    break;
	case AlpsIntPar:         ipar_ [key.index()] = atoi(val);    break;
	case AlpsDoublePar:      dpar_ [key.index()] = atof(val);    break;
	case AlpsStringPar:      spar_ [key.index()] = val;          break;
	case AlpsStringArrayPar: sapar_[key.index()].push_back(val); break;
	}
    }
    
    /** Read the parameters from the stream specified in the argument.
	The stream is interpreted as a lines separated by newline characters.
	The first word on each line is tested for match with the keywords
	specified in the createKeywordList() method. If there is
	a match then the second word will be interpreted as the value for the
	corresponding parameter. Any further words on the line are discarded.
	Every non-matching line is discarded. 
	
	If the keyword corresponds to a non-array parameter then the new value
	simply overwrites the old one. Otherwise, i.e., if it is a
	StringArrayPar, the value is appended to the list of strings in that
	array. 
    */
    void readFromStream(std::istream& parstream);

    /** Read parameters from a file. */
    void readFromFile(const char * paramfile);

    /**  Read parameters from the command line */
    void readFromArglist(const int argnum, const char * const * arglist);
    
    /** Write keyword-value pairs to the stream specified in the argument.
	Each keyword-value pair is separated by a newline character. 
    */
    void writeToStream(std::ostream& outstream) const;
  
    /** The constructor allocate memory for parameters. */
    AlpsParameterSet(int c, int i, int d, int s, int sa) :
	keys_(),
	bpar_(new bool[c]),
	ipar_(new int[i]),
	dpar_(new double[d]),
	spar_(new std::string[s]),
	sapar_(new std::vector<std::string>[sa]) 
	{}
    
    /** The destructor deletes all data members. */
    virtual ~AlpsParameterSet() {
	keys_.clear();
	obsoleteKeys_.clear();
	delete[] bpar_; bpar_ = 0;
	delete[] ipar_; ipar_ = 0;
	delete[] dpar_; dpar_ = 0;
	delete[] spar_; spar_ = 0;
	delete[] sapar_; sapar_ = 0;
    }
};

#endif
