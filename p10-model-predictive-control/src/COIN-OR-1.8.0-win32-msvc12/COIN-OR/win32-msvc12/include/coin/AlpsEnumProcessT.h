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


#ifndef AlpsEnumProcessT_H
#define AlpsEnumProcessT_H

//#############################################################################

/** This enumerative constant describes the various process types. */

enum AlpsProcessType {
    /** Serial */
  AlpsProcessTypeSerial = 1,
  /** Master process */
  AlpsProcessTypeMaster,
  /** */
  AlpsProcessTypeHub,
  /** */
  AlpsProcessTypeWorker,
  /** */
  AlpsProcessTypeCG,
  /** */
  AlpsProcessTypeVG,
  /** */
  AlpsProcessTypeCP,
  /** */
  AlpsProcessTypeVP,
  /** */
  AlpsProcessTypeAny
};

#endif
