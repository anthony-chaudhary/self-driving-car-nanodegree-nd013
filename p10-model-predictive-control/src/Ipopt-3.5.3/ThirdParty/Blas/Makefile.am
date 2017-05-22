# Copyright (C) 2003, 2007 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am 735 2006-06-04 06:10:05Z andreasw $

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = get.blas INSTALL.BLAS

if SKIPMAKE
  noinst_LTLIBRARIES =
else
  noinst_LTLIBRARIES = libcoinblas.la
endif

nodist_libcoinblas_la_SOURCES = \
	dasum.f \
	daxpy.f \
	dcopy.f \
	ddot.f \
	dgbmv.f \
	dgemm.f \
	dgemv.f \
	dger.f \
	dnrm2.f \
	drot.f \
	drotg.f \
	drotm.f \
	drotmg.f \
	dsbmv.f \
	dscal.f \
	dsdot.f \
	dspmv.f \
	dspr2.f \
	dspr.f \
	dswap.f \
	dsymm.f \
	dsymv.f \
	dsyr2.f \
	dsyr2k.f \
	dsyr.f \
	dsyrk.f \
	dtbmv.f \
	dtbsv.f \
	dtpmv.f \
	dtpsv.f \
	dtrmm.f \
	dtrmv.f \
	dtrsm.f \
	dtrsv.f \
	idamax.f \
	lsame.f \
	xerbla.f

libcoinblas_la_LDFLAGS = $(LT_LDFLAGS)

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES = $(MAKEOKFILE)

include ../../BuildTools/Makemain.inc
