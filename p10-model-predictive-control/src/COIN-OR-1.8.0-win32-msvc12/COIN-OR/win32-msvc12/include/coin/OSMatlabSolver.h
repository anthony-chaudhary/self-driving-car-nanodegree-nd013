/* $Id: OSMatlabSolver.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file OSMatlab.h
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

#ifndef OSMATLAB_H
#define OSMATLAB_H
//
//OS headers
#include "OSCoinSolver.h"
#include "OSResult.h"
#include "OSiLReader.h"
#include "OSiLWriter.h"
#include "OSInstance.h"
#include "OSDefaultSolver.h"
#include "OSDataStructures.h"
#include "OSSolverAgent.h"
#include "OSErrorClass.h"

//end OS headers

#include <string>

/*! \class OSMatlab
 *  \brief The OSMatlab  Class.
 *
 * @author Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * the OSMatlab class is used by the matlabSolver
 * mex (Matlab EXecutable) which takes MATLAB data
 * structures and creats an OSiL string.
 *
 * for now we can only handle linear integer and
 * quadratic programming problems, not general
 * nonlinear problems
 */
class OSMatlab
{

public:

    /** the OSMatlab class constructor */
    OSMatlab();

    /** the OSMatlab class destructor */
    ~OSMatlab() ;

    /** sparseMat is a pointer to an OS
     * Sprase Matrix data structure
     */
    SparseMatrix *sparseMat;

    /** bl is a pointer to the lower bounds
     * on the constraints
     */
    double *bl;

    /** bu is a pointer to the upper bounds
     * on the constraints
     */
    double *bu;

    /** obj is a pointer to the objective function
     * coefficients
     */
    double *obj;

    /** vl is a pointer to the lower bounds
     * on the varialbes
     */
    double *vl;

    /** vu is a pointer to the upper bounds
     * on the variables
     */
    double *vu;

    /** numVar is the number of variables
     * in the problem
     */
    int numVar;

    /** numCon is the number of constraints
     * in the problem
     */
    int numCon;

    /** varType is a pointer to the variable type
     * eg C, B, I
     */
    char *varType;

    /** objType indicates whether or not we have a max (1)
     * or a min (0)
     */
    bool objType;

    /** numQTerms is the number of quadratic terms */
    int numQTerms;

    /** qRows is a pointer to the row index of each
     * quadratic term
     */
    int *qRows;

    /** qIndex1 is a pointer to the index of the first
     * variable in each of the quadratic terms
     */
    int *qIndex1;

    /** qIndex2 is a pointer to the index of the second
     * variable in each of the quadratic terms
     */
    int *qIndex2;

    /** qVal is a pointer to the coefficient value of each
     * of the quadratic terms.
     */
    double *qVal;

    /** solverType is the a pointer to the sovler that will
     * be requested
     */
    DefaultSolver *solverType;

    /** instanceName is the name of the problem instance */
    std::string instanceName;

    /** sSolverName is the name of the solver */
    std::string sSolverName;

    /** is the address of the solver service */
    std::string sAgentAddress;

    /**
     * Solve the problem instance
     *
     * @return a string with the solution in OSrL format
     */
    std::string solve();

    /**
     * Create an OSInstance
     */
    void createOSInstance();

    /** osinstance is a pointer to an OSInstance
     * object that gets created from the MATLAB
     * data structures
     */
    OSInstance *osinstance;

    /** is the osil instance that gets created from
     * the MATLAB data structures
     */
    std::string osil;

};//OSMatlabclass

#endif
