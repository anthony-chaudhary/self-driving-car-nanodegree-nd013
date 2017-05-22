// ******************** FlopCpp **********************************************
// File: MP_model.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _MP_model_hpp_
#define _MP_model_hpp_

#include <ostream>
#include <vector>
#include <set>
#include <string>

#include "MP_expression.hpp"
#include "MP_constraint.hpp"
#include <CoinPackedVector.hpp>
class OsiSolverInterface;

namespace flopc {

    class MP_variable;
    class MP_index;
    class MP_set;

    /** @brief Inteface for hooking up to internal flopc++ message handling.
        @ingroup PublicInterface
        In more advanced use of FlopC++, it may be desirable to get access to
        internal calls for messages.  In essence, sub-class this Messenger
        class, and register it with the MP_model.  Also overload whichever
        message events you wish to handle.
    */
    class Messenger {
    public:
        virtual void logMessage(int level, const char * const msg){}
        friend class MP_model;
    private:
        virtual void constraintDebug(std::string name, const std::vector<Coef>& cfs) {}
        virtual void objectiveDebug(const std::vector<Coef>& cfs) {}
        virtual void statistics(int bm, int m, int bn, int n, int nz) {}
        virtual void generationTime(double t) {}
    protected:
        virtual ~Messenger() {}
    };

    /** Internal use: used when Normal output is selected. Uses cout.
        @ingroup INTERNAL_USE
    */
    class NormalMessenger : public Messenger {
        friend class MP_model;
    private:
        virtual void statistics(int bm, int m, int bn, int n, int nz);
        virtual void generationTime(double t);
    };

    /** Internal use: used when Verbose output is selected. Uses cout.
        @ingroup INTERNAL_USE
    */
    class VerboseMessenger : public NormalMessenger {
        friend class MP_model;
    private:
        virtual void constraintDebug(std::string name, const std::vector<Coef>& cfs);
        virtual void objectiveDebug(const std::vector<Coef>& cfs);
    };

    /** @brief This is the anchor point for all constructs in a FlopC++ model.
        @ingroup PublicInterface
        The constructors take an OsiSolverInterface, and (optionally) a
        replacemente for the Messenger class.
        There are some built-in changes to the verbosity for output.
        <br>
        The main methods to use are:
        @li add(MP_constraint & c)
        @li add(MP_variable* v)
        @li maximize() and minimize()
        <br>
        The main ideas are to construct a model, construct domains where
        things are defined over, then construct variables, constraints, and
        add them in.   Finally, one attaches data and the model is "complete".
        Then minimize is called, the model is attached and the
        OsiSolverInterface is called.
        @note There are variations on adding objectives and maximize/minimize  
        @see verbose()
        @see silent().
        @see Messenger
    */
    class MP_model {
        friend class MP_constraint;
    public:
        /// used when calling the solve() method.  
        typedef enum {MINIMIZE=1, MAXIMIZE=-1} MP_direction;

        /** @brief Reflects the state of the solution from solve() 
        */
        typedef enum {  
            /// if the solve method is called and the optimal solution found.
            OPTIMAL, 
            /// if solve is called and solver finds model primal infeasible.
            PRIMAL_INFEASIBLE, 
            /// if solve is called and solver finds the model dual infeasible.
            DUAL_INFEASIBLE, 
            /// if solve is called and solver abandons the problem
            /// (time?, iter limit?)
            ABANDONED,
            /// A solver is placed in the constructor, 
            /// but it is not yet attached or solved.
            SOLVER_ONLY,
            /// A solver is attached, but not yet solved
            ATTACHED,
            /// No solver is attached.
            DETACHED
        } MP_status;

        /// Constructs an MP_model from an OsiSolverInterface *.
        MP_model(OsiSolverInterface* s, Messenger* m = new NormalMessenger);

        ~MP_model() {
            delete messenger;
        }

        /** @brief Returns the current status of the model-solver interaction.
            This method will return the current understanding of the model in
            regard to the solver's state.  
            @note It is not kept up to date if a call is made directly to the 
            solver.  Only if the MP_model interface is used.
            @see MP_status
        */
        MP_status getStatus()const {
            return mSolverState;
        }
        /// used to silence FlopC++
        void silent() {
            delete messenger;
            messenger = new Messenger;
        }
        /// used to help understanding and debugging FlopC++'s behavior.
        void verbose() {
            delete messenger;
            messenger = new VerboseMessenger;
        }

