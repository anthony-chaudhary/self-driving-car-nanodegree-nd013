/* $Id: OSrLWriter.h 4263 2011-09-09 20:19:26Z Gassmann $ */
/** @file OSrLWriter.h
 *
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Dalhousie University, Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */


#ifndef OSrLWRITER_H
#define OSrLWRITER_H

#include "OSResult.h"
#include <string>



/*! \class OSrLWriter OSrLWriter.h "OSrLWriter.h"
 *  \brief Take an OSResult object and write a string that validates against OSrL.
 *
 *
 */
class OSrLWriter
{
private:
    /**
     * m_OSResult is an object in the class OSResult, the data
     * in m_OSResult are written to a string that validates against OSrL
     */
    OSResult *m_OSResult;
public:

    /**
     *
     * Default constructor.
     */
    OSrLWriter( );

    /**
     *
     * Class destructor.
     */
    ~OSrLWriter();

    /**
     * create an osrl string from an OSResult object
     * <p>
     * @param theosresult is a pointer to an OSResult object
     * @return a string with the OSResult data that validates against the OSrL schema.
     * </p>
     */
    std::string writeOSrL( OSResult *theosresult);


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
    /**
     * create an osrl char* from an OSResult object
     * necessary kludge since on Linux the catch doesn't like strings
     * <p>
     * @param theosresult is a pointer to an OSResult object
     * @return a string with the OSResult data that validates against the OSrL schema.
     * </p>
     */
    //char* writeOSrLWrap( OSResult *theosresult);
};
#endif
