/* $Id: OSIpoptSolver.h 4996 2015-04-14 20:38:45Z Gassmann $ */
/** @file IpoptSolver.h
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2014, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

#ifndef IPOPTSOLVER_H
#define IPOPTSOLVER_H

#include "OSConfig.h"
#include "OSDefaultSolver.h"
#include "OSrLWriter.h"
#include "OSInstance.h"
#include "OSParameters.h"
#include "OSnLNode.h"
#include "OSiLReader.h"
#include "OSoLReader.h"
#include "OSInstance.h"
#include "OSExpressionTree.h"
#include "OSnLNode.h"
#include "OSGeneral.h"
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


#include "IpTNLP.hpp"
#include "IpIpoptApplication.hpp"

// for Stefan
class IpoptProblem : public Ipopt::TNLP
{
public:

    /** the IpoptProblemclass constructor */
    IpoptProblem(OSInstance *osinstance_ , OSOption *osoption_, OSResult *osresult_, std::string *ipoptErrorMsg_);

    /** the IpoptProblem class destructor */
    virtual ~IpoptProblem();


    OSInstance *osinstance;

    OSOption *osoption;

    OSResult *osresult;

    std::string *ipoptErrorMsg;

    /** IPOpt specific methods for defining the nlp problem */
    virtual bool get_nlp_info(Ipopt::Index& n, Ipopt::Index& m, Ipopt::Index& nnz_jac_g,
                              Ipopt::Index& nnz_h_lag, IndexStyleEnum& index_style);

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
    //@{
    /** This method is called when the algorithm is complete so the TNLP can store/write the solution */
    virtual void finalize_solution(Ipopt::SolverReturn status,
                                   Ipopt::Index n, const Ipopt::Number* x, const Ipopt::Number* z_L, const Ipopt::Number* z_U,
                                   Ipopt::Index m, const Ipopt::Number* g, const Ipopt::Number* lambda,
                                   Ipopt::Number obj_value,
                                   const Ipopt::IpoptData* ip_data,
                                   Ipopt::IpoptCalculatedQuantities* ip_cq);
    //@}



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
    IpoptProblem(const IpoptProblem&);
    IpoptProblem& operator=(const IpoptProblem&);
    //@}

};


/*! \class IpoptSolver
 *  \brief The IpoptSolver class solves problems using Ipopt.
 *
 * @author Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * this class takes an OSiL instance and optimizes it using
 * the COIN-OR Ipopt solver
 *
 */
//class IpoptSolver : public DefaultSolver, public TNLP{

class IpoptSolver : public DefaultSolver
{
public:

    /** the IpoptSolver class constructor */
    IpoptSolver();

    /** the IpoptSolver class destructor */
    ~IpoptSolver();

    Ipopt::SmartPtr<Ipopt::TNLP> nlp;

    Ipopt::SmartPtr<Ipopt::IpoptApplication> app;


    /** solve results in an instance being read into the Ipopt
     * data structures and optimize */
    virtual void  solve() throw (ErrorClass) ;

    /*! \fn void buildSolverInstance()
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


private:
    OSrLWriter  *osrlwriter;

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
    //  IpoptSolver();
    //IpoptSolver(const IpoptSolver&);
    //IpoptSolver& operator=(const IpoptSolver&);
    //@}
    //std::string ipoptErrorMsg;
    std::string *ipoptErrorMsg;
};


#endif /*IPOPTSOLVER_H*/
