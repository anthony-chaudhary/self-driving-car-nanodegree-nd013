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

#ifndef BlisNodeDesc_h_
#define BlisNodeDesc_h_

//#############################################################################

#include "CoinWarmStartBasis.hpp"

#include "AlpsNodeDesc.h"
#include "BcpsNodeDesc.h"

#include "BlisHelp.h"
#include "BlisModel.h"

//#############################################################################


class BlisNodeDesc : public BcpsNodeDesc {

 private:
    
    /** Branched direction to create it. For updating pseudocost. */
    int branchedDir_;

    /** Branched object index to create it. For updating pseudocost. */
    int branchedInd_;

    /** Branched value to create it. For updating pseudocost. */
    double branchedVal_;

    /** Warm start. */
    CoinWarmStartBasis *basis_;
    
 public:

    /** Default constructor. */
    BlisNodeDesc() :
        BcpsNodeDesc(),
        branchedDir_(0),
        branchedInd_(-1),
        branchedVal_(0.0),
	basis_(NULL)
        {}

    /** Useful constructor. */
    BlisNodeDesc(BlisModel* m) 
	:
	BcpsNodeDesc(m),
        branchedDir_(0),
        branchedInd_(-1),
        branchedVal_(0.0),
	basis_(NULL)
	{}

    /** Destructor. */
        virtual ~BlisNodeDesc() { delete basis_; basis_ = NULL;  }

    /** Set basis. */ 
    void setBasis(CoinWarmStartBasis *&ws) { 
        if (basis_) { delete basis_; }
        basis_= ws;
        ws = NULL; 
    }

    /** Get warm start basis. */
    CoinWarmStartBasis * getBasis() const { return basis_; }

    /** Set branching direction. */
    void setBranchedDir(int d) { branchedDir_ = d; }

    /** Get branching direction. */
    int getBranchedDir() const { return branchedDir_; }

    /** Set branching object index. */
    void setBranchedInd(int d) { branchedInd_ = d; }

    /** Get branching object index. */
    int getBranchedInd() const { return branchedInd_; }

    /** Set branching value. */
    void setBranchedVal(double d) { branchedVal_ = d; }

    /** Get branching direction. */
    double getBranchedVal() const { return branchedVal_; }

 protected:

    /** Pack blis portion of node description into an encoded. */
    AlpsReturnStatus encodeBlis(AlpsEncoded *encoded) const {
	AlpsReturnStatus status = AlpsReturnStatusOk;

	encoded->writeRep(branchedDir_);
	encoded->writeRep(branchedInd_);
	encoded->writeRep(branchedVal_);

	// Basis
	int ava = 0;
	if (basis_) {
	    ava = 1;
	    encoded->writeRep(ava);
	    BlisEncodeWarmStart(encoded, basis_);
	}
	else {
	    encoded->writeRep(ava);
	}
	
	return status;
    }

    /** Unpack blis portion of node description from an encoded. */
    AlpsReturnStatus decodeBlis(AlpsEncoded &encoded) {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	
	encoded.readRep(branchedDir_);
	encoded.readRep(branchedInd_);
	encoded.readRep(branchedVal_);
	
	// Basis
	int ava;
	encoded.readRep(ava);
	if (ava == 1) {
            if (basis_) delete basis_;
	    basis_ = BlisDecodeWarmStart(encoded, &status);
	}
	else {
	    basis_ = NULL;
	}
	
	return status;
    }

 public:

    /** Pack node description into an encoded. */
    virtual AlpsReturnStatus encode(AlpsEncoded *encoded) const {
    	AlpsReturnStatus status = AlpsReturnStatusOk;
	
	status = encodeBcps(encoded);
	status = encodeBlis(encoded);
	
	return status;
    }

    /** Unpack a node description from an encoded. Fill member data. */
    virtual AlpsReturnStatus decode(AlpsEncoded &encoded) {
	
    	AlpsReturnStatus status = AlpsReturnStatusOk;
	
	status = decodeBcps(encoded);
	status = decodeBlis(encoded);

	return status;
    }
    
};
#endif
