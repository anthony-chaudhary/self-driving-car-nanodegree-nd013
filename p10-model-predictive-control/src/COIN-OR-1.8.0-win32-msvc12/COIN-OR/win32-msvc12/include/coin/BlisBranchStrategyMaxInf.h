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

#ifndef BlisBranchStrategyMaxInf_h_
#define BlisBranchStrategyMaxInf_h_

#include "BcpsBranchObject.h"
#include "BcpsBranchStrategy.h"
#include "BlisModel.h"

/** This class implements maximum infeasibility branching. */
class BlisBranchStrategyMaxInf : public BcpsBranchStrategy {

 private:

  /** Illegal Assignment operator.*/
    BlisBranchStrategyMaxInf& operator=(const BlisBranchStrategyMaxInf& rhs);
    
 public:
    
    /** MaxInf Constructor. */
    BlisBranchStrategyMaxInf() {
	type_ = static_cast<int>(BlisBranchingStrategyMaxInfeasibility);
    }

    /** MaxInf Constructor. */
    BlisBranchStrategyMaxInf(BlisModel *model) : BcpsBranchStrategy(model) {
	type_ = static_cast<int>(BlisBranchingStrategyMaxInfeasibility);
    }
    
    /** Destructor. */
    virtual ~BlisBranchStrategyMaxInf() {}
    
    /** Copy constructor. */
    BlisBranchStrategyMaxInf(const BlisBranchStrategyMaxInf &);
    
    /** Clone a brancing strategy. */
    virtual BcpsBranchStrategy * clone() const {
	return new BlisBranchStrategyMaxInf(*this);
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
