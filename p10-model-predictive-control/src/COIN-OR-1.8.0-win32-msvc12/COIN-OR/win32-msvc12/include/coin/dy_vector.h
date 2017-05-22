/*
  This file is a part of the Dylp LP distribution.
 
        Copyright (C) 2005 -- 2007 Lou Hafer
 
        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca
 
  This code is licensed under the terms of the Eclipse Public License (EPL).
*/
 
#ifndef _DYLP_VECTOR_H
#define _DYLP_VECTOR_H

/* The part that requires information from the private header file config.h
 * is only needed for building DyLP itself, so we only do this if DYLP_INTERNAL
 * had been defined.
 */
#ifdef DYLP_INTERNAL

/*
  @(#)dy_vector.h         4.5         11/06/04
  svn/cvs: $Id: dy_vector.h 436 2011-06-08 21:06:45Z stefan $
*/

#include <DylpConfig.h>
 
/*
  Why, you might ask, are we including ctype.h? Well, it's required by the
  ANSI C specification, so it's pretty well guaranteed to exist. And, at least
  in Solaris and Linux environments that I'm familiar with, it'll pull in the
  compile-time symbols that specify big- or little-endian, which we really
  want.
*/
#include <ctype.h>
 
/*
  A bunch of standard definitions.
*/
#include "dylib_std.h"
 
/*
  In a few rare instances, the declarations here will be unused, but for dylp
  this is a good bet.
*/
#include <math.h>

/*
  Some subset of these will work on any system. Check config_dylp.h to see
  which ones are actually in use.
*/
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
# include <ieeefp.h>
#endif
#ifdef HAVE_SUNMATH_H
# include <sunmath.h>
#endif

 
/*
  The Theory: quiet_nan is used to indicate failure (by returning NaN)
  without triggering a signal the client may not be prepared to catch. The
  idea is that any reasonable checks in the client will detect NaN fairly
  quickly.  signalling_nan is used when there's no advantage in delaying a
  signal.
 
  The Reality: Neither seems to trigger a signal, and many computing
  environments can't tell the difference. But it's coded into dylp, and it'd
  be much ado to change. Hence the compile-time ugliness that follows.
 
  In the Sun Workshop environment, quiet_nan and signalling_nan are declared
  in sunmath.h and found in libsunmath. With release 5.0, sunmath.h includes
  some declarations of type `long long', which isn't supported under the -Xc
  (strict ANSI compatibility) option for cc. So, we extract only the
  definitions we need.  Unfortunately, sunmath.h is present only in the Sun
  Workshop programming environment. Sun without Workshop has only the
  require file nan.h, which is inadequate.
 
  For a long while, GNU C didn't distinguish QNaN and SNaN. More recently,
  its support for IEEE 754 seems to have improved, but it's not clear that we
  can count on everyone having a recent GCC environment just yet. Here, too,
  nan.h is inadequate.  The easy way out is to simply #define them as macros
  that return the proper bit pattern. Arguably this would make more sense in
  general than Sun's implementation as functions.
 
  According to IEEE 754, the proper bit patterns are:
 
    0x7ff00000 00000000                  for Inf
    0x7fffffff ffffffff                  for QNaN
    0x7ff00000 00000001                  for SNaN
   
  It works this way: The IEEE definition of NaN is
    Bits         Value
    63           sign --- don't care for a NaN, but nice to be positive (0)
    62:52        exponent --- must be maximum value, 0x7ff
    51:0         fraction --- must not be zero (a fraction of zero is the
                 representation of infinity). Sun documentation defines QNaN
                 as having bit 51 of the fraction set to 1, SNaN as having
                 bit 51 set to 0.
 
  Creating the proper constants qualifies as a serious gross hack. And if you
  have a little-endian machine (the 80x86 family being far and away the most
  common example), you need to flip the byte order.
*/
  typedef union { unsigned char fpchr[8] ; double fpdbl ; } fpunion_t ;
