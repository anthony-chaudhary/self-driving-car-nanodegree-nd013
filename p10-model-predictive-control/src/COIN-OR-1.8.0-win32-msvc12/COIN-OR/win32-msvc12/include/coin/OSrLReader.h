/* $Id: OSrLReader.h 4820 2014-07-09 14:17:48Z Gassmann $ */
/** @file OSrLReader.h
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


#ifndef _OSRLREADER_H
#define _OSRLREADER_H


#include "OSResult.h"
#include "OSrLParserData.h"
#include "OSgLParserData.h"
#include "OSnLParserData.h"
#include "OSErrorClass.h"


#include <string>


/*! \class OSrLReader
 *  \brief The OSrLReader Class.
 *
 * @author Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for parsing an OSrL string and creating an
 * OSResult object from the string.
 */
class OSrLReader
{


public:

    /**
     *
     * OSrL class constructor.
     */
    OSrLReader( );

    /**
     *
     * OSrL class destructor.
     */
    ~OSrLReader();


    /**
     * Get an OSResult object from an OSrL string.
     *
     * @param osrl an OSrL string.
     * @return the OSResult object corresponding to the OSrL string.
     */
    OSResult *readOSrL(const std::string& posrl) throw(ErrorClass);

private:
    /** m_osresult is the OSResult object returned by
     * the OSrLReader
     */
    OSResult *m_osresult;

    /** m_parserData is the object used in the parser to
     * temporarily store data from the OSrL schema elements.
     */
    OSrLParserData *m_parserData;

    /** m_osglData is the object used in the parser to
     * temporarily store data from the OSgL schema elements.
     */
    OSgLParserData *m_osglData;

    /** m_osnlData is the object used in the parser to
     * temporarily store data from the OSnL schema elements.
     */
    OSnLParserData *m_osnlData;
};
#endif
