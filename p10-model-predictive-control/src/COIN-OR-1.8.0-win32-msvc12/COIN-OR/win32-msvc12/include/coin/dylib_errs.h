#ifndef _DYLIB_ERRS_H
#define _DYLIB_ERRS_H

/*
  This file is part of the support library for the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

/*
  sccs: @(#)errs.h	2.3	03/18/04
  svn/cvs: $Id: dylib_errs.h 407 2010-12-31 20:48:48Z lou $
*/

#include "dylib_std.h"
#ifdef _DYLIB_FORTRAN
#include "dylib_fortran.h"
#endif

void errinit(const char *emsgpath, const char *elogpath, bool errecho),
     errterm(void) ;

void errmsg(int errid, ... ),
       warn(int errid, ... ) ;

#ifdef _DYLIB_FORTRAN
void errmsg_(integer *errid, char *ident, ... ) ;
void warn_(integer *errid, char *ident, ... ) ;
#endif

#endif /* _DYLIB_ERRS_H */
