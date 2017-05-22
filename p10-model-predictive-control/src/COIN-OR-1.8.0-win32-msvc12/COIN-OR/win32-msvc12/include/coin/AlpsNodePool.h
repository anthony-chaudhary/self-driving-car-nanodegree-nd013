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

#ifndef AlpsNodePool_h_
#define AlpsNodePool_h_

#include <vector>

#include "AlpsHelperFunctions.h"
#include "AlpsPriorityQueue.h"
#include "AlpsTreeNode.h"
#include "AlpsKnowledgePool.h"

//#############################################################################
/** Node pool is used to store the nodes to be processed. */
//#############################################################################

class AlpsNodePool : public AlpsKnowledgePool {
    
 private:
    AlpsNodePool(const AlpsNodePool&);
    AlpsNodePool& operator=(const AlpsNodePool&);
    
    AlpsPriorityQueue<AlpsTreeNode*> candidateList_;
    
 public:
    AlpsNodePool() {}
    virtual ~AlpsNodePool() {
	//std::cout << "- delete nodes pool, size = " << getNumKnowledges() << std::endl;
	if (!candidateList_.empty()) {
            deleteGuts();
	}
    }
    
    /** Query the number of nodes in the node pool. */
    inline int getNumKnowledges() const { return static_cast<int> (candidateList_.size()); }
    
    /** Get the "best value" of the nodes in node pool. */
    inline double getBestKnowledgeValue() const { 
        const std::vector<AlpsTreeNode *>& pool=candidateList_.getContainer();
        int k;
        int size = static_cast<int> (pool.size());
        double bestQuality = ALPS_OBJ_MAX;
        AlpsTreeNode * node = NULL;
        for (k = 0; k < size; ++k) {
            node = pool[k];
            if (node->getQuality() < bestQuality) {
                bestQuality = node->getQuality();
            }
        }
        return bestQuality;
    }

    /** Get the "best" nodes in node pool. */
    inline AlpsTreeNode *getBestNode() const { 
        const std::vector<AlpsTreeNode *>& pool=candidateList_.getContainer();
        int k;
        int size = static_cast<int> (pool.size());
        double bestQuality = ALPS_OBJ_MAX;
        AlpsTreeNode * bestNode = NULL;
        AlpsTreeNode * node = NULL;
        
        for (k = 0; k < size; ++k) {
            node = pool[k];
            if (node->getQuality() < bestQuality) {
                bestQuality = node->getQuality();
                bestNode = node;
            }
        }
        return bestNode;
    }
    
    /** Check whether there are still nodes in the node pool. */
    inline bool hasKnowledge() const{ return ! (candidateList_.empty()); }
    
    /** Get the node with highest priority. Doesn't remove it from the pool*/
    inline std::pair<AlpsKnowledge*, double> getKnowledge() const {
        return std::make_pair( static_cast<AlpsKnowledge *>
			       (candidateList_.top()),
			       candidateList_.top()->getQuality() );
    }
    
    /** Remove the node with highest priority from the pool*/
    inline void popKnowledge() {
	candidateList_.pop();
    }

    /** Remove the node with highest priority from the pool and the elite 
	list*/
    /** Add a node to node pool. */
    inline void addKnowledge(AlpsKnowledge* node, double priority) {
	AlpsTreeNode * nn = dynamic_cast<AlpsTreeNode*>(node);
	//     if(!nn) {
	//AlpsTreeNode * nonnn = const_cast<AlpsTreeNode*>(nn);
	candidateList_.push(nn);
	//     }
	//    else 
	// std::cout << "Add node failed\n";
	//     else
	// throw CoinError();
    }

    /** Get a constant reference to the priority queue that stores nodes. */
    inline const AlpsPriorityQueue<AlpsTreeNode*>&
	getCandidateList() const { return candidateList_; }
    
    /** Set strategy and resort heap. */
    void setNodeSelection(AlpsSearchStrategy<AlpsTreeNode*>& compare) {
	candidateList_.setComparison(compare);
    }

    /** Delete all the nodes in the pool and free memory. */
    void deleteGuts() {
	std::vector<AlpsTreeNode* > nodeVec = candidateList_.getContainer();
	std::for_each(nodeVec.begin(), nodeVec.end(), DeletePtrObject());
        candidateList_.clear();
        assert(candidateList_.size() == 0);
        
	//std::cout << "-- delete nodes in pool" << std::endl;
    }

    /** Remove all the nodes in the pool (does not free memory). */
    void clear() {
	candidateList_.clear();
    }
};

#endif


