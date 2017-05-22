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

#ifndef BcpsSubTree_h_
#define BcpsSubTree_h_

#include <vector>
#include "AlpsSubTree.h"
#include "BcpsObjectPool.h"

//#############################################################################

//#############################################################################
/** This class is the data structure for storing a subtree within BCPS. The
    biggest addition to the fields that already exist withink ALPS is the
    storage for the global list of objects that are active within that
    subtree. Initally, this will be implemeted as a std::set, but later on
    should be changed to something more efficient such as a hash table or
    something like that. */
// *FIXME* : Implement hashing for object storage. 
//#############################################################################

class BcpsSubTree : public virtual AlpsSubTree {
 private:
    /** This is the list of objects that exist in the subtree. */
    BcpsConstraintPool *constraintPool_;
    BcpsVariablePool *variablePool_;
    
 public:
    BcpsSubTree() 
        :
        constraintPool_(new BcpsConstraintPool),
        variablePool_(new BcpsVariablePool)
        {}
    virtual ~BcpsSubTree() 
        {
            delete constraintPool_;
            delete variablePool_;
        }

    BcpsConstraintPool* getConstraintPool() const 
        {
            return constraintPool_;
        }
    
    BcpsVariablePool* getVariablePool() const 
        {
            return variablePool_;
        }
};


//#############################################################################
//#############################################################################

#endif
