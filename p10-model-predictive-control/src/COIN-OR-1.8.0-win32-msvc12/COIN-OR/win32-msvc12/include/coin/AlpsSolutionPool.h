/*===========================================================================*
 * This file is part of the Abstract Library for Parallel Search (ALPS).     *
 *                                                                           *
 * ALPS is distributed under the Eclipse Public License as part of the       *
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
 *===========================================================================*/

#ifndef AlpsSolutionPool_h_
#define AlpsSolutionPool_h_

#include "AlpsKnowledgePool.h"
#include "AlpsSolution.h"

//#############################################################################

/** In the solution pool we assume that the lower the priority value the more
    desirable the solution is. */
class AlpsSolutionPool : public AlpsKnowledgePool {
    // *FIXME* ? : we do want to allow solutions with the same priority, but do
    // *FIXME* ? : we want to allow identical solutions?
 private:
    AlpsSolutionPool(const AlpsSolutionPool&);
    AlpsSolutionPool& operator=(const AlpsSolutionPool&);

 private:
//    std::multimap< double, CoinSharedPtr<AlpsSolution> > solutions_;
    std::multimap< double, AlpsSolution* > solutions_;
    int maxNumSolutions_;

//    inline void addSolution(CoinSharedPtr<AlpsSolution> sol, 
//			    double priority) {
//	solutions_.insert(std::make_pair(priority, sol));
//	if (maxNumSolutions_ > 0 && solutions_.size() > maxNumSolutions_) {
//	    std::multimap< double, CoinSharedPtr<AlpsSolution> >::
//		iterator si = solutions_.end();
//	    --si;
//	    solutions_.erase(si);
//	}
//    }

 public:
    AlpsSolutionPool(int maxsols = 1) : maxNumSolutions_(maxsols) {}
    virtual ~AlpsSolutionPool() {
	if (! solutions_.empty()) {
	    clean();
	}
    }

    /** query the current number of solutions */
    //   int getNumSolutions() const { return solutions_.size(); }
    inline int getNumKnowledges() const { 
	return static_cast<int> (solutions_.size()); 
    }

    /** return true if there are any solution stored in the solution pool */
    inline bool hasKnowledge() const 
	{ return solutions_.empty() ? false : true; }

    /**Get a solution from solution pool, doesn't remove it from the pool. 
       It is implemented same as getBestKnowledge(). */
//    inline std::pair<AlpsKnowledge*, double> getKnowledge() const {
//	return std::make_pair(solutions_.begin()->second.get(),
//			      solutions_.begin()->first);
//    }
    inline std::pair<AlpsKnowledge*, double> getKnowledge() const {
	return std::make_pair(static_cast<AlpsKnowledge *>
			      (solutions_.begin()->second),
			      solutions_.begin()->first);
    }

    /**Remove a solution from the pool*/
    inline void popKnowledge() {
	throw CoinError("Can not call popKnowledge()",
			"popKnowledge()", "AlpsSolutionPool");
    }

    /** Append the solution to the end of the vector of solutions. The solution
	pool takes over the ownership of the solution */
    //   void addSolution(const AlpsSolution* sol, double priority) {
    //   CoinSharedPtr<const AlpsSolution> ssol(sol);
    //   addSolution(ssol, priority);
    // }

//  inline void addKnowledge(AlpsKnowledge* sol, double priority=0) {
//	CoinSharedPtr<AlpsSolution> 
//	    ssol( dynamic_cast<AlpsSolution*>(sol) );
//	addSolution(ssol, priority);
//  }
    inline void addKnowledge(AlpsKnowledge* sol, double priority) {
	std::pair<const double, AlpsSolution*> ps(priority, dynamic_cast<AlpsSolution*>(sol));
	solutions_.insert(ps);
	//solutions_.insert(std::make_pair(priority, 
//					 (AlpsSolution*)sol));
  	if ((maxNumSolutions_ > 0) && 
	    (static_cast<int>(solutions_.size()) > maxNumSolutions_)) {
  	    std::multimap< double, AlpsSolution* >::iterator si = 
		solutions_.end();
  	    --si;
	    AlpsSolution* sol = si->second;
  	    solutions_.erase(si);
	    delete sol;
	}
    }

