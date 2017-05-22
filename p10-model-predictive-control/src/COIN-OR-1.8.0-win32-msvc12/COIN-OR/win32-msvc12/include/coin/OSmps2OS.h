/* $Id: OSmps2OS.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file OSmps2OS.h
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2013, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */


#ifndef OSMPS2OS_H
#define OSMPS2OS_H


#include <CoinMpsIO.hpp>
#include <CoinPackedMatrix.hpp>
#include <string>
#include "OSInstance.h"
#include "OSOption.h"
#include "OSoLReader.h"


/*! \class OSmps2OS
 *  \brief The OSmps2OS  Class.
 *
 * @author Robert Fourer, Jun Ma, Kipp Martin
 *
 * \remarks
 * the OSmps2osil class is used for reading an instance in MPS format
 * and creating an OSInstance object in OSiL format
 * and possibly an OSOption object in OSoL format (if the MPS file
 * contains nonstandard sections such as SOS)
 */
class OSmps2OS
{
public:
    /** the OSmps2OS class constructor */
    OSmps2OS( std::string mpsfilename);

    /** the OSmps2os class destructor */
    ~OSmps2OS();

    /** set the osol string */
    void setOsol(std::string osol);

    /** set the job ID */
    void setJobID(std::string jobID);

    /**
     * create an OSInstance from the MPS instance representation
     * and an OSOption in case of nonstandard sections such as SOS
     *
     * @return whether the objects are created successfully.
     */
    bool createOSObjects();

    /** osinstance is a pointer to the OSInstance object that gets
     * created from the instance represented in MPS format
     */
    OSInstance *osinstance;

    /** osoption is a pointer to an OSOption object that gets created
     *  if the MPS file contains nonstandard sections such as SOS
     */
    OSOption *osoption;

    /**
     * we may need to parse an OSoL file if the MPS file contains
     * an SOS os BASIS section
     */
    OSoLReader *osolreader;

    /** osol is a string containing the content of the OS option file
     *  (it may be empty if no option file was provided by the user). 
     *  If osoption is NULL, the option information is found in osol.
     */
    std::string osol;

    /** jobID is a string containing a jobID that may have been supplied
     *  on the command line (it may be empty). 
     *  If osoption is not NULL, the jobID has been duplicated to osoption.
     */
    std::string jobID;
private:
    /** m_MpsData is a pointer to a CoinMpsIO object */
    CoinMpsIO *m_MpsData;

    /**  m_CoinPackedMatrix is a pointer to a CoinPackedMatrix object */
    CoinPackedMatrix *m_CoinPackedMatrix;

    /** Data structures to hold special ordered sets */
    int m_nOfSOS;
    CoinSet ** m_SOS;

    /** Data structures to hold the quadratic objective */ 
    int * m_quadColumnStart;
    int * m_quadColumnIdx;
    double * m_quadElements;

    /** Data structures to hold the cone information */
    int m_nOfCones;
    int * m_coneStart;
    int * m_coneIdx;
    int * m_coneType;
};
#endif

