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

#ifndef BcpsTreeNode_h_
#define BcpsTreeNode_h_

#include "BcpsNodeDesc.h"

#include <vector>

#include "AlpsTreeNode.h"
#include "AlpsNodeDesc.h"

#include "BcpsBranchObject.h"
#include "BcpsObjectPool.h"

//#############################################################################
/**
   This class contain the data for a BCPS search tree node. At this level, we
   consider a tree node to be simply a list of objects. The objects are
   organized by type. A differencing scheme is implemented here by looking for
   differences between the lists of each type in the current node and its
   parent.
*/
//#############################################################################

class BcpsTreeNode : public AlpsTreeNode {

 protected:

    /** Branching object for this node, which has information of how to
        execute branching. */
    BcpsBranchObject *branchObject_;

 protected:

    /** Generate constraints. The generated constraints are stored
	in constraint pool. The default implementation does nothing. */
    virtual int generateConstraints(BcpsModel *model, 
				    BcpsConstraintPool *conPool) {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	return status;
    }

    /** Generate variables. The generated varaibles are stored
	in variable pool. The default implementation does nothing. */
    virtual int generateVariables(BcpsModel *model, 
				  BcpsVariablePool *varPool) {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	return status;
    }

    /** Choose a branching object. */
    virtual int chooseBranchingObject(BcpsModel *model) = 0;

    /** Extract node information (bounds, constraints, variables) from 
	this node and load the information into the relaxation solver,
	such as linear programming solver.*/
    virtual int installSubProblem(BcpsModel *model) = 0;

    /** Handle bounding status:
     *  - relaxed feasible but not integer feasible,
     *  - integer feasible,
     *  - infeasible,
     *  - unbounded,
     *  - fathomed (for instance, reaching objective limit for LP).
     * Set node status accordingly.
     * \param staus    Input   The solution status of bounding.
     * \param keepOn   Output  Whether to keep on bounding.
     * \param fathomed Output  Whether this node is fathomed. 
     */
    virtual int handleBoundingStatus(int status, bool &keepOn, bool &fathomed){
	// Default do nothing.
	return BcpsReturnStatusOk;
    }
    
 public:

    /** Default constructor. */
    BcpsTreeNode() : branchObject_(NULL) { }

    /** Destructor. */
    virtual ~BcpsTreeNode(){ delete branchObject_; }

    /** This methods performs the processing of the node. For branch and bound,
	this would mean performing the bounding operation. The minimum
	requirement for this method is that it change the status to either
	internal or fathomed so the tree manager can deal with it
	afterwards. The status of the node when it begins processing will be
	active. */
    virtual int process(bool isRoot = false, bool rampUp = false);

    /** Bounding procedure to estimate quality of this node. */
    virtual int bound(BcpsModel *model) = 0;
    
    /** This method must be invoked on a \c pregnant node (which has all the
	information needed to create the children) and should create the
	children's decriptions. The stati of the children
	can be any of the ones \c process() can return. */
    virtual std::vector< CoinTriple<AlpsNodeDesc*, AlpsNodeStatus, double> > 
	branch() = 0;
    
    /** Return the branching object. */
    const BcpsBranchObject * branchObject() const { return branchObject_; }

    /** Set the branching object. */
    void setBranchObject(BcpsBranchObject * b) { branchObject_ = b; }

 protected:

    /** Pack Bcps portion of node into an encoded object. */
    AlpsReturnStatus encodeBcps(AlpsEncoded *encoded) const {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	int type = 0;
	if (branchObject_) {
	    type = branchObject_->getType();
	    encoded->writeRep(type);
	    status = branchObject_->encode(encoded);
	}
	else {
	    encoded->writeRep(type);
	}
	return status;
    }

#if 0 // Can't docode a down(blis) branching object here.
    /** Unpack Bcps portion of node from an encoded object. */
    AlpsReturnStatus decodeBcps(AlpsEncoded &encoded) {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	int mark;
	encoded.readRep(mark);
	
	if (mark == 1) {
	    // branchObject_ is not NULL.
	    status = branchObject_->encode(encoded);
	}

	return status;
    }
#endif
    
};

#endif
