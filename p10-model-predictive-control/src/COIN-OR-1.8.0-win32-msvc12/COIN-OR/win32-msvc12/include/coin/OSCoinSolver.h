/* $Id: OSCoinSolver.h 4562 2013-01-02 12:31:12Z Gassmann $ */
/** @file CoinSolver.h
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

#ifndef COINSOLVER_H
#define COINSOLVER_H

#include "OSConfig.h"
#include "OSDefaultSolver.h"
#include "OSrLWriter.h"
#include "OSErrorClass.h"
#include "OSiLReader.h"
#include "OSoLReader.h"
#include "CbcModel.hpp"

#include <string>

class OsiSolverInterface;
class CoinPackedMatrix;

/*! \class CoinSolver class.h "CoinSolver.h"
 *  \brief Implements a solve method for the Coin solvers.
 *
 * This class implements a solve method for the Coin solvers
 * It reads an OSInstance object and puts into the Coin OSI format
 */
class CoinSolver : public DefaultSolver
{

public:

    /*! \fn  CoinSolver::CoinSolver()
     *  \brief The class constructor.
     */
    CoinSolver();

    /*! \fn  CoinSolver::~CoinSolver()
     *  \brief The class destructor.
     */
    ~CoinSolver();

    /*! \fn void CoinSolver::solve()
     *  \brief The implementation of the corresponding virtual function.
     *  \return void.
     */
    virtual void  solve() throw(ErrorClass);

    /*! \fn void CoinSolver::buildSolverInstance()
     *  \brief The implementation of the corresponding virtual function.
     *  \return void.
     */
    virtual void  buildSolverInstance() throw(ErrorClass);

    /*! \fn void setSolverOptions()
     *  \brief The implementation of the corresponding virtual function.
     *  \return void.
     */
    virtual void  setSolverOptions() throw(ErrorClass);


    /*! \fn bool CoinSolver::setCoinPackedMatrix()
     *  \brief  Create a CoinPackedMatrix
     *  \return true if a CoinPackedMatrix successfully created.
     */
    bool setCoinPackedMatrix();

    /*! \fn string CoinSolver::getCoinSolverType(string osol_)
     *  \brief  Get the solver type, e.g. clp or glpk
     *  \param  a string that is an instance of OSoL
     *  \return a string which contains the value of clp or glpk.
     */
    std::string getCoinSolverType(std::string osol_);

    /*! \fn string CoinSolver::dataEchoCheck()
     *  \brief Print out problem parameters
     *  \return void
     */
    void dataEchoCheck();

    /**
     * osiSolver is the osi solver object -- in this case clp, glpk, cbc, cplex, symphony or dylp
     */
    OsiSolverInterface *osiSolver;



    /**
     * m_osilreader is an OSiLReader object used to create an osinstance from an
     * osil string if needed
     */
    OSiLReader *m_osilreader;

    /**
     * m_osolreader is an OSoLReader object used to create an osoption from an
     * osol string if needed
     */
    OSoLReader *m_osolreader;


    void writeResult(OsiSolverInterface *solver);

    // use this for when we solve with Cbc --AND-- have integer variables
    void writeResult(CbcModel *model);



private:





    /**
     * m_CoinPackedMatrix is a Coin Packed Matrix ojbect
     */
    CoinPackedMatrix *m_CoinPackedMatrix ;

    /** osrlwriter object used to write osrl from an OSResult object */
    OSrLWriter  *osrlwriter;

    /** when Cbc is the solver, these are the arguments sent to
     * Cbc Solve
     */
    const char **cbc_argv;

    /** the number of arguments in the argument list to the Cbc Solver
     */
    int num_cbc_argv;

    double cpuTime;

};
#endif
