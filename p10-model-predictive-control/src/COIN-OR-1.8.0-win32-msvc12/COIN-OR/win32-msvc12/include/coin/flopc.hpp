// ******************** FlopCpp **********************************************
// File: flopc.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _flopc_hpp_
#define _flopc_hpp_

#include "MP_variable.hpp"
#include "MP_set.hpp"
#include "MP_index.hpp"
#include "MP_constant.hpp"
#include "MP_data.hpp"
#include "MP_constraint.hpp"
#include "MP_expression.hpp"
#include "MP_boolean.hpp"
#include "MP_model.hpp"

/** @defgroup PublicInterface Public interface
    @brief Classes in this group are for normal modeling purposes.
    They are intended for consumption outside the library.
*/
/** @defgroup INTERNAL_USE Internal (private) interface.
    @brief Classes in this group are used for internal purposes.
    They are not intended for consumption outside the library.
*/
/** @brief All flopc++ code is contained within the flopc namespace.
    
    Flopc++ is an open source algebraic modelling language implemented as
    a C++ class library.  It uses the common COIN-OR OsiSolverInterface
    abstract interface to allow for easy integration with many of today's top
    Math Programming solvers.
    <ul> The main 3 components are listed below.  Much of the rest of
    the code is to facilitate the operator overloading makes this such
    a powerful modeling environment.
    @li Linear Variables  MP_variable
    @li Linear Set  MP_set
    @li Linear Index MP_index
    @li Linear Constraints MP_constraint
    </ul>
    <br>
    @note The classes in @group PublicInterface are intended for consumption
    outside the library.
    
 */
namespace flopc {

    /** @brief Global function for performing a Functor on each member of a
        MP_domain
        @ingroup PublicInterface
    */
  inline void forall(const MP_domain& d, const Functor& f) {
    d.Forall(&f);
  }
    
  /** @brief Global function for performing a Functor without having a set
      to operate on.
      @ingroup PublicInterface
  @todo This turns into a no-op? 
  */
  inline void forall(const Functor& f) {
  forall(MP_domain::getEmpty(), f);
  }

    /** @brief Global function which copies members of MP_domain d into
        another (possibly non-empty) MP_domain
        @ingroup PublicInterface
    */
  inline void operator<<=(const MP_domain& s, const MP_domain& d) {
    d.Forall( s->makeInsertFunctor());
  }

    /** @brief This is one of the main entry points for execution 
        @ingroup PublicInterface
    This calls the OsiSolverInterface to execute the solver with the
    objective of  MINIMIZING the argment MP_expression.
    @li Assumes that the OsiSolverInterface is already set
    @li Assumes a model is already loaded (and is the default model)
    */
  inline void minimize(const MP_expression &obj) {
    MP_model::getDefaultModel().minimize(obj);
  }

    /** @brief This is one of the main entry points for execution 
        @ingroup PublicInterface
    This calls the OsiSolverInterface to execute the solver with the
    objective of  MINIMIZING THE MAXIMUM of the MP_expression evaluation
    of the MP_set
    @todo true?
    @li Assumes that the OsiSolverInterface is already set
    @li Assumes a model is already loaded (and is the default model)
    */
  inline void minimize_max(MP_set& d, const MP_expression &obj) {
    MP_model::getDefaultModel().minimize_max(d,obj);
  }

    /** @brief This is one of the main entry points for execution 
        @ingroup PublicInterface
    This calls the OsiSolverInterface to execute the solver with the
    objective of  MAXIMIZING of the MP_expression 
    @li Assumes that the OsiSolverInterface is already set
    @li Assumes a model is already loaded (and is the default model)
    */
  inline void maximize(const MP_expression &obj) {
    MP_model::getDefaultModel().maximize(obj);
  }

} // End of namespace flopc
#endif
