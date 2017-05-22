/* $Id: OSoLReader.h 4820 2014-07-09 14:17:48Z Gassmann $ */
/** @file OSoLReader.h
 *
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin,
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */


#ifndef OSOLREADER_H
#define OSOLREADER_H

#include <string>

#include "OSErrorClass.h"
#include "OSOption.h"
#include "OSoLParserData.h"
#include "OSgLParserData.h"
#include "OSnLParserData.h"


/*! \class OSoLReader
 *  \brief Used to read an OSoL string.
 *
 * \remarks
 * <p> This class wraps around the OSoL parser and sends
 * the parser an OSoL string and is returned an OSOption
 * object. </p>
 */
class OSoLReader
{

public:

    /**
     * Default constructor.
     */
    OSoLReader( );

    /**
     * Class destructor.
     */
    ~OSoLReader();

    /**
     * parse the OSoL solver options.
     *
     * <p>
     * @param osol is a string that holds the solver options.
     * @return the instance as an OSOption object.
     * </p>
     */
    OSOption *readOSoL(const std::string& osol) throw(ErrorClass);

private:
    /** m_osoption is the OSOption object returned by
     * the OSoLReader
     */
    OSOption *m_osoption;

    /** m_parserData is the object used in the parser to temporarily store data
     */
    OSoLParserData *m_parserData;

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
