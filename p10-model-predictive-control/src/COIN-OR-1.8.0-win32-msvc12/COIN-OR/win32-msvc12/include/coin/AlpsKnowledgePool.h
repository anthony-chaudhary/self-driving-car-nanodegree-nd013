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

#ifndef AlpsKnowledgePool_h
#define AlpsKnowledgePool_h

#include <climits>
#include <iostream>
#include <vector>

#include "CoinError.hpp"
#include "AlpsKnowledge.h"

//#############################################################################
//#############################################################################

class AlpsKnowledgePool {
 private:
  AlpsKnowledgePool(const AlpsKnowledgePool&);
  AlpsKnowledgePool& operator=(const AlpsKnowledgePool&);

 public:
  AlpsKnowledgePool() {}                     // Need: otherwise 
  virtual ~AlpsKnowledgePool() {}            // won't compile.

  /** Add a knowledge to pool */
  virtual void addKnowledge(AlpsKnowledge * nk, double priority) = 0;
 
  /** Query how many knowledges are in the pool.*/
  virtual int getNumKnowledges() const = 0;

  /** Query a knowledge, but doesn't remove it from the pool*/
  virtual std::pair<AlpsKnowledge*, double> getKnowledge() const = 0;

  /** Remove the queried knowledge from the pool*/
  virtual void popKnowledge() {
    throw CoinError("Can not call popKnowledge()",
		    "popKnowledge()", "AlpsKnowledgePool");
  }

  /** Check whether the pool has knowledge. */
  virtual bool hasKnowledge() const{
    throw CoinError("Can not call hasKnowledge()",
		    "hasKnowledge()", "AlpsKnowledgePool");
  }

  /** Set the quantity limit of knowledges that can be stored in the pool. */
  virtual void setMaxNumKnowledges(int num) {
    std::cout << "Can not call setMaxNumKnowledges without overriding"
	      << std::endl;
    throw CoinError("Can not call  setMaxNumKnowledges()",
    		    "setMaxNumKnowledges()", "AlpsKnowledgePool");
  }

  /** Query the quantity limit of knowledges. */
  virtual int getMaxNumKnowledges() const {
    // throw CoinError("Can not call getMaxNumKnowledges()",
    //		    "getMaxNumKnowledges()", "AlpsKnowledgePool");
    return INT_MAX;
  }
 
  /** Query the best knowledge in the pool.*/
  virtual std::pair<AlpsKnowledge*, double> 
    getBestKnowledge() const {
    throw CoinError("Can not call  getBestKnowledge()",
		    "getBestKnowledge()", "AlpsKnowledgePool");
  }
 
  /** Get a reference to all the knowledges in the pool.*/
  virtual void getAllKnowledges (std::vector<std::pair<AlpsKnowledge*, 
				 double> >& kls) const {
    std::cout << "Can not call  getAllKnowledge() without overriding"
	      << std::endl;
    throw CoinError("Can not call  getAllKnowledge()",
		    "getAllKnowledge()", "AlpsKnowledgePool");
  }

};

#endif
 
