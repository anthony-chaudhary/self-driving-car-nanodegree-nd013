/* $Id: OSCommandLine.h 4403 2011-12-08 14:03:28Z Gassmann $ */
/** @file OSCommandLine.h
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2011-2012, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */


#ifndef OSCOMMANDLINE_H
#define OSCOMMANDLINE_H

#include "OSInstance.h"
#include "OSOption.h"

#include <string>


/*! \class OSCommandLine
 *  \brief This class is used to store command line options for the
 *  OSSolverService executable and to provide methods to manipulate them.
 *
 *  @author Horand Gassmann, Jun Ma, Kipp Martin
 *
 *  \remarks
 *  the OSSolverService requires numerous options and these
 *  options are stored in the OSCommandLine class
 */

class OSCommandLine
{
public:
    /** osinstance is a representation of the instance in OSInstance format
     *
     */
    OSInstance *osinstance;

    /** osoption is a representation of the solver options in OSOption format
     *
     */
    OSOption *osoption;

    /** serviceLocation is the URL of the remote solver when
     *  a local solver is not used
     */
    std::string serviceLocation;

    /** the service method the OSSolverService should execute, i.e.
     *  solve, send, getJobID, kill, knock, or retrieve
     */
    std::string serviceMethod;

    /** the name of the solver to be invoked locally,
     *  e.g -solver Ipopt
     */
    std::string solverName;

    /** configFile is the name of the file that holds the configuration options
     *  if the OSSolverService reads its options from a file rather than command
     *  line inputs
     */
    std::string configFile;

    /** osilFile is the name of the file that holds the model
     *  instance in OSiL format
     */
    std::string osilFile;
    
    /** osil is the content of the osilFile 
     */
    std::string osil;

    /** osilOutputFile is the name of the file to which the instance can be
     *  written in OSiL format. This is especially useful for converting the
     *  instance from other representation formats such as AMPL nl format or MPS format.
     *  If this parameter is empty, the instance will not be saved.
     */
    std::string osilOutputFile;

    /** osolFile is the name of the file that holds the solver options
     *  in OSoL format
     */
    std::string osolFile;

    /** osol is the content of the osolFile
     */
    std::string osol;
    
    /** osolOutputFile is the name of the file to which the solver options can be
     *  written in OSoL format. This is especially useful when an instance represented
     *  in another representation format such as AMPL nl format or MPS format
     *  contains array-valued options such as initial values or basis information.
     *  If this parameter is empty, the solver options will not be saved.
     */
    std::string osolOutputFile;

    /** osrlFile is the name of the file where the solver should write
     *  the result (in OSrL format)
     */
    std::string osrlFile;

    /** name of the file containing the instance in LINDO instruction
     *  list format
     */
    std::string insListFile;

    /** insList is the content of the insListFile -- this is not implemented
     */
    std::string insList;

    /** name of an input file with xml in OS process language format,
     *  used for example to knock on a server, for example
     *  -osplInput ../data/osplFiles/demo.ospl
     */
    std::string osplInputFile;

    /** osplInput is the content of the osplInputFile
     */
    std::string osplInput;

    /** name of an output file where the solver should write the result of a
     *  knock or kill service request
     *
     */
    std::string osplOutputFile;

    /** the name of the file that holds an instance in MPS format */
    std::string mpsFile;

    /** the string that holds an instance in MPS format */
    std::string mps;

    /** the name of the file that holds an instance in AMPL nl format */
    std::string nlFile;

    /** the string that holds an instance in AMPL nl format */
    std::string nl;

    /** the name of the file that holds an instance in GAMS dat format */
    std::string datFile;

    /** the string that holds an instance in GAMS dat format */
    std::string dat;

    /** the name of the file that holds the GAMS control parameters */
    std::string gamsControlFile;

    /** this parameter is a path to the browser on the local machine.
     *  If this optional parameter is specified then the solver result
     *  in OSrL format is transformed using XSLT into HTML and displayed
     *  in the browser, e.g.
     *    -browser /Applications/Firefox.app/Contents/MacOS/firefox
     */
    std::string browser;

    /** this parameter controls the amount of output to print
     *  the higher the number, the more output is generated
     *  details about print levels can be found in OSOutput.h
     */
    int printLevel;

    /** this optional parameter contains the path to a logfile
     *  that can be used as an alternate output stream in addition
     *  to the normal output to stdout
     */
    std::string logFile;

    /** this parameter controls the amount of output to send
     *  to the log file (if used)
     *  the higher the number, the more output is generated
     *  details about print levels can be found in OSOutput.h
     */
    int filePrintLevel;

    /** the JobID */
    std::string jobID;

    /** if this parameter is true we print the contents of the file
     *  help.txt and return
     */
    bool invokeHelp;

    /** if this parameter is true we echo the values of the options
     *  found on the command line
     */
    bool listOptions;

    /** if this parameter is true we print the current version
     *  of the OS project
     */
    bool writeVersion;
    
    /** if this parameter is true we print the current instance
     *  as read from an osil, nl or mps file
     */
    bool printModel;

    /** this parameter contains a string representation (!) of 
     *  the row number if only a single row (constraint or objective)
     *  of the current instance is to be printed
     *  String representations are easier to parse in OSParseosss.l and
     *  are easier to recognize as being present or absent
     */
    std::string printRowNumberAsString;

    /** if this parameter is true we quit/exit
     *  - only used in the interactive shell
     */
    bool quit;

    /** constructor method */
    OSCommandLine();
    
    /** destructor method */
    ~OSCommandLine();

    /** a function to reset the command line to default values
     *  useful especially in the interactive shell
     */
    void reset_options();

    /** a function to print the current command line option values
     */
    std::string list_options();

    /** to avoid ambiguity it might be necessary to convert the
     *  solver name to lower case ...
     */
    void convertSolverNameToLowerCase();

    /** ... or to upper case
     */
    void convertSolverNameToUpperCase();
};

#endif

/*  end header file */
