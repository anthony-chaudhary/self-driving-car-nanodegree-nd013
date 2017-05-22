# Copyright (C) 2006, 2007 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Common Public License.
#
## $Id: coin.m4 1112 2008-08-22 16:39:12Z andreasw $
#
# Author: Andreas Wachter    IBM      2006-04-14

# This file defines the common autoconf macros for COIN
#

# Check requirements
AC_PREREQ(2.59)

###########################################################################
#                           COIN_MAIN_SUBDIRS                             #
###########################################################################

# This macro sets up the recursion into configure scripts into
# subdirectories.  Each possible subdirectory should be provided as a
# new argument to this macro.  The current limit is 10 subdirectories.
# This automatically also checks for the Data subdirectory.

AC_DEFUN([AC_COIN_MAIN_SUBDIRS],
[AC_ARG_VAR([COIN_SKIP_PROJECTS],[Set to the subdirectories of projects that should be skipped in the configuration])

m4_ifvaln([$1],[AC_MSG_CHECKING(whether directory $1 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $1; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$1/configure; then
                  coin_subdirs="$coin_subdirs $1"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($1)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$2],[AC_MSG_CHECKING(whether directory $2 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $2; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$2/configure; then
                  coin_subdirs="$coin_subdirs $2"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($2)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$3],[AC_MSG_CHECKING(whether directory $3 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $3; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$3/configure; then
                  coin_subdirs="$coin_subdirs $3"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($3)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$4],[AC_MSG_CHECKING(whether directory $4 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $4; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$4/configure; then
                  coin_subdirs="$coin_subdirs $4"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($4)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$5],[AC_MSG_CHECKING(whether directory $5 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $5; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$5/configure; then
                  coin_subdirs="$coin_subdirs $5"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($5)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$6],[AC_MSG_CHECKING(whether directory $6 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $6; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$6/configure; then
                  coin_subdirs="$coin_subdirs $6"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($6)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$7],[AC_MSG_CHECKING(whether directory $7 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $7; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$7/configure; then
                  coin_subdirs="$coin_subdirs $7"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($7)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$8],[AC_MSG_CHECKING(whether directory $8 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $8; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$8/configure; then
                  coin_subdirs="$coin_subdirs $8"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($8)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$9],[AC_MSG_CHECKING(whether directory $9 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $9; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$9/configure; then
                  coin_subdirs="$coin_subdirs $9"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($9)
                else
                  AC_MSG_RESULT(no)
                fi])
m4_ifvaln([$10],[AC_MSG_CHECKING(whether directory $10 is available)
                coin_skip=no
                if test x"$COIN_SKIP_PROJECTS" != x; then
                  for dir in $COIN_SKIP_PROJECTS; do
                    if test $dir = $10; then
                      coin_skip=yes
                    fi
                  done
                fi
                if test $coin_skip = yes; then
                  AC_MSG_RESULT(skipping)
                elif test -r $srcdir/$10/configure; then
                  coin_subdirs="$coin_subdirs $10"
                  AC_MSG_RESULT(yes)
                  AC_CONFIG_SUBDIRS($10)
                else
                  AC_MSG_RESULT(no)
                fi])
]) # AC_COIN_MAIN_SUBDIRS

###########################################################################
#                            COIN_CHECK_FILE                              #
###########################################################################

# A simple replacement for AC_CHECK_FILE that works for cross compilation

AC_DEFUN([AC_COIN_CHECK_FILE],
[if test -r $1; then
  $2
  :
else
  $3
  :
fi
])

###########################################################################
#                        COIN_THIRDPARTY_SUBDIRS                          #
###########################################################################

# This macro sets up the recursion into the configure script in a
# subdirectory for compilation of third party code.  The first
# argument is just the string that appears in the configure output.
# The second argument is the directory with the configure script, and
# the third one is a file that should exists in that directory.  If
# this file does not exist, we assume that the user has not downloaded
# the code, and we are not going to compile it

AC_DEFUN([AC_COIN_THIRDPARTY_SUBDIRS],
[AC_MSG_CHECKING(whether code for third party package $1 is available)
coin_skip=no
if test x"$COIN_SKIP_PROJECTS" != x; then
  for dir in $COIN_SKIP_PROJECTS; do
    if test $dir = $2; then
      coin_skip=yes
    fi
  done
fi
if test $coin_skip = yes; then
  AC_MSG_RESULT(skipping)
else
  coin_tmp=`echo $srcdir/$2/$3`
  # There is probably a more elegant way to get the first thing out here...
  for i in $coin_tmp; do
    coin_tmp2=$i
    #break 1
  done
  if test -r $coin_tmp2; then
    coin_subdirs="$coin_subdirs $2"
    AC_MSG_RESULT(yes)
    AC_CONFIG_SUBDIRS($2)
  else
    AC_MSG_RESULT(no)
  fi
fi
]) # AC_COIN_THIRDPARTY_SUBDIRS

###########################################################################
#                           COIN_CHECK_VPATH                              #
###########################################################################

# This macro sets the variable coin_vpath_config to true if this is a
# VPATH configuration, otherwise it sets it to false.
AC_DEFUN([AC_COIN_CHECK_VPATH],
[AC_MSG_CHECKING(whether this is a VPATH configuration)
if test `cd $srcdir; pwd` != `pwd`; then
  coin_vpath_config=yes;
else
  coin_vpath_config=no;
fi
AC_MSG_RESULT($coin_vpath_config)
]) # AC_COIN_CHECK_VPATH

###########################################################################
#                         COIN_PROJECTDIR_INIT                            #
###########################################################################

# This macro does everything that is required in the early part in the
# configure script, such as defining a few variables.  This should only
# be used in the main directory of a project directory (the one under
# which src is)

AC_DEFUN([AC_COIN_PROJECTDIR_INIT],
[# Initialize the ADDLIBS variable
ADDLIBS="-lm $LIBS"
AC_SUBST(ADDLIBS)

# Initialize the FADDLIBS variable (which is to be used with a fortran
# compiler and will not include FLIBS)
FADDLIBS="$LIBS"
AC_SUBST(FADDLIBS)

# A useful makefile conditional that is always false
AM_CONDITIONAL(ALWAYS_FALSE, false)

# We set the following variable so that we know later in AC_COIN_FINALIZE
# that we are in a project main directory
coin_projectdir=yes
]) # AC_COIN_PROJECTDIR_INIT

###########################################################################
#                          COIN_DEBUG_COMPILE                             #
###########################################################################

# enable the configure flags --enable-debug and --enable-debug-prjct
# (where prcjt is the name of the project in lower case) and set the
# variable coin_debug_compile to true or false This is used by
# COIN_PROG_CXX, COIN_PROG_CC and COIN_PROG_F77 to determine the
# compilation flags.  This macro also makes the switches
# --with-prjct-verbosity and --with-prjct-checklevel available, which
# define the preprocessor macros COIN_PRJCT_VERBOSITY and
# COIN_PRJCT_CHECKLEVEL to the specified value (default is 0).
#
# The project specific flags are only made available, if one gives the
# name of the project as first argument to this macro.

AC_DEFUN([AC_COIN_DEBUG_COMPILE],
[AC_BEFORE([$0],[AC_COIN_PROG_CXX])dnl
AC_BEFORE([$0],[AC_COIN_PROG_CC])dnl
AC_BEFORE([$0],[AC_COIN_PROG_F77])dnl

AC_MSG_CHECKING([whether we want to compile in debug mode])

AC_ARG_ENABLE([debug],
[AC_HELP_STRING([--enable-debug],
                [compile all projects with debug options tests])],
[case "${enableval}" in
   yes) coin_debug_compile=true
        if test "${enable_shared+set}" = set; then :; else
          enable_shared=no
        fi 
        ;;
   no)  coin_debug_compile=false
        ;;
   *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug)
        ;;
esac],
[coin_debug_compile=false])

m4_ifvaln([$1],
[AC_ARG_ENABLE(debug-m4_tolower($1),
 [AC_HELP_STRING([--enable-debug-m4_tolower($1)],
                 [compile this project ($1) with debug options])],
 [case "${enableval}" in
    yes) coin_debug_compile=true
         enable_shared=no
         ;;
    no)  coin_debug_compile=false
         ;;
    *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug-m4_tolower($1))
         ;;
 esac],[:])
]) # m4_ifvaln([$1],

if test $coin_debug_compile = true; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi

m4_ifvaln([$1],
[AC_ARG_WITH(m4_tolower($1)-verbosity,
             AC_HELP_STRING([--with-m4_tolower($1)-verbosity],
                            [specify the debug verbosity level for project $1]),
             [if test "$withval" = yes; then
                withval=1
              fi
              m4_tolower(coin_$1_verbosity)=$withval],
             [m4_tolower(coin_$1_verbosity)=0])
 AC_DEFINE_UNQUOTED(m4_toupper(COIN_$1_VERBOSITY),
                    m4_tolower($coin_$1_verbosity),
                    [Define to the debug verbosity level (0 is no output)])

 AC_ARG_WITH(m4_tolower($1)-checklevel,
             AC_HELP_STRING([--with-m4_tolower($1)-checklevel],
                            [specify the sanity check level for project $1]),
             [if test "$withval" = yes; then
                withval=1
              fi
              m4_tolower(coin_$1_checklevel)=$withval],
             [m4_tolower(coin_$1_checklevel)=0])
 AC_DEFINE_UNQUOTED(m4_toupper(COIN_$1_CHECKLEVEL),
                    m4_tolower($coin_$1_checklevel),
                    [Define to the debug sanity check level (0 is no test)])

# We use the following variable to have a string with the upper case
# version of the project name
COIN_PRJCT=m4_toupper($1)

]) # m4_ifvaln([$1],
 
]) # AC_COIN_DEBUG_COMPILE

###########################################################################
#                          COIN_MINGW_LD_FIX                              #
###########################################################################

# This macro is included by any PROG_compiler macro, to set the LD
# environment variable on MinGW to the correct value (link). But note that
# if we're building in cygwin with -mno-cygwin, we still want ld! If we're
# building from cygwin with MSVC tools (cl/link), then we do want link and
# you'd better have your PATH variable straight, else you'll be doing file
# links instead of code links! Arguably, LDFLAGS should include -mno-cygwin
# but in practice all linking seems to be handled through the compilers, so
# CFLAGS and CXXFLAGS suffice.

AC_DEFUN([AC_COIN_MINGW_LD_FIX],
[case $build in
  *-mingw*)
    if test "${LD+set}" = set; then :; else
      LD=link
    fi
    ;;
 esac
 case $enable_doscompile in
   msvc)
     if test "x${LD+set}" = xset; then :; else
       LD=link
     fi
     ;;
 esac
])

###########################################################################
#                        COIN_ENABLE_DOSCOMPILE                           #
###########################################################################

# This macro is invoked by any PROG_compiler macro to establish the
# --enable-doscompile option, used when one wants to compile an executable
# under Cygwin which also runs directly under DOS (without requiring
# Cygwin1.dll). The job of this macro is to make sure the option is correct and
# to set enable_doscompile. Legal values are mingw, msvc, and no (disabled).
# mingw: Fake mingw under cygwin, using GCC tools and -mno-cygwin. The most
#	 important thing here is that preprocess, compile, and link steps
#	 *all* see -mno-cygwin.
# msvc:  Assume the presence of cl/link. It's the user's responsibility to
#        make sure their PATH is correct. In particular, that MSVC link is
#	 found and not cygwin link (we want to do code linking, not file
#	 linking).
# It's the responsibility of individual PROG_compiler macros to ensure that
# they correctly set the compiler search list and preprocess, compile, and
# link flags. This is tied to compiler setup because in practice invocations
# of the preprocessor and linker are made through the compiler.

AC_DEFUN([AC_COIN_ENABLE_DOSCOMPILE],
[AC_REQUIRE([AC_CANONICAL_BUILD])
 AC_ARG_ENABLE([doscompile],
  [AC_HELP_STRING([--enable-doscompile],
                  [Under Cygwin, compile so that executables run under DOS.
		   Set to mingw to use gcc/g++/ld with -mno-cygwin.
		   Set to msvc to use cl/link (or icl/link).
		   Default when mentioned: mingw.
		   Default when not mentioned: disabled.])],
  [if test "$enable_doscompile" != no; then
     case $build in
       *-cygwin* | *-mingw*) ;;
       *) AC_MSG_ERROR([--enable-doscompile option makes sense only under Cygwin or MinGW]) ;;
     esac
   fi],
  [enable_doscompile=no])
 case "$enable_doscompile" in
   mingw)
     case $build in
       *-mingw*) enable_doscompile=no ;;
     esac
     ;;
   msvc|no) ;;
   yes) enable_doscompile=mingw ;;
   *) AC_MSG_ERROR([Invalid value $enable_doscompile for --enable-doscompile.
		    Try configure --help=recursive.])
      ;;
  esac
  if test "$enable_doscompile" != no ; then
    AC_MSG_NOTICE([DOS compile style is: $enable_doscompile])
  fi
])

###########################################################################
#                             COIN_PROG_CXX                               #
###########################################################################

# Find the compile command by running AC_PROG_CXX (with compiler names for
# different operating systems) and put it into CXX (unless it was given by the
# user). Then find an appropriate value for CXXFLAGS. If either of CXXFLAGS or
# PRJCT_CXXFLAGS is defined, that value is used (replace PRJCT with the upper
# case name of this project).  It is possible to provide additional -D flags
# in the variable CXXDEFS, and additional compilation flags with ADD_CXXFLAGS.

AC_DEFUN([AC_COIN_PROG_CXX],
[AC_REQUIRE([AC_COIN_PROG_CC]) #Let's try if that overcomes configuration problem with VC++ 6.0
AC_REQUIRE([AC_COIN_ENABLE_DOSCOMPILE])
AC_LANG_PUSH(C++)

AC_ARG_VAR(CXXDEFS,[Additional -D flags to be used when compiling C++ code.])
AC_ARG_VAR(ADD_CXXFLAGS,[Additional C++ compiler options])
AC_ARG_VAR(DBG_CXXFLAGS,[Debug C++ compiler options])
AC_ARG_VAR(OPT_CXXFLAGS,[Optimize C++ compiler options])

coin_has_cxx=yes

save_cxxflags="$CXXFLAGS"
# For *-*-solaris*, promote Studio/Workshop compiler to front of list.
case $build in
  *-cygwin* | *-mingw*)
  	     if test "$enable_doscompile" = msvc ; then
	       comps="icl cl"
	     else
	       comps="g++ cl"
	     fi ;;
  *-*-solaris*)
  	     comps="CC xlC_r aCC g++ c++ pgCC icpc gpp cxx cc++ cl FCC KCC RCC" ;;
  *-darwin*) comps="g++ c++ CC" ;;
          *) comps="xlC_r aCC CC g++ c++ pgCC icpc gpp cxx cc++ cl FCC KCC RCC" ;;
esac

# We delete the cached value, since the test might not have been
# performed with our choice of compilers earlier
$as_unset ac_cv_prog_CXX || test "${ac_cv_prog_CXX+set}" != set || { ac_cv_prog_CXX=; export ac_cv_prog_CXX; }
# AC_MSG_NOTICE([C++ compiler candidates: $comps])
AC_PROG_CXX([$comps])
if test -z "$CXX" ; then
  AC_MSG_ERROR([Failed to find a C++ compiler!])
fi

# It seems that we need to cleanup something here for the Windows 
case "$CXX" in
  cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
    sed -e 's/^void exit (int);//' confdefs.h >> confdefs.hh
    mv confdefs.hh confdefs.h
    ;;
esac

