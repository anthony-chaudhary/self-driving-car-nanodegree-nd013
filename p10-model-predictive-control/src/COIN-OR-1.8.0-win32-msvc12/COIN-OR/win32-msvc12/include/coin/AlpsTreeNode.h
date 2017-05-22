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

#ifndef AlpsTreeNode_h_
#define AlpsTreeNode_h_

#include <algorithm>
#include <utility>
#include <vector>

#include "CoinError.hpp"
#include "CoinSort.hpp"

#include "Alps.h"
#include "AlpsKnowledge.h"
#include "AlpsNodeDesc.h"

typedef int AlpsNodeIndex_t;

class AlpsKnowledgeBroker;
class AlpsSubTree;

//#############################################################################
/** This class holds one node of the search tree. Note that the generic search
    procedure doesn't know anything about the nodes in the
    tree other than their index, lower bound, etc. Other application-specific
    data is contained in derived classes, but is not needed for the basic
    operation of the search tree.*/
//#############################################################################

class AlpsTreeNode : public AlpsKnowledge { 
 private:
    AlpsTreeNode(const AlpsTreeNode&);
    AlpsTreeNode& operator=(const AlpsTreeNode&);

 protected:
    /** The subtree own this node. */
    //AlpsSubTree*       subTree_;
    
    /** Whether the node is being worked on at the moment */
    bool               active_;

    /** The unique index of the tree node (across the whole search tree).*/
    AlpsNodeIndex_t    index_;

    /** The depth of the node (in the whole tree -- the root is at depth 0). */
    int                depth_;

    /** The solution estimate. The smaller the better. */
    double             solEstimate_;

    /** The quality of this node. The smaller the better. */
    double             quality_;
    
    /** The parent of the tree node. */
    AlpsTreeNode*      parent_;

    /** The index of parent of the tree node. Used in decoding sub tree. */
    AlpsNodeIndex_t    parentIndex_;

    /** The number of children. */
    int                numChildren_;
   
#if defined(ALPS_MAX_CHILD_NUM) // *FIXME* : Do we want ifdefs?
    /** The array of pointers to the children. */
    AlpsTreeNode*      children_[ALPS_MAX_CHILD_NUM];
#else
    AlpsTreeNode**     children_;
#endif

    /** Indicate whether the node description is explicit(1) or relative(0). 
	Default is relative. */
    int                explicit_;
    
    /** The actual description of the tree node. */
    AlpsNodeDesc*      desc_;

    /** The current status of the node. */
    AlpsNodeStatus     status_;

    /** A pointer to the knowledge broker of the process where this node is
	processed. */
    // Need broker to get incumbent value and add solution when process().
    AlpsKnowledgeBroker*  knowledgeBroker_;
    
    /** Various mark used in splitting and passing subtrees. */
    // 0: default; 1: in subtree to be sent: 2: in subtree's node pool 
    int sentMark_;   
    
    /** When processing it, if it is in the diving processing. */
    bool diving_;
    
 public:
    AlpsTreeNode() 
	:
	active_(false),
	index_(-1),
	depth_(-1),
        solEstimate_(ALPS_OBJ_MAX),
	quality_(ALPS_OBJ_MAX),   // Smaller than default incumbentValue
	parent_(0),
	parentIndex_(-1),
	numChildren_(0),
#if defined(ALPS_MAX_CHILD_NUM) // *FIXME* : Do we want ifdefs?
	// AlpsTreeNode*     children_[ALPS_MAX_CHILD_NUM];
#else
	children_(0),
#endif
        explicit_(0),
	desc_(0),
	status_(AlpsNodeStatusCandidate),
	knowledgeBroker_(0),
	sentMark_(0),
        diving_(false)
	{ setType(AlpsKnowledgeTypeNode); }
    
    virtual ~AlpsTreeNode() {
        assert(numChildren_ == 0);
	//std::cout << "---- delete Alps part of node " << index_ << std::endl;
#if ! defined(ALPS_MAX_CHILD_NUM)
	if (children_ != 0) {
	    delete [] children_;
	    children_ = 0;
	}
#endif
	if (desc_ != 0) {
	    delete desc_;  
	    desc_ = 0;
	}
    }
    
