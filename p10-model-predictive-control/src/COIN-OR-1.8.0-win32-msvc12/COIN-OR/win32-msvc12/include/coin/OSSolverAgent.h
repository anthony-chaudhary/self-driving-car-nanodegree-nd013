/* $Id: OSSolverAgent.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** \file OSSolverAgent.h
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

 */

#ifndef	OSSOLVERAGENT_H
#define OSSOLVERAGENT_H

#include "OShL.h"

/*! \class OSSolverAgent OSSolverAgent.h "OSSolverAgent.h"
 *  \brief Used by a client to invoke a remote solver.
 *
 * \remarks
	<p> This is an implementation of the virtual class OShL. We
	need to implement the following virtual methods. </p>

	<p><b>The following key methods are invoked:</b> </p>
	<ol>
	<li> solve </li>
	<li> kill </li>
	<li> send </li>
	<li> retrieve </li>
	<li> knock </li>
	<li> getJobID </li>
	</ol>
 *
 *
 */

class OSSolverAgent : public OShL
{
public:

    /**
     * Default constructor.
     * @param solverURI is the location of remote solver or scheduler
     */
    OSSolverAgent(std::string solverURI) ;

    /**
     * Class destructor.
     */
    ~OSSolverAgent();

    /**
     * implement the solve() method which is a virtual function in OShL, this is synchronous
     *
     * <p>
     * @param osil a string that holds the problem instance
     * @param osol is a string of options for the solver
     * @return osrl which is a string with the result.
     * </p>
     */
    std::string solve(std::string osil, std::string osol);

    /**
     * implement the getJobID() method which is a virtual function in OShL
     *
     * <p>
     * @param osol is the string with the options in OSoL format
     * @return a string which is the jobID
     * </p>
     */
    std::string getJobID(std::string osol);

    /**
     * implement the send() method which is a virtual function in OShL
     *
     * <p>
     * @param osil is the string with the instance in OSiL format
     * @param osol is the string with the options in OSoL format
     * @return a bool which is true if the job is successfuly submitted
     * </p>
     */
    bool send(std::string osil, std::string osol);

    /**
     * implement the kill() method which is a virtual function in OShL
     *
     * <p>
     * @param osol is the string with the options in OSoL format
     * @return a string which is in OSpL format
     * </p>
     */
    std::string kill(std::string osol);

    /**
     * implement the retrieve() method which is a virtual function in OShL
     *
     * <p>
     * @param osol is the string with the options in OSoL format
     * @return a string which is in the result of the optimization is OSrL fomrat
     * </p>
     */
    std::string retrieve(std::string osol);

    /**
     * implement the knock() method which is a virtual function in OShL
     *
     * <p>
     * @param ospl is the string with the process information in OSpL format
     * @param osol is the string with the options in OSoL format
     * @return a string which is the knock result in OSpL format.
     * </p>
     */
    std::string knock(std::string ospl, std::string osol);

    /**
     * implement the fileUpload() method which is a virtual function in OShL
     *
     * <p>
     * @param osilFileName is the name of the file
     * with the OSiL instance to be written on the server
     * @param osil is a string with the OSiL problem instance
     * </p>
     */
    std::string fileUpload(std::string osilFileName, std::string osil);

private:

    /** solverAddress is the URI for the solver*/
    std::string solverAddress;

    /** solverPortNumber is the port number for the solver */
    unsigned short solverPortNumber;

    /** postURI is the path to the solver that follows 
     * the first '/' in the solverAddress
     */
    std::string postURI;

}; //class OSSolverAgent

#endif