# Autoconf incorrectly concludes that cl recognises -g. It doesn't.
case "$CXX" in
  cl* | */cl* | CL* | */CL* )
    if test "$ac_cv_prog_cxx_g" = yes ; then
      ac_cv_prog_cxx_g=no
      AC_MSG_NOTICE([Overruling autoconf; cl does not recognise -g.])
    fi ;;
esac
CXXFLAGS="$save_cxxflags"

# Check if a project specific CXXFLAGS variable has been set
if test x$COIN_PRJCT != x; then
  eval coin_tmp=\${${COIN_PRJCT}_CXXFLAGS+set}
  if test x$coin_tmp = xset; then
    eval CXXFLAGS=\${${COIN_PRJCT}_CXXFLAGS}
  fi
fi

if test x"$CXXFLAGS" = x; then

# ToDo decide whether we want -DNDEBUG for optimization
  coin_add_cxxflags=
  coin_opt_cxxflags=
  coin_dbg_cxxflags=
  coin_warn_cxxflags=

  if test "$GXX" = "yes"; then
    case "$CXX" in
      icpc* | */icpc*)
        ;;
      *)
# ToDo decide about unroll-loops
        coin_opt_cxxflags="-O3 -fomit-frame-pointer"
        coin_add_cxxflags="-pipe"
        coin_dbg_cxxflags="-g"
        coin_warn_cxxflags="-Wimplicit -Wparentheses -Wreturn-type -Wcast-qual -Wall -Wpointer-arith -Wwrite-strings -Wconversion -Wno-unknown-pragmas"
        case $build in
          *-darwin*)
            ;;
          *)
            coin_warn_cxxflags="-pedantic-errors $coin_warn_cxxflags"
            ;;
        esac

        case $enable_doscompile in
          mingw)
            CXXFLAGS="-mno-cygwin"
            AC_TRY_LINK(,[int i=0; i++;],[coin_add_cxxflags="-mno-cygwin $coin_add_cxxflags"])
            CXXFLAGS=
          ;;
        esac
    esac
  fi

# Note that we do not need to cover GCC in the following tests.

  if test -z "$coin_opt_cxxflags"; then
    case $build in
      *-cygwin* | *-mingw*)
        case "$CXX" in
          cl* | */cl* | CL* | */CL*)
	    # The MT and MTd options are mutually exclusive
            coin_opt_cxxflags='-MT -O2'
            coin_add_cxxflags='-nologo -EHsc -GR -wd4996 -D_CRT_SECURE_NO_DEPRECATE'
            coin_dbg_cxxflags='-MTd'
            ;;
          icl* | */icl* | ICL* | */ICL*)
	    # The MT and MTd options are mutually exclusive
            coin_opt_cxxflags='-MT -Ox'
            coin_add_cxxflags='-nologo -EHsc -GR -D_CRT_SECURE_NO_DEPRECATE'
            coin_dbg_cxxflags='-MTd -debug'
            ;;
        esac
        ;;
      *-linux-*)
        case "$CXX" in
          icpc* | */icpc*)
            coin_opt_cxxflags="-O3 -ip -mp1"
            coin_add_cxxflags=""
            coin_dbg_cxxflags="-g"
            # Check if -i_dynamic is necessary (for new glibc library)
            CXXFLAGS=
            AC_TRY_LINK(,[int i=0; i++;],[],
                        [coin_add_cxxflags="-i_dynamic $coin_add_cxxflags"])
            ;;
          pgCC* | */pgCC*)
            coin_opt_cxxflags="-fast"
            coin_add_cxxflags="-Kieee -pc 64"
            coin_dbg_cxxflags="-g"
            ;;
        esac
        ;;
      *-ibm-*)
        case "$CXX" in
          xlC* | */xlC* | mpxlC* | */mpxlC*)
            coin_opt_cxxflags="-O -qarch=auto -qcache=auto -qtune=auto -qmaxmem=-1"
            coin_add_cxxflags="-bmaxdata:0x80000000 -qrtti=dyna -qsuppress=1500-036 -qsuppress=1500-029 -qsourcetype=c++"
            coin_dbg_cxxflags="-g"
            ;;
        esac
        ;;
      *-hp-*)
        case "$CXX" in
          aCC* | */aCC* )
            coin_opt_cxxflags="-O"
            coin_add_cxxflags="-AA"
            coin_dbg_cxxflags="-g"
            ;;
        esac
        ;;
      *-*-solaris*)
          coin_opt_cxxflags="-O4"
          coin_dbg_cxxflags="-g"
        ;;
    esac
  fi

# Generic flag settings. If these don't work, add a case above.

  if test "$ac_cv_prog_cxx_g" = yes && test -z "$coin_dbg_cxxflags" ; then
    coin_dbg_cxxflags="-g"
  fi

  if test -z "$coin_opt_cxxflags"; then
    # Try if -O option works if nothing else is set
    CXXFLAGS=-O
    AC_TRY_LINK([],[int i=0; i++;],[coin_opt_cxxflags="-O"])
  fi

  # if PM doesn't want the warning messages, take them out
  if test x"$coin_skip_warn_cxxflags" = xyes; then
    coin_warn_cxxflags=
  fi

# Do final setup of flags based on values determined above.

  if test x${DBG_CXXFLAGS+set} != xset; then
    DBG_CXXFLAGS="$coin_dbg_cxxflags $coin_add_cxxflags $coin_warn_cxxflags"
  fi
  if test x${OPT_CXXFLAGS+set} != xset; then
    OPT_CXXFLAGS="$coin_opt_cxxflags $coin_add_cxxflags -DNDEBUG $coin_warn_cxxflags"
  fi

  DBG_CXXFLAGS="$DBG_CXXFLAGS $ADD_CXXFLAGS $CXXDEFS"
  OPT_CXXFLAGS="$OPT_CXXFLAGS $ADD_CXXFLAGS $CXXDEFS"

  if test "$coin_debug_compile" = "true"; then
    CXXFLAGS="$DBG_CXXFLAGS"
  else
    CXXFLAGS="$OPT_CXXFLAGS"
  fi

# Handle the case where CXXFLAGS was set externally.
else
  CXXFLAGS="$CXXFLAGS $ADD_CXXFLAGS $CXXDEFS"
  if test x${DBG_CXXFLAGS+set} != xset; then
    DBG_CXXFLAGS="$CXXFLAGS"
  fi
  if test x${OPT_CXXFLAGS+set} != xset; then
    OPT_CXXFLAGS="$CXXFLAGS"
  fi
fi

# If CXXFLAGS contains -mno-cygwin, CPPFLAGS must also have it.

case "$CXXFLAGS" in
  *-mno-cygwin*)
    if test x${CPPFLAGS+set} != xset ; then
      CPPFLAGS="-mno-cygwin"
    else
      case "$CPPFLAGS" in
        *-mno-cygwin*)
	  ;;
	*)
	  CPPFLAGS="$CPPFLAGS -mno-cygwin"
	  ;;
      esac
    fi ;;
esac

# Try if CXXFLAGS works
save_CXXFLAGS="$CXXFLAGS"
AC_TRY_LINK([],[int i=0; i++;],[],[CXXFLAGS=])
if test -z "$CXXFLAGS"; then
  AC_MSG_WARN([The flags CXXFLAGS="$save_CXXFLAGS" do not work.  I will now just try '-O', but you might want to set CXXFLAGS manually.])
  CXXFLAGS='-O'
  AC_TRY_LINK([],[int i=0; i++;],[],[CXXFLAGS=])
  if test -z "$CXXFLAGS"; then
    AC_MSG_WARN([This value for CXXFLAGS does not work.  I will continue with empty CXXFLAGS, but you might want to set CXXFLAGS manually.])
  fi
fi

AC_MSG_NOTICE([C++ compiler options are: $CXXFLAGS])

AC_ARG_VAR(MPICXX,[C++ MPI Compiler])
if test x"$MPICXX" = x; then :; else
  AC_MSG_NOTICE([Will use MPI C++ compiler $MPICXX])
  CXX="$MPICXX"
fi

case "$CXX" in
  cl* | */cl* | CL* | */CL* )
    AC_COIN_MINGW_LD_FIX
    ;;
esac

AC_LANG_POP(C++)
]) # AC_COIN_PROG_CXX


###########################################################################
#                             COIN_CXXLIBS                                #
###########################################################################

# Determine the C++ runtime libraries required for linking a C++ library
# with a Fortran or C compiler.  The result is available in CXXLIBS.

AC_DEFUN([AC_COIN_CXXLIBS],
[AC_REQUIRE([AC_PROG_CXX])dnl
AC_LANG_PUSH(C++)
AC_ARG_VAR(CXXLIBS,[Libraries necessary for linking C++ code with Fortran compiler])
if test -z "$CXXLIBS"; then
  if test "$GXX" = "yes"; then
    case "$CXX" in
      icpc* | */icpc*)
        CXXLIBS="-lstdc++"
        ;;
      *)
        CXXLIBS="-lstdc++ -lm" # -lgcc"
        ;;
    esac
  else
    case $build in
     *-mingw32 | *-cygwin* )
      case "$CXX" in
      cl* | */cl* | CL* | */CL*)
        CXXLIBS=nothing;;
      esac;;
     *-linux-*)
      case "$CXX" in
      icpc* | */icpc*)
        CXXLIBS="-lstdc++"
             ;;
      pgCC* | */pgCC*)
        CXXLIBS="-lstd -lC -lc"
             ;;
      esac;;
    *-ibm-*)
      CXXLIBS="-lC -lc"
      ;;
    *-hp-*)
      CXXLIBS="-L/opt/aCC/lib -l++ -lstd_v2 -lCsup_v2 -lm -lcl -lc"
      ;;
    *-*-solaris*)
      CXXLIBS="-lCstd -lCrun"
    esac
  fi
fi
if test -z "$CXXLIBS"; then
  AC_MSG_WARN([Could not automatically determine CXXLIBS (C++ link libraries; necessary if main program is in Fortran or C).])
else
  AC_MSG_NOTICE([Assuming that CXXLIBS is \"$CXXLIBS\".])
fi
if test x"$CXXLIBS" = xnothing; then
  CXXLIBS=
fi
AC_LANG_POP(C++)
]) # AC_COIN_CXXLIBS

###########################################################################
#                           COIN_CHECK_HEADER                             #
###########################################################################

# This macro checks for a header file, but it does so without the
# standard header.  This avoids warning messages like:
#
# configure: WARNING: dlfcn.h: present but cannot be compiled
# configure: WARNING: dlfcn.h:     check for missing prerequisite headers?
# configure: WARNING: dlfcn.h: see the Autoconf documentation
# configure: WARNING: dlfcn.h:     section "Present But Cannot Be Compiled"
# configure: WARNING: dlfcn.h: proceeding with the preprocessor's result
# configure: WARNING: dlfcn.h: in the future, the compiler will take precedence

# My guess that the problem lay with CPPFLAGS seems to be correct. With this
# macro reduced to a direct call to AC_CHECK_HEADERS, there are no warnings
# now that CPPFLAGS contains -mno-cygwin when it needs it. -- lh, 061214 --

# AW 070609: I restored the previous version, since otherwise the warnings
# still pop up for the cl compiler

AC_DEFUN([AC_COIN_CHECK_HEADER],
[#if test x"$4" = x; then
#  hdr="#include <$1>"
#else
#  hdr="$4"
#fi
#AC_CHECK_HEADERS([$1],[$2],[$3],[$hdr])
AC_CHECK_HEADERS([$1],[$2],[$3],[$4])
]) # AC_COIN_CHECK_HEADER

###########################################################################
#                       COIN_CHECK_CXX_CHEADER                             #
###########################################################################

# This macro checks for C header files that are used from C++.  For a give
# stub (e.g., math), it first checks if the C++ library (cmath) is available.
# If it is, it defines HAVE_CMATH (or whatever the stub is).  If it is not
# available, it checks for the old C head (math.h) and defines HAVE_MATH_H
# if that one exists.

AC_DEFUN([AC_COIN_CHECK_CXX_CHEADER],
[AC_LANG_PUSH(C++)
AC_COIN_CHECK_HEADER([c$1],[$2],[$3],[$4])
if test "$ac_cv_header_c$1" != "yes"; then
  AC_COIN_CHECK_HEADER([$1.h],[$2],[$3],[$4])
fi
AC_LANG_POP(C++)
]) # AC_COIN_CHECK_CXX_CHEADER

###########################################################################
#                             COIN_PROG_CC                                #
###########################################################################

# Find the compile command by running AC_PROG_CC (with compiler names
# for different operating systems) and put it into CC (unless it was
# given my the user), and find an appropriate value for CFLAGS.  It is
# possible to provide additional -D flags in the variable CDEFS.

AC_DEFUN([AC_COIN_PROG_CC],
[AC_REQUIRE([AC_COIN_MINGW_LD_FIX])
AC_REQUIRE([AC_COIN_ENABLE_DOSCOMPILE])
AC_LANG_PUSH(C)

# For consistency, we set the C compiler to the same value of the C++
# compiler, if the C++ is set, but the C compiler isn't (only for CXX=cl)
if test x"$CXX" != x; then
  case "$CXX" in
    cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
      if test x"$CC" = x; then
        CC="$CXX"
        AC_MSG_WARN([C++ compiler name provided as $CXX, but CC is unset. Setting CC to $CXX])
      fi
      ;;
  esac
fi

AC_ARG_VAR(CDEFS,[Additional -D flags to be used when compiling C code.])
AC_ARG_VAR(ADD_CFLAGS,[Additional C compiler options])
AC_ARG_VAR(DBG_CFLAGS,[Debug C compiler options])
AC_ARG_VAR(OPT_CFLAGS,[Optimize C compiler options])

coin_has_cc=yes

save_cflags="$CFLAGS"

# For *-*-solaris*, promote Studio/Workshop cc compiler to front of list.
# Depending on the user's PATH, when Studio/Workshop cc is not present we may
# find /usr/ucb/cc, which is almost certainly not a good choice for the C
# compiler. In this case, put cc after gcc.

case $build in
  *-cygwin* | *-mingw*)
  	     if test "$enable_doscompile" = msvc ; then
	       comps="icl cl"
	     else
	       comps="gcc cl"
	     fi ;;
  *-*-solaris*)
	     AC_CHECK_PROG(sol_cc_compiler,cc,cc,[],[],/usr/ucb/cc)
	     if test "$sol_cc_compiler" = "cc" ; then
	       comps="cc xlc gcc pgcc icc"
	     else
	       comps="xlc gcc pgcc icc cc"
	     fi
	     ;;
  *-linux-*) comps="xlc gcc cc pgcc icc" ;;
  *)         comps="xlc_r xlc cc gcc pgcc icc" ;;
esac

# We delete the cached value, since the test might not have been
# performed with our choice of compilers earlier
$as_unset ac_cv_prog_CC || test "${ac_cv_prog_CC+set}" != set || { ac_cv_prog_CC=; export ac_cv_prog_CC; }
# AC_MSG_NOTICE([C compiler candidates: $comps])
AC_PROG_CC([$comps])
if test -z "$CC" ; then
  AC_MSG_ERROR([Failed to find a C compiler!])
fi
# Autoconf incorrectly concludes that cl recognises -g. It doesn't.
case "$CC" in
  cl* | */cl* | CL* | */CL* )
    if test "$ac_cv_prog_cc_g" = yes ; then
      ac_cv_prog_cc_g=no
      AC_MSG_NOTICE([Overruling autoconf; cl does not recognise -g.])
    fi ;;
esac
CFLAGS="$save_cflags"

# Check if a project specific CFLAGS variable has been set
if test x$COIN_PRJCT != x; then
  eval coin_tmp=\${${COIN_PRJCT}_CFLAGS+set}
  if test x$coin_tmp = xset; then
    eval CFLAGS=\${${COIN_PRJCT}_CFLAGS}
  fi
