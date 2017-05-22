/* $Id: OSiLReader.h 4829 2014-07-28 19:14:53Z Gassmann $ */
/** @file OSiLReader.h
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


#ifndef OSILREADER_H
#define OSILREADER_H

#include "OSInstance.h"
#include "OSiLParserData.h"
#include "OSgLParserData.h"
#include "OSnLParserData.h"
#include "OSErrorClass.h"

#include <string>


/*! \class OSiLReader
 *  \brief Used to read an OSiL string.
 *
 * \remarks
 * <p> This class wraps around the OSiL parser and sends
 * the parser an OSiL string and is returned an OSInstance
 * object. </p>
 */
class OSiLReader
{

public:

    /**
     * Default constructor.
     */
    OSiLReader( );

    /**
     * Class destructor.
     */
    ~OSiLReader();

    /**
     * parse the OSiL model instance.
     *
     * <p>
     * @param osil a string that holds the problem instance.
     * @return the instance as an OSInstance object.
     * </p>
     */
    OSInstance* readOSiL(const std::string& osil) throw(ErrorClass);

private:
    /** m_osinstance is the OSInstance object returned by
     * the OSiLReader
     */
    OSInstance *m_osinstance;

    /** m_parserData is the object used in the parser to temporarily store data
     */
    OSiLParserData *m_parserData;

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
