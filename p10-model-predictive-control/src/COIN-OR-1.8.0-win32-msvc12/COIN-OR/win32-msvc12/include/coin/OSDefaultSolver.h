/* $Id: OSDefaultSolver.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file DefaultSolver.h
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



#ifndef DEFAULTSOLVER
#define DEFAULTSOLVER


#include "OSInstance.h"
#include "OSResult.h"
#include "OSOption.h"

#include <string>

/*! \class DefaultSolver DefaultSolver.h "DefaultSolver.h"
 *  \brief The Default Solver Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin,
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 */
class DefaultSolver
{

public:



    /** osil holds the problem instance as a std::string */
    std::string osil;

    /** osol holds the options for the solver */
    std::string osol;


    /** osrl holds the solution or result of the model */
    std::string osrl;


    /** osinstance holds the problem instance in-memory as an OSInstance object */
    OSInstance *osinstance;


    /** osoption holds the solver options in-memory as an OSOption object */
    OSOption  *osoption;

    /** osresult holds the solution or result of the model in-memory as an OSResult object */
    OSResult  *osresult;



    /**
     * sSolverName is the name of the Coin solver used, e.g. glpk, or clp
     */
    std::string sSolverName;


    /**
     * bCallbuildSolverInstance is set to true if buildSolverService has
     * been called
     */
    bool bCallbuildSolverInstance;


    /**
     * bSetSolverOptions is set to true if setSolverOptions has
     * been called, false otherwise
     */
    bool bSetSolverOptions;


    /** solve is a virtual function -- the actual solvers will
     * implement their own solve method
     */
    virtual void solve() = 0 ;

    /** buildSolverInstance is a virtual function -- the actual solvers will
     * implement their own buildSolverInstance method  -- the solver instance is the instance
     * the individual solver sees in its API
     */
    virtual void buildSolverInstance() = 0 ;

    /** setSolverOptions is a virtual function -- the actual solvers will
     * implement their own setSolverOptions method  -- the solver options are the options
     * the individual solver sees in its API
     */
    virtual void setSolverOptions() = 0 ;

    /**
     * default constructor.
     */
    DefaultSolver();

    /**
     * default destructor.
     */
    virtual	~DefaultSolver() = 0;







};


#endif
