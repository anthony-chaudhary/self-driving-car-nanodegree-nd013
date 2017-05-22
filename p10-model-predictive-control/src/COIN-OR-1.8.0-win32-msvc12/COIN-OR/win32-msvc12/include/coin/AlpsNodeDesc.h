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

#ifndef AlpsNodeDesc_h_
#define AlpsNodeDesc_h_

//#include "AlpsModel.h"

class AlpsModel;

//#############################################################################
/** A class to refer to the description of a search tree node.
 *FIXME* : write a better doc...
 */
//#############################################################################
class AlpsNodeDesc {

 protected:

    /** A pointer to model. */
    // Should allow change model due to presolve
    AlpsModel* model_;
     
 public:

    AlpsNodeDesc() {}
    AlpsNodeDesc(AlpsModel* m)
	{ model_ = m; }

    virtual ~AlpsNodeDesc() {}

    inline AlpsModel* getModel() const { return model_; }
    inline void setModel(AlpsModel* m) { model_ = m; }

    /** Pack node description into an encoded. */
    virtual AlpsReturnStatus encode(AlpsEncoded *encoded) const {
    	AlpsReturnStatus status = AlpsReturnStatusOk;
	// Should never be called.
	assert(0);
	return status;
    }

    /** Unpack a node description from an encoded. Fill member data. */
    virtual AlpsReturnStatus decode(AlpsEncoded &encoded) {
    	AlpsReturnStatus status = AlpsReturnStatusOk;
	assert(0);
	return status;
    }
};

#endif
