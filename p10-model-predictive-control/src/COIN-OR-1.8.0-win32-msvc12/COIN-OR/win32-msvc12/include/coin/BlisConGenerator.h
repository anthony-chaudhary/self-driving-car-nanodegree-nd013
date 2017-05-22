/*===========================================================================*
 * This file is part of the BiCePS Linear Integer Solver (BLIS).             *
 *                                                                           *
 * BLIS is distributed under the Eclipse Public License as part of the       *
 * COIN-OR repository (http://www.coin-or.org).                              *
 *                                                                           *
 * Authors:                                                                  *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *                                                                           *
 * Conceptual Design:                                                        *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *          Laszlo Ladanyi, IBM T.J. Watson Research Center                  *
 *          Matthew Saltzman, Clemson University                             *
 *                                                                           * 
 *                                                                           *
 * Copyright (C) 2001-2013, Lehigh University, Yan Xu, and Ted Ralphs.       *
 * All Rights Reserved.                                                      *
 *===========================================================================*/


//#############################################################################
// This file is modified from COIN/Cbc/CbcCutGenerator.hpp
//#############################################################################


#ifndef BlisConGenerator_h_
#define BlisConGenerator_h_

#include "OsiSolverInterface.hpp"
#include "OsiCuts.hpp"


class BlisModel;

class OsiRowCut;
class OsiRowCutDebugger;
class CglCutGenerator;


//#############################################################################

/** Interface between Blis and Cut Generation Library.
    \c BlisConGenerator is intended to provide an intelligent 
    interface between Blis and the cutting plane algorithms in the CGL. 
    A \c BlisConGenerator is bound to a \c CglCutGenerator and 
    to an \c BlisModel. It contains parameters which control when and 
    how the \c generateCuts method of the \c CglCutGenerator will be called.
    
    The builtin decision criteria available to use when deciding whether to
    generate cons are: at root, autmatic, every <i>X</i> nodes, when a 
    solution is found, and when a subproblem is found to be infeasible. 
*/

class BlisConGenerator  {

protected:
    /** The client model. */
    BlisModel *model_;
    
    /** The CglCutGenerator object. */
    CglCutGenerator * generator_;

    //------------------------------------------------------
    // CON CONTROL
    //------------------------------------------------------

    /** When to call CglCutGenerator::generateCuts routine. 
        BlisCutStrategyNone:                    disable
        BlisCutStrategyRoot:                    just root
        BlisCutStrategyAuto:                    automatically decided by BLIS
	BlisCutStrategyPeriodic:                Generate every 't' nodes
    */
    BlisCutStrategy strategy_;

    /** The frequency of calls to the cut generator. */
    int cutGenerationFrequency_;
    
    /** Name of generator. */
    std::string name_;
    
    /** Whether to call the generator in the normal place. */
    bool normal_;
    
    /** Whether to call the generator when a new solution is found. */
    bool atSolution_;

    /** Whether to call generator when a subproblem is found to be 
        infeasible.*/
    bool whenInfeasible_;
    
    //------------------------------------------------------
    // CON STATISTICS
    //------------------------------------------------------
    
    /** Number of cons generated. */
    int numConsGenerated_;
    
    /** Number of cons used. */
    int numConsUsed_;
    
    /** Used CPU/User time. */
    double time_;
    
    /** The times of calling this generator. */
    int calls_;

    /** The times of calling this generator and no cons found. */
    int noConsCalls_;
    
 public:

    /**@name Constructors and destructors */
    //@{
    /** Default constructor. */
    BlisConGenerator()
        :
        model_(NULL),
        generator_(NULL),
        strategy_(BlisCutStrategyAuto),
        cutGenerationFrequency_(1),
        normal_(true),
        atSolution_(false),
        whenInfeasible_(false),
        numConsGenerated_(0),
        numConsUsed_(0),
        time_(0),
        calls_(0),
        noConsCalls_(0)
        { name_ = "UNKNOWN"; }
    
