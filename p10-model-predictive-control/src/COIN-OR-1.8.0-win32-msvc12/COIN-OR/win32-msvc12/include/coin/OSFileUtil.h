/* $Id: OSFileUtil.h 4926 2015-01-28 21:50:12Z Gassmann $ */
/** @file FileUtil.h
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2015, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

#ifndef FILEUTIL_H
#define FILEUTIL_H


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

/*!  \class FileUtil
 *  \brief class used to make it easy to read and write files.
 *
 * @author Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * <p>The <code>FileUtil</code> class contains methods for reading
 * and writing files from strings used by many classes in the
 * Optimization Services (OS) framework. </p>
 *
 */
class FileUtil
{
public:
public:

    /** the class constructor */
    FileUtil();

    /** the class destructor */
    ~FileUtil();

    /**
     * read a file and return contents as a string.
     *
     * @param fname holds the name of the file.
     * @return the file contents as a string.
     */
    std::string getFileAsString(const  char* fname);

    /**
     * read a file and return contents as a char pointer.
     *
     * @param fname holds the name of the file.
     * @return the file contents as a char pointer.
     */
    char* getFileAsChar(const  char* fname);

    /**
     * write a file from an input string.
     *
     * @param fname holds the name of the file to be written.
     * @param thestring holds the string to be written to the file.
     * @return true if file successfuly written.
     */
    bool writeFileFromString(char* fname, std::string thestring);

    /**
     * write a file from an input string.
     *
     * @param fname holds the name of the file to be written.
     * @param thestring holds the string to be written to the file.
     * @return true if file successfuly written.
     */
    bool writeFileFromString(std::string fname, std::string thestring);

    /**
     * write a file from an input char pointer.
     *
     * @param fname holds the name of the file to be written.
     * @param ch holds a pointer to a char array to be written to the file.
     * @return true if file successfully written.
     */
    bool writeFileFromChar(char* fname, char* ch);
};//class FileUtil
#endif
