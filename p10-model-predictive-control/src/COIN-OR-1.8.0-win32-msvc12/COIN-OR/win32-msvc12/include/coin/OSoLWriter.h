/* $Id: OSoLWriter.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file OSoLWriter.h
 *
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */


#ifndef OSoLWRITER_H
#define OSoLWRITER_H

#include "OSOption.h"
#include <string>

/*! \class OSoLWriter OSoLWriter.h "OSoLWriter.h"
 *  \brief Take an OSOption object and write
 *  a string that validates against the OSoL schema.
 *
 *
 */
class OSoLWriter
{
private:
    /**
     * m_OSOption is an object in the class OSOption
     */
    const OSOption *m_OSOption;
public:

    /**
     *
     * Default constructor.
     */
    OSoLWriter( );

    /**
     *
     * Class destructor.
     */
    ~OSoLWriter();

    /**
     * create an osol string from an OSOption object
     * <p>
     * @param theosoption is a pointer to an OSOption object
     * @return a string with the OSOption data that validates against the OSoL schema.
     * </p>
     */
    std::string writeOSoL( OSOption *theosoption);

    /** m_bWriteBase64 is set to true if we encode the linear constraint coefficients
     * in base64 binary
     */
    bool m_bWriteBase64;

    /** m_bWhiteSpace is set to true if we write white space in the file
     */
    bool m_bWhiteSpace;

    /** m_sB64encoded is a string of data (start, colIdx, rowIdx, or values) from
     * linear constraints coefficients encoded in base64 binary
     */
    std::string m_sB64encoded;
};
#endif