    /** Useful constructor. */
    BlisConGenerator(BlisModel * model,
		     CglCutGenerator * generator,
		     const char * name = NULL,
		     BlisCutStrategy strategy = BlisCutStrategyAuto,
		     int cutGenerationFrequency_ = 1,
		     bool normal = true, 
		     bool atSolution = false, 
		     bool infeasible = false);
    
    /** Copy constructor. */
    BlisConGenerator (const BlisConGenerator &);
    
    /** Assignment operator. */
    BlisConGenerator & operator=(const BlisConGenerator& rhs);
    
    /** Destructor. */
    virtual ~BlisConGenerator()
        {
            if (generator_) {
                delete generator_;
                generator_ = NULL;
            }
        }
    //@}
    
    /** \name Generate Constraints */
    //@{
    /** Generate cons for the client model.

	Evaluate the state of the client model and decide whether to 
	generate cons. The generated cons are inserted into and returned 
	in the collection of cons \p cs.

	The routine returns true if reoptimisation is needed (because the 
	state of the solver interface has been modified).
    */
    virtual bool generateConstraints(BcpsConstraintPool &conPool); 
    //@}

    /**@name Gets and sets */
    //@{
    /** Set the client model.
	In addition to setting the client model, refreshModel also calls
	the \c refreshSolver method of the CglCutGenerator object.
    */
    /** Get a pointer to the model */
    inline BlisModel *getModel() { return model_; }

    /** Set the model */
    inline void setModel(BlisModel *m) { model_ = m; }
    
    /** Refresh the model */
    void refreshModel(BlisModel * model);
    
    /** return name of generator. */
    void setName(const char *str) { name_ = str; }
    
    /** return name of generator. */
    inline std::string name() const { return name_; }
    
    /** Set the con generation strategy. */
    void setStrategy(BlisCutStrategy value) { strategy_ = value; }
    
    /** Get the con generation interval. */
    inline BlisCutStrategy strategy() const { return strategy_; }
    
    /** Set the con generation strategy. */
    void setCutGenerationFreq(int freq) { cutGenerationFrequency_ = freq; }
    
    /** Get the con generation interval. */
    inline int cutGenerationFreq() const { return cutGenerationFrequency_; }
    
    /** Get whether the con generator should be called in the normal place. */
    inline bool normal() const { return normal_; }
    
    /** Set whether the con generator should be called in the normal place. */
    inline void setNormal(bool value) { normal_ = value; }
    
    /** Get whether the con generator should be called when a solution 
        is found. */
    inline bool atSolution() const { return atSolution_; }
    
    /** Set whether the con generator should be called when a solution 
        is found. */
    inline void setAtSolution(bool value) { atSolution_ = value; }

    /** Get whether the con generator should be called when the subproblem is
	found to be infeasible. */
    inline bool whenInfeasible() const { return whenInfeasible_; }
    
    /** Set whether the con generator should be called when the subproblem is
	found to be infeasible. */
    inline void setWhenInfeasible(bool value) { whenInfeasible_ = value; }
    
    /** Get the \c CglCutGenerator bound to this \c BlisConGenerator. */
    inline CglCutGenerator * generator() const { return generator_; }
    
    /** Get number of generated cons. */
    inline int numConsGenerated() { return numConsGenerated_; }

    /** Increase the number of generated cons. */
    inline void addNumConsGenerated(int n) { numConsGenerated_ += n; }
    
    /** Get number of used cons. */
    inline int numConsUsed() { return numConsUsed_; }
    
    /** Increase the number of generated cons. */
    inline void addNumConsUsed(int n) { numConsUsed_ += n; }

    /** Cpu time used. */
    inline double time() const { return time_; }
    
    /** Increase Cpu time used. */
    inline void addTime(double t) { time_ += t; }
    
    /** Number called. */
    inline int calls() const { return calls_; }

    /** Increase the number of called. */
    inline void addCalls(int n=1) { calls_ += n; }

    /** Number called and no cons found. */
    inline int noConsCalls() const { return noConsCalls_; }

    /** Increase the number of no cons called. */
    inline void addNoConsCalls(int n=1) { noConsCalls_ += n; }
    //@}  
};

#endif
