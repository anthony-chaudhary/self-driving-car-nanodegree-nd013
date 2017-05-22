# Copyright (C) 2007 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: configure.ac 1130 2008-08-22 18:07:44Z stefan $

# Author:  Andreas Waechter            IBM    2006-04-13
# Revised: Lou Hafer		       SFU    2007-09-15

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([ThirdPartyASL],[1.0.15],[http://projects.coin-or.org/BuildTools/newticket])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Common Public License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(solvers/asl.h)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the system type
AC_CANONICAL_BUILD

# A bit of initial setup.
AC_COIN_PROJECTDIR_INIT

# Turn off warnings and pedantic mode since they could make trouble on some
# platforms, and we can't fix issues in third-party code.
coin_skip_warn_cflags=yes

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE(ASL)

# We need egrep and ln -s somewhere
AC_PROG_EGREP
AC_PROG_LN_S

# Get the name of the C compiler and compiler flags
AC_COIN_PROG_CC

# Initialize automake and libtool
AC_COIN_INIT_AUTO_TOOLS

# See what we're actually going to do for ASL. This will define ampllib.
AC_COIN_HAS_ASL

MAKEOKFILE=.MakeOk
if test $coin_has_asl != no && test "$use_asldir" != BUILD; then
  skip_further_test=yes
  rm -f $MAKEOKFILE
  AC_MSG_NOTICE([The ASL sources don't need to be compiled])
  MAKEOKFILE=
else
  touch $MAKEOKFILE
fi
AC_SUBST(MAKEOKFILE)
AM_CONDITIONAL(SKIPMAKE, test -z "$MAKEOKFILE")

#############################################################################
#                          AMPL SOLVER LIBRARY                              #
#############################################################################

if test x$skip_further_test != xyes; then

# Set a substitutable variable with the library name, and determine the
# compilation style.

  AC_SUBST(AMPLSOLVER_A)
  AMPLSOLVER_A=$ampllib
  case "$CC" in
    cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
      msvc_compile=yes ;;
    *)
      msvc_compile=no ;;
  esac
  AC_MSG_NOTICE([Configuring ASL to build $AMPLSOLVER_A.])

# Determine flags to be passed on the command line to (n)make. The main
# activity is to groom CFLAGS and set up ASLMAKEFLAGS to pass CFLAGS on the
# (n)make command line.
# NON_STDIO no longer appears in any source file; removed all mention.
# No_Control87 seems to be specific to DEC Alpha. Try leaving it undefined.

  if test -z "$ASLMAKEFLAGS" ; then
    asladdcflags=
    case $build in
      *-linux*)
        case $CC in
          pgcc* | */pgcc* | xlc* | */xlc* | blrts_xlc* | */blrts_xlc*)
            asladdcflags="$asladdcflags -DNO_fpu_control";;
        esac
        ;;
    esac

# Various feature checks. Force the language to be C while we do this to avoid
# issues with C++ (re)declarations.

    AC_LANG_PUSH(C)

# The critical function for NO_RUSAGE is getrusage(). Use a standard test.

    AC_MSG_CHECKING([for getrusage])
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM([[]],[[getrusage()]])],
      [AC_MSG_RESULT([yes])],
      [asladdcflags="$asladdcflags -DNO_RUSAGE"
       AC_MSG_RESULT([no])])

# Check for long long; we need to define NO_LONG_LONG if it's not present.  ASL
# makefile.u and makefile.vc both deal with this, in their own way.
# Unfortunately, when trying to build with cl under cygwin, the makefile.vc
# solution, comptry.bat, fails. To safely discard comptry.bat, we need to do
# this check. It's hardly likely that long long is missing on any modern
# system, but it's easy to check.

    AC_CHECK_TYPE([long long],[],
      [asladdcflags="$asladdcflags -DNO_LONG_LONG"],[])

# Back to whatever language we were in.

    AC_LANG_POP(C)

# On BlueGene disable dynamic library loading

    if test x"$enable_bg" = xyes; then
      asladdcflags="$asladdcflags -DNO_FUNCADD"
    fi

# Groom CFLAGS before we merge it with asladdcflags.
# Remove -Werror if present, to make sure we are not interrupting compilation
# of ASL because of warning messages. The cl equivalent is -WX.

    bla=
    if test x$msvc_compile = xyes ; then
      warnToErr="-WX"
    else
      warnToErr="-Werror"
    fi
    for i in $CFLAGS; do
      if test "$i" != "$warnToErr"; then
        bla="$bla $i"
      fi
    done
    ASLMAKEFLAGS="CFLAGS=\"$bla $asladdcflags\""

# Somehow, the flags specified here cause ASL to segfault on the AIX
# compiler, so take them out.

    case $CC in
      xlc* | */xlc* | mpxlc* | */mpxlc*)
        ASLMAKEFLAGS=`echo $ASLMAKEFLAGS | sed -e 's/-qtune=auto//g' -e 's/-qarch=auto//g' -e 's/-qcache=auto//g' -e 's/-qhot//g' -e sed 's/-O3/-O/g'`
        ;;
      pgcc* | */pgcc*)
        ASLMAKEFLAGS=`echo $ASLMAKEFLAGS | sed -e 's/-fast/-O1/g'`
        ;;
    esac

# Limit GNU make to one thread. But remember that if we're building with
# cl, ultimately we're going to use nmake, which knows nothing of this.

    if test x$msvc_compile = xno ; then
      rm -rf conftest.mak
      ($am_make -v > conftest.grp) 2>&5
      if $EGREP GNU conftest.grp >&5 ; then
	ASLMAKEFLAGS="-j 1 $ASLMAKEFLAGS"
      fi
      rm -f conftest.grp
    fi
  fi

# End if-then to construct ASLMAKEFLAGS. Announce the result.

  AC_MSG_NOTICE([Command line flags for (n)make \"$ASLMAKEFLAGS\".])
  AC_ARG_VAR(ASLMAKEFLAGS,
  	[Compilation flags necessary for compiling the AMPL solver library])

fi

# Set automake conditional and generate the output files.

AM_CONDITIONAL(MSVC_COMPILE, test x$msvc_compile = xyes)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([compile_MS_ASL],[chmod a+x compile_MS_ASL])
AC_CONFIG_FILES([compile_Unix_ASL],[chmod a+x compile_Unix_ASL])

AC_COIN_FINALIZE
