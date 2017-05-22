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

#ifndef AlpsSubTree_h_
#define AlpsSubTree_h_

#include <cassert>
#include <list>

#include "CoinError.hpp"
#include "CoinSort.hpp"

#include "AlpsSearchStrategy.h"
#include "AlpsKnowledge.h"
#include "AlpsNodePool.h"
#include "AlpsPriorityQueue.h"
#include "AlpsTreeNode.h"

class AlpsKnowledgeBroker;

//#############################################################################

/** This class contains the data pertaining to a particular subtree in the
    search tree. In order to improve scalability, we will try to deal with
    entire subtrees as much as possible. They will be the basic unit of work
    that will be passed between processes.
*/
class AlpsSubTree : public AlpsKnowledge {

 protected:

    /** The root of the sub tree. */
    AlpsTreeNode* root_;
   
    /** A node pool containing the leaf nodes awaiting processing. */
    AlpsNodePool* nodePool_;

    /** A node pool used when diving. */
    AlpsNodePool* diveNodePool_;
    
    /** Diving node comparing rule. */
    AlpsSearchStrategy<AlpsTreeNode*> * diveNodeRule_;

    /** Diving depth */
    int diveDepth_;
    
    //   /** The next index to be assigned to a new search tree node */
    //   AlpsNodeIndex_t nextIndex_;

    /** This is the node that is currently being processed. Note that since
	this is the worker, there is only one. */
    AlpsTreeNode* activeNode_;

    /** A quantity indicating how good this subtree is. */
    double quality_;

    /** A pointer to the knowledge broker of the process where this subtree is
	processed. */
    // Need broker to query model && parameters.
    AlpsKnowledgeBroker*  broker_;
    
 protected:

    /** The purpose of this method is to remove nodes that are not needed in
	the description of the subtree. The argument node must have status
	<code>fathomed</code>. First, the argument node is removed, and then
	the parent is examined to determine whether it has any children
	left. If it has none, then this function is called recursively on the
	parent. This removes all nodes that are no longer needed. */
    void removeDeadNodes(AlpsTreeNode*& node);

    /** This function replaces \c oldNode with \c newNode in the tree. */
    void replaceNode(AlpsTreeNode* oldNode, AlpsTreeNode* newNode);

    /** Fathom all nodes on this subtree. 
     *  Set activeNode_ and root_ to NULL.
     */
    void fathomAllNodes();

 public:
    
    /** Default constructor. */
    AlpsSubTree();
    
    /** Useful constructor. */
    AlpsSubTree(AlpsKnowledgeBroker* kb);
        
    /** Destructor. */
    virtual ~AlpsSubTree();

 public:

    /** Get pointer to active node */
    inline AlpsTreeNode* activeNode() { return activeNode_; }

    /** Set pointer to active node */
    inline void setActiveNode(AlpsTreeNode *activeNode)
    { activeNode_ = activeNode; }

    /** Create children nodes from the given parent node. */
    void createChildren(AlpsTreeNode* parent,
			std::vector< CoinTriple<AlpsNodeDesc*, AlpsNodeStatus, 
			double> >& children,
                        AlpsNodePool *kidNodePool = NULL);
    
    /** @name query and set member functions
     */
    //@{
    /** Get the root node of this subtree. */
    inline AlpsTreeNode* getRoot() const { return root_; }

    /** Set the root node of this subtree. */
    inline void setRoot(AlpsTreeNode* r) { root_ = r; }

    /** Access the node pool. */
    inline AlpsNodePool* nodePool() { return nodePool_; }

    /** Access the node pool. */
    inline AlpsNodePool* diveNodePool() { return diveNodePool_; }

    /** Set node pool. Delete previous node pool and nodes in pool if exit.*/
    inline void setNodePool(AlpsNodePool* np) { 
        if (nodePool_ != NULL) {
            delete nodePool_; 
            nodePool_ = NULL;
        }
        nodePool_ = np;
    }

    /** Set node pool. Delete previous node pool, but not the nodes in pool.*/
    inline void changeNodePool(AlpsNodePool* np) { 
	if (nodePool_ != NULL) {
	    // Remove all elements first.
	    nodePool_->clear();
	    // Delete an empty pool.
	    assert(nodePool_->hasKnowledge() == false);
	    delete nodePool_;
	    nodePool_ = NULL;
	}
	nodePool_ = np;
    }

    /** Get the quality of the best node in the subtree. */
    double getBestKnowledgeValue() const;

    /** Get the "best" node in the subtree. */
    AlpsTreeNode *getBestNode() const;

    /** Get the knowledge broker. */
    inline AlpsKnowledgeBroker*  getKnowledgeBroker() const { return broker_; }
    
    /** Set a pointer to the knowledge broker. */
    inline void setKnowledgeBroker(AlpsKnowledgeBroker* kb) {
        assert(kb);
        broker_ = kb;
    }
    
    /** Get the quality of this subtree. */
    inline double getQuality() const { return quality_; }

    /** Get the emtimated quality of this subtree. */
    inline double getSolEstimate() const { 
        if (root_) {
            return root_->getSolEstimate();
        }
        else {
            return ALPS_OBJ_MAX;
        };
    }

    /** Increment dive depth */
    void incDiveDepth(int num=1) {  diveDepth_ += num; }

    /** Get dive depth */
    int getDiveDepth() { return diveDepth_; }

    /** Set dive depth */
    void setDiveDepth(int num) { diveDepth_ = num; }
    
