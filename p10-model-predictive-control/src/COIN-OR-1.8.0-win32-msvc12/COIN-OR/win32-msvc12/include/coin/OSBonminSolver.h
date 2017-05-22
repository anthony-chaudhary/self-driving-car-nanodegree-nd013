/* $Id: OSBonminSolver.h 4513 2012-07-08 20:15:42Z Gassmann $ */
/** @file OSBonmnSolver.h
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin,
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

#ifndef BONMINSOLVER_H
#define BONMINSOLVER_H

#include "OSConfig.h"
#include "OSDefaultSolver.h"
#include "OSrLWriter.h"
#include "OSInstance.h"
#include "OSParameters.h"
#include "OSnLNode.h"
#include "OSiLReader.h"
#include "OSrLReader.h"
#include "OSoLReader.h"
#include "OSInstance.h"
#include "OSExpressionTree.h"
#include "OSnLNode.h"
#include "OSDataStructures.h"
#include "OSFileUtil.h"
#include "OSErrorClass.h"

#include "OSResult.h"
#include "OSInstance.h"
#include "OSOption.h"

#include <cstddef>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <stack>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "BonCbc.hpp"
#include "BonTMINLP.hpp"
#include "BonBonminSetup.hpp"

// for Stefan
class BonminProblem : public Bonmin::TMINLP
{



public:

    /** the BonminProblemclass constructor */
    BonminProblem(OSInstance *osinstance_ , OSOption *osoption_);

    /** the BonminProblem class destructor */
    virtual ~BonminProblem();

    OSInstance *osinstance;

    OSOption *osoption;

    Bonmin::TMINLP::SolverReturn status;


    /** now for some pure Bonmin methods */


    /** \name Overloaded functions specific to a TMINLP.*/
    //@{
    /** Pass the type of the variables (INTEGER, BINARY, CONTINUOUS) to the optimizer.
       \param n size of var_types (has to be equal to the number of variables in the problem)
    \param var_types types of the variables (has to be filled by function).
    */
    virtual bool get_variables_types(Ipopt::Index n, VariableType* var_types);

    /** Pass info about linear and nonlinear variables.*/
    virtual bool get_variables_linearity(Ipopt::Index n, Ipopt::TNLP::LinearityType* var_types);

    /** Pass the type of the constraints (LINEAR, NON_LINEAR) to the optimizer.
    \param m size of const_types (has to be equal to the number of constraints in the problem)
    \param const_types types of the constraints (has to be filled by function).
    */
    virtual bool get_constraints_linearity(Ipopt::Index m, Ipopt::TNLP::LinearityType* const_types);
    //@}

    /** \name Overloaded functions defining a TNLP.
       * This group of function implement the various elements needed to define and solve a TNLP.
       * They are the same as those in a standard Ipopt NLP problem*/
    //@{
    /** Method to pass the main dimensions of the problem to Ipopt.
          \param n number of variables in problem.
          \param m number of constraints.
          \param nnz_jac_g number of nonzeroes in Jacobian of constraints system.
          \param nnz_h_lag number of nonzeroes in Hessian of the Lagrangean.
          \param index_style indicate wether arrays are numbered from 0 (C-style) or
          from 1 (Fortran).
          \return true in case of success.*/
    virtual bool get_nlp_info(Ipopt::Index& n, Ipopt::Index&m, Ipopt::Index& nnz_jac_g,
                              Ipopt::Index& nnz_h_lag, Ipopt::TNLP::IndexStyleEnum& index_style);



    /** Bonmin specific methods for defining the nlp problem */
    //virtual bool get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
    //                       Index& nnz_h_lag, IndexStyleEnum& index_style);

    /** Method to return the bounds for my problem */
    virtual bool get_bounds_info(Ipopt::Index n, Ipopt::Number* x_l, Ipopt::Number* x_u,
                                 Ipopt::Index m, Ipopt::Number* g_l, Ipopt::Number* g_u);

    /** Method to return the starting point for the algorithm */
    virtual bool get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number* x,
                                    bool init_z, Ipopt::Number* z_L, Ipopt::Number* z_U,
                                    Ipopt::Index m, bool init_lambda,
                                    Ipopt::Number* lambda);

    /** Method to return the objective value */
    virtual bool eval_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number& obj_value);

    /** Method to return the gradient of the objective */
    virtual bool eval_grad_f(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Number* grad_f);

    /** Method to return the constraint residuals */
    virtual bool eval_g(Ipopt::Index n, const Ipopt::Number* x, bool new_x, Ipopt::Index m, Ipopt::Number* g);

    /** Method to return:
    *   1) The structure of the jacobian (if "values" is NULL)
    *   2) The values of the jacobian (if "values" is not NULL)
    */
    virtual bool eval_jac_g(Ipopt::Index n, const Ipopt::Number* x, bool new_x,
                            Ipopt::Index m, Ipopt::Index nele_jac, Ipopt::Index* iRow, Ipopt::Index *jCol,
                            Ipopt::Number* values);

    /** Method to return:
    *   1) The structure of the hessian of the lagrangian (if "values" is NULL)
    *   2) The values of the hessian of the lagrangian (if "values" is not NULL)
    */
    virtual bool eval_h(Ipopt::Index n, const Ipopt::Number* x, bool new_x,
                        Ipopt::Number obj_factor, Ipopt::Index m, const Ipopt::Number* lambda,
                        bool new_lambda, Ipopt::Index nele_hess, Ipopt::Index* iRow,
                        Ipopt::Index* jCol, Ipopt::Number* values);

    //@}


    virtual bool get_scaling_parameters(Ipopt::Number& obj_scaling,
                                        bool& use_x_scaling, Ipopt::Index n,
                                        Ipopt::Number* x_scaling,
                                        bool& use_g_scaling, Ipopt::Index m,
                                        Ipopt::Number* g_scaling);

    /** @name Solution Methods */
    /** Method called by Ipopt at the end of optimization.*/
    virtual void finalize_solution(Bonmin::TMINLP::SolverReturn status_,
                                   Ipopt::Index n, const Ipopt::Number* x, Ipopt::Number obj_value);
    //@}

    virtual const SosInfo * sosConstraints() const
    {
        return NULL;
    }
    virtual const BranchingInfo* branchingInfo() const
    {
        return NULL;
    }

    void printSolutionAtEndOfAlgorithm()
    {
        printSol_ = true;
    }





