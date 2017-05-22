/* $Id: OSdtoa.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file MathUtil.h
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 *
 * \remarks
 * Copyright (C) 2005-2011, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 * <p>The <code>MathUtil</code> class contains methods for performing
 * mathematics related operations used by many classes in the
 * Optimization Services (OS) framework. </p>
 *
 */


#ifndef OSDTOA_H
#define OSDTOA_H

#ifdef __cplusplus
extern "C" {
#endif

    double os_strtod(const char *str, char **strEnd );
    char *os_dtoa(double d, int mode, int ndigits, int *decpt, int *sign, char **rve);
    void os_freedtoa(char *s);

#ifdef __cplusplus
}
#endif

#endif // end  OSDTOA_H
