/*===========================================================================*
 * This file is part of the BiCePS Linear Integer Solver (BLIS).             *
 *                                                                           *
 * BLIS is distributed under the Eclipse Public License as part of the       *
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
 * All Rights Reserved.                                                      *
 *===========================================================================*/


//#############################################################################
// Borrow ideas from COIN/Cbc
//#############################################################################


#include "BcpsBranchObject.h"

#include "BlisModel.h"


//#############################################################################


class BlisBranchObjectInt : public BcpsBranchObject {

 protected:

    /** Down_[0]: the lower bound of down arm; 
        Down_[1]: the upper bound of down arm; */
    double down_[2];
    
    /** Up_[0]: the lower bound of upper arm; 
        Up_[1]: the upper bound of upper arm; */
    double up_[2];

 public:
    
    /** Default constructor. */
    BlisBranchObjectInt()    
        : 
        BcpsBranchObject()
        {
	    type_ = BlisBranchingObjectTypeInt;
            down_[0] = 0.0;
            down_[1] = 0.0;
            up_[0] = 0.0;
            up_[1] = 0.0;
        }

    /** Construct a branching object, which branching on variable varInd. 
        \param varInd     the index of integer variable in object set
        \param direction  the direction of first branching: 1(up), -1(down)
        \param value      the fractional solution value of variable varInd 
    */
    BlisBranchObjectInt(BlisModel * model, 
                        int varInd,
                        int direction,
                        double value)
        : 
        BcpsBranchObject(model, varInd, direction, value)
        {
	    type_ = BlisBranchingObjectTypeInt;
            int iColumn = model->getIntColIndices()[objectIndex_];
            down_[0] = model->solver()->getColLower()[iColumn];
            down_[1] = floor(value_);
            up_[0] = ceil(value_);
            up_[1] = model->getColUpper()[iColumn];
        }

    /** Construct a branching object, which branching on variable varInd. 
        \param varInd     the index of integer variable in object set
        \param intScore   the integer score/goodness
        \param dblScore   the double score/goodness
        \param direction  the direction of first branching: 1(up), -1(down)
        \param value      the fractional solution value of variable varInd 
    */
    BlisBranchObjectInt(BlisModel * model,
                        int varInd,
                        int intScore,
                        double dblScore,
                        int direction,
                        double value)
        : 
        BcpsBranchObject(model, varInd, intScore, dblScore, direction, value)
        {
	    type_ = BlisBranchingObjectTypeInt;
            int iColumn = model->getIntColIndices()[objectIndex_];
            down_[0] = model->solver()->getColLower()[iColumn];
            down_[1] = floor(value_);
            up_[0] = ceil(value_);
            up_[1] = model->getColUpper()[iColumn];
        }
    
    /** Create a degenerate branching object.
        Specifies a `one-direction branch'. Calling branch() for this 
        object will always result in lowerValue <= x <= upperValue. 
        Used to fix a variable when lowerValue = upperValue.
    */
    BlisBranchObjectInt(BlisModel * model,
                        int varInd, 
                        int direction,
                        double lowerValue, 
                        double upperValue)
        :
        BcpsBranchObject(model, varInd, direction, lowerValue)
        {
	    type_ = BlisBranchingObjectTypeInt;
            numBranchesLeft_ = 1;
            down_[0] = lowerValue;
            down_[1] = upperValue;
            up_[0] = lowerValue;
            up_[1] = upperValue;
        }
  
    /** Copy constructor. */
    BlisBranchObjectInt(const BlisBranchObjectInt &);
    
    /** Assignment operator. */
    BlisBranchObjectInt & operator = (const BlisBranchObjectInt& rhs);
    
    /** Clone. */
    virtual BcpsBranchObject * clone() const {
        return (new BlisBranchObjectInt(*this));
    }

    /** Destructor. */
    virtual ~BlisBranchObjectInt() {}
  
    /** Set the bounds for the variable according to the current arm
	of the branch and advances the object state to the next arm.
	Returns change in guessed objective on next branch. */
    virtual double branch(bool normalBranch = false);

    /** \brief Print something about branch - only if log level high. */
    virtual void print(bool normalBranch);

    /** Get down arm bounds. */
    const double *getDown() const { return down_; }

    /** Get upper arm bounds. */
    const double *getUp() const { return up_; }

 protected:

    /** Pack Blis portion to an encoded object. */
    AlpsReturnStatus encodeBlis(AlpsEncoded *encoded) const {
	assert(encoded);
	AlpsReturnStatus status = AlpsReturnStatusOk;
	int j;
	// TODO: N-way.
	for (j = 0; j < 2; ++j) {
	    encoded->writeRep(down_[j]);
	}
	for (j = 0; j < 2; ++j) {
	    encoded->writeRep(up_[j]);
	}
	
	return status;
    }

    /** Unpack Blis portion from an encoded object. */
    AlpsReturnStatus decodeBlis(AlpsEncoded &encoded) {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	int j;
	// TODO: N-way.
	for (j = 0; j < 2; ++j) {
	    encoded.readRep(down_[j]);
	}
	for (j = 0; j < 2; ++j) {
	    encoded.readRep(up_[j]);
	}
	
	return status;
    }

 public:

    /** Pack to an encoded object. */
    virtual AlpsReturnStatus encode(AlpsEncoded *encoded) const {
	AlpsReturnStatus status = AlpsReturnStatusOk;

	status = encodeBcps(encoded);
	status = encodeBlis(encoded);
	
	return status;
    }

    /** Unpack a branching object from an encoded object. */
    virtual AlpsReturnStatus decode(AlpsEncoded &encoded) {
	
	AlpsReturnStatus status = AlpsReturnStatusOk;

	status = decodeBcps(encoded);
	status = decodeBlis(encoded);
	
	return status;
    }
    
};