private:
    bool printSol_;

private:
    /**@name Methods to block default compiler methods.
     * The compiler automatically generates the following three methods.
     *  Since the default compiler implementation is generally not what
     *  you want (for all but the most simple classes), we usually
     *  put the declarations of these methods in the private section
     *  and never implement them. This prevents the compiler from
     *  implementing an incorrect "default" behavior without us
     *  knowing. (See Scott Meyers book, "Effective C++")
     *
     */
    //@{
    //  HS071_NLP();

    //@}



    std::string bonminErrorMsg;


};


/*! \class BonminSolver
 *  \brief The BonminSolver class solves problems using Ipopt.
 *
 * @author Jun Ma, Horand Gassmann, Kipp Martin
 * @version 1.0, 07/05/2008
 * @since OS 1.0
 *
 * \remarks
 * this class takes an OSiL instance and optimizes it using
 * the COIN-OR Ipopt solver
 *
 */


class BonminSolver : public DefaultSolver
{
public:



    /** the BonminSolver class constructor */
    BonminSolver();

    /** the IpoptSolver class destructor */
    ~BonminSolver();



    Ipopt::SmartPtr<BonminProblem> tminlp;

    // this is a Bonmin BonCbc object;
    Bonmin::Bab bb;

    Bonmin::TMINLP::SolverReturn status;

    //SmartPtr<IpoptApplication> app;


    /** solve results in an instance being read into the Bonmin
     * data structrues and optimized */
    virtual void  solve() throw (ErrorClass) ;

    /*! \fn void CoinSolver::buildSolverInstance()
     *  \brief The implementation of the virtual functions.
     *  \return void.
     */
    virtual void  buildSolverInstance() throw(ErrorClass);

    /*! \fn void setSolverOptions()
     *  \brief The implementation of the virtual functions.
     *  \return void.
     */
    virtual void  setSolverOptions() throw(ErrorClass);

    /**
     * use this for debugging, print out the instance that
     * the solver thinks it has and compare this with the OSiL
     * file
     */
    void dataEchoCheck();

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



    /**
     * use this to write the solution information to an
     * OSResult  object
     */
    void writeResult();


private:
    OSrLWriter  *osrlwriter;

    Bonmin::BonminSetup bonminSetup;

    std::string bonminErrorMsg;
};


#endif /*BONMINSOLVER_H*/