    /** Calcuate  and return the quality of this subtree, which is measured
	by the quality of the specified number of nodes.*/
    double calculateQuality();
 
    /* Get the index of the next generated node and increment next index
       by one.*/ 
    int nextIndex();

    /** Get the index of the next generated node.*/
    int getNextIndex() const;
    
    /** Set the index of the next generated node. */
    void setNextIndex(int next);

    /** Return the number of nodes on this subtree. */
    int getNumNodes() const {
	assert(nodePool_ && diveNodePool_);
        int nn = 0;
        if (activeNode_) {
            if ( (activeNode_->getStatus() != AlpsNodeStatusFathomed) &&
		 (activeNode_->getStatus() != AlpsNodeStatusBranched) ) {
                ++nn;
            }
        }
	return (nn + nodePool_->getNumKnowledges() + 
                diveNodePool_->getNumKnowledges());
    }

    /** Set the node comparision rule. */
    void setNodeSelection(AlpsSearchStrategy<AlpsTreeNode*>* nc) {
	nodePool_->setNodeSelection(*nc);
    }
    //@}

    /** The function split the subtree and return a subtree of the 
	specified size or available size. */
    AlpsSubTree* splitSubTree(int& returnSize, int size = 10);
    
    /** Explore the subtree from \c root as the root of the subtree for given
	number of nodes or time, depending on which one reach first. 
	Only for serial code. */
    virtual AlpsReturnStatus exploreSubTree(AlpsTreeNode* root,
                                            int nodeLimit,  
                                            double timeLimit,
                                            int & numNodesProcessed, /* Output */
                                            int & numNodesBranched,  /* Output */
                                            int & numNodesDiscarded, /* Output */
                                            int & numNodesPartial,  /* Output */
                                            int & depth);            /* Output */
    
    /** Explore the subtree for certain amount of work/time. 
	leaveAsIt means exit immediately after reseach limits:
	do not put activeNode_ in pool, do not move nodes in 
	divePool_ in regular pool.
    */
    AlpsReturnStatus exploreUnitWork(bool leaveAsIt,
                                     int unitWork,
                                     double unitTime,
                                     AlpsExitStatus & solStatus,/*not for parallel*/
                                     int & numNodesProcessed, /* Output */
                                     int & numNodesBranched,  /* Output */
                                     int & numNodesDiscarded, /* Output */
                                     int & numNodesPartial,  /* Output */
                                     int & depth,             /* Output */
                                     bool & betterSolution);  /* Output */
    
    /** Generate required number (specified by a parameter) of nodes. 
	This function is used by master and hubs. */
    virtual int rampUp(int minNumNodes,
                       int requiredNumNodes,
                       int& depth,
                       AlpsTreeNode* root = NULL);

    using  AlpsKnowledge::encode ;
    /** This method should encode the content of the subtree and return a
	pointer to the encoded form. Only parallel code need this function. */
    virtual AlpsEncoded* encode() const;
    
    /** This method should decode and return a pointer to a \em brand \em new
	\em object, i.e., the method must create a new object on the heap from
	the decoded data instead of filling up the object for which the method
	was invoked. Only parallel code need this function.*/
    virtual AlpsKnowledge* decode(AlpsEncoded& encoded) const;

    /** Create a AlpsSubtree object dynamically. Only parallel code need 
	this function.*/    
    virtual AlpsSubTree* newSubTree() const {
	return new AlpsSubTree;
    }

    /** Remove nodes in pools in the subtree. Do not free memory. */
    void clearNodePools() {
        if (nodePool_) {
            nodePool_->clear();
        }
        if (diveNodePool_) {
            diveNodePool_->clear();
        }
    }

    /** Set root and active node to null */
    void nullRootActiveNode() {
        root_ = NULL;
        activeNode_ = NULL;
    }

    /** Move nodes in node pool, null active node. */
    void reset() {
        // Move nodes in diving pool to normal pool.
        AlpsTreeNode *tempNode = NULL;
        while (diveNodePool_->getNumKnowledges() > 0) {
            tempNode = dynamic_cast<AlpsTreeNode *>
                (diveNodePool_->getKnowledge().first);
            diveNodePool_->popKnowledge();
            nodePool_->addKnowledge(tempNode, tempNode->getQuality());
        }
        if (activeNode_) {   
            nodePool_->addKnowledge(activeNode_, activeNode_->getQuality());
            activeNode_ = NULL;
        }

        diveDepth_ = 0;
    }
    
};
#endif

//#############################################################################
// The way to create children:
//-----------------------------------------------------------------------------
// In AlpsSubTree::exploreSubTree(root)
// If (pregnant) 
// => KnapTreeNode::branch() 
// => AlpsSubTree::createChildren(...)  {
//   AlpsTreeNode::setNumChildren(...) (allocate memory if not);
//   KnapTreeNode:: createNewTreeNode(...); 
//   AlpsSubTree::setChildren;
//   AlspSubTree::setStatus }
//#############################################################################

//#############################################################################
// The way to remove nodes:
//-----------------------------------------------------------------------------
// In AlpsSubTree::exploreSubTree(root)
// If (fathomed)
// => AlpsSubTree::removeDeadNode(node) {
//      AlpsTreeNode::removeChild(node) {
//        AlpsTreeNode::removeDescendants();
//      }
//    Check whether parent has children; 
//      if (yes), recursively removeDeadNode(parent) 
//#############################################################################
