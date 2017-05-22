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

#ifndef AlpsSubTreePool_h_
#define AlpsSubTreePool_h_

#include "AlpsHelperFunctions.h"
#include "AlpsSubTree.h"

//#############################################################################

/** The subtree pool is used to store subtrees */
class AlpsSubTreePool : public AlpsKnowledgePool {

 private:
    AlpsSubTreePool(const AlpsSubTreePool&);
    AlpsSubTreePool& operator=(const AlpsSubTreePool&);

    AlpsPriorityQueue<AlpsSubTree*> subTreeList_;

 public:
    AlpsSubTreePool() {}
    virtual ~AlpsSubTreePool() {
	if (!subTreeList_.empty()) {
	    deleteGuts();
	}
    }
   
    /** Query the number of subtrees in the pool. */
    inline int getNumKnowledges() const { return static_cast<int> (subTreeList_.size()); }
  
    /** Check whether there is a subtree in the subtree pool. */
    inline bool hasKnowledge() const{ return ! (subTreeList_.empty()); }
    
    /** Get a subtree from subtree pool, doesn't remove it from the pool*/
    inline std::pair<AlpsKnowledge*, double> getKnowledge() const {
	return std::make_pair( static_cast<AlpsKnowledge *>
			       (subTreeList_.top()), 
			       subTreeList_.top()->getQuality() );
    }

    /** Remove a subtree from the pool*/
    inline void popKnowledge() {
	subTreeList_.pop();
    }
  
    /** Add a subtree to the subtree pool. */
    inline void addKnowledge(AlpsKnowledge* subTree, double priority) {
	AlpsSubTree * st = dynamic_cast<AlpsSubTree* >(subTree);
	subTreeList_.push(st);
    }

    /** Return the container of subtrees. */  
    inline const AlpsPriorityQueue< AlpsSubTree*>&  
	getSubTreeList() const { return subTreeList_; }

    /** Set comparison function and resort heap. */
    void setComparison(AlpsSearchStrategy<AlpsSubTree*>& compare) {
	subTreeList_.setComparison(compare);
    }

    /** Delete the subtrees in the pool. */
    void deleteGuts() {
	std::vector<AlpsSubTree* > treeVec = subTreeList_.getContainer();
	std::for_each(treeVec.begin(), treeVec.end(), DeletePtrObject());
        subTreeList_.clear();
        assert(subTreeList_.size() == 0);
    }

    /** Get the quality of the best subtree. */
    double getBestQuality() {
        double quality = ALPS_OBJ_MAX;
        
        std::vector<AlpsSubTree* > subTreeVec = subTreeList_.getContainer();

        std::vector<AlpsSubTree* >::iterator pos1, pos2;

        pos1 = subTreeVec.begin();
        pos2 = subTreeVec.end();

        for (; pos1 != pos2; ++pos1) {
            (*pos1)->calculateQuality();
            if ((*pos1)->getQuality() < quality) {
                quality = (*pos1)->getQuality();
            }
        }

        return quality;
    }
};

#endif