    bool operator<(const AlpsTreeNode& compNode)
	{ return quality_ < compNode.getQuality(); }
    
    /** Access the desc so that can modify it. */
    AlpsNodeDesc* modifyDesc() { return desc_; }
    AlpsNodeDesc* getDesc() const { return desc_; }
    void setDesc(AlpsNodeDesc* desc) { desc_ = desc; }    

    /** Functions to access/set the knwoledge broker */
    inline AlpsKnowledgeBroker*  getKnowledgeBroker() const 
	{ return knowledgeBroker_; }
    inline void setKnowledgeBroker(AlpsKnowledgeBroker* kb) 
	{ knowledgeBroker_ = kb; }

    /** The purpose of this function is be able to create the children of 
	a node after branching. */
    /* FIXME: I think that we probably want the argument to be a diff'd
       description, but this is open to debate. Maybe we should
       overload this method and have a version that creates a diff'd
       description, as well as one that creates an explicit
       description. */
    virtual AlpsTreeNode* createNewTreeNode(AlpsNodeDesc*& desc) const = 0;

    /** Query/set the current status. */
    ///@{
    inline AlpsNodeStatus getStatus() const { return status_; }
    inline void setStatus(const AlpsNodeStatus stat) { status_ = stat; }
    ///@}
    
    /** Query functions about specific stati. */
    ///@{
    inline bool isCandidate() const {
	return status_ == AlpsNodeStatusCandidate; }
    inline bool isEvaluated() const {
	return status_ == AlpsNodeStatusEvaluated; }
    inline bool isPregnant() const  {
	return status_ == AlpsNodeStatusPregnant; }
    inline bool isBranched() const  {
	return status_ == AlpsNodeStatusBranched; }
    inline bool isFathomed() const  {
	return status_ == AlpsNodeStatusFathomed; }
    inline bool isDiscarded() const  {
	return status_ == AlpsNodeStatusDiscarded; }
    ///@}
    
    /** Query/set node in-process indicator. */
    ///@{
    inline bool isActive() const { return active_; }
    inline void setActive(const bool yesno) { active_ = yesno; }
    ///@}

    /** Query/set node identifier (unique within subtree). */
    ///@{
    inline AlpsNodeIndex_t getIndex() const { return index_; }
    inline void setIndex(const AlpsNodeIndex_t index) { index_ = index; }
    ///@}
   
    /** Query/set what depth the search tree node is at. */
    ///@{
    inline int getDepth() const { return depth_; }
    inline void setDepth(const int depth) { depth_ = depth; }
    ///@}

    /** Query/set the solution estimate	of the node. */
    ///@{
    inline double getSolEstimate() const { return solEstimate_; }
    inline void setSolEstimate(double est) { solEstimate_ = est; }
    ///@}

    /** Query/set the quality of the node. */
    ///@{
    inline double getQuality() const { return quality_; }
    inline void setQuality(double quality) { quality_ = quality; }
    ///@}

    /** Query/set what the number of children. */
    ///@{
    inline int getNumChildren() const { return numChildren_; }
    inline void setNumChildren(const int numChildren) {
	numChildren_ = numChildren;
#if ! defined(ALPS_MAX_CHILD_NUM)
	if (children_ != 0) {
	    delete [] children_;
	    children_ = 0;
	}
	children_ = new AlpsTreeNode*[numChildren_];
#endif
    }
    // Change by s
    inline void modifyNumChildren(const int s) { numChildren_ += s; }
    ///@}

    // *FIXME* : Sanity check. Maybe we should check that the argument is in
    // the correct range, but how do we do that? This makes the code
    // inefficient so it should be done with #ifdefs so it can be compiled
    // out. But in that case, we should move this code to the implementation
    // file and it won't get inlined anymore.

    /** Query/set pointer to the ith child. */
    ///@{
    inline AlpsTreeNode* getChild(const int i) const { return children_[i]; }
 
    //FIXME: Compiler complains about this second declaration. Not sure how to
    //declare a const and a non-const version of a function with the same
    //arguments...
    // /** Returns a const pointer to the ith child. */
    // const AlpsTreeNode* getChild(const int i) const { return children_[i]; }
    inline void setChild(const int i, AlpsTreeNode* node)
	{ children_[i] = node; }
    ///@}

