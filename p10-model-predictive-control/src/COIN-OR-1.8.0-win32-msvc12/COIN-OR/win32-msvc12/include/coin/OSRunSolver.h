/* $Id: OSRunSolver.h 4263 2011-09-09 20:19:26Z Martin $ */
/** @file OSRunSolver.h
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin,
 *
 * \remarks
 * Copyright (C) 2005-2013, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

#ifndef OSRUNSOLVER_H
#define OSRUNSOLVER_H

#include "OSDefaultSolver.h"

/**
 *  This class is used to invoke a solver locally
 */

/**
 *  A wrapper around the solve() method
 *  @param solverName: The name of the solver selected by the user
 *      If empty, a default solver is selected
 *  @param osol: A string containing the user options in osol format
 *  @param osinstance: A pointer to an OSInstance object containing the instance to be optimized
 *  @return the solution (or error message) in OSrL format
 */
std::string runSolver(std::string solverName, 
                      std::string osol, OSInstance *osinstance);

/**
 *  Alternate signature for this method
 *  @param solverName: The name of the solver selected by the user
 *      If empty, a default solver is selected
 *  @param osoption: A pointer to an OSOption object containing the options to be passed to the solver
 *  @param osil: A string containing the instance to be optimized
 *  @return the solution (or error message) in OSrL format
 */
std::string runSolver(std::string solverName, 
                      OSOption *osoption, std::string osil);

/**
 *  Alternate signature for this method
 *  @param solverName: The name of the solver selected by the user
 *      If empty, a default solver is selected
 *  @param osol: A string containing the user options in osol format
 *  @param osil: A string containing the instance to be optimized
 *  @return the solution (or error message) in OSrL format
 */
std::string runSolver(std::string solverName, 
                      std::string osol, std::string osil);

/**
 *  Alternate signature for this method
 *  @param solverName: The name of the solver selected by the user
 *      If empty, a default solver is selected
 *  @param osoption: A pointer to an OSOption object containing the options to be passed to the solver
 *  @param osinstance: A pointer to an OSInstance object containing the instance to be optimized
 *  @return the solution (or error message) in OSrL format
 */
std::string runSolver(std::string solverName, 
                      OSOption *osoption, OSInstance *osinstance);

/**
 *  A method to select the solver
 *  @param solverName: The name of the solver selected by the user
 *      If empty, a default solver is selected based on the characteristics of the problem
 *  @param osinstance: A pointer to an OSInstance object containing the instance to be optimized
 *  @return a pointer to the selected solver or NULL if no such solver exists on the system
 */
DefaultSolver* selectSolver(std::string solverName, OSInstance *osinstance);

#endif
