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

#ifndef Bcps_h_
#define Bcps_h_

//#############################################################################
// Return code.
//#############################################################################

enum BcpsReturnStatus {
   BcpsReturnStatusOk = 0,
   BcpsReturnStatusErr
};

//#############################################################################

enum BcpsKnowledgeType{
   BcpsKnowledgeTypeConstraint  = 11,
   BcpsKnowledgeTypeVariable  = 12
};

//#############################################################################

enum BcpsValidRegion{
    BcpsValidLocal = 0,
    BcpsValidGlobal
};

//#############################################################################

#endif

