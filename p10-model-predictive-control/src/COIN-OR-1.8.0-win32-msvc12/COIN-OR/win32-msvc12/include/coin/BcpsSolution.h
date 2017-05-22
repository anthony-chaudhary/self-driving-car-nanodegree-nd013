/*===========================================================================*
 * This file is part of the Branch, Constrain and Price Software (BiCePS)    *
 *                                                                           *
 * BiCePS is distributed under the Eclipse Public License as part of the     *
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
 * Copyright (C) 2001-2013, Lehigh University, Yan Xu, and Ted Ralphs.       *
 * All Rights Reserved.                                                      *
 *===========================================================================*/

#ifndef BcpsSolution_h_
#define BcpsSolution_h_

#include "AlpsSolution.h"
#include "BcpsObject.h"

//#############################################################################
/** This class holds the solution objects. At this level, a solution is just
    considered to be a list of objects with associated values. */
//#############################################################################

class BcpsSolution : public AlpsSolution {

 private:

    BcpsSolution(const BcpsSolution&);
    BcpsSolution& operator=(const BcpsSolution&);
   
 protected:

    /** Size of values_. */
    int size_;
    
    /** List of objects associated with values. Can be NULL. */
    BcpsObject_p *objects_;

    /** Solution values. */
    double* values_;

    /** Quality/Objective value associated with this solution. */
    double quality_;
    
 public:
    
    /** Default constructor. */
    BcpsSolution() 
	: 
	size_(0), objects_(NULL), values_(NULL), quality_(ALPS_OBJ_MAX)
	{}
    
    /** Useful constructor. */
    BcpsSolution(int size, const double *values, double q)
	:
	size_(size), objects_(NULL), values_(NULL), quality_(q) {
	
	if (size > 0) {
	    values_ = new double [size];
	    memcpy(values_, values, sizeof(double) * size);
	}
	else {
	    std::cout << "ERROR: Solution size = " << size << std::endl;
	    assert(size > 0);
	}
    }

    /** Construct an object using the given arrays. Note that the new objects
	takes over the pointers and NULLs them out in the calling method. */
    BcpsSolution(int size, BcpsObject_p*& objects, double*& values, double q)
	:
	size_(size), objects_(objects), values_(values), quality_(q) {
	objects = NULL;
	values = NULL;
    }
    
    /** Distructor. */
    virtual ~BcpsSolution() {
	if (objects_) {
	    for (int i = 0; i < size_; ++i) {
		delete objects_[i];
	    }
	    delete[] objects_;
	}
	delete[] values_;
    }
   
    /** Get the appropriate data member. */
    /**@{ */
    inline int getSize() const { return size_; }
    inline const BcpsObject_p* getObjects() const { return objects_; }
    inline const double* getValues() const { return values_; }
    inline double getQuality() const { return quality_; }
    /**@} */

   /** Set/assign the appropriate data member. */
    /**@{ */
    inline void setSize(int s) { size_ = s; }
    inline void assignObjects(BcpsObject_p *& obj){
	objects_ = obj;
	obj = NULL;
    }
    inline void setValues(const double *vs, int s) { 
	if (!values_) delete [] values_;
	values_ = new double [s];
	size_ = s;
	memcpy(values_, vs, s * sizeof(double));
    }
    inline void setQuality(double q) { quality_ = q; }
    /**@} */
    
    /** Select the fractional/nonzero elements from the solution array and
	return a new object in compacted form. */
    /**@{ */
    virtual BcpsSolution* selectNonzeros(const double etol = 1e-5) const;
    virtual BcpsSolution* selectFractional(const double etol = 1e-5) const;
    /**@} */

    /** Print out the solution.*/
    virtual void print(std::ostream& os) const {
	for (int j = 0; j < size_; ++j) {
	    if (values_[j] > 1.0e-15 || values_[j] < -1.0e-15) {
		os << "x[" << j << "] = " << values_[j] << std::endl;
	    }
	}
    }
    
    //------------------------------------------------------
    // Parallel.
    //------------------------------------------------------
   
    /** Pack Bcps part of solution into an encoded objects. */
    AlpsReturnStatus encodeBcps(AlpsEncoded *encoded) const;

    /** Unpack Bcps part of solution from an encoded objects. */
    AlpsReturnStatus decodeBcps(AlpsEncoded & encoded);
    
};

//#############################################################################
//#############################################################################

#endif