        /// allows for replacement of the solver used.
        void setSolver(OsiSolverInterface* s) {
            Solver = s;
        }

        /// allows access to the OsiSolverInterface *
        OsiSolverInterface* operator->() {
            return Solver;
        }

        /// Adds a constrataint block to the model.
        MP_model& add(MP_constraint& c);

        /** Binds the data and calls the solver to maximize the current
            objective expression
        */
        void maximize();
        /** Binds the data and calls the solver to maximize the parameter obj
            objective expression
        */
        void maximize(const MP_expression &obj);
        /** Binds the data and calls the solver to minimize the current
            objective expression
        */
        void minimize();
        /** Binds the data and calls the solver to minimize the parameter obj
            objective expression
        */
        void minimize(const MP_expression &obj);
   
        /** Binds the data and calls the solver to minimize maximum value of
            the parameter obj objective expression
        */
        void minimize_max(MP_set& d, const MP_expression &obj);

        /// sets the "current objective" to the parameter  o
        void setObjective(const MP_expression& o);
	/** @brief attaches the symantic representation of a model and data to
            a particular OsiSolverInterface
            @note this is called as a part of minimize(), maximize(), and 
            minimize_max();
            This takes the symantic representation of the model, generates
            coefficients for the matrices and 
            adds them into the OsiSolverInterface.  The OsiSolverInterface may
            be specified at construction time, 
            or as late as the call to attach()
	*/
        void attach(OsiSolverInterface *solver=NULL);
	/** @brief detaches an OsiSolverInterface object from the model.  
            In essence, this will clean up any intermediate storage. A model 
            may then be attached to another solverInterface.
            @note a solver may only be attached to one solver at a time
            @todo verify that on "attach", old solver is detached.
	*/
        void detach();
	/** calls the appropriate solving methods in the OsiSolverInterface.
            @note this is called as a part of minimize(), maximize(), and 
            minimize_max()
            It expects that the object function is already set and only the
            direction is to be specified.
            @todo should the direction be defaulted?
	*/ 
        MP_model::MP_status solve(const MP_model::MP_direction &dir);
        /** Accessors for the results after a call to maximize()/minimize()
            @todo should these be private with accessors?  What if not set yet?
            @todo what if not a complete result? What if only one LP in the IP?
        */
        const double* solution;
        const double* reducedCost;
        const double* rowPrice;
        const double* rowActivity;
        /** Useful for getting an appropriate value to pass in as "infinity"
            @note some solvers may be more or less sensitive to the value.
        */
        double getInfinity() const;

        /// Adds a variable to the MP_model.
        void add(MP_variable* v);
        /// Adds a constraint to the MP_model
        void addRow(const Constraint& c); 

        /** Can be used to get the default model
            @todo explain the default and current model concepts.
        */
        static MP_model &getDefaultModel();
        /** Can be used to get the current model
            @todo explain the default and current model concepts.
        */
        static MP_model *getCurrentModel();
        /** Gets the current messenger.
         */
        Messenger *getMessenger(){ 
            return messenger;
        }
    private:
        typedef std::set<MP_variable* >::iterator varIt;
        typedef std::set<MP_constraint* >::iterator conIt;
        static MP_model& default_model;
        static MP_model* current_model;
        MP_model(const MP_model&);
        MP_model& operator=(const MP_model&);

        Messenger* messenger;
   
    
        static void assemble(std::vector<Coef>& v, std::vector<Coef>& av);
        void add(MP_constraint* c);
        MP_expression Objective;
        std::set<MP_constraint *> Constraints;
        std::set<MP_variable *> Variables;
    public:
        /// @todo should this be private?
        OsiSolverInterface* Solver; 
    private:
        int m;
        int n;
        int nz;
        int *Cst;
        int *Clg;
        int *Rnr;
        double *Elm;
        double *bl;
        double *bu;
        double *c;
        double *l;
        double *u;
        MP_status mSolverState;
    };
    
    /// allows print of result from call to solve();
    std::ostream &operator<<(std::ostream &os, 
                             const MP_model::MP_status &condition);
    /// allows print of direction used when calling solve. (MIN/MAX)
    std::ostream &operator<<(std::ostream &os, 
                             const MP_model::MP_direction &direction);

} // End of namespace flopc
#endif