fi

if test x"$CFLAGS" = x; then

  coin_add_cflags=
  coin_opt_cflags=
  coin_dbg_cflags=
  coin_warn_cflags=

  if test "$GCC" = "yes"; then
    case "$CC" in
      icc* | */icc*)
        ;;
      *)
        coin_opt_cflags="-O3 -fomit-frame-pointer"
        coin_add_cflags="-pipe"
        coin_dbg_cflags="-g"
        coin_warn_cflags="-Wimplicit -Wparentheses -Wsequence-point -Wreturn-type -Wcast-qual -Wall -Wno-unknown-pragmas"
        case $build in
          *-darwin*)
            ;;
          *)
            coin_warn_cflags="-pedantic-errors $coin_warn_cflags"
            ;;
        esac
	case $enable_doscompile in
	  mingw)
	    CFLAGS="-mno-cygwin"
	    AC_TRY_LINK([],[int i=0; i++;],
			[coin_add_cflags="-mno-cygwin $coin_add_cflags"])
	    CFLAGS=
	  ;;
	esac
    esac
  fi
  if test -z "$coin_opt_cflags"; then
    case $build in
      *-cygwin* | *-mingw*)
        case "$CC" in
          cl* | */cl* | CL* | */CL*)
            coin_opt_cflags='-MT -O2'
            coin_add_cflags='-nologo -wd4996 -D_CRT_SECURE_NO_DEPRECATE'
            coin_dbg_cflags='-MTd'
            ;;
          icl* | */icl* | ICL* | */ICL*)
            coin_opt_cflags='-MT -Ox'
            coin_add_cflags='-nologo -D_CRT_SECURE_NO_DEPRECATE'
            coin_dbg_cflags='-MTd -debug'
            ;;
        esac
        ;;
      *-linux-*)
        case "$CC" in
          icc* | */icc*)
            coin_opt_cflags="-O3 -ip -mp1"
            coin_add_cflags=""
            coin_dbg_cflags="-g"
            # Check if -i_dynamic is necessary (for new glibc library)
            CFLAGS=
            AC_TRY_LINK([],[int i=0; i++;],[],
                        [coin_add_cflags="-i_dynamic $coin_add_cflags"])
            ;;
          pgcc* | */pgcc*)
            coin_opt_cflags="-fast"
            coin_add_cflags="-Kieee -pc 64"
            coin_dbg_cflags="-g"
            ;;
        esac
        ;;
      *-ibm-*)
        case "$CC" in
          xlc* | */xlc* | mpxlc* | */mpxlc*)
            coin_opt_cflags="-O -qarch=auto -qcache=auto -qtune=auto -qmaxmem=-1"
            coin_add_cflags="-bmaxdata:0x80000000 -qsuppress=1500-036 -qsuppress=1500-029"
            coin_dbg_cflags="-g"
          ;;
        esac
        ;;
      *-hp-*)
        coin_opt_cflags="-O"
        coin_add_cflags="-Ae"
        coin_dbg_cflags="-g"
        ;;
      *-*-solaris*)
        coin_opt_cflags="-xO4"
        coin_dbg_cflags="-g"
        ;;
      *-sgi-*)
        coin_opt_cflags="-O -OPT:Olimit=0"
        coin_dbg_cflags="-g"
        ;;
    esac
  fi

  if test "$ac_cv_prog_cc_g" = yes && test -z "$coin_dbg_cflags" ; then
    coin_dbg_cflags="-g"
  fi

  if test -z "$coin_opt_cflags"; then
    # Try if -O option works if nothing else is set
    CFLAGS="-O"
    AC_TRY_LINK([],[int i=0; i++;],[coin_opt_cflags="-O"])
  fi

  # if PM doesn't want the warning messages, take them out
  if test x"$coin_skip_warn_cflags" = xyes; then
    coin_warn_cflags=
  fi

  if test x${DBG_CFLAGS+set} != xset; then
    DBG_CFLAGS="$coin_dbg_cflags $coin_add_cflags $coin_warn_cflags"
  fi
  if test x${OPT_CFLAGS+set} != xset; then
    OPT_CFLAGS="$coin_opt_cflags $coin_add_cflags -DNDEBUG $coin_warn_cflags"
  fi

  DBG_CFLAGS="$DBG_CFLAGS $ADD_CFLAGS $CDEFS"
  OPT_CFLAGS="$OPT_CFLAGS $ADD_CFLAGS $CDEFS"

  if test "$coin_debug_compile" = "true"; then
    CFLAGS="$DBG_CFLAGS"
  else
    CFLAGS="$OPT_CFLAGS"
  fi
else
  CFLAGS="$CFLAGS $ADD_CFLAGS $CDEFS"
  if test x${DBG_CFLAGS+set} != xset; then
    DBG_CFLAGS="$CFLAGS"
  fi
  if test x${OPT_CFLAGS+set} != xset; then
    OPT_CFLAGS="$CFLAGS"
  fi
fi

# If CFLAGS contains -mno-cygwin, CPPFLAGS must also have it.

case "$CFLAGS" in
  *-mno-cygwin*)
    if test x${CPPFLAGS+set} != xset ; then
      CPPFLAGS="-mno-cygwin"
    else
      case "$CPPFLAGS" in
        *-mno-cygwin*)
	  ;;
	*)
	  CPPFLAGS="$CPPFLAGS -mno-cygwin"
	  ;;
      esac
    fi ;;
esac

# Try if CFLAGS works
save_CFLAGS="$CFLAGS"
AC_TRY_LINK([],[int i=0; i++;],[],[CFLAGS=])
if test -z "$CFLAGS"; then
  AC_MSG_WARN([The value CFLAGS="$save_CFLAGS" do not work.  I will now just try '-O', but you might want to set CFLAGS manually.])
  CFLAGS='-O'
  AC_TRY_LINK([],[int i=0; i++;],[],[CFLAGS=])
  if test -z "$CFLAGS"; then
    AC_MSG_WARN([This value for CFLAGS does not work.  I will continue with empty CFLAGS, but you might want to set CFLAGS manually.])
  fi
fi

AC_MSG_NOTICE([C compiler options are: $CFLAGS])

AC_ARG_VAR(MPICC,[C MPI Compiler])
if test x"$MPICC" = x; then :; else
  AC_MSG_NOTICE([Will use MPI C compiler $MPICC])
  CC="$MPICC"
fi

# Correct ADDLIBS initialization if we are using the MS compiler
case "$CC" in
  cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
    ADDLIBS=
    AC_COIN_MINGW_LD_FIX
    ;;
esac

AC_LANG_POP(C)
]) # AC_COIN_PROG_CC

###########################################################################
#                             COIN_PROG_F77                               #
###########################################################################

# Find the compile command by running AC_PROG_F77 (with compiler names
# for different operating systems) and put it into F77 (unless it was
# given by the user), and find an appropriate value for FFLAGS

AC_DEFUN([AC_COIN_PROG_F77],
[AC_REQUIRE([AC_COIN_MINGW_LD_FIX])
AC_REQUIRE([AC_COIN_ENABLE_DOSCOMPILE])
AC_REQUIRE([AC_COIN_PROG_CC])
AC_REQUIRE([AC_COIN_F77_COMPS])
AC_LANG_PUSH([Fortran 77])

AC_ARG_VAR(ADD_FFLAGS,[Additional Fortran compiler options])
AC_ARG_VAR(DBG_FFLAGS,[Debug Fortran compiler options])
AC_ARG_VAR(OPT_FFLAGS,[Optimize Fortran compiler options])

coin_has_f77=yes

save_fflags="$FFLAGS"

# We delete the cached value, since the test might not have been
# performed with our choice of compilers earlier
$as_unset ac_cv_prog_F77 || test "${ac_cv_prog_F77+set}" != set || { ac_cv_prog_F77=; export ac_cv_prog_F77; }

# This is a real belt-and-suspenders approach. AC_COIN_FIND_F77 will use
# coin_f77_comps to see if there's a program that matches one of the names.
# If there's no such program, F77 = unavailable. If we match the name,
# feed AC_PROG_F77 the same search list, just to be sure it's a functioning
# compiler.
# AC_MSG_NOTICE([Fortran compiler candidates: $coin_f77_comps])
AC_COIN_FIND_F77
if test "$F77" != "unavailable" ; then
  AC_PROG_F77($coin_f77_comps)
else
  AC_MSG_WARN([Failed to find a Fortran compiler!])
fi

FFLAGS="$save_fflags"

# Check if a project specific FFLAGS variable has been set
if test x$COIN_PRJCT != x; then
  eval coin_tmp=\${${COIN_PRJCT}_FFLAGS+set}
  if test x$coin_tmp = xset; then
    eval FFLAGS=\${${COIN_PRJCT}_FFLAGS}
  fi
fi

if test "$F77" != "unavailable" && test x"$FFLAGS" = x ; then

  coin_add_fflags=
  coin_opt_fflags=
  coin_dbg_fflags=
  coin_warn_fflags=

  if test "$G77" = "yes"; then
    coin_opt_fflags="-O3 -fomit-frame-pointer"
    coin_add_fflags="-pipe"
    coin_dbg_fflags="-g"
    case $enable_doscompile in
      mingw)
	FFLAGS="-mno-cygwin"
	AC_TRY_LINK(,[      write(*,*) 'Hello world'],
		    [coin_add_fflags="-mno-cygwin $coin_add_fflags"])
	FFLAGS=
      ;;
    esac
  else
    case $build in
      *-cygwin* | *-mingw*)
        case $F77 in
          ifort* | */ifort* | IFORT* | */IFORT* )
            coin_opt_fflags='-MT -O3'
            coin_add_fflags='-fpp -nologo'
            coin_dbg_fflags='-MTd -debug'
          ;;
          compile_f2c*)
            coin_opt_fflags='-MT -O2'
            coin_add_fflags='-nologo -wd4996'
            coin_dbg_fflags='-MTd'
          ;;
        esac
        ;;
      *-linux-*)
        case $F77 in
          ifc* | */ifc* | ifort* | */ifort*)
            coin_opt_fflags="-O3 -ip"
            coin_add_fflags="-cm -w90 -w95"
            coin_dbg_fflags="-g -CA -CB -CS"
            # Check if -i_dynamic is necessary (for new glibc library)
            FFLAGS=
            AC_TRY_LINK(,[      write(*,*) 'Hello world'],[],
                        [coin_add_fflags="-i_dynamic $coin_add_fflags"])
            ;;
          pgf77* | */pgf77* | pgf90* | */pgf90*)
            coin_opt_fflags="-fast"
            coin_add_fflags="-Kieee -pc 64"
            coin_dbg_fflags="-g"
          ;;
        esac
        ;;
      *-ibm-*)
        case "$F77" in
          xlf* | */xlf* | mpxlf* | */mpxlf* )
            coin_opt_fflags="-O -qarch=auto -qcache=auto -qtune=auto -qmaxmem=-1"
            coin_add_fflags="-bmaxdata:0x80000000 -qsuppress=1500-036 -qsuppress=1500-029"
            coin_dbg_fflags="-g -C"
            ;;
        esac
        ;;
      *-hp-*)
        coin_opt_fflags="+O3"
        coin_add_fflags="+U77"
        coin_dbg_fflags="-C -g"
        ;;
      *-*-solaris*)
        coin_opt_fflags="-O4"
        coin_dbg_fflags="-g"
        ;;
      *-sgi-*)
        coin_opt_fflags="-O5 -OPT:Olimit=0"
        coin_dbg_fflags="-g"
        ;;
    esac
  fi

  if test "$ac_cv_prog_f77_g" = yes && test -z "$coin_dbg_fflags" ; then
    coin_dbg_fflags="-g"
  fi

  if test -z "$coin_opt_fflags"; then
    # Try if -O option works if nothing else is set
    FFLAGS=-O
    AC_TRY_LINK(,[      integer i], [coin_opt_fflags="-O"])
  fi

  # if PM doesn't want the warning messages, take them out
  if test x"$coin_skip_warn_fflags" = xyes; then
    coin_warn_fflags=
  fi

  if test x${DBG_FFLAGS+set} != xset; then
    DBG_FFLAGS="$coin_dbg_fflags $coin_add_fflags $coin_warn_fflags"
  fi
  if test x${OPT_FFLAGS+set} != xset; then
    OPT_FFLAGS="$coin_opt_fflags $coin_add_fflags $coin_warn_fflags"
  fi

  DBG_FFLAGS="$DBG_FFLAGS $ADD_FFLAGS"
  OPT_FFLAGS="$OPT_FFLAGS $ADD_FFLAGS"

  if test "$coin_debug_compile" = "true"; then
    FFLAGS="$DBG_FFLAGS"
  else
    FFLAGS="$OPT_FFLAGS"
  fi
else
  FFLAGS="$FFLAGS $ADD_FFLAGS"
  if test x${DBG_FFLAGS+set} != xset; then
    DBG_FFLAGS="$FFLAGS"
  fi
  if test x${OPT_FFLAGS+set} != xset; then
    OPT_FFLAGS="$FFLAGS"
  fi
fi

# If FFLAGS contains -mno-cygwin, CPPFLAGS must have it.
case "$FFLAGS" in
  *-mno-cygwin*)
    if test x${CPPFLAGS+set} != xset ; then
      CPPFLAGS="-mno-cygwin"
    else
      case "$CPPFLAGS" in
        *-mno-cygwin*)
	  ;;
	*)
	  CPPFLAGS="$CPPFLAGS -mno-cygwin"
	  ;;
      esac
    fi ;;
esac

# Try if FFLAGS works
if test "$F77" != "unavailable" ; then
  AC_TRY_LINK(,[      integer i],[],[FFLAGS=])
  if test -z "$FFLAGS"; then
    AC_MSG_WARN([The flags FFLAGS="$FFLAGS" do not work.  I will now just try '-O', but you might want to set FFLAGS manually.])
    FFLAGS='-O'
    AC_TRY_LINK(,[      integer i],[],[FFLAGS=])
    if test -z "$FFLAGS"; then
      AC_MSG_WARN([This value for FFLAGS does not work.  I will continue with empty FFLAGS, but you might want to set FFLAGS manually.])
    fi
  fi
fi

AC_MSG_NOTICE([Fortran compiler options are: $FFLAGS])

AC_ARG_VAR(MPIF77,[Fortran MPI Compiler])
if test x"$MPIF77" = x; then :; else
  AC_MSG_NOTICE([Will use MPI Fortran compiler $MPIF77])
  F77="$MPIF77"
fi

case "$F77" in
  ifort* | */ifort* | IFORT* | */IFORT*)
    AC_COIN_MINGW_LD_FIX
    ;;
esac

AC_LANG_POP([Fortran 77])
]) # AC_COIN_PROG_F77

###########################################################################
#                           COIN_F77_WRAPPERS                             #
###########################################################################

# Calls autoconfs AC_F77_WRAPPERS and does additional corrections to FLIBS

AC_DEFUN([AC_COIN_F77_WRAPPERS],
[AC_BEFORE([AC_COIN_PROG_F77],[$0])dnl
AC_BEFORE([AC_PROG_F77],[$0])dnl

AC_LANG_PUSH([Fortran 77])

save_FLIBS="$FLIBS"

AC_F77_WRAPPERS

if test x"$save_FLIBS" != x; then
  FLIBS="$save_FLIBS"
else
  # This is to correct a missing exclusion in autoconf 2.59
  if test x"$FLIBS" != x; then
    my_flibs=
    for flag in $FLIBS; do
      case $flag in
        -lcrt*.o) ;;
               *) my_flibs="$my_flibs $flag" ;;
      esac
    done
    FLIBS="$my_flibs"
  fi

  case $build in
  # The following is a fix to define FLIBS for ifort on Windows
  # In its original version, it linked in libifcorert.lib or libifcorertd.lib on Windows/ifort explicitly.
  # However, this seem to create a dependency on libifcorert.dll (or libifcorertd.dll) in the executables.
  # This is seem to be unnecessary, libifcorert(d).lib has been removed from the link line.
     *-cygwin* | *-mingw*)
       case "$F77" in
         ifort* | */ifort* | IFORT* | */IFORT*)
           FLIBS="-link $LIBS /NODEFAULTLIB:libc.lib"
