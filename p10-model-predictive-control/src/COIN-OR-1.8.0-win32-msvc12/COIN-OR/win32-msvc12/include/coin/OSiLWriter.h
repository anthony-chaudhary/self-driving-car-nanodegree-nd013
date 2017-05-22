/* $Id: OSiLWriter.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file OSiLWriter.h
 *
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
 */


#ifndef OSiLWRITER_H
#define OSiLWRITER_H

#include <string>
#include "OSInstance.h"

/*! \class OSiLWriter OSiLWriter.h "OSiLWriter.h"
 *  \brief Take an OSInstance object and write
 * a string that validates against the OSiL schema.
 *
 *
 */
class OSiLWriter
{
private:
    /**
     * m_OSInstance is an object in the class OSInstance
     */
    const OSInstance *m_OSInstance;
public:

    /**
     *
     * Default constructor.
     */
    OSiLWriter( );

    /**
     *
     * Class destructor.
     */
    ~OSiLWriter();

    /**
     * create an osil string from an OSInstance object
     * <p>
     * @param theosinstance is a pointer to an OSInstance object
     * @return a string with the OSInstance data that validates against the OSiL schema.
     * </p>
     */
    std::string writeOSiL( const OSInstance *theosinstance);



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