/*
  Yes, all this really is needed to get all the various compilers to quit
  complaining. We need the `(unsigned char)' to prevent some compilers from
  complaining about the initialiser being out of range. Goes to the ANSI C
  rule that `Character constants not preceded by the letter L have type int.'
*/
#ifdef WORDS_BIGENDIAN
    static fpunion_t QNaNbits UNUSED = { { (unsigned char) '\177',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\376' } } ;
    static fpunion_t SNaNbits UNUSED = { { (unsigned char) '\177',
                                        (unsigned char) '\360',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\001' } } ;
    static fpunion_t Infbits UNUSED = { { (unsigned char) '\177',
                                       (unsigned char) '\360',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0' } } ;
#else
    static fpunion_t QNaNbits UNUSED = { { (unsigned char) '\376',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                       (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\377',
                                        (unsigned char) '\177' } } ;
    static fpunion_t SNaNbits UNUSED = { { (unsigned char) '\001',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\0',
                                        (unsigned char) '\360',
                                        (unsigned char) '\177' } } ;
    static fpunion_t Infbits UNUSED = { { (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\0',
                                       (unsigned char) '\360',
                                       (unsigned char) '\177' } } ;
#endif /* WORDS_BIGENDIAN */

/*
  If we didn't find a quiet_nan function, fake it with a macro.
*/

#ifndef DYLP_HAS_QUIET_NAN
# define quiet_nan(zz_dummy_zz) (QNaNbits.fpdbl)
#endif 

/*
  On some machines, HUGE_VAL isn't actually IEEE infinity. Make sure that
  it really is IEEE infinity.
*/
 
#undef HUGE_VAL
#define HUGE_VAL (Infbits.fpdbl)

/*
  In a Sun/Solaris environment, the definitions and functions that support
  IEEE floating point are in ieeefp.h. This seems to be true even if GNU
  compilers are being used instead of Sun Workshop compilers. In a GNU/Linux
  environment, the necessary definitions seem to live in math.h. The upshot
  is that we need to explicitly pull in ieeefp.h here for a Sun environment.
 
  In a Microsoft environment the correct functions look to be _finite and
  _isnan from float.h.

  Assign the proper names to finite and isnan, based on the values deduced by
  configure. Again, check config_dylp to see the actual names. If either name
  is already defined, bet that it's the correct definition.
*/

#ifndef finite
# define finite DYLP_ISFINITE
#endif
#ifndef isnan
# define isnan  DYLP_ISNAN
#endif

#endif

/*
  Packed Vectors
 
  The packed vector type consists of a header plus an array of <index, value>
  pairs for the non-default entries of the vector.
 
  pkcoeff_struct
 
  Field		Description
  -----         -----------
  ndx           the column/row index for the coefficient
  val           the value of the coefficient
 
  pkvec_struct
 
  Field		Description
  -----         -----------
  ndx           the common index for all coefficients when the vector is a
  		row or column from a matrix
  nme           name associated with this vector, if any
  dim           length of the vector when unpacked
  dflt          the default value of coefficients not in coeffs
  cnt           number of non-default coefficients in the coeffs array
  sze           allocated capacity (in pkcoeff_struct's) of the coeffs array
  coeffs	the array of (column/row index, coefficient) pairs
 
  NOTE: pkvec_struct->coeffs is indexed from 0 and sized accordingly.
*/
 
typedef struct { int ndx ;
                   double val ; } pkcoeff_struct ;
 
typedef struct { int ndx ;
                   const char *nme ;
                   int dim ;
                   double dflt ;
                   int cnt ;
                   int sze ;
                   pkcoeff_struct *coeffs ; } pkvec_struct ;
 
pkvec_struct *pkvec_new(int sze) ;
bool pkvec_resize(pkvec_struct *pkvec, int sze) ;
void pkvec_free(pkvec_struct *pkvec) ;
 
bool pkvec_check(pkvec_struct *pkvec, const char *caller) ;
 
double pkvec_2norm(pkvec_struct *vec) ;
 
double exvec_1norm(double *vec, int len),
       exvec_ssq(double *vec, int len),
       exvec_2norm(double *vec, int len),
       exvec_infnorm(double *vec, int len, int *p_jmax) ;
 
double pkvec_dotexvec(pkvec_struct *pkvec, double *exvec) ;
 
#endif /* _DYLP_VECTOR_H */