#           if "$coin_debug_compile" = true ; then
#             FLIBS="-link $LIBS /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib"
#           else
#             FLIBS="-link $LIBS /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmtd.lib"
#           fi
           ;;
         compile_f2c*)
           FLIBS=`$F77 -FLIBS` ;;
       esac;;
     *-hp-*)
         FLIBS="$FLIBS -lm";;
     *-ibm-*)
         FLIBS=`echo $FLIBS | sed 's/-lc)/-lc/g'` ;;
     *-linux-*)
       case "$F77" in
         pgf77* | */pgf77* | pgf90* | */pgf90*)
# ask linker to go through the archives multiple times
# (the Fortran compiler seems to do that automatically...
           FLIBS="-Wl,--start-group $FLIBS -Wl,--end-group" ;;
       esac
  esac
  ac_cv_f77_libs="$FLIBS"
fi

AC_LANG_POP([Fortran 77])

]) # AC_COIN_F77_WRAPPERS

###########################################################################
#                             COIN_FIND_F77                               #
###########################################################################

# Attempt to preempt autoconf by locating an appropriate F77 program. This
# macro will not trigger a fatal error if a suitable compiler cannot be
# found. It should be called before COIN_PROG_F77 or COIN_TRY_FLINK.

AC_DEFUN([AC_COIN_FIND_F77],
[AC_REQUIRE([AC_COIN_ENABLE_DOSCOMPILE])
AC_REQUIRE([AC_COIN_F77_COMPS])
AC_MSG_NOTICE([Trying to determine Fortran compiler name])
AC_CHECK_PROGS([F77],[$coin_f77_comps],[unavailable])
])

# Auxilliary macro to make sure both COIN_PROG_F77 and COIN_FIND_F77 use
# the same search lists for compiler names.
# For *-*-solaris*, promote Studio/Workshop compilers to front of list.
AC_DEFUN([AC_COIN_F77_COMPS],
[case $build in
  *-cygwin* | *-mingw*)
     if test "$enable_doscompile" = msvc ; then
       coin_f77_comps="ifort fl32 compile_f2c"
     else
       coin_f77_comps="gfortran g77 ifort fl32 compile_f2c"
     fi ;;
  *-*-solaris*)
     coin_f77_comps="f95 f90 f77 xlf_r fort77 gfortran g77 pgf90 pgf77 ifort ifc frt af77" ;;
  *) coin_f77_comps="xlf_r fort77 gfortran f77 g77 pgf90 pgf77 ifort ifc frt af77" ;;
 esac
])

###########################################################################
#                          COIN_INIT_AUTOMAKE                             #
###########################################################################

# This macro calls the regular INIT_AUTOMAKE and MAINTAINER_MODE
# macros, and defines additional variables and makefile conditionals,
# that are used in the maintainer parts of the makfile.  It also
# checks if the correct versions of the autotools are used.
#
# This also defines the AC_SUBST variables:
# abs_source_dir     absolute path to source code for this package
# abs_bin_dir        absolute path to the directory where binaries are
#                    going to be installed (prefix/bin)
# abs_lib_dir        absolute path to the directory where libraries are
#                    going to be installed (prefix/lib)
# abs_include_dir    absolute path to the directory where the header files
#                    are installed (prefix/include)

AC_DEFUN([AC_COIN_INIT_AUTOMAKE],
[AC_REQUIRE([AC_PROG_EGREP])

# AC_MSG_NOTICE([Beginning automake initialisation.])
# Stuff for automake
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

coin_have_externals=no
if test "$enable_maintainer_mode" = yes; then

  # If maintainer mode is chosen, we make sure that the correct versions
  # of the tools are used, and that we know where libtool.m4 is (to
  # recreate acinclude.m4)

  AC_SUBST(LIBTOOLM4)
  LIBTOOLM4=
  # Normally, $HOME
  AUTOTOOLS_DFLT=$HOME

  AC_CACHE_CHECK([whether we are using the correct autotools],
                 [ac_cv_use_correct_autotools],
                 [ac_cv_use_correct_autotools=check])

  if test $ac_cv_use_correct_autotools = check; then
    ac_cv_use_correct_autotools=yes
    # Check if we have autoconf
    AC_CHECK_PROG([have_autoconf],[autoconf],[yes],[no])
    if test $have_autoconf = no; then
      AC_MSG_ERROR([You specified you want to use maintainer mode, but I cannot find autoconf in your path.])
    fi

    # Check whether autoconf is the correct version
    correct_version='2.59'
    grep_version=`echo  $correct_version | sed -e 's/\\./\\\\\\./g'`
    AC_MSG_CHECKING([whether we are using the correct version ($correct_version) of autoconf])
    autoconf --version > confauto.out 2>&1
    if $EGREP $grep_version confauto.out >/dev/null 2>&1; then
      AC_MSG_RESULT([yes])
    else
      rm -f confauto.out
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([You don't have the correct version of autoconf as the first one in your path.])
    fi
    rm -f confauto.out

    # Check if the executable autoconf is picked up from the correct location
    AC_MSG_CHECKING([whether autoconf is coming from the correct location])
    autoconf_dir=`which autoconf | sed -e 's=/autoconf=='`
    autoconf_dir=`cd $autoconf_dir; pwd`
    if test x$AUTOTOOLS_DIR = x; then
      want_dir=$AUTOTOOLS_DFLT/bin
    else
      want_dir=$AUTOTOOLS_DIR/bin
    fi
    if test $autoconf_dir = `cd $want_dir; pwd`; then
      AC_MSG_RESULT([yes])
    else
      rm -f confauto.out
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([The autoconf executable should be picked up from \$AUTOTOOLS_DFLT/bin or \$AUTOTOOLS_DIR/bin.])
    fi

    # Check if we have automake
    AC_CHECK_PROG([have_automake],[automake],[yes],[no])
    if test $have_automake = no; then
      AC_MSG_ERROR([You specified you want to use maintainer mode, but I cannot find automake in your path.])
    fi
  
    # Check whether automake is the correct version
    correct_version='1.9.6'
    grep_version=`echo  $correct_version | sed -e 's/\\./\\\\\\./g'`
    AC_MSG_CHECKING([whether we are using the correct version ($correct_version) of automake])
    automake --version > confauto.out 2>&1
    if $EGREP $grep_version confauto.out >/dev/null 2>&1; then
      AC_MSG_RESULT([yes])
    else
      rm -f confauto.out
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([You don't have the correct version of automake as the first one in your path.])
    fi
    rm -f confauto.out

    # Check if the executable automake is picked up from the correct location
    AC_MSG_CHECKING([whether automake is coming from the correct location])
    automake_dir=`which automake | sed -e 's=/automake=='`
    automake_dir=`cd $automake_dir; pwd`
    if test x$AUTOTOOLS_DIR = x; then
      want_dir=$AUTOTOOLS_DFLT/bin
    else
      want_dir=$AUTOTOOLS_DIR/bin
    fi
    if test $automake_dir = `cd $want_dir; pwd`; then
      AC_MSG_RESULT([yes])
    else
      rm -f confauto.out
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([The automake executable should be picked up from \$AUTOTOOLS_DFLT/bin or \$AUTOTOOLS_DIR/bin.])
    fi

    # Check if this is the correct version of libtool (with escaped dots)
    if test x$AUTOTOOLS_DIR = x; then
      want_dir=$AUTOTOOLS_DFLT/share
    else
      want_dir=$AUTOTOOLS_DIR/share
    fi
    correct_version='1.5.22'
    grep_version=`echo  $correct_version | sed -e 's/\\./\\\\\\./g'`
    AC_COIN_CHECK_FILE([$want_dir/libtool/ltmain.sh],
	               [have_ltmain=yes],
                       [have_ltmain=no])
    AC_MSG_CHECKING([whether we are using the correct version ($correct_version) of libtool.])
    if test $have_ltmain = yes; then
    if $EGREP $grep_version $want_dir/libtool/ltmain.sh >/dev/null 2>&1; then
        AC_MSG_RESULT([yes])
      else
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([You don't have the correct version of libtool.])
      fi
    else
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([I cannot find the ltmain.sh file.])
    fi
  fi

  # Check if we can find the libtool file
  if test x$AUTOTOOLS_DIR = x; then
    want_dir=$AUTOTOOLS_DFLT/share
  else
    want_dir=$AUTOTOOLS_DIR/share
  fi
  AC_COIN_CHECK_FILE([$want_dir/aclocal/libtool.m4],
                     [LIBTOOLM4="$want_dir/aclocal/libtool.m4"],
                     [AC_MSG_ERROR([I cannot find the libtool.m4 file.])])

  # Check if we have an Externals file
  if test -r $srcdir/Externals; then
    coin_have_externals=yes
  fi
  # Check if subversion is installed and understands https
  AC_CHECK_PROG([have_svn],[svn],[yes],[no])
  if test x$have_svn = xyes; then
    AC_CACHE_CHECK([whether svn understands https],
                   [ac_cv_svn_understands_https],
                   [svn --version > confauto.out 2>&1
                    if $EGREP https confauto.out >/dev/null 2>&1; then
                      ac_cv_svn_understands_https=yes
                    else
                      ac_cv_svn_understands_https=no
                      have_svn=no
                      ac_cv_prog_have_svn=no
                    fi
                    rm -f confauto.out])
  fi

  # Find the location of the BuildTools directory
  BUILDTOOLSDIR=
  if test -r $srcdir/BuildTools/coin.m4; then
    BUILDTOOLSDIR=$srcdir/BuildTools
  else
    if test -r $srcdir/../BuildTools/coin.m4; then
      BUILDTOOLSDIR=$srcdir/../BuildTools
    else
      if test -r $srcdir/../../BuildTools/coin.m4; then
        BUILDTOOLSDIR=$srcdir/../../BuildTools
      else
        AC_MSG_ERROR(Cannot find the BuildTools directory)
      fi
    fi
  fi
  AC_SUBST(BUILDTOOLSDIR)

  # The following variable is set to the name of the directory where
  # the autotool scripts are located
  AC_SUBST(AUX_DIR)
  AUX_DIR=$ac_aux_dir
fi

# helpful variable for the base directory of this package
abs_source_dir=`cd $srcdir; pwd`
AC_SUBST(abs_source_dir)

# Stuff for example Makefiles
if test x$prefix = xNONE; then
  full_prefix=$ac_default_prefix
else
  full_prefix=$prefix
fi
full_prefix=`cd $full_prefix ; pwd`
AC_SUBST(abs_lib_dir)
abs_lib_dir=$full_prefix/lib
AC_SUBST(abs_include_dir)
abs_include_dir=$full_prefix/include
AC_SUBST(abs_bin_dir)
abs_bin_dir=$full_prefix/bin

AM_CONDITIONAL(HAVE_EXTERNALS,
               test $coin_have_externals = yes && test x$have_svn = xyes)

# AC_MSG_NOTICE([End automake initialisation.])

]) # AC_COIN_INIT_AUTOMAKE

###########################################################################
#                          COIN_CREATE_LIBTOOL                            #
###########################################################################

# This does all the tests necessary to create the libtool script in the
# package base directory.  If this is used, then the COIN_INIT_AUTO_TOOLS
# test in the subdirectories will be able to skip the libtool tests and
# just use the one in the package base directory.

m4_define([AC_COIN_CREATE_LIBTOOL],
[AC_CANONICAL_BUILD

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE

# Get the name of the C compiler and appropriate compiler options
AC_COIN_PROG_CC

# Get the name of the C++ compiler and appropriate compiler options
AC_COIN_PROG_CXX

# Get the name of the Fortran compiler and appropriate compiler options
AC_COIN_PROG_F77

# Initialize automake and libtool
# AC_MSG_NOTICE([Calling INIT_AUTO_TOOLS from CREATE_LIBTOOL.])
AC_COIN_INIT_AUTO_TOOLS
# AC_MSG_NOTICE([Finished INIT_AUTO_TOOLS from CREATE_LIBTOOL.])
])

###########################################################################
#                         COIN_INIT_AUTO_TOOLS                            #
###########################################################################

# Initialize the auto tools automake and libtool, with all
# modifications we want for COIN packages.
#
# RPATH_FLAGS        link flags for hardcoding path to shared objects

# This is a trick to have this code before AC_COIN_PROG_LIBTOOL
AC_DEFUN([AC_COIN_DISABLE_STATIC],
[
# Test if force_shared has been set
if test "x$1" = xforce_shared; then
  if test x$enable_shared = xno; then
    AC_MSG_ERROR([Shared libraries are disabled by user, but this is not feasible with the given options])
  fi
  enable_shared=yes;
else
  # On Cygwin and AIX, building DLLs doesn't work
  case $build in
    *-cygwin*)
      coin_disable_shared=yes
      platform=Cygwin
    ;;
    *-aix*)
      coin_disable_shared=yes
      platform=AIX
    ;;
    *-mingw*)
      coin_disable_shared=yes
      platform="Msys"
#      case "$CXX" in
#        cl*)
#          coin_disable_shared=yes
#          platform="Msys with cl"
#      ;;
#      esac
    ;;
  esac
fi
if test x"$coin_disable_shared" = xyes; then
  if test x"$enable_shared" = xyes; then
    AC_MSG_WARN([On $platform, shared objects are not supported. I'm disabling your choice.])
  fi
  enable_shared=no
fi
# By default, we only want the shared objects to be compiled
AC_DISABLE_STATIC
])

m4_define([AC_COIN_INIT_AUTO_TOOLS],
[{AC_BEFORE([AC_COIN_PROG_CXX],[$0])
AC_BEFORE([AC_COIN_PROG_CC],[$0])
AC_BEFORE([AC_COIN_PROG_F77],[$0])

# START
AC_COIN_DISABLE_STATIC([$1])

# Initialize automake
AC_COIN_INIT_AUTOMAKE

LIBTOOL=
if test -r ../libtool; then
  coin_config_dir=..
  LIBTOOL='$(SHELL) $(top_builddir)/../libtool'
fi
if test "x$LIBTOOL" = x; then
  if test -r ../../libtool; then
    coin_config_dir=../..
    LIBTOOL='$(SHELL) $(top_builddir)/../../libtool'
  fi
fi

if test "x$LIBTOOL" = x; then
# AC_MSG_NOTICE([Creating libtool script (calling COIN_PROG_LIBTOOL).])
  # Stuff for libtool
  AC_COIN_PROG_LIBTOOL
# AC_MSG_NOTICE([Finished COIN_PROG_LIBTOOL.])
  # set RPATH_FLAGS to the compiler link flags required to hardcode location
  # of the shared objects
  AC_COIN_RPATH_FLAGS($abs_lib_dir)

else

  AC_MSG_NOTICE([Using libtool script in directory $coin_config_dir])
  # get all missing information from the config.log file 

  # output variables and defines
  as_save_IFS=$IFS
  IFS='
'
  for oneline in `cat $coin_config_dir/config.status`; do
    case "$oneline" in
         # First some automake conditionals
      s,@am__fastdep* | s,@AR@* | s,@CPP@*  | s,@CPPFLAGS@* | s,@CXXCPP@*  | \
      s,@RANLIB@* | s,@STRIP@* | s,@ac_ct_AR@* | s,@ac_ct_RANLIB@* | \
      s,@ac_ct_STRIP@* | s,@host* | s,@LN_S@* | s,@RPATH_FLAGS@* | \
      s,@ac_c_preproc_warn_flag@* |  s,@ac_cxx_preproc_warn_flag@* ) 
        command=`echo $oneline | sed -e 's/^s,@//' -e 's/@,/="/' -e 's/,;t t/"/'`
#        echo "$command"
        eval "$command"
        ;;
      s,@DEFS@* )
        command=`echo $oneline | sed -e 's/^s,@DEFS@,/defsline="/' -e 's/,;t t/"/'`
#        echo "$command"
        eval "$command"
        ;;
    esac
  done
  IFS=$as_save_IFS

  # And some defines (assuming here that the packages base dir
  # doesn't have a config.h file
  for word in $defsline; do
#    echo word $word 
    case $word in
      -DHAVE_@<:@A-Z_@:>@*_H=1 | -DSTDC_HEADERS=1 )
        i=`echo $word | sed -e 's/-D/#define /' -e 's/=/ /'`
#        echo dd $i
        echo $i >>confdefs.h
        ;;
    esac
  done
fi

# AC_MSG_NOTICE([End of INIT_AUTO_TOOLS.])

# ToDo
# For now, don't use the -no-undefined flag, since the Makefiles are
# not yet set up that way.  But we need to fix this, when we want
# to comile DLLs under Windows.
LT_LDFLAGS=
AC_SUBST(LT_LDFLAGS)

#END
}])


###########################################################################
#                      COIN_PATCH_LIBTOOL_CYGWIN                          #
###########################################################################

# Patches to libtool for cygwin. Lots for cl, a few for GCC.
# For cl:
# - cygpath is not correctly quoted in fix_srcfile_path
# - paths generated for .lib files is not run through cygpath -w


AC_DEFUN([AC_COIN_PATCH_LIBTOOL_CYGWIN],
[ case "$CXX" in
    cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*) 
      AC_MSG_NOTICE(Applying patches to libtool for cl compiler)
      sed -e 's|fix_srcfile_path=\"`cygpath -w \"\$srcfile\"`\"|fix_srcfile_path=\"\\\`'"$CYGPATH_W"' \\\"\\$srcfile\\\"\\\`\"|' \
	  -e 's|fix_srcfile_path=\"\"|fix_srcfile_path=\"\\\`'"$CYGPATH_W"' \\\"\\$srcfile\\\"\\\`\"|' \
	  -e 's%compile_deplibs=\"\$dir/\$old_library \$compile_deplibs\"%compile_deplibs="'\`"$CYGPATH_W"' \$dir/\$old_library | sed -e '"'"'sY\\\\\\\\Y/Yg'"'"\`' \$compile_deplibs\"'% \
	  -e 's%compile_deplibs=\"\$dir/\$linklib \$compile_deplibs\"%compile_deplibs="'\`"$CYGPATH_W"' \$dir/\$linklib | sed -e '"'"'sY\\\\\\\\Y/Yg'"'"\`' \$compile_deplibs\"'% \
	  -e 's%lib /OUT:%lib -OUT:%' \
	  -e "s%cygpath -w%$CYGPATH_W%" \
	  -e 's%$AR x \\$f_ex_an_ar_oldlib%bla=\\`lib -nologo -list \\$f_ex_an_ar_oldlib | xargs echo '"$mydos2unix"'\\`; echo \\$bla; for i in \\$bla; do lib -nologo -extract:\\$i \\$f_ex_an_ar_oldlib; done%' \
	  -e 's/$AR t/lib -nologo -list/' \
	  -e 's%f_ex_an_ar_oldlib="\($?*1*\)"%f_ex_an_ar_oldlib='\`"$CYGPATH_W"' \1`%' \ 
	  -e 's%^archive_cmds=.*%archive_cmds="\\$CC -o \\$lib \\$libobjs \\$compiler_flags \\\\\\`echo \\\\\\"\\$deplibs\\\\\\" | \\$SED -e '"\'"'s/ -lc\\$//'"\'"'\\\\\\` -link -dll~linknames="%' \
	  -e 's%old_archive_cmds="lib -OUT:\\$oldlib\\$oldobjs\\$old_deplibs"%old_archive_cmds="if test -r \\$oldlib; then bla=\\"\\$oldlib\\"; else bla=; fi; lib -OUT:\\$oldlib \\\\\\$bla\\$oldobjs\\$old_deplibs"%' \
      libtool > conftest.bla

      mv conftest.bla libtool
      chmod 755 libtool
      ;;
    *)
      AC_MSG_NOTICE(Applying patches to libtool for GNU compiler)
      sed -e 's|fix_srcfile_path=\"`cygpath -w \"\$srcfile\"`\"|fix_srcfile_path=\"\\\`'"$CYGPATH_W"' \\\"\\$srcfile\\\"\\\`\"|' \
	  -e 's|"lib /OUT:\\$oldlib\\$oldobjs\\$old_deplibs"|"\\$AR \\$AR_FLAGS \\$oldlib\\$oldobjs\\$old_deplibs~\\$RANLIB \\$oldlib"|' \
	  -e 's|libext="lib"|libext="a"|' \
      libtool > conftest.bla

      mv conftest.bla libtool
      chmod 755 libtool
      ;;
  esac ]) # COIN_PATCH_LIBTOOL_CYGWIN

