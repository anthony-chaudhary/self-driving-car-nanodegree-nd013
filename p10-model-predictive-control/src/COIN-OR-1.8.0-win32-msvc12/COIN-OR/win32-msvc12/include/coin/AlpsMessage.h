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

#ifndef AlpsMessage_h_
#define AlpsMessage_h_

#include "CoinMessageHandler.hpp"

//#############################################################################
/** A list of Alps print out messages. Note this is not the messages sent
    round among processes. */
enum ALPS_Message
{
    ALPS_DONATE_AFTER,
    ALPS_DONATE_BEFORE,
    ALPS_DONATE_FAIL,
    ALPS_DONATE_SPLIT,
    ALPS_DONATE_WHOLE,
    ALPS_DATAFILE,
    ALPS_KNOWLEDGE_GEN,
    ALPS_HUB_NUM,
    ALPS_LAUNCH,
    ALPS_LOADBAL_HUB,
    ALPS_LOADBAL_HUB_FAIL,
    ALPS_LOADBAL_HUB_NO,
    ALPS_LOADBAL_HUB_PERIOD,
    ALPS_LOADBAL_MASTER,
    ALPS_LOADBAL_MASTER_NO,
    ALPS_LOADBAL_MASTER_PERIOD,
    ALPS_LOADBAL_WORKER_ASK,
    ALPS_LOADREPORT_MASTER,
    ALPS_LOADREPORT_MASTER_F,
    ALPS_LOADREPORT_MASTER_N,
    ALPS_LOADREPORT_MASTER_F_N,
    ALPS_MSG_HOW,
    ALPS_NODE_COUNT,
    ALPS_NODE_MEM_SIZE,
    ALPS_PARAMFILE,
    ALPS_PEAK_MEMORY,
    ALPS_RAMPUP_HUB,
    ALPS_RAMPUP_HUB_FAIL,
    ALPS_RAMPUP_HUB_NODES,
    ALPS_RAMPUP_HUB_NODES_AUTO,
    ALPS_RAMPUP_HUB_RECV,
    ALPS_RAMPUP_HUB_SOL,
    ALPS_RAMPUP_HUB_START,
    ALPS_RAMPUP_MASTER,
    ALPS_RAMPUP_MASTER_FAIL,
    ALPS_RAMPUP_MASTER_NODES,
    ALPS_RAMPUP_MASTER_NODES_AUTO,
    ALPS_RAMPUP_MASTER_SOL,
    ALPS_RAMPUP_MASTER_START,
    ALPS_RAMPUP_WORKER_RECV,
    ALPS_RAMPUP_WORKER_SOL,
    ALPS_SEARCH_WORKER_START,
    ALPS_SOLUTION_COUNT,
    ALPS_SOLUTION_SEARCH,
    ALPS_STATIC_BALANCE_BEG,
    ALPS_STATIC_BALANCE_END,
    ALPS_TERM_FORCE_NODE,
    ALPS_TERM_FORCE_SOL,
    ALPS_TERM_FORCE_TIME,
    ALPS_TERM_HUB_INFORM,
    ALPS_TERM_MASTER_START,
    ALPS_TERM_MASTER_INFORM,
    ALPS_TERM_WORKER_INFORM,
    ALPS_T_FAILED,
    ALPS_T_FEASIBLE,
    ALPS_T_INFEASIBLE,
    ALPS_T_NODE_LIMIT,
    ALPS_T_NO_MEMORY,
    ALPS_T_COMPLETE,
    ALPS_T_SOL_LIMIT,
    ALPS_T_TIME_LIMIT,
    // Following are for serial only
    ALPS_S_NODE_COUNT,
    ALPS_S_SEARCH_START,
    ALPS_S_SEARCH_SOL,
    ALPS_S_FINAL_SOL,
    ALPS_S_FINAL_SOL_WD,
    ALPS_S_FINAL_NO_SOL,
    ALPS_S_FINAL_NODE_PROCESSED,
    ALPS_S_FINAL_NODE_FULL,
    ALPS_S_FINAL_NODE_PARTIAL,
    ALPS_S_FINAL_NODE_BRANCHED,
    ALPS_S_FINAL_NODE_DISCARDED,
    ALPS_S_FINAL_NODE_LEFT,
    ALPS_S_FINAL_DEPTH,
    ALPS_S_FINAL_CPU,
    ALPS_S_FINAL_WALLCLOCK,
    ALPS_WORK_UNIT,
    ALPS_DUMMY_END
};

//#############################################################################

class AlpsMessage : public CoinMessages {

 public:
    /**@name Constructors etc */
    //@{
    AlpsMessage(Language language=us_en);
    //@}
};

#endif
