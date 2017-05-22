/*
  This file is a part of the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

#ifndef _DY_CMDINT_H
#define _DY_CMDINT_H

/*
  @(#)dy_cmdint.h	3.3	06/22/04
  svn/cvs: $Id: dy_cmdint.h 407 2010-12-31 20:48:48Z lou $

  Declarations specific to dylp's command interpreter.
*/

#include "dylib_std.h"
#include "dylib_io.h"
#include "dylib_errs.h"

/*
  We need dylp.h only for the typedefs of lpopts_struct and lptols_struct,
  and extern declarations for dy_logchn and dy_gtxecho.
*/
#define DYLP_INTERNAL
#include "dylp.h"

/*
  cmdint.c
*/

/*
  Return codes for command execution routines called from the command
  interpreter:

    cmdOK	execution of the command was adequately successful, further
		command interpretation should continue.
    cmdHALTNOERROR execution of the command was adequately successful, but break
		out of the command interpretation loop.
    cmdHALTERROR an error occurred during execution of the command, break
		out of the command interpretation loop.

  As return codes for process_cmds, the interpretation is slightly different:
    cmdOK	command interpretation was ended by an eof on the top level
		command channel (this is the normal case when command execution
		completes without error).
    cmdHALTNOERROR some command returned a cmdHALTNOERROR return code.
    cmdHALTERROR either a command returned a cmdHALTERROR return code, or a
		fatal error occurred in process_cmds.
*/

typedef enum { cmdOK, cmdHALTERROR, cmdHALTNOERROR } cmd_retval ;

cmd_retval dy_processcmds(ioid cmdchn, bool silent,
			  lpopts_struct *lpopts, lptols_struct *lptols) ;

#endif	/* _DY_CMDINT_H */
