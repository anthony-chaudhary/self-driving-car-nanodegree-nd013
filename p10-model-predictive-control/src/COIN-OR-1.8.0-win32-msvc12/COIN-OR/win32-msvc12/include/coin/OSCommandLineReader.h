/* $Id: OSCommandLineReader.h 4263 2011-09-09 20:19:26Z Gassmann $ */
/** @file OSCommandLineReader.h
 *
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2011-2013, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */


#ifndef _OSCOMMANDLINEREADER_H
#define _OSCOMMANDLINEREADER_H


#include "OSCommandLine.h"
#include "OSErrorClass.h"


#include <string>


/*! \class OSCommandLineReader
 *  \brief The OSCommandLineReader Class.
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * A class for parsing a command line string and creating an
 * OSCommandLine object from the string.
 * This method can be used in OSSolverService, OSAmplClient, 
 * as well as the interactive shell 
 */
class OSCommandLineReader
{


public:

    /**
     *
     * OSCommandLineReader class constructor.
	 * @param osss is the command line to be parsed
	 *
     */
    OSCommandLineReader( );

    /**
     *
     * OSCommandLineReader class destructor.
     */
    ~OSCommandLineReader();


    /**
     * Get an OSCommandLine object from a command line string.
     *
     * @param osss a command line string.
     * @return the OSCommandLine object corresponding to the command line string.
     * @remark Calls method parseString once and if a configFile item is found
     *         calls method parseString two more times (with the config file contents
     *         and again with the original command line) 
     */
    OSCommandLine *readCommandLine(const std::string& osss) throw(ErrorClass);


    /**
     * Parse a string and store it into an OSCommandLine object
     *
     * @param osss a command line string.
     * @return the OSCommandLine object corresponding to the command line string.
     */
    OSCommandLine *parseString(const std::string& osss) throw(ErrorClass);

private:
    /** m_oscommandline is the OSCommandLine object returned by
     * the OSCommandLineReader
     */
    OSCommandLine *m_oscommandline;
};
#endif
