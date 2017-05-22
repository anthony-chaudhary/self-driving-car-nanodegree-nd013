/* $Id: OShL.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file OShL.h
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


#ifndef OSHL_H
#define OSHL_H

#include <string>


/*! \class OShL OShL.h "OShL.h"
 *  \brief An interface that specified virtual methods to be implemented by agents.
 *
 * \remarks
<p> This is a virtual class that lists all of the methods a client
(or scheduler/solver)  should implement</p>
 *
 *
 */
class OShL
{
public:

    /**
     *
     * Default constructor.
     */
    OShL();

    /**
     *
     * Class destructor.
     */
    virtual	~OShL() = 0;

    /**
     * submit an instance with its options for a synchronous solution
     *
     * <p>
     * @param osil is the string with the instance in OSiL format
     * @param osol is the string with the options in OSoL format
     * @return a string which is the result in OSrL format.
     * </p>
     */
    virtual std::string solve(std::string osil, std::string osol) = 0;

    /**
     * get a jobID for use in the send method
     *
     * <p>
     * @param osol is the string with the options in OSoL format
     * @return a string which is the jobID
     * </p>
     */
    virtual std::string getJobID(std::string osol) = 0;

    /**
     * submit an instance with its options for an asynchronous solution
     *
     * <p>
     * @param osil is the string with the instance in OSiL format
     * @param osol is the string with the options in OSoL format
     * @return a bool which is true if the job is successfully submitted
     * </p>
     */
    virtual bool send(std::string osil, std::string osol) = 0;

    /**
     * kill an instance that is running
     *
     * <p>
     * @param osol is the string with the options in OSoL format
     * @return a string which is in OSpL format
     * </p>
     */
    virtual std::string kill(std::string osol) = 0;

    /**
     * retrieve an instance result that ran in asynchronous mode
     *
     * <p>
     * @param osol is the string with the options in OSoL format
     * @return a string which is in the result of the optimization is OSrL fomrat
     * </p>
     */
    virtual std::string retrieve(std::string osol) = 0;

    /**
     * knock to get information on the current status of a job
     *
     * <p>
     * @param ospl is the string with the process information in OSpL format
     * @param osol is the string with the options in OSoL format
     * @return a string which is the knock result in OSpL format.
     * </p>
     */
    virtual std::string knock(std::string ospl, std::string osol) = 0;
};

#endif

