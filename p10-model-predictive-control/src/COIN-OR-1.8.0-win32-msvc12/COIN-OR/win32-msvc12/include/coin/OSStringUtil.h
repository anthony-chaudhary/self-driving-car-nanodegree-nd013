/* $Id: OSStringUtil.h 2698 2009-06-09 04:14:07Z kmartin $ */
/** @file OSStringUtil.h
 *
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin,
 *
 * \remarks
 * Copyright (C) 2010, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

/* There is no OSxL schema. Instead, OSxL is used to denote an arbitrary schema or the
 * entire collection of schemas. This file contains some utility routines that are used
 * in other specific schema writers, such as OSiLWriter, etc.
 */

#include <sstream>

/**
 * writeStringData
 *
 * Prepare and output a string that may contain special characters (single or double quotes)
 *
 * @param str holds the string to be output.
 * If the string does not contain double quotes, it is output surrounded by double quotes,
 * if the string contains double quotes, it is output surrounded by single quotes,
 * @return the prepared string, ready to be printed
 *
 */
std::string writeStringData(std::string str);

std::string makeStringFromInt(std::string theString, int theInt);