    /** query the maximum number of solutions */
    inline int getMaxNumKnowledges() const { return maxNumSolutions_; }

    /** reset the maximum number of solutions */
//  inline void setMaxNumKnowledges(int maxsols) {
//	if (maxsols > 0) {
//	    if (solutions_.size() > maxsols) {
//		std::multimap< double, CoinSharedPtr<AlpsSolution> >::
//	 	    iterator si = solutions_.begin();
//		for (int i = 0; i < maxsols; ++i)
//		    ++si;
//		solutions_.erase(si, solutions_.end());
//	    }
//	}
//	maxNumSolutions_ = maxsols;
//  }
    inline void setMaxNumKnowledges(int maxsols) {
	if (maxsols > 0) {
	    if (static_cast<int>(solutions_.size()) > maxsols) {
		std::multimap<double, AlpsSolution*>::
		    iterator si = solutions_.begin();
		for (int i = 0; i < maxsols; ++i)
		    ++si;
		solutions_.erase(si, solutions_.end());
	    }
	}
	maxNumSolutions_ = maxsols;
    }

    /** Return the best solution. The callee must not delete the returned
	pointer! */
//    inline std::pair<AlpsKnowledge*, double> getBestKnowledge() const {
//	return std::make_pair(solutions_.begin()->second.get(),
//			      solutions_.begin()->first);
//    }
    inline std::pair<AlpsKnowledge*, double> getBestKnowledge() const {
	return std::make_pair(static_cast<AlpsKnowledge *>
			      (solutions_.begin()->second),
			      solutions_.begin()->first);
    }

    /** Return all the solutions of the solution pool in the provided argument
	vector. The callee must not delete the returned pointers! */
//  inline void getAllKnowledges
//	(std::vector<std::pair<AlpsKnowledge*, double> >& sols) const {
//	sols.reserve(sols.size() + solutions_.size());
//	std::multimap< double, CoinSharedPtr<AlpsSolution> >::
//	    const_iterator si;
//	for (si = solutions_.begin(); si != solutions_.end(); ++si) {
//	    sols.push_back(std::make_pair(si->second.get(), si->first));
//	}
//  }
    inline void getAllKnowledges
	(std::vector<std::pair<AlpsKnowledge*, double> >& sols) const {
	sols.reserve(sols.size() + solutions_.size());
	std::multimap<double, AlpsSolution*>::const_iterator si;
	for (si = solutions_.begin(); si != solutions_.end(); ++si) {
	    sols.push_back(std::make_pair(static_cast<AlpsKnowledge *>
					  (si->second), si->first));
	}
    }
    
    /** Delete all the solutions in pool. */
    void clean() {
	while (!solutions_.empty()) {
  	    std::multimap< double, AlpsSolution* >::iterator si = 
		solutions_.end();
  	    --si;
	    AlpsSolution* sol = si->second;
  	    solutions_.erase(si);
	    delete sol;
	    sol = NULL;
	}
	
	//for_each(solutions_.begin(), solutions_.end(), DeletePtrObject());
    }
};



#define AlpsSolutionInterface(ref)					\
int getNumSolutions() const {						\
   (ref).getNumSolutions();						\
}									\
int getMaxNumSolutions() const {					\
   return (ref).getMaxNumSolutions();					\
}									\
void setMaxNumSolutions(int num) {					\
   (ref).setMaxNumSolutions(num);					\
}									\
bool hasSolution() const {						\
   return (ref).hasSolution();						\
}									\
std::pair<const AlpsSolution*, double> getBestSolution() const {	\
   return (ref).getBestSolution();					\
}									\
void getAllSolutions							\
   (std::vector<std::pair<const AlpsSolution*, double> >& sols) {	\
   return (ref).getAllSolutions(sols);					\
}									\
void addSolution(const AlpsSolution* sol, double priority) {		\
   (ref).addSolution(sol, priority);					\
}

#endif
