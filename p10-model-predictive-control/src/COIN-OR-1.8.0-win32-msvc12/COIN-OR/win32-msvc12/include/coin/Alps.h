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
 * All Rights Reserved.                                                      *
 *===========================================================================*/

#ifndef Alps_h_
#define Alps_h_

#include <cfloat>
#include <cstdio>

#include "AlpsConfig.h"
#include "CoinFinite.hpp"

//#############################################################################

#if defined(__linux__)
#define ALPS_MEMORY_USAGE 1
#endif

//#############################################################################
/** The possible values for clock type. */
//#############################################################################

enum AlpsClockType {
   AlpsClockTypeCpu,
   AlpsClockTypeWallClock
};

//#############################################################################
/** The possible values for static load balancing scheme. */
//#############################################################################

enum AlpsStaticBalanceScheme {
    AlpsRootInit = 0,
    AlpsSpiral
};

//#############################################################################
/** The possible stati for the search nodes. */
//#############################################################################

enum AlpsNodeStatus {
    AlpsNodeStatusCandidate,
    AlpsNodeStatusEvaluated,
    AlpsNodeStatusPregnant,
    AlpsNodeStatusBranched,
    AlpsNodeStatusFathomed,
    AlpsNodeStatusDiscarded
};

//#############################################################################
/** Search Strategies. */
//#############################################################################

enum AlpsSearchType {
    AlpsSearchTypeBestFirst = 0,
    AlpsSearchTypeBreadthFirst,
    AlpsSearchTypeDepthFirst,
    AlpsSearchTypeBestEstimate,
    AlpsSearchTypeHybrid
};

//#############################################################################
/** Type of knowledge like solution, node, cut...*/
//#############################################################################

typedef int KnowledgeType;

enum AlpsKnowledgeType{
   AlpsKnowledgeTypeModel = 0,
   AlpsKnowledgeTypeModelGen, 
   AlpsKnowledgeTypeNode,
   AlpsKnowledgeTypeSolution,
   AlpsKnowledgeTypeSubTree,
   AlpsKnowledgeTypeUndefined
};

//#############################################################################
// Search return status
//#############################################################################

enum AlpsExitStatus {
    AlpsExitStatusUnknown = -1,
    AlpsExitStatusOptimal,
    AlpsExitStatusTimeLimit, 
    AlpsExitStatusNodeLimit,
    AlpsExitStatusSolLimit,
    AlpsExitStatusFeasible,
    AlpsExitStatusInfeasible,
    AlpsExitStatusNoMemory,
    AlpsExitStatusFailed,
    AlpsExitStatusUnbounded
};

//#############################################################################
// Return code.
//#############################################################################

enum AlpsReturnStatus {
    AlpsReturnStatusOk = 0,
    AlpsReturnStatusErr,
    AlpsReturnStatusErrNoInt,  /* No integer variable.*/
    AlpsReturnStatusErrNoMem
};

//#############################################################################
// Seach phase
//#############################################################################

enum AlpsPhase {
    AlpsPhaseRampup = 0,
    AlpsPhaseSearch,
    AlpasPhaseRampdown
};

#define ALPS_NODE_PROCESS_TIME  0.0123
#define ALPS_NONE 0
#define ALPS_NOT_SET -1

//#############################################################################
// Big number
//#############################################################################

#define ALPS_DBL_MAX          COIN_DBL_MAX
#define ALPS_INC_MAX          1.0e80
#define ALPS_OBJ_MAX          1.0e75
#define ALPS_OBJ_MAX_LESS     1.0e70
#define ALPS_BND_MAX          1.0e20
#define ALPS_INFINITY         1.0e20

#define ALPS_INT_MAX          COIN_INT_MAX

//#############################################################################
// Small number
//#############################################################################

#define ALPS_ZERO             1.0e-14
#define ALPS_GEN_TOL          1.0e-6
#define ALPS_QUALITY_TOL      1.0e-5
#define ALPS_SMALL_3          1.0e-3
#define ALPS_SMALL_4          1.0e-4
#define ALPS_SMALL_5          1.0e-5

//#############################################################################

#define ALPS_PRINTF           printf

#define ALPS_DMSG             printf


//#############################################################################

#define  ALPS_MAX( x, y )          ( ( (x) > (y) ) ? (x) : (y) )
#define  ALPS_MIN( x, y )          ( ( (x) < (y) ) ? (x) : (y) )
#define  ALPS_FABS(x)              ( (x < 0.0) ? -(x) : (x) )
#define  ALPS_ABS(x)               ( (x < 0) ? -(x) : (x) )

//#############################################################################

typedef struct ALPS_PS_STATS 
{
    int qualityBalance_;
    int quantityBalance_;
    int interBalance_;
    int intraBalance_;
    int workerAsk_;
    int donateSuccess_;
    int donateFail_;
    int subtreeSplit_;
    int subtreeWhole_;
    int subtreeChange_;
} AlpsPsStats;
    
//#############################################################################


#endif