###########################################################################
#                    COIN_PATCH_LIBTOOL_SOLARIS                           #
###########################################################################
# If we want to do a 64-bit build with GCC on Solaris, the system search
# libraries need to point to 64-bit subdirectories. If they do not already do
# that, fix them. This patch is evolving, as are GCC compilers.  GCC 4.2.1
# reports the correct search list, given the correct call. GCC 4.1.1 does not.
# `Correct call' means -m64 is specified. `Correct search list' seems to amount
# to prepending the list of 64-bit subdirectories to the 32-bit directories.
# Both SPARC and x86 have this issue, but a different hardware id string is
# required depending on the underlying CPU. The macro executes isainfo to get
# the string. This will fail, of course, if we're cross-compiling. The
# alternative is to fail on a regular basis each time a new CPU identifier is
# needed. This macro will also fail if the search list reported with
# -print-search-dirs differs between the C, C++, and Fortran compilers; each
# have their own setting in libtool.  If GCC reports the correct search list
# given the -m64 flag, the best solution is to define CC='gcc -m64', and
# similarly for CXX, F77, so that libtool will make the correct call.
###########################################################################
AC_DEFUN([AC_COIN_PATCH_LIBTOOL_SOLARIS],
[ if test "$GCC" = yes && \
     (echo $CXXFLAGS $CFLAGS $FFLAGS | $EGREP 'm64' >/dev/null 2>&1) ; then
    hdwisa=`isainfo | sed -e 's/\(@<:@^ @:>@*\) .*$/\1/'`
    if `$EGREP 'sys_lib_search_path_spec=' libtool | $EGREP -v $hdwisa >/dev/null 2>&1` ; then
      AC_MSG_NOTICE([Applying patches to libtool for 64-bit GCC compilation])
      fixlibtmp=`$CC -m64 -print-search-dirs | $EGREP '^libraries:'`
      fixlibtmp=`echo $fixlibtmp | sed -e 's/libraries: =//' -e 's/:/ /g'`
      if `echo "$fixlibtmp" | $EGREP -v $hdwisa  >/dev/null 2>&1` ; then
	# AC_MSG_NOTICE(Compensating for broken gcc)
	for lib in $fixlibtmp ; do
	  if test -d "${lib}${hdwisa}" ; then
	    syslibpath64="$syslibpath64 ${lib}${hdwisa}/"
	  fi
	done
	syslibpath64="${syslibpath64} ${fixlibtmp}"
      else
	syslibpath64="$fixlibtmp"
      fi
      sed -e 's|sys_lib_search_path_spec=".*"|sys_lib_search_path_spec="'"$syslibpath64"'"|' libtool > conftest.bla
      mv conftest.bla libtool
      chmod 755 libtool  
    fi
    # AC_MSG_NOTICE(Result is )
    # $EGREP 'sys_lib_search_path_spec=' libtool
  fi ])	# COIN_PATCH_LIBTOOL_SOLARIS

###########################################################################
#                           COIN_PROG_LIBTOOL                             #
###########################################################################

# Setup the libtool stuff together with any modifications to make it
# work on additional platforms

AC_DEFUN([AC_COIN_LIBTOOL_WRAPPER],
[AC_BEFORE([AC_COIN_BLA],[$0])
AC_PROG_LIBTOOL])

AC_DEFUN([AC_COIN_BLA],[echo bla])

AC_DEFUN([AC_COIN_PROG_LIBTOOL],
[# No longer needed now that CPPFLAGS is correctly set -- lh, 061214 --
 # AC_REQUIRE([AC_COIN_DLFCN_H])

# NEW: If libtool exists in the directory higher up, we use that one
#      instead of creating a new one

# It turns out that the code for AC_PROG_LIBTOOL is somehow AC_REQUIRED
# out in front of this macro body. You'll notice that LIBTOOL is already
# defined here.  We'll have to count on this macro not being called if libtool
# already exists, or at least move the libtool fixes outside the conditional.
# AC_MSG_NOTICE([Entering coin_prog_libtool, LIBTOOL = "$LIBTOOL".])
# This test is therefore removed.  -- lh, 061214 --
# if test "x$LIBTOOL" = x; then

# AC_MSG_NOTICE([Calling PROG_LIBTOOL.])
  AC_PROG_LIBTOOL
# AC_MSG_NOTICE([Finished PROG_LIBTOOL.])
  AC_SUBST(ac_c_preproc_warn_flag)
  AC_SUBST(ac_cxx_preproc_warn_flag)

  AC_MSG_NOTICE([Build is "$build".])
  mydos2unix='| dos2unix'
  case $build in
    *-mingw*)
      CYGPATH_W=echo
      mydos2unix=
      ;;
  esac

  case $build in
    # Here we need to check if -m32 is specified.  If so, we need to correct
    # sys_lib_search_path_spec
    *x86_64-*)
      if test "$GCC" = yes && (echo $CXXFLAGS $CFLAGS $FFLAGS | $EGREP 'm32' >& /dev/null); then 
        AC_MSG_NOTICE(Applying patches to libtool for 32bit compilation)
        sed -e 's|sys_lib_search_path_spec=".*"|sys_lib_search_path_spec="/lib /usr/lib"|' libtool > conftest.bla
        mv conftest.bla libtool
        chmod 755 libtool  
      fi
      ;;

    *-solaris*)
      AC_COIN_PATCH_LIBTOOL_SOLARIS
      ;;
    # Cygwin. Ah, cygwin. Too big and ugly to inline; see the macro.
    *-cygwin* | *-mingw*)
      AC_COIN_PATCH_LIBTOOL_CYGWIN
      ;;
    *-darwin*)
      AC_MSG_NOTICE(Applying patches to libtool for Darwin)
      sed -e 's/verstring="${wl}-compatibility_version ${wl}$minor_current ${wl}-current_version ${wl}$minor_current.$revision"/verstring="-compatibility_version $minor_current -current_version $minor_current.$revision"/' \
        -e 's/ -dynamiclib / -dynamiclib -single_module /g' \
      libtool > conftest.bla

      mv conftest.bla libtool
      chmod 755 libtool
      ;;
  esac
# This fi matches the commented `if test "x$LIBTOOL" = x;' up at the head of
# the macro. -- lh, 061214 --
# fi

# AC_MSG_NOTICE([End libtool initialisation.])
]) # AC_COIN_PROG_LIBTOOL

# This is a trick to force the check for the dlfcn header to be done before
# the checks for libtool
# No longer needed now that CPPFLAGS is correctly set.  -- lh, 061214 --
# ACDEFUN([AC_COIN_DLFCN_H],
# [AC_LANG_PUSH(C)
# AC_COIN_CHECK_HEADER([dlfcn.h])
# AC_LANG_POP(C)
# ]) # AC_COIN_DLFCN_H

###########################################################################
#                            COIN_RPATH_FLAGS                             #
###########################################################################

# This macro, in case shared objects are used, defines a variable
# RPATH_FLAGS that can be used by the linker to hardwire the library
# search path for the given directories.  This is useful for example
# Makefiles

AC_DEFUN([AC_COIN_RPATH_FLAGS],
[RPATH_FLAGS=

if test $enable_shared = yes; then
  case $build in
    *-linux-*)
      if test "$GXX" = "yes"; then
        RPATH_FLAGS=
        for dir in $1; do
          RPATH_FLAGS="$RPATH_FLAGS -Wl,--rpath -Wl,$dir"
        done
      fi ;;
    *-darwin*)
        RPATH_FLAGS=nothing ;;
    *-ibm-*)
      case "$CXX" in
      xlC* | */xlC* | mpxlC* | */mpxlC*)
        RPATH_FLAGS=nothing ;;
      esac ;;
    *-hp-*)
        RPATH_FLAGS=nothing ;;
    *-mingw32)
        RPATH_FLAGS=nothing ;;
    *-*-solaris*)
        RPATH_FLAGS=
        for dir in $1; do
          RPATH_FLAGS="$RPATH_FLAGS -R$dir"
        done
  esac

  if test "$RPATH_FLAGS" = ""; then
    AC_MSG_WARN([Could not automatically determine how to tell the linker about automatic inclusion of the path for shared libraries.  The test examples might not work if you link against shared objects.  You will need to set the LD_LIBRARY_PATH, DYLP_LIBRARY_PATH, or LIBDIR variable manually.])
  fi
  if test "$RPATH_FLAGS" = "nothing"; then
    RPATH_FLAGS=
  fi
fi

AC_SUBST(RPATH_FLAGS)
]) # AC_COIN_RPATH_FLAGS

###########################################################################
#                        COIN_LINK_INPUT_CMD                              #
###########################################################################

# This macro determines which command should be used to "link" files
# that are input to the generated executables.  On Windows, the codes
# using the native Windows system libraries, cannot understand symbolic
# links, and a copy should be used instead of 'ln -s'.
# The result is stored in coin_link_input_cmd

AC_DEFUN([AC_COIN_LINK_INPUT_CMD],
[AC_REQUIRE([AC_PROG_LN_S])
AC_BEFORE([AC_COIN_PROG_CC], [$0])
AC_BEFORE([AC_COIN_ENABLE_DOSCOMPILE], [$0])

AC_MSG_CHECKING([which command should be used to link input files])
coin_link_input_cmd="$LN_S"
if test "$enable_doscompile" = mingw; then
  coin_link_input_cmd=cp
fi
case "$CC" in
  cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
    coin_link_input_cmd=cp ;;
esac
AC_MSG_RESULT($coin_link_input_cmd)
])

###########################################################################
#                              COIN_FINALIZE                              #
###########################################################################

# This macro should be called at the very end of the configure.ac file.
# It creates the output files (by using AC_OUTPUT), and might do some other
# things (such as generating links to data files in a VPATH configuration).
# It also prints the "success" message.
# Note: If the variable coin_skip_ac_output is set to yes, then no output
# files are written.

