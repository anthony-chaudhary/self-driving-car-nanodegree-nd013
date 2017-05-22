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

#ifndef BlisMessage_H_
#define BlisMessage_H_

//#############################################################################

#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

/** This deals with Blis messages. */
#include "CoinMessageHandler.hpp"

//#############################################################################

enum BLIS_Message
{
    BLIS_CUTOFF_INC,
    BLIS_CUT_STAT_FINAL,
    BLIS_CUT_STAT_NODE,
    BLIS_GAP_NO,
    BLIS_GAP_YES,
    BLIS_HEUR_BEFORE_ROOT,
    BLIS_HEUR_STAT_FINAL,
    BLIS_HEUR_STAT_NODE,
    BLIS_ROOT_PROCESS,
    BLIS_ROOT_TIME,
    BLIS_W_LP,
    ///
    BLIS_DUMMY_END
};

//#############################################################################

class BlisMessage : public CoinMessages 
{
public:
    
    /**@name Constructors etc */
    //@{
    /** Constructor */
    BlisMessage(Language language=us_en);
    //@}
};

//#############################################################################

#endif
