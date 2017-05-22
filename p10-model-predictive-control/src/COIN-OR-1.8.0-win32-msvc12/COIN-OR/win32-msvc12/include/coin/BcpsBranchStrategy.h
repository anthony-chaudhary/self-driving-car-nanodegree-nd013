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

#ifndef BcpsBranchStrategy_h_
#define BcpsBranchStrategy_h_

#include "BcpsBranchObject.h"

class BcpsModel;

//#############################################################################
// NOTE: Borrow ideas from COIN/Cbc.
//#############################################################################


/** Branching strategy specifies:
    (1) how to select a candidate set of branching objects
    (2) how to compare two branching objects
*/
class BcpsBranchStrategy {
    
 private:
    /** Disable assignment operator. */
    BcpsBranchStrategy & operator=(const BcpsBranchStrategy& rhs);
    
 protected:

    /** Type of branching strategy. */
    int type_;
    
    /** Pointer to model. */
    BcpsModel *model_;

    /** Following members are used to store candidate branching objects.
        NOTE: They are required to be cleared before starting another 
        round of selecting.*/
    //@{
    /** Number of candidate branching objects. */
    int numBranchObjects_;
    /** The set of candiate branching objects. */
    BcpsBranchObject ** branchObjects_;
    //@}
    
    /** Following members are used to store information about best 
        branching object found so far. 
        NOTE: They are required to be cleared before starting another 
        round of selecting.*/
    //@{
    /** Best branching object found so far. */
    BcpsBranchObject * bestBranchObject_;
    /** Change up for best. */
    double bestChangeUp_;
    /** Number of infeasibilities for up. */
    int bestNumberUp_;
    /** Change down for best. */
    double bestChangeDown_;
    /** Number of infeasibilities for down. */
    int bestNumberDown_;
    //@}

 public:

    /** Default Constructor. */
    BcpsBranchStrategy()
        :
        model_(NULL),
        numBranchObjects_(0),
        branchObjects_(NULL),
        bestChangeUp_(0.0),
        bestNumberUp_(0),
        bestChangeDown_(0.0),
        bestNumberDown_(0)
        {}

    /** Useful Constructor. */
    BcpsBranchStrategy(BcpsModel *m)
        :
        model_(m),
        numBranchObjects_(0),
        branchObjects_(NULL),
        bestChangeUp_(0.0),
        bestNumberUp_(0),
        bestChangeDown_(0.0),
        bestNumberDown_(0)
        {}
    
    /** Destructor. */
    virtual ~BcpsBranchStrategy() {
        for (int k = 0; k < numBranchObjects_; ++k) {
            delete branchObjects_[k];
        }
        delete [] branchObjects_;
    }
    
    /** Clone a branch strategy. */
    virtual BcpsBranchStrategy * clone() const = 0;
    
    /** Get type. */
    int getType() { return type_; }
    
    /** Set type. */
    void setType(int t) { type_ = t; }

    /** Set model. */
    void setModel(BcpsModel *m) { model_ = m; }

    /** Set/get branching objects. */
    //@{
    int getNumBranchObjects() { return numBranchObjects_; }
    void getNumBranchObjects(int num) { numBranchObjects_ = num; }
    BcpsBranchObject ** getBranchObjects() { return branchObjects_; }
    void setBranchObjects(BcpsBranchObject **obj) { branchObjects_ = obj; }
    BcpsBranchObject *getBestBranchObject() { return bestBranchObject_; }
    void setBestBranchObject(BcpsBranchObject *ob) { bestBranchObject_ = ob; }
    //@}

    /** Clear branching strategy environment before starting a new round
        of selecting the best branch object. */
    virtual void clearBest(BcpsModel * model) {
        bestBranchObject_ = NULL;
        bestChangeUp_ = 0.0;
        bestNumberUp_ = 0;
        bestChangeDown_ = 0.0;
        bestNumberDown_ = 0;
    }

    /** Create a set of candidate branching objects. */
    virtual int createCandBranchObjects(int numPassesLeft, double ub){ 
	return 0; 
    }
    
    /** Compare branching object thisOne to bestSoFar. If thisOne is better 
	than bestObject, return branching direction(1 or -1), otherwise
	return 0. 
	If bestSorFar is NULL, then always return branching direction(1 or -1).
    */
    virtual int betterBranchObject(BcpsBranchObject * b,
				   BcpsBranchObject * bestSoFar) = 0;
    
    /** Compare branching objects in branchObjects_. Return the index of the 
	best branching object. Also, set branch direction in the best object. 
    */
    virtual BcpsBranchObject *bestBranchObject();
};

#endif