AC_DEFUN([AC_COIN_FINALIZE],
[
AC_REQUIRE([AC_COIN_LINK_INPUT_CMD])
if test x$coin_skip_ac_output != xyes; then

  FADDLIBS="$ADDLIBS"
  if test x"$coin_need_flibs" = xyes; then
    ADDLIBS="$ADDLIBS $FLIBS"
  fi

  # library extension
  AC_SUBST(LIBEXT)
  case "$CC" in
    cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
         LIBEXT=lib ;;
      *) LIBEXT=a ;;
  esac

  # Define VPATH_DISTCLEANFILES to be everything that needs to be
  # cleaned for distclean in a vpath configuration
  AC_SUBST(VPATH_DISTCLEANFILES)
  VPATH_DISTCLEANFILES="$coin_vpath_link_files"

  # Take out subdirectories if their configuration concluded that they
  # don't need to be compiled
  if test x"$coin_ac_skip_subdirs" != x; then
    new_subdirs=
    for i in $subdirs; do
      skipme=no
      for j in $coin_ac_skip_subdirs; do
        if test $i = $j; then
          skipme=yes;
        fi
      done
      if test $skipme = no; then
        new_subdirs="$new_subdirs $i"
      fi
    done
    subdirs="$new_subdirs"
  fi

  AC_OUTPUT

  if test x"$coin_vpath_link_files" = x; then : ; else
    lnkcmd="$coin_link_input_cmd"
    if test "$lnkcmd" = cp; then
      AC_MSG_NOTICE(Copying data files for VPATH configuration)
    else
      AC_MSG_NOTICE(Creating VPATH links for data files)
    fi
    for file in $coin_vpath_link_files; do
      dir=`AS_DIRNAME(["./$file"])`
      if test -d $dir; then : ; else
        AS_MKDIR_P($dir)
      fi
      rm -f $file
      $lnkcmd $abs_source_dir/$file $file
    done
  fi

  if test x$coin_projectdir = xyes; then
    AC_MSG_NOTICE([Configuration of $PACKAGE_NAME successful])
  else
    AC_MSG_NOTICE([Main configuration of $PACKAGE_NAME successful])
  fi
else
  AC_MSG_NOTICE([No configuration of $PACKAGE_NAME necessary])
fi

]) #AC_COIN_FINALIZE

###########################################################################
#                             COIN_VPATH_LINK                             #
###########################################################################

# This macro makes sure that a symbolic link is created to a file in
# the source code directory tree if we are in a VPATH compilation, and
# if this package is the main package to be installed

AC_DEFUN([AC_COIN_VPATH_LINK],
[AC_REQUIRE([AC_COIN_CHECK_VPATH])
if test $coin_vpath_config = yes; then
  coin_vpath_link_files="$coin_vpath_link_files $1"
fi
]) #AC_COIN_VPATH_LINK

###########################################################################
#                       COIN_ENABLE_GNU_PACKAGES                          #
###########################################################################

# This macro defined the --enable-gnu-packages flag.  This can be used
# to check if a user wants to compile GNU packges (such as readline or
# zlib) into the executable.  By default, GNU packages are disabled.
# This also defines the automake conditional COIN_ENABLE_GNU_PACKAGES

AC_DEFUN([AC_COIN_ENABLE_GNU_PACKAGES],
[AC_ARG_ENABLE([gnu-packages],
               [AC_HELP_STRING([--enable-gnu-packages],
                               [compile with GNU packages (disabled by default)])],
	       [coin_enable_gnu=$enableval],
	       [coin_enable_gnu=no])
]) # AC_COIN_ENABLE_GNU_PACKAGES

###########################################################################
#                           COIN_CHECK_GNU_ZLIB                           #
###########################################################################

# This macro checks for the libz library.

AC_DEFUN([AC_COIN_CHECK_GNU_ZLIB],
[AC_REQUIRE([AC_COIN_ENABLE_GNU_PACKAGES])
AC_BEFORE([AC_COIN_PROG_CXX],[$0])
AC_BEFORE([AC_COIN_PROG_CC],[$0])
AC_BEFORE([AC_COIN_PROG_F77],[$0])
AC_BEFORE([$0],[AC_COIN_FINALIZE])

coin_has_zlib=no
if test $coin_enable_gnu = yes; then
  AC_COIN_CHECK_HEADER([zlib.h],[coin_has_zlib=yes])

  if test $coin_has_zlib = yes; then
    AC_CHECK_LIB([z],[gzopen],
                 [ADDLIBS="-lz $ADDLIBS"],
                 [coin_has_zlib=no])
  fi

  if test $coin_has_zlib = yes; then
    AC_DEFINE([COIN_HAS_ZLIB],[1],[Define to 1 if zlib is available])
  fi
fi

AM_CONDITIONAL(COIN_HAS_ZLIB,test x$coin_has_zlib = xyes)
]) # AC_COIN_CHECK_GNU_ZLIB


###########################################################################
#                          COIN_CHECK_GNU_BZLIB                           #
###########################################################################

# This macro checks for the libbz2 library.

AC_DEFUN([AC_COIN_CHECK_GNU_BZLIB],
[AC_REQUIRE([AC_COIN_ENABLE_GNU_PACKAGES])
AC_BEFORE([AC_COIN_PROG_CXX],[$0])
AC_BEFORE([AC_COIN_PROG_CC],[$0])
AC_BEFORE([AC_COIN_PROG_F77],[$0])
AC_BEFORE([$0],[AC_COIN_FINALIZE])

coin_has_bzlib=no
if test $coin_enable_gnu = yes; then
  AC_COIN_CHECK_HEADER([bzlib.h],[coin_has_bzlib=yes])

  if test $coin_has_bzlib = yes; then
    AC_CHECK_LIB([bz2],[BZ2_bzReadOpen],
                 [ADDLIBS="-lbz2 $ADDLIBS"],
                 [coin_has_bzlib=no])
  fi

  if test $coin_has_bzlib = yes; then
    AC_DEFINE([COIN_HAS_BZLIB],[1],[Define to 1 if bzlib is available])
  fi
fi
]) # AC_COIN_CHECK_GNU_BZLIB


###########################################################################
#                         COIN_CHECK_GNU_READLINE                         #
###########################################################################

# This macro checks for GNU's readline.  It verifies that the header
# readline/readline.h is available, and that the -lreadline library
# contains "readline".  It is assumed that #include <stdio.h> is included
# in the source file before the #include<readline/readline.h>

AC_DEFUN([AC_COIN_CHECK_GNU_READLINE],
[AC_REQUIRE([AC_COIN_ENABLE_GNU_PACKAGES])
AC_BEFORE([AC_COIN_PROG_CXX],[$0])
AC_BEFORE([AC_COIN_PROG_CC],[$0])
AC_BEFORE([AC_COIN_PROG_F77],[$0])
AC_BEFORE([$0],[AC_COIN_FINALIZE])

coin_has_readline=no
if test $coin_enable_gnu = yes; then
  AC_COIN_CHECK_HEADER([readline/readline.h],
                       [coin_has_readline=yes],[],
                       [#include <stdio.h>])

  coin_save_LIBS="$LIBS"
  LIBS=
  # First we check if tputs and friends are available
  if test $coin_has_readline = yes; then
    AC_SEARCH_LIBS([tputs],[ncurses termcap curses],[],
                   [coin_has_readline=no])
  fi

  # Now we check for readline
  if test $coin_has_readline = yes; then
    AC_CHECK_LIB([readline],[readline],
                 [ADDLIBS="-lreadline $LIBS $ADDLIBS"],
                 [coin_has_readline=no])
  fi

  if test $coin_has_readline = yes; then
    AC_DEFINE([COIN_HAS_READLINE],[1],[Define to 1 if readline is available])
  fi

  LIBS="$coin_save_LIBS"
fi
]) # AC_COIN_CHECK_GNU_READLINE

###########################################################################
#                             COIN_DATA_PATH                              #
###########################################################################

# This macro defines a preprocessor macro with the absolute path to a
# subdirectory of Data.  The argument of this macro is the name of the
# subdirectory (in correct case), and the name of the macro is
# COIN_DATA_DIR_PATH, where dir is replaced by the capitalized name of
# the directory.  The path ends with a separator ("/" for linux and
# '\\' for Windows).  The default value for this path can be
# overwritten with the input variable with the same name
# (COIN_DATA_DIR_PATH).  At this point we chech only for the
# $srcdir/../Data subdirectory.

AC_DEFUN([AC_COIN_DATA_PATH],
[AC_MSG_CHECKING([absolute path to data directory $1])

AC_ARG_VAR(m4_toupper(COIN_DATA_$1_PATH),[Set to absolute path to Data/$1 subdirectory])

if test x"$m4_toupper(COIN_DATA_$1_PATH)" = x; then
  m4_toupper(COIN_DATA_$1_PATH)=`cd $srcdir/../Data/$1; pwd`
fi

# Under Cygwin, use Windows path.  Add separator
case $build in
  *-cygwin*)
    m4_toupper(COIN_DATA_$1_PATH)=`cygwin -w $m4_toupper(COIN_DATA_$1_PATH)`\\
    ;;
  *)
    m4_toupper(COIN_DATA_$1_PATH)="$m4_toupper(COIN_DATA_$1_PATH)/"
    ;;
esac

if test -r $m4_toupper(COIN_DATA_$1_PATH); then
  AC_DEFINE_UNQUOTED(m4_toupper(COIN_DATA_$1_PATH),["$m4_toupper(COIN_DATA_$1_PATH)"],
            [Define to absolute path for Data subdirectory $1])
  AC_MSG_RESULT($m4_toupper(COIN_DATA_$1_PATH))
else
  AC_MSG_ERROR(Directory $m4_toupper(COIN_DATA_$1_PATH) does not exist)
fi
]) # AC_COIN_HAS_DATA

###########################################################################
#                       COIN_LINK_FROM_FILELIST                           #
###########################################################################

# This macro creates links (or copies, if necessary) to files listed
# as content in a text file (second argument) into a target directory
# (first argument), which is created if it doesn't exist yet.  If s link
# already exists, nothing happens.

AC_DEFUN([AC_COIN_LINKCOPY_FROM_FILELIST],
[cmd="$3"
if test -e $srcdir/$2 ; then
  my_target_dir="$1"
  my_link_files=`cat $srcdir/$2`
  my_dirname=`AS_DIRNAME($2)`
#  if test -e $my_target_dir; then : ; else
#    AS_MKDIR_P($my_target_dir)
#  fi
  for i in $my_link_files; do
    #rm -rf $my_target_dir/$i
    if test -e $my_target_dir/$i; then : ; else
      dirn2=`AS_DIRNAME($my_target_dir/$i)`
      if test -e $dirn2; then : ; else
        AS_MKDIR_P($dirn2)
      fi
      $cmd $abs_source_dir/$my_dirname/$i $my_target_dir/$i
    fi
  done
else
  AC_MSG_WARN([File list file $2 missing!])
fi
])

AC_DEFUN([AC_COIN_LINK_FROM_FILELIST], 
[
AC_REQUIRE([AC_COIN_LINK_INPUT_CMD])
echo Creating links in $1 ...
AC_COIN_LINKCOPY_FROM_FILELIST($1, $2, $coin_link_input_cmd)
])

###########################################################################
#                       COIN_COPY_FROM_FILELIST                           #
###########################################################################

# Like COIN_LINK_FROM_FILELIST, but copies the files.

AC_DEFUN([AC_COIN_COPY_FROM_FILELIST], 
[
echo Creating copies in $1 ...
AC_COIN_LINKCOPY_FROM_FILELIST($1, $2, [cp])
])

###########################################################################
#                          COIN_EXAMPLE_FILES                             #
###########################################################################

# This macro determines the names of the example files (using the
# argument in an "ls" command) and sets up the variables EXAMPLE_FILES
# and EXAMPLE_CLEAN_FILES.  If this is a VPATH configuration, it also
# creates soft links to the example files.

AC_DEFUN([AC_COIN_EXAMPLE_FILES],
[AC_REQUIRE([AC_COIN_CHECK_GNU_ZLIB])
AC_REQUIRE([AC_COIN_CHECK_VPATH])
files=`cd $srcdir; ls $1`
# We need to do the following loop to make sure that are no newlines
# in the variable
for file in $files; do
  EXAMPLE_FILES="$EXAMPLE_FILES $file"
done
if test $coin_vpath_config = yes; then
  lnkcmd=
  if test "$enable_doscompile" != no; then
    lnkcmd=cp
  fi
  case "$CC" in
    cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
      lnkcmd=cp ;;
  esac
  if test "$lnkcmd" = cp; then
    AC_MSG_NOTICE([Copying example files ($1)])
  else
    AC_PROG_LN_S
    AC_MSG_NOTICE([Creating links to the example files ($1)])
    lnkcmd="$LN_S"
  fi
  for file in $EXAMPLE_FILES; do
    rm -f $file
    $lnkcmd $srcdir/$file $file
  done
  EXAMPLE_CLEAN_FILES="$EXAMPLE_CLEAN_FILES $1"
else
  EXAMPLE_CLEAN_FILES="$EXAMPLE_CLEAN_FILES"
fi

# In case there are compressed files, we create a variable with the
# uncompressed names
EXAMPLE_UNCOMPRESSED_FILES=
for file in $EXAMPLE_FILES; do
  case $file in
    *.gz)
      EXAMPLE_UNCOMPRESSED_FILES="$EXAMPLE_UNCOMPRESSED_FILES `echo $file | sed -e s/.gz//`"
      ;;
  esac
done

AC_SUBST(EXAMPLE_UNCOMPRESSED_FILES)
AC_SUBST(EXAMPLE_FILES)
AC_SUBST(EXAMPLE_CLEAN_FILES)
]) #AC_COIN_EXAMPLE_FILES

###########################################################################
#                            COIN_HAS_PROJECT                             #
###########################################################################

# This macro sets up usage of a Coin package.  It defines the
# PKGSRCDIR and PKGOBJDIR variables, refering to the main source and
# object directory of the package, respectively.  It also defines
# a COIN_HAS_PKG preprocessor macro and makefile conditional.  The
# argument should be the name (Pkg) of the project (in correct lower
# and upper case)

AC_DEFUN([AC_COIN_HAS_PROJECT],
[AC_MSG_CHECKING([for COIN project $1])

# First check, if the sub-project is actually available (ToDo: allow
# other locations)

m4_tolower(coin_has_$1)=unavailable
if test x"$COIN_SKIP_PROJECTS" != x; then
  for dir in $COIN_SKIP_PROJECTS; do
    if test $dir = $1; then
      m4_tolower(coin_has_$1)=skipping
    fi
  done
fi

if test $m4_tolower(coin_has_$1) != skipping; then
  if test $PACKAGE_TARNAME = m4_tolower($1); then
    m4_tolower(coin_has_$1)=.
  else
    if test -d $srcdir/../$1; then
      m4_tolower(coin_has_$1)=../$1
    fi
  fi
fi

if test $m4_tolower(coin_has_$1) != unavailable &&
   test $m4_tolower(coin_has_$1) != skipping; then
  # Set the #define if the component is available
  AC_DEFINE(m4_toupper(COIN_HAS_$1),[1],[Define to 1 if the $1 package is used])

  # Set the variables for source and object code location
  AC_SUBST(m4_toupper($1SRCDIR))
  m4_toupper($1SRCDIR)=`cd $srcdir/$m4_tolower(coin_has_$1); pwd`
  AC_SUBST(m4_toupper($1OBJDIR))
  m4_toupper($1OBJDIR)=`pwd`/$m4_tolower(coin_has_$1)
  AC_SUBST(m4_toupper($1DOCDIR))
  m4_toupper($1DOCDIR)=$abs_lib_dir/../share/doc/coin/$1
fi

  # Define the Makefile conditional
AM_CONDITIONAL(m4_toupper(COIN_HAS_$1),
               [test $m4_tolower(coin_has_$1) != unavailable &&
                test $m4_tolower(coin_has_$1) != skipping])
AC_MSG_RESULT([$m4_tolower(coin_has_$1)])
]) # AC_COIN_HAS_PROJECT

