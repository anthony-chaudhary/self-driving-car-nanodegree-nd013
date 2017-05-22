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
   

//#############################################################################
// Borrow ideas from COIN/Cbc
//#############################################################################


#ifndef BcpsBranchObject_h_
#define BcpsBranchObject_h_

#include "BcpsModel.h"

#include "Alps.h"
#include "AlpsEncoded.h"


//#############################################################################


/** BcpsBranchObject contains the member data required when choosing
    branching entities and excuting actual branching. It also has 
    the member funtions to do branching by adjusting bounds, etc. 
    in solver. Branching objects can be simple integer variables or more
    complicated objects like SOS. */

class BcpsBranchObject {
    
 protected:

    /** Type of branching. */
    int type_;
    
    /** The model that owns this branch object. */
    BcpsModel *model_;
    
    /** Branch object index. The index is not the same as variable 
        index. For integer branching, the index refers to the position
        in the integer object array/vector. */
    int objectIndex_;

    /** Quality/Goodness of this object. They are set when creating
        candiate branching entities, and used when comparing two 
        branching enities. 
	Derived class can add more metrics.
    */
    //@{ 
    /** The score of branching up. Used for binary branching only. */
    double upScore_;
    
    /** The score of branching down. Used for binary branching only. */
    double downScore_;
    //@}
    
    /** Information required to do branching. Used for binary branching only.*/
    //@{ 
    /** The direction of the active branch. Down is -1, up is 1. */
    int direction_;
    
    /** Current branching value. For integer, it can be fractional solution 
        value. */
    double value_;
    
    /** Number of arms remaining to be evaluated. */
    int numBranchesLeft_;
    //@}

 public:

    /** Default Constructor. */
    BcpsBranchObject()
        :
	type_(0),
        model_(NULL),
        objectIndex_(-1),
        upScore_(0),
        downScore_(0),
        direction_(0),
        value_(0.0),
        numBranchesLeft_(0)
        {}
    
    /** Useful constructor. */
    BcpsBranchObject(BcpsModel * model)
        :
	type_(0),
        model_(model),
        objectIndex_(-1),
        upScore_(0),
        downScore_(0),
        direction_(0),
        value_(0.0),
        numBranchesLeft_(2) 
        {}

    /** Useful constructor. */
    BcpsBranchObject(BcpsModel * model, 
                     int objectIndex, 
                     int direction , 
                     double value)
        :
	type_(0),
        model_(model),
        objectIndex_(objectIndex),
        upScore_(0),
        downScore_(0),
        direction_(direction),
        value_(value),
        numBranchesLeft_(2) 
        {}

    /** Useful constructor. */
    BcpsBranchObject(BcpsModel * model, 
                     int objectIndex, 
                     int upScore,
                     double downScore,
                     int direction , 
                     double value)
        :
	type_(0),
        model_(model),
        objectIndex_(objectIndex),
        upScore_(upScore),
        downScore_(downScore),
        direction_(direction),
        value_(value),
        numBranchesLeft_(2) 
        {}
    
    /** Copy constructor. */
    BcpsBranchObject (const BcpsBranchObject &);
    
    /** Destructor. */
    virtual ~BcpsBranchObject() { /* Do nothing */}
    
    /** Assignment operator. */
    BcpsBranchObject & operator = ( const BcpsBranchObject& rhs);

    /** Clone a object. */
    virtual BcpsBranchObject * clone() const = 0;
  
    /** Get type. */
    int getType() { return type_; }
    
    /** Set type. */
    void setType(int t) { type_ = t; }

    /** The number of branch arms created for this branch object. */
    virtual int numBranches() const { return 2; }
    
    /** The number of branch arms left to be evaluated. */
    virtual int numBranchesLeft() const { return numBranchesLeft_; }
    
    /**Perform branching as specified by the branching object. 
       Also, update the status of this branching object. */
    // THINK: what's the use of normalBranch?
    virtual double branch(bool normalBranch = false) = 0;
    
    /** Print information about this branching object. */
    virtual void print(bool normalBranch) {}

    /** Return true if branching should fix object bounds. */
    virtual bool boundBranch() const { return true; }

    /** Object objectIndex. */
    inline int getObjectIndex() const { return objectIndex_; }
    
    /** Set object objectIndex. */
    inline void setObjectIndex(int ind) {  objectIndex_ = ind; }
    
    /** Get integer score. */
    inline double getUpScore() const { return upScore_; }

    /** Set integer score. */
    inline void setUpScore(double score) { upScore_ = score; }
    
    /** Get double score. */
    inline double getDownScore() const { return downScore_; }    

    /** Get double score. */
    inline void setDownScore(double score) { downScore_ = score; }    
    
    /** Returns a code indicating the active arm of the branching object. */
    inline int getDirection() const { return direction_; }

    /** Set the direction of the branching object. */
    inline void setDirection(int direction) { direction_ = direction; }
    
    /** Return object branching value. */
    inline double getValue() const { return value_; }
    
    /** Return model. */
    inline BcpsModel * model() const { return  model_; }

 protected:

    /** Pack Bcps portion to an encoded object. */
    AlpsReturnStatus encodeBcps(AlpsEncoded *encoded) const {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	assert(encoded);
	encoded->writeRep(objectIndex_);
	encoded->writeRep(upScore_);
	encoded->writeRep(downScore_);
	encoded->writeRep(direction_);
	encoded->writeRep(value_);
	encoded->writeRep(numBranchesLeft_);

	return status;
    }

    /** Unpack Bcps portion from an encoded object. */
    AlpsReturnStatus decodeBcps(AlpsEncoded &encoded) {
	AlpsReturnStatus status = AlpsReturnStatusOk;

	encoded.readRep(objectIndex_);
	encoded.readRep(upScore_);
	encoded.readRep(downScore_);
	encoded.readRep(direction_);
	encoded.readRep(value_);
	encoded.readRep(numBranchesLeft_);

	return status;
    }

 public:

    /** Pack to an encoded object. */
    virtual AlpsReturnStatus encode(AlpsEncoded *encoded) const {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	// Should never be called.
	assert(0);
	return status;
    }

    /** Unpack a branching object from an encoded object. */
    virtual AlpsReturnStatus decode(AlpsEncoded &encoded) {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	// Should never be called.
	assert(0);
	return status;
    }
    
};

#endif
