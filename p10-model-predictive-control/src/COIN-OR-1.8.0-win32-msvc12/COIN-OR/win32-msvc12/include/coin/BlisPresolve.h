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

#ifndef BlisPresolve_H_
#define BlisPresolve_H_

#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#include "OsiPresolve.hpp"

//#############################################################################

/** A interface to Osi/Coin Presolve. */
class BlisPresolve : public OsiPresolve
{
private:
    
    CoinPresolveMatrix *preMatrix_;
    CoinPostsolveMatrix *postMatrix_;

public:
    
    /** Default constructor (empty object) */
    BlisPresolve() :
        preMatrix_(0), 
        postMatrix_(0) {}
    
    /** Virtual destructor */
    virtual ~BlisPresolve() { 
        delete preMatrix_; 
        delete postMatrix_; 
    }

    /** Presolve */
    virtual OsiSolverInterface *preprocess(OsiSolverInterface & origModel,
                                           double feasibilityTolerance=0.0,
                                           bool keepIntegers=true,
                                           int numberPasses=5,
                                           const char * prohibited=NULL);

    /** Postsolve */
    virtual void postprocess(bool updateStatus=true);
};

#endif

//#############################################################################