###########################################################################
#                        COIN_HAS_USER_LIBRARY                            #
###########################################################################
# This macro sets up usage of a user library with header files. The assumption
# is that the header file(s) and library do not reside in standard system
# directories, hence both the include directory and link flags must be
# specified. There are two mandatory arguments and two optional arguments.
#
# The first argument (mandatory) should be a name (LibraryName) for the
# library.  The second argument (mandatory) should be an abbreviation in
# upper case letters (LBRY) for the library. Ultimately, the macro will
# specify two variables, LBRYINCDIR and LBRYLIB, to be substituted in files
# generated during configuration; a preprocessor symbol COIN_HAS_LBRY; and a
# matching automake conditional COIN_HAS_LBRY. LBRYINCDIR should specify the
# directory containing include files for the library. LBRYLIB should specify
# the flags necessary to link to the library. A variable coin_has_lbry will
# be set to true or false, as appropriate. A variable lbry_libcheck will be
# be set to yes or no; no indicates link checks should not be attempted.
#
# The macro defines three configure arguments, --with-libraryname-incdir,
# --with-libraryname-lib, and --disable-libraryname-libcheck, by converting
# LibraryName to lower case.
#
# LBRYINCDIR and LBRYLIB can be specified as environment variables or as
# part of the configure command using --with-libraryname-incdir and
# --with-libraryname-lib, respectively. Command line arguments override
# environment variables.
#
# If a third argument is given, it should specify a file in LBRYINCDIR.  The
# macro will check for the presence of the file. If a fourth argument is given,
# it should specify a function name, `fname'.  The macro will attempt to link a
# trivial program containing a parameterless call to the function, `fname()',
# using the LBRYLIB flags. The link check uses C as the language; this has been
# adequate to date but has limitations. It is possible to disable the link
# check by specifying --disable-libraryname-libcheck. This is a workaround for
# instances where the link check does not work properly, for whatever reason.
# If you're trying to link to a Fortran library, consider using F77_FUNC or
# FC_FUNC to obtain a mangled fname appropriate for use from C code. For a C++
# library, you're on your own unless the library declares some function with
# extern "C" linkage. Otherwise, you'll have to somehow find the mangled C++
# name.

AC_DEFUN([AC_COIN_HAS_USER_LIBRARY],
[ AC_REQUIRE([AC_COIN_PROJECTDIR_INIT])
  AC_MSG_CHECKING(if user provides library for $1)

# Check for header file directory

  AC_ARG_WITH(m4_tolower($1)-incdir,
      AS_HELP_STRING([--with-m4_tolower($1)-incdir],
		     [specify the header file directory for library $1]),
      [$2INCDIR=`cd $withval; pwd`])

# Check for library directory

  AC_ARG_WITH(m4_tolower($1)-lib,
      AS_HELP_STRING([--with-m4_tolower($1)-lib],
		     [specify the flags used to link with the library $1]),
      [$2LIB=$withval])

# Switch to disable library check if requested

  AC_ARG_ENABLE(m4_tolower($1)-libcheck,
      AS_HELP_STRING([--enable-m4_tolower($1)-libcheck],
		     [use disable-m4_tolower($1)-libcheck to skip the link
		      check at configuration time]),
      [m4_tolower($1)_libcheck=$enableval],
      [m4_tolower($1)_libcheck=yes])

# At this point, if we're going to use the library, both LBRYINCDIR and
# LBRYLIB must be defined and not empty.

  if test x"$$2INCDIR" != x || test x"$$2LIB" != x; then
    if test x"$$2INCDIR" = x || test x"$$2LIB" = x; then
      AC_MSG_ERROR([You need to specify both an include directory and link flags to use library $1. Use --with-m4_tolower($1)-incdir of environment variable $$2INCDIR to specify the include directory. Use --with-m4_tolower($1)-lib or environment variable $$2LIB to specify link flags.])
    fi
    m4_tolower(coin_has_$2)=true
    AC_MSG_RESULT(yes)
  else
    m4_tolower(coin_has_$2)=false
    AC_MSG_RESULT(no)
  fi

# If we have instructions for use, consider header and link checks.

  if test $m4_tolower(coin_has_$2) = true; then

# If argument 3 (file) is given, check for the file. Typically this will be a
# header file, but that's not assumed.

    m4_ifval([$3],
        [AC_COIN_CHECK_FILE([$$2INCDIR/$3],[],
	     [AC_MSG_ERROR([Cannot find file $3 in $$2INCDIR])])])

# Now see if we can link the function. There are arguments for and against
# assuming argument 3 is a header file declaring the function. A correct
# function declaration is the main argument in favour. Having to cope with
# possible dependencies or other oddities are the main arguments against.
# Force the use of C as the best single choice amongst C++, C, and Fortran.
# Obviously, this has limits.

    m4_ifvaln([$4],
        [if test x"$m4_tolower($1)_libcheck" != xno; then
	   coin_save_LIBS="$LIBS"
	   LIBS="$$2LIB $ADDLIBS"
	   coin_$2_link=no
	   AC_LANG_PUSH(C)
	   for fnm in $4 ; do
	     AC_MSG_CHECKING([whether symbol $fnm is available with $2])
	     AC_LINK_IFELSE([AC_LANG_PROGRAM([[]],[[$fnm()]])],
		 [AC_MSG_RESULT(yes)
		  coin_$2_link=yes
		  break],
		 [AC_MSG_RESULT(no)])
	   done
	   AC_LANG_POP(C)
	   if test x"$coin_$2_link" = xyes ; then
	     LIBS="$coin_save_LIBS"
	   else
	     AC_MSG_ERROR([Cannot find symbol(s) $4 with $2])
	   fi
	 fi])

# If we make it this far, we've verified the file and linked the function. Add
# the necessary link flags to ADDLIBS and define the preprocessor symbol
# COIN_HAS_LBRY.

    ADDLIBS="$$2LIB $ADDLIBS"
    AC_DEFINE(COIN_HAS_$2,[1],[Define to 1 if the $1 package is available])
  fi

# Arrange for configure to substitute LBRYINCDIR and LBRYLIB and create the
# automake conditional. These actions must occur unconditionally.

  AC_SUBST($2INCDIR)
  AC_SUBST($2LIB)
  AM_CONDITIONAL(COIN_HAS_$2, test $m4_tolower(coin_has_$2) = true)
]) #AC_COIN_HAS_USER_LIBRARY 

###########################################################################
#                               COIN_HAS_ASL                              #
###########################################################################

# This macro checks if the user has provide arguments that say where
# the precompiled ASL files should be found (with the --with-asldir
# flag).  If this is not the case, we check if the ThirdParty/ASL
# directory has been configured, which indicates that the files will
# be in that directory and can be used.

