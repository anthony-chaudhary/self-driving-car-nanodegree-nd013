# Copyright (C) 2006, 2008 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am 735 2006-06-04 06:10:05Z andreasw $

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = get.lapack INSTALL.LAPACK

if SKIPMAKE
  noinst_LTLIBRARIES =
else
  noinst_LTLIBRARIES = libcoinlapack.la
endif

LAPACK_FILES = \
	dgetf2.f \
	dgetrf.f \
	dgetrs.f \
	dlae2.f \
	dlaev2.f \
	dlamch.f \
	dlanst.f \
	dlansy.f \
	dlapy2.f \
	dlarf.f \
	dlarfb.f \
	dlarfg.f \
	dlarft.f \
	dlartg.f \
	dlascl.f \
	dlaset.f \
	dlasr.f \
	dlasrt.f \
	dlaswp.f \
	dlassq.f \
	dlatrd.f \
	dorg2l.f \
	dorg2r.f \
	dorgql.f \
	dorgqr.f \
	dorgtr.f \
	dpotf2.f \
	dpotrf.f \
	dpotrs.f \
	dsteqr.f \
	dsterf.f \
	dsyev.f \
	dsytd2.f \
	dsytrd.f \
	ieeeck.f \
	ilaenv.f \
	iparmq.f

nodist_libcoinlapack_la_SOURCES = $(LAPACK_FILES)

libcoinlapack_la_LDFLAGS = $(LT_LDFLAGS)

dlamch.o: dlamch.f
	$(F77) $(AM_FFLAGS) $(DLAMCH_FFLAGS) -c -o $@ $<

dlamch.obj: dlamch.f
	$(F77) $(AM_FFLAGS) $(DLAMCH_FFLAGS) -c -o $@ `$(CYGPATH_W) '$<'`

dlamch.lo: dlamch.f
	$(LIBTOOL) --tag=F77 --mode=compile $(F77) $(AM_FFLAGS) $(DLAMCH_FFLAGS) -c -o $@ $<

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES = $(LAPACK_FILES) $(MAKEOKFILE)

include ../../BuildTools/Makemain.inc
