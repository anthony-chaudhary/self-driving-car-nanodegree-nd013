/*===========================================================================*
 * This file is part of the BiCePS Linear Integer Solver (BLIS).             *
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
 * All Rights Reserved.                                                      *
 *===========================================================================*/


//#############################################################################
// NOTE: Borrow ideas from COIN/Cbc
//#############################################################################


#ifndef BlisBranchStrategyStrong_h_
#define BlisBranchStrategyStrong_h_

#include "BcpsBranchObject.h"
#include "BcpsBranchStrategy.h"
#include "BlisModel.h"


//#############################################################################


typedef struct {
    int objectIndex;            // object index
    BcpsBranchObject * bObject; // the branching object
    int numIntInfUp;            // without odd ones
    int numObjInfUp;            // just odd ones
    bool finishedUp;            // true if solver finished
    int numIntInfDown;          // without odd ones
    int numObjInfDown;          // just odd ones
    bool finishedDown;          // true if solver finished
} BlisStrong;


//#############################################################################


/** This class implements strong branching. */
class BlisBranchStrategyStrong : public BcpsBranchStrategy {

 private:

    /** Illegal Assignment operator.*/
    BlisBranchStrategyStrong& operator=(const BlisBranchStrategyStrong& rhs);
    
 public:
    
    /** Strong Constructor. */
    BlisBranchStrategyStrong() {
	type_ = static_cast<int>(BlisBranchingStrategyStrong);
    }

    /** Strong Constructor. */
    BlisBranchStrategyStrong(BlisModel *model)
        : BcpsBranchStrategy(model) {
	type_ = static_cast<int>(BlisBranchingStrategyStrong);
    }
    
    /** Destructor. */
    virtual ~BlisBranchStrategyStrong() {}
    
    /** Copy constructor. */
    BlisBranchStrategyStrong(const BlisBranchStrategyStrong &);
    
    /** Clone a brancing strategy. */
    virtual BcpsBranchStrategy * clone() const {
	return new BlisBranchStrategyStrong(*this);
    }
    
    /** Create a set of candidate branching objects. */
    virtual int createCandBranchObjects(int numPassesLeft, double ub);
    
    /** Compare branching object thisOne to bestSoFar. If thisOne is better 
	than bestObject, return branching direction(1 or -1), otherwise
	return 0. 
	If bestSorFar is NULL, then always return branching direction(1 or -1).
    */
    virtual int betterBranchObject(BcpsBranchObject * thisOne,
				   BcpsBranchObject * bestSoFar);
};

#endif