AC_DEFUN([AC_COIN_HAS_ASL],
[coin_aslobjdir=../ThirdParty/ASL
coin_aslsrcdir=$srcdir/$coin_aslobjdir

# Determine the name of the ASL library
case "$CC" in
  cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
    ampllib=amplsolv.lib ;;
  *)
    ampllib=amplsolver.a ;;
esac

AC_ARG_WITH([asldir],
            AC_HELP_STRING([--with-asldir],
                           [specify path to AMPL solver directory (or BUILD for compilation, or "no" for disabling AMPL)]),
            [use_asldir=$withval], [use_asldir=])

if test "$use_asldir" = BUILD; then
  if test "$PACKAGE_NAME" != ThirdPartyASL; then
    # If we are configuring ThirdParty/ASL, don't check
    if test -r $coin_aslobjdir/.MakeOk; then :; else
      AC_MSG_ERROR([option \"BUILD\" specified for asldir, but directory is not configure (sources missing?)])
    fi
  fi
elif test -z "$use_asldir"; then
 # try to find sources - if not given don't compile
  if test "$PACKAGE_NAME" != ThirdPartyASL; then
    if test -r $coin_aslobjdir/.MakeOk; then
      use_asldir=BUILD
    else
      use_asldir=no
    fi
  else
    use_asldir=no
  fi
elif test "$use_asldir" != "no"; then
  AC_COIN_CHECK_FILE([$use_asldir/$ampllib],[],
                     [AC_MSG_ERROR([ASL directory \"$use_asldir\" specified, but library missing])])
  AC_COIN_CHECK_FILE([$use_asldir/asl.h],[],
                     [AC_MSG_ERROR([ASL directory \"$use_asldir\" specified, but header files are missing])])
  use_asldir=`cd $use_asldir; pwd`
  case $build in
    *-cygwin*) use_asldir=`cygpath -w $use_asldir | sed -e sX\\\\\\\\X/Xg` ;;
  esac
fi

# Variable containing ASL library (including full path)
AC_SUBST(ASLLIB)
# Variable containing flags for including ASL header files
AC_SUBST(ASL_CPPFLAGS)

if test "$use_asldir" = BUILD; then
  coin_aslobjdir=`cd $coin_aslobjdir; pwd`
  ASLLIB=`$CYGPATH_W $coin_aslobjdir/$ampllib | sed -e sX\\\\\\\\X/Xg`
  coin_aslsrcdir=`cd $coin_aslsrcdir; pwd`
  ASL_CPPFLAGS="-I"`$CYGPATH_W $coin_aslobjdir | sed -e sX\\\\\\\\X/Xg`" -I"`$CYGPATH_W $coin_aslsrcdir/solvers | sed -e sX\\\\\\\\X/Xg`
elif test "$use_asldir" != no; then
  ASLLIB=`$CYGPATH_W $use_asldir/$ampllib | sed -e sX\\\\\\\\X/Xg`
  ASL_CPPFLAGS="-I"`$CYGPATH_W $use_asldir | sed -e sX\\\\\\\\X/Xg`
fi

if test "$use_asldir" != no; then
  AC_LANG_PUSH(C)
  AC_CHECK_LIB(dl,[dlopen],[ASLLIB="$ASLLIB -ldl"],[])
  coin_has_asl=yes
  AC_DEFINE([COIN_HAS_ASL],[1],
            [If defined, the Ampl Solver Library is available.])
  AC_LANG_POP(C)
else
  coin_has_asl=no
fi
AM_CONDITIONAL(COIN_HAS_ASL, test $coin_has_asl = yes)
]) # AC_COIN_HAS_ASL

###########################################################################
#                            COIN_TRY_FLINK                               #
###########################################################################

# Auxilliary macro to test if a Fortran function name can be linked,
# given the current settings of LIBS.  We determine from the context, what
# the currently active programming language is, and cast the name accordingly.
# The first argument is the name of the function/subroutine, in small letters,
# the second argument are the actions taken when the test works, and the
# third argument are the actions taken if the test fails.

AC_DEFUN([AC_COIN_TRY_FLINK],
[case $ac_ext in
  f)
    AC_TRY_LINK(,[      call $1],[$2],[$3])
    ;;
  c)
    AC_F77_FUNC($1,cfunc$1)
    if test x"$coin_need_flibs" = xyes; then
      flink_try=no;
    else
      AC_TRY_LINK([void $cfunc$1();],[$cfunc$1()],
                  [flink_try=yes],[flink_try=no])
    fi
    if test $flink_try = yes; then
      $2
    else
      if test x"$FLIBS" != x; then
        flink_save_libs="$LIBS"
        LIBS="$LIBS $FLIBS"
        AC_TRY_LINK([void $cfunc$1();],[$cfunc$1()],
                    [LIBS="$flink_save_libs"
                     $2
                     coin_need_flibs=yes],
                    [LIBS="$flink_save_libs"
                     $3])
      else
        $3
      fi
    fi
    ;;
  cc|cpp)
    AC_F77_FUNC($1,cfunc$1)
    if test x"$coin_need_flibs" = xyes; then
      flink_try=no;
    else
      AC_TRY_LINK([extern "C" {void $cfunc$1();}],[$cfunc$1()],
                  [flink_try=yes],[flink_try=no])
    fi
    if test $flink_try = yes; then
      $2
    else
      if test x"$FLIBS" != x; then
        flink_save_libs="$LIBS"
        LIBS="$LIBS $FLIBS"
        AC_TRY_LINK([extern "C" {void $cfunc$1();}],[$cfunc$1()],
                    [LIBS="$flink_save_libs"
                     $2
                     coin_need_flibs=yes],
                    [LIBS="$flink_save_libs"
                     $3])
      else
        $3
      fi
    fi
    ;;
esac
]) # AC_COIN_TRY_FLINK

###########################################################################
#                             COIN_HAS_BLAS                               #
###########################################################################

# This macro checks for a library containing the BLAS library.  It
# tries standard libraries, and if none is found to be working, it
# checks whether the BLAS ThirdParty/Blas directory has been configured.
# It adds to ADDLIBS any flags required to link with an externally provided
# BLAS.  It defines the makefile conditional and preprocessor macro
# COIN_HAS_BLAS, if blas is available, and it defines the makefile conditional
# COIN_BUILD_BLAS, if blas is compiled within COIN.

AC_DEFUN([AC_COIN_HAS_BLAS],
[if test "$PACKAGE_NAME" = ThirdPartyBlas || test "$PACKAGE_NAME" = ThirdPartyLapack || test "$PACKAGE_NAME" = ThirdPartyMumps; then
  coin_blasobjdir=../Blas
else
  coin_blasobjdir=../ThirdParty/Blas
fi
coin_blassrcdir=$srcdir/$coin_blasobjdir

AC_ARG_WITH([blas],
            AC_HELP_STRING([--with-blas],
                           [specify BLAS library (or BUILD for compilation)]),
            [use_blas=$withval], [use_blas=])

MAKEOKFILE=.MakeOk
# Check if user supplied option makes sense
if test x"$use_blas" != x; then
  if test "$use_blas" = "BUILD"; then
    # Don't check for course code if this is executed in ThirdParty/Blas
    if test "$PACKAGE_NAME" != ThirdPartyBlas; then
      if test -r $coin_blasobjdir/.MakeOk; then :; else
        AC_MSG_ERROR([option \"BUILD\" specified for Blas, but $coin_blasobjdir directory is not properly configured])
      fi
    fi
  elif test "$use_blas" != no ; then
    AC_MSG_CHECKING([whether user supplied BLASLIB=\"$use_blas\" works])
    LIBS="$use_blas $LIBS"
    ADDLIBS="$use_blas $ADDLIBS"
    AC_COIN_TRY_FLINK([daxpy],
                      [AC_MSG_RESULT([yes])],
                      [AC_MSG_RESULT([no])
                       AC_MSG_ERROR([user supplied BLAS library \"$use_blas\" does not work])])
  fi
else
# Try to autodetect the library for blas based on build system
  #AC_MSG_CHECKING([default locations for BLAS])
  skip_lblas_check=no
  case $build in
    *-sgi-*) 
      SAVE_LIBS="$LIBS"
      AC_MSG_CHECKING([whether -lcomplib.sgimath has BLAS])
      LIBS="-lcomplib.sgimath $LIBS"
      AC_COIN_TRY_FLINK([daxpy],
                        [AC_MSG_RESULT([yes])
                         use_blas=-lcomplib.sgimath;
                         ADDLIBS="-lcomplib.sgimath $ADDLIBS"],
                        [AC_MSG_RESULT([no])
                         SAVE_LIBS="$LIBS"])
      ;;

# Ideally, we'd use -library=sunperf, but it's an imperfect world. Studio
# cc doesn't recognise -library, it wants -xlic_lib. Studio 12 CC doesn't
# recognise -xlic_lib. Libtool doesn't like -xlic_lib anyway. Sun claims
# that CC and cc will understand -library in Studio 13. The main extra
# function of -xlic_lib and -library is to arrange for the Fortran run-time
# libraries to be linked for C++ and C. We can arrange that explicitly.
    *-*-solaris*)
      SAVE_LIBS="$LIBS"
      AC_MSG_CHECKING([for BLAS in libsunperf])
      LIBS="-lsunperf $FLIBS $LIBS"
      AC_COIN_TRY_FLINK([daxpy],
                        [AC_MSG_RESULT([yes])
                         use_blas='-lsunperf'
                         ADDLIBS="-lsunperf $ADDLIBS"
			 coin_need_flibs=yes],
                        [AC_MSG_RESULT([no])
                         LIBS="$SAVE_LIBS"])
      ;;
    *-cygwin* | *-mingw*)
# On cygwin, consider -lblas only if doscompile is disabled. The prebuilt
# library will want to link with cygwin, hence won't run standalone in DOS.
      if test "$enable_doscompile" = mingw; then
	skip_lblas_check=yes
      fi
      case "$CC" in
        cl* | */cl* | CL* | */CL* | icl* | */icl* | ICL* | */ICL*)
          SAVE_LIBS="$LIBS"
          AC_MSG_CHECKING([for BLAS in MKL])
          LIBS="mkl_intel_c.lib mkl_sequential.lib mkl_core.lib $LIBS"
          AC_COIN_TRY_FLINK([daxpy],
                            [AC_MSG_RESULT([yes])
                             use_blas='mkl_intel_c.lib mkl_sequential.lib mkl_core.lib'
                             ADDLIBS="mkl_intel_c.lib mkl_sequential.lib mkl_core.lib $ADDLIBS"],
                            [AC_MSG_RESULT([no])
                             LIBS="$SAVE_LIBS"])
          ;;
      esac
      ;;
  esac

  if test -z "$use_blas" && test $skip_lblas_check = no; then
    SAVE_LIBS="$LIBS"
    AC_MSG_CHECKING([whether -lblas has BLAS])
    LIBS="-lblas $LIBS"
    AC_COIN_TRY_FLINK([daxpy],
		      [AC_MSG_RESULT([yes])
		       ADDLIBS="-lblas $ADDLIBS"
		       use_blas='-lblas'],
		      [AC_MSG_RESULT([no])
	               LIBS="$SAVE_LIBS"])
  fi

# If we have no other ideas, consider building BLAS.
  if test -z "$use_blas"; then
    if test "$PACKAGE_NAME" != ThirdPartyBlas; then
      if test -r $coin_blasobjdir/.MakeOk; then
        use_blas=BUILD
      fi
    fi
  fi
fi

if test "$use_blas" = BUILD; then
  coin_need_flibs=yes
fi

AM_CONDITIONAL([COIN_HAS_BLAS],[test x"$use_blas" != x])
AM_CONDITIONAL([COIN_BUILD_BLAS],[test "$use_blas" = BUILD])

if test x"$use_blas" = x || test "$use_blas" = no; then
  coin_has_blas=no
else
  coin_has_blas=yes
  AC_DEFINE([COIN_HAS_BLAS],[1],
            [If defined, the BLAS Library is available.])
fi
]) # AC_COIN_HAS_BLAS

###########################################################################
#                            COIN_HAS_LAPACK                              #
###########################################################################

# This macro checks for a library containing the LAPACK library.  It
# tries standard libraries, and if none is found to be working, it
# checks whether the LAPACK ThirdParty/Lapack directory has been
# configured.  It adds to ADDLIBS any flags required to link with an
# externally provided LAPACK.  It defines the makefile conditional and
# preprocessor macro COIN_HAS_LAPACK, if lapack is available, and it
# defines the makefile conditional COIN_BUILD_LAPACK, if lapack is
# compiled within COIN.

AC_DEFUN([AC_COIN_HAS_LAPACK],
[coin_lapackobjdir=../ThirdParty/Lapack
coin_lapacksrcdir=$srcdir/$coin_lapackobjdir

AC_ARG_WITH([lapack],
            AC_HELP_STRING([--with-lapack],
                           [specify LAPACK library (or BUILD for compilation)]),
            [use_lapack=$withval], [use_lapack=])

# Check if user supplied option makes sense
if test x"$use_lapack" != x; then
  if test "$use_lapack" = "BUILD"; then
    # Don't check for course code if this is executed in ThirdParty/Lapack
    if test "$PACKAGE_NAME" != ThirdPartyLapack; then
      if test -r $coin_lapackobjdir/.MakeOk; then :; else
        AC_MSG_ERROR([option \"BUILD\" specified for LAPACK, but $coin_lapackobjdir directory is not configured])
      fi
    fi
  else
    AC_MSG_CHECKING([whether user supplied LAPACKLIB=\"$use_lapack\" works])
    LIBS="$use_lapack $LIBS"
    ADDLIBS="$use_lapack $ADDLIBS"
    AC_COIN_TRY_FLINK([dsyev],
                      [AC_MSG_RESULT([yes])],
                      [AC_MSG_RESULT([no])
                       AC_MSG_ERROR([user supplied LAPACK library \"$use_lapack\" does not work])])
  fi
else
  if test x$coin_has_blas = xyes; then
    # First try to see if LAPACK is already available with BLAS library
    AC_MSG_CHECKING([whether LAPACK is already available with BLAS library])
    AC_COIN_TRY_FLINK([dsyev],
                      [AC_MSG_RESULT([yes]); use_lapack=ok],
                      [AC_MSG_RESULT([no])])
  fi
  skip_llapack_check=no
  if test -z "$use_lapack"; then
    # Try to autodetect the library for lapack based on build system
    case $build in
      *-sgi-*) 
        SAVE_LIBS="$LIBS"
        AC_MSG_CHECKING([whether -lcomplib.sgimath has LAPACK])
        LIBS="-lcomplib.sgimath $LIBS"
        AC_COIN_TRY_FLINK([dsyev],
                          [AC_MSG_RESULT([yes])
                           use_lapack=-lcomplib.sgimath;
                           ADDLIBS="-lcomplib.sgimath $ADDLIBS"],
                          [AC_MSG_RESULT([no])
                           SAVE_LIBS="$LIBS"])
        ;;

# See comments in COIN_HAS_BLAS.
      *-*-solaris*)
      SAVE_LIBS="$LIBS"
      AC_MSG_CHECKING([for LAPACK in libsunperf])
      LIBS="-lsunperf $FLIBS $LIBS"
      AC_COIN_TRY_FLINK([dsyev],
                        [AC_MSG_RESULT([yes])
                         use_blas='-lsunperf'
                         ADDLIBS="-lsunperf $ADDLIBS"
			 coin_need_flibs=yes],
                        [AC_MSG_RESULT([no])
                         LIBS="$SAVE_LIBS"])
        ;;
# On cygwin, do this check only if doscompile is disabled. The prebuilt library
# will want to link with cygwin, hence won't run standalone in DOS.
      *-cygwin*)
	if test "$enable_doscompile" = mingw; then
	  skip_llapack_check=yes
	fi
	;;
    esac
  fi

  if test -z "$use_lapack" && test $skip_llapack_check = no; then
    SAVE_LIBS="$LIBS"
    AC_MSG_CHECKING([whether -llapack has LAPACK])
    LIBS="-llapack $LIBS"
    AC_COIN_TRY_FLINK([dsyev],
		      [AC_MSG_RESULT([yes])
		       ADDLIBS="-llapack $ADDLIBS"
		       use_lapack='-llapack'],
		      [AC_MSG_RESULT([no])
		       LIBS="$SAVE_LIBS"])
  fi

# If we have no other ideas, consider building LAPACK.
  if test -z "$use_lapack"; then
    if test "$PACKAGE_NAME" != ThirdPartyLapack; then
      if test -r $coin_lapackobjdir/.MakeOk; then
        use_lapack=BUILD
      fi
    fi
  fi
fi

if test "$use_lapack" = BUILD; then
  coin_need_flibs=yes
fi

AM_CONDITIONAL([COIN_HAS_LAPACK],[test x"$use_lapack" != x])
AM_CONDITIONAL([COIN_BUILD_LAPACK],[test "$use_lapack" = BUILD])

if test x"$use_lapack" = x; then
  coin_has_lapack=no
else
  coin_has_lapack=yes
  AC_DEFINE([COIN_HAS_LAPACK],[1],
            [If defined, the LAPACK Library is available.])
fi
]) # AC_COIN_HAS_LAPACK

###########################################################################
#                            COIN_HAS_MUMPS                               #
###########################################################################

# This macro checks for a library containing the MUMPS library.  It
# checks if the user has provided an argument for the MUMPS library,
# and if not, it checks whether the MUMPS ThirdParty/Mumps directory has
# been configured.  It adds to ADDLIBS any flags required to link with
# an externally provided MUMPS.  It defines the makefile conditional
# and preprocessor macro COIN_HAS_MUMPS, if MUMPS is available, and it
# defines the makefile conditional COIN_BUILD_MUMPS, if MUMPS is
# compiled within COIN.

AC_DEFUN([AC_COIN_HAS_MUMPS],
[
if test "$PACKAGE_NAME" = ThirdPartyMumps; then
  coin_mumpsobjdir=../Mumps
else
  coin_mumpsobjdir=../ThirdParty/Mumps
fi
coin_mumpssrcdir=$abs_source_dir/$coin_mumpsobjdir/MUMPS

MAKEOKFILE=.MakeOk

#check if user provides a MUMPS library (that works)
AC_LANG_PUSH(C)
AC_COIN_HAS_USER_LIBRARY(mumps, MUMPS, dmumps_c.h, dmumps_c)
AC_LANG_POP(C)

if test "$coin_has_mumps" = "true"; then  # user provided mumps library
  use_mumps=yes
  coin_has_mumps=yes

  MUMPS_INCFLAGS="-I\`\$(CYGPATH_W) $MUMPSINCDIR\`"
  ADDLIBS="$MUMPSLIB $ADDLIBS"

else # no user provided library, so we try to build our own
  use_mumps=BUILD

  # Check if the MUMPS' ThirdParty project has been configured
  if test "$PACKAGE_NAME" != ThirdPartyMumps; then
    if test -r $coin_mumpsobjdir/.MakeOk; then
      use_mumps=BUILD
      # Mumps needs pthreads
      AC_LANG_PUSH(C)
      save_LIBS="$LIBS"
      LIBS="$LIBS $FLIBS"
      AC_CHECK_LIB([pthread],[pthread_create],[LIBS="-lpthread $save_LIBS"; ADDLIBS="-lpthread $ADDLIBS"],[LIBS="save_LIBS"])
      AC_LANG_POP(C)
  
      MUMPS_INCFLAGS="-I\`\$(CYGPATH_W) $coin_mumpssrcdir/libseq\` -I\`\$(CYGPATH_W) $coin_mumpssrcdir/include\`"
    else
      use_mumps=
    fi
  fi

  # if a user provided library is used, then COIN_HAS_USER_LIBRARY takes care of the COIN_HAS_MUMPS conditional and preprocessor symbol
  AM_CONDITIONAL([COIN_HAS_MUMPS],[test x"$use_mumps" != x])
  if test x"$use_mumps" != x; then
    AC_DEFINE([COIN_HAS_MUMPS],[1],[If defined, the MUMPS Library is available.])
    coin_has_mumps=yes
  else
    coin_has_mumps=no
  fi
  AC_MSG_CHECKING([whether MUMPS is available])
  AC_MSG_RESULT([$coin_has_mumps])
fi

if test x"$use_mumps" != x; then
  # we need the Fortran runtime libraries if we want to link with C/C++
  coin_need_flibs=yes

  AC_SUBST(MUMPS_INCFLAGS)
fi

AM_CONDITIONAL([COIN_BUILD_MUMPS],[test "$use_mumps" = BUILD])

]) # AC_COIN_HAS_MUMPS

###########################################################################
#                             COIN_HAS_GLPK                               #
###########################################################################
#
# This macro checks for the GLPK package. GLPK provides two capabilities,
# an LP and MIP solver (GLPK) and the GNU Mathprog modelling language (GMPL).
# The macro checks for either Glpk or Gmpl, according to the value specified as
# the parameter. Use one of Glpk or Gmpl. Use *exactly* these strings, eh?
#
#
# The macro first uses COIN_HAS_USER_LIBRARY to see if the user has specified
# a preexisting library (this allows the use of any glpk version, if the user
# is fussy). The macro then checks for ThirdParty/Glpk.
#
# This macro will define the following variables for Glpk:
#   coin_has_glpk	true or false
#   GLPKLIB		link flags for GLPK (if user supplied)
#   GLPKINCDIR		location of glpk include files
#   COIN_HAS_GLPK	Preprocessor symbol, defined to 1
#   COIN_HAS_GLPK	Automake conditional
#   COIN_BUILD_GLPK	Automake conditional, defined only if Glpk is to be
#			built in ThirdParty/Glpk
#
# With the exception of COIN_BUILD_GLPK, an identical set of variables is
# defined for Gmpl.

AC_DEFUN([AC_COIN_HAS_GLPK],
[
if test "$PACKAGE_NAME" = ThirdPartyGlpk; then
  coin_glpkobjdir=../Glpk
else
  coin_glpkobjdir=../ThirdParty/Glpk
fi
coin_glpksrcdir=$abs_source_dir/$coin_glpkobjdir

use_thirdpartyglpk=no

# Check for the requested component. If the user specified an external glpk
# library don't force a ThirdParty build, let the error propagate.

m4_if([$1],[Glpk],
[AC_COIN_HAS_USER_LIBRARY([Glpk],[GLPK],[glpk.h],
    [_glp_lpx_simplex glp_lpx_simplex])
 if test x"$coin_has_glpk" = xfalse && test x"$GLPKLIB" = x ; then
   use_thirdpartyglpk=try
 fi])
m4_if([$1],[Gmpl],
[AC_COIN_HAS_USER_LIBRARY([Gmpl],[GMPL],[glpmpl.h],
    [_glp_mpl_initialize glp_mpl_initialize])
 if test x"$coin_has_gmpl" = xfalse && test x"$GMPLLIB" = x ; then
   use_thirdpartyglpk=try
 fi])

# If the user has supplied an external library, use it. Otherwise, consider
# a build in ThirdParty/Glpk. If we build, assume we get both glpk and gmpl.

if test x"$use_thirdpartyglpk" = xtry ; then
  MAKEOKFILE=.MakeOk
  # Check if the Glpk's ThirdParty project has been configured
  if test "$PACKAGE_NAME" != ThirdPartyGlpk; then
    if test -r $coin_glpkobjdir/.MakeOk; then
      use_thirdpartyglpk=build
    else
      use_thirdpartyglpk=no
    fi
  else
    use_thirdpartyglpk=build
  fi

# If we're building, set the library and include directory variables, create a
# preprocessor symbol, define a variable that says we're using glpk/gmpl, and
# another to indicate a link check is a bad idea (hard to do before the library
# exists).

  if test x"$use_thirdpartyglpk" = xbuild ; then
    m4_toupper($1INCDIR)="$coin_glpksrcdir/glpk/include"
    AC_SUBST(m4_toupper($1INCDIR))
    AC_DEFINE(m4_toupper(COIN_HAS_$1),[1],
	      [Define to 1 if $1 package is available])
    m4_tolower(coin_has_$1)=true
    m4_tolower($1_libcheck)=no
    AC_MSG_NOTICE([Using $1 in ThirdParty/Glpk])
  fi
fi

# Define the necessary automake conditionals.

AM_CONDITIONAL(m4_toupper(COIN_HAS_$1),
	       [test x"$m4_tolower(coin_has_$1)" = xtrue])
AM_CONDITIONAL([COIN_BUILD_GLPK],[test x"$use_thirdpartyglpk" = xbuild])

]) # AC_COIN_HAS_GLPK