    /** Remove the pointer to given child from the list of children. This
	method deletes the child as well. An error is thrown if the argument is
	not a pointer to a child. */
    void removeChild(AlpsTreeNode*& child);

    /** Add a child to the list of children for this node. */
    void addChild(AlpsTreeNode*& child);

    /** Removes all the descendants of the node. We might want to do this in
	some cases where we are cutting out a subtree and replacing it with
	another one. */
    void removeDescendants();

    /** Get/set subtree. */
    //inline AlpsSubTree* getSubTree() const { return subTree_; }
    //inline void setSubTree(AlpsSubTree* tree) { subTree_ = tree; }
    
    /** Get/set the parent of the node */
    ///@{
    inline AlpsTreeNode* getParent() const { return parent_; }
    inline void setParent(AlpsTreeNode* parent) { parent_ = parent; }
    ///@}

    /** Get/set the index of the parent of the node. Used in decode subtree. */
    ///@{
    inline AlpsNodeIndex_t getParentIndex() const { return parentIndex_; }
    inline void setParentIndex(AlpsNodeIndex_t index) 
	{ parentIndex_ = index; }
    ///@}

    /** Get/set the indication of whether the node has full or differencing 
	description. */
    ///@{
    inline int getExplicit() const { return explicit_; }
    inline void setExplicit(int fp) { explicit_ = fp; }
    ///@}

    /** Convert explicit description to difference, and vise-vesa */
    ///@{
    virtual void convertToExplicit() {}
    virtual void convertToRelative() {}
    ///@}

    /** If the this node is in a diving process. */
    ///@{
    inline int getDiving() const { return diving_; }
    inline void setDiving(const bool d) { diving_ = d; }
     ///@}

    /** Various marks used in parallel code. */
    ///@{
    inline int getSentMark() const { return sentMark_; }
    inline void setSentMark(const int tf) { sentMark_ = tf; }
    ///@}

    /** 
     *  Perform the processing of the node. For branch and bound, this would
     *  mean performing the bounding operation. The minimum requirement for
     *  this method is that it set the status of the node.
     *  <ul>
     *  <li> active_ flag is set
     *  <li> the description is explicit
     *  <li> status_ is not pregnant nor fathomed
     *  </ul>
     *  
     *  The status of the node is set to one of the following (and children may
     *  be internally stored, etc.):
     *  <ul>
     *  <li> [candidate:] the node is put back into the priority queue for
     *       later processing
     *  <li> [evaluated:] the node is put back into the priority queue for
     *       later branching
     *  <li> [pregnant:] the node has already decided how to branch and will
     *       give birth to the children when instructed with the \c branch()
     *       function which will be the next method invoked for the node.
     *  <li> [branched:] the node has tried to create new nodes by calling
     *       <code>createChildren<\code>.
     *  <li> [fathomed:] the node will not be further considered in this phase.
     *  </ul>
     *  \param isRoot  Indicate if this node is a root of a subtree.
     *  \param rampUp  Indicate if it is in ramp up period. Only useful
     *                 for parallel code. 
     * Currently, the return code of this method is not used. 
     */
    virtual int process(bool isRoot = false, bool rampUp = false) = 0;
    
    /** This method must be invoked on a \c pregnant node (which has all the
	information needed to create the children) and should create the
	children in the data structure of the node. The stati of the children
	can be any of the ones \c process() can return.  The third component 
	is the \c priority.

	NOTE: This method may be almost empty if the descriptions of the
	children were created when the node became pregnant. In that case this
	routine is pretty much just copying data. */
    virtual std::vector< CoinTriple<AlpsNodeDesc*, AlpsNodeStatus, double> > 
	branch() = 0;

 protected:
    
    /** Pack Alps portion of node into an encoded object. */
    AlpsReturnStatus encodeAlps(AlpsEncoded *encoded) const;
    
    /** Unpack Alps portion of node from an encoded object. */
    AlpsReturnStatus decodeAlps(AlpsEncoded &encoded);

};
#endif
