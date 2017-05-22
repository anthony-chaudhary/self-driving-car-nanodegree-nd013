#ifndef OsiDylpMessages_H
#define OsiDylpMessages_H

/*! \legal
  Copyright (C) 2004 -- 2007
  Lou Hafer, International Business Machines Corporation and others. All
  Rights Reserved.

  This file is a portion of the COIN/OSI interface for dylp and is licensed
  under the terms of the Eclipse Public License (EPL)
*/

/*
  sccs: @(#)OsiDylpMessages.hpp	1.5     09/16/04
  cvs: $Id: OsiDylpMessages.hpp 1312 2008-10-10 00:26:32Z lou $
*/


#include "CoinMessageHandler.hpp"

/*
  Enum used to specify ODSI messages to the message handler. There is no
  need for the order here to match the order of message definition in
  OsiDylpMessages.cpp, but all enum values must be here. ODSI_DUMMY_END must
  be last, however.
*/

typedef enum { ODSI_TEST_MSG,
	       ODSI_MPSFILEIO,
	       ODSI_UNSUPFORCEDO,
	       ODSI_IGNOREDHINT,
	       ODSI_EMPTYODWSB,
	       ODSI_NOTODWSB,
	       ODSI_ODWSBBADSIZE,
	       ODSI_ODWSBBADSTATUS,
	       ODSI_ODWSBSHORTBASIS,
	       ODSI_CWSBREJECT,
	       ODSI_PRESOL_STATS,
	       ODSI_PRESOL_PASS,
	       ODSI_POSTSOL,
	       ODSI_POSTSOL_ACT,
	       ODSI_COLD,
	       ODSI_WARM,
	       ODSI_HOT,
	       ODSI_ALLDYLP,
	       ODSI_ATTACH,
	       ODSI_DETACH,
	       ODSI_NOSOLVE,
	       ODSI_FAILEDCALL,
	       ODSI_ACCESS_STALE,
	       ODSI_SHORTSTATS,
	       ODSI_CONFUSION,
	       ODSI_TABLEAU_INIT_FAIL,
	       ODSI_NOTOWNER,
	       ODSI_NOTOPTIMAL,
	       ODSI_NOTVALID,
	       ODSI_NOTFULLSYS,
	       ODSI_NOTSIMPLEX,
	       ODSI_BADSTATE,
	       ODSI_BADACTIVEBASIS,
	       ODSI_DUMMY_END } OsiDylpMessageID_enum ;

#endif /* OsiDylpMessages_H */
