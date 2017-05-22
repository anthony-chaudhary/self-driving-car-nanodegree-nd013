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
// This file is modified from COIN/Cbc/CbcHeuristic.hpp
//#############################################################################


#ifndef BlisHeuristic_h_
#define BlisHeuristic_h_

#include <string>
#include <vector>

#include "CoinPackedMatrix.hpp"
#include "OsiCuts.hpp"

#include "Blis.h"

class BlisModel;


//#############################################################################


/** Heuristic base class */
class BlisHeuristic {

 private:

    /** Illegal Assignment operator */
    BlisHeuristic & operator = (const BlisHeuristic& rhs);
    
 protected:

    /** Pointer to the model */
    BlisModel *model_;
    
    /** Heuristics name */
    char *name_;
    
    /** When to call findSolution() routine. 
        BlisHeurStrategyNone:     disable
        BlisHeurStrategyRoot:     just root
        BlisHeurStrategyAuto:     automatically decided by BLIS
        BlisHeurStrategyPeriodic: every 't' nodes
	BlisHeurStrategyBeforeRoot: before solving first LP
    */
    BlisHeurStrategy strategy_;

    /** The frequency with which to call the heuristic */
    int heurCallFrequency_;
    
    /** Number of solutions found. */
    int numSolutions_;

    /** Used CPU/User time. */
    double time_;

    /** The times of calling this heuristic. */
    int calls_;

    /** The times of calling this heuristic and no solutions found. */
    int noSolsCalls_;
    
public:
    
    /** Default Constructor. */
    BlisHeuristic() {
        model_ = NULL;
        name_ = strdup("Unknown");
        strategy_ = BlisHeurStrategyAuto;
	heurCallFrequency_ = 1;
        numSolutions_ = 0;
        time_ = 0.0;
        calls_ = 0;
        noSolsCalls_ = 0;
    }
    
    /** Useful constructor. */
    BlisHeuristic(BlisModel *model, const char *name,
		  BlisHeurStrategy strategy, int heurCallFrequency) {
        model_ = model;
        if (name) {
            name_ = strdup(name);
        }
        else {
            name_ = strdup("Unknown");
        }
        strategy_ = strategy;
	heurCallFrequency_ = heurCallFrequency;
        numSolutions_ = 0;
        time_ = 0.0;
        calls_ = 0;
        noSolsCalls_ = 0;
    }

    /** Distructor. */
    virtual ~BlisHeuristic() { if (name_) free(name_); }

    /** Copy constructor. */
    BlisHeuristic(const BlisHeuristic & rhs) {        
        model_ = rhs.model_;
        name_ = strdup(rhs.name_);
        strategy_ = rhs.strategy_; // What if disabled?
	heurCallFrequency_ = rhs.heurCallFrequency_;
        numSolutions_ = 0;
        time_ = 0.0;
        calls_ = 0;
        noSolsCalls_ = 0;
    }
 
    /** update model (This is needed if cliques update matrix etc). */
    virtual void setModel(BlisModel * model) { model_ = model ;}
    
    /** Get/set strategy. */
    //@{
    virtual void setStrategy(BlisHeurStrategy strategy) {strategy_ = strategy;}
    virtual BlisHeurStrategy strategy() const { return strategy_; }
    //@]

    /** Get/set call frequency. */
    //@{
    virtual void setHeurCallFrequency(int freq) { heurCallFrequency_ = freq; }
    virtual int heurCallFrequency() const { return heurCallFrequency_; }
    //@]

    /** Clone a heuristic. */
    virtual BlisHeuristic * clone() const {
	BlisHeuristic *h = NULL;
	assert(0);
	return h;
    }
    
    /** returns 0 if no solution, 1 if valid solution
        with better objective value than one passed in
        Sets solution values if good, sets objective value 
        This is called after cuts have been added - so can not add cuts
    */
    virtual bool searchSolution(double & objectiveValue, 
                               double * newSolution)=0;
    
    /** returns 0 if no solution, 1 if valid solution, -1 if just
        returning an estimate of best possible solution
        with better objective value than one passed in
        Sets solution values if good, sets objective value 
        (only if nonzero code) 
        This is called at same time as cut generators - so can add cuts
        Default is do nothing
    */
    virtual bool searchSolution(double & objectiveValue,
                               double * newSolution,
                               OsiCuts & cs) { return 0; }
    
    /** return name of generator. */
    inline const char * name() const { return name_; }
    
    /** Record number of solutions found. */
    inline void addNumSolutions(int num=1) { numSolutions_ += num; }

    /** Number of solutions found. */
    inline int numSolutions() const { return numSolutions_; }
    
    /** Record Cpu time used. */
    inline void addTime(double t=0.0) { time_ += t; }

    /** Cpu time used. */
    inline double time() const { return time_; }
    
    /** Record number of times called. */
    inline void addCalls(int c=1) { calls_ += c; }
    
    /** Number of times called. */
    inline int calls() const { return calls_; } 

    /** Number called and no cons found. */
    inline int noSolCalls() const { return noSolsCalls_; }

    /** Increase the number of no cons called. */
    inline void addNoSolCalls(int n=1) { noSolsCalls_ += n; }
};

#endif

