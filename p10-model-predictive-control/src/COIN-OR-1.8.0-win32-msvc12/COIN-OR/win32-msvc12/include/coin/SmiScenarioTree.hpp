// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SmiScenarioTree_H
#define SmiScenarioTree_H

/** Scenario Tree

 This class is used for storing and accessing scenario trees.

 */

#include <vector>
#include <map>
#include <iostream>
#include <cassert>

#include "CoinPragma.hpp"

/** SmiTreeNode template class.

 Manages pointers to parent, child and sibling for tree navigation.
 Template class instance is a pointer to an object that
 must be created with "new" operator.
 */

template<class T>
class SmiTreeNode {
	//friend void SmiTreeNodeUnitTest();
public:

	typedef std::map<int, SmiTreeNode<T>*> child_label_map;

	bool hasParent() {
		return (parent_ != NULL);
	}
	bool hasChild() {
		return (child_ != NULL);
	}
	bool hasSibling() {
		return (sibling_ != NULL);
	}

	SmiTreeNode<T> *getParent() {
		return parent_;
	}
	SmiTreeNode<T> *getChild() {
		return child_;
	}
	SmiTreeNode<T> *getSibling() {
		return sibling_;
	}

	void setLastChildLabel(int label) {
		child_labels_.insert(std::make_pair(label, this->getChild()));
	}

	SmiTreeNode<T> *getChildByLabel(int n) {

		/*
		 //AJK debug code
		 child_label_map::iterator begpos = child_labels_.begin();
		 child_label_map::iterator endpos = child_labels_.end();
		 while(begpos!=endpos)
		 {
		 printf(" found label %d \n",begpos->first);
		 ++begpos;
		 }
		 */
		typename child_label_map::iterator pos = child_labels_.find(n);
		if (pos != child_labels_.end())
			return pos->second;
		else
			return NULL;
	}

	int depth() {
		return depth_;
	}
	int numChildren() {
		return nchild_;
	}
	int scenario() {
		return scen_;
	}
	void setScenario(int s) {
		scen_ = s;
	}
	
	// Returns new child node with cd, linked in the tree
	SmiTreeNode<T> * addChild(T cd, int scenario) {
		SmiTreeNode<T> *c = new SmiTreeNode(cd);

		c->parent_ = this;
		c->depth_ = depth_ + 1;
		c->sibling_ = child_;
		c->scen_ = scenario;
		nchild_++;
		child_ = c;
		return c;
	}

	std::vector<SmiTreeNode<T> *> *getChildren() {
		SmiTreeNode<T> *pnode = this;
		int i = this->numChildren();
		if (i == 0) {
			return NULL;
		}
		std::vector<SmiTreeNode<T> *> *vec = new std::vector<SmiTreeNode<T> *> (i);
		(*vec)[--i] = pnode = pnode->getChild();
		while (i > 0)
			(*vec)[--i] = pnode = pnode->getSibling();
		return vec;
	}

	T getDataPtr() {
		return ptr_;
	}

	//--------------------------------------------------------------------------
	/**@name Constructors, destructors and major modifying methods*/
	//@{
	/// Default Constructor creates an empty node
	SmiTreeNode<T> () {
		parent_ = NULL;
		child_ = NULL;
		sibling_ = NULL;
		nchild_ = 0;
		depth_ = 0;
		scen_ = -1;
	}

	/// Constructor from P
	SmiTreeNode<T> (T p) {
		parent_ = NULL;
		child_ = NULL;
		sibling_ = NULL;
		nchild_ = 0;
		depth_ = 0;
		ptr_ = p;
		scen_ = -1;
	}

	/// Destructor

	~SmiTreeNode<T>() {
		delete sibling_;
		delete child_;
	}

	//@}

protected:

	void setChild(SmiTreeNode<T> *c) {
		child_ = c;
	}
	void setSibling(SmiTreeNode<T> *s) {
		sibling_ = s;
	}
	SmiTreeNode<T> *getParentP() {
		return parent_;
	}
	SmiTreeNode<T> *getChildP() {
		return child_;
	}
	SmiTreeNode<T> *getSiblingP() {
		return sibling_;
	}

private:
	SmiTreeNode<T> *parent_;
	SmiTreeNode<T> *child_;
	SmiTreeNode<T> *sibling_;
	int scen_;
	int nchild_;
	int depth_;
	T ptr_;
	child_label_map child_labels_;

};

//#############################################################################
/** A function that tests the methods in the SmiTreeNode class. The
 only reason for it not to be a member method is that this way it doesn't
 have to be compiled into the library. And that's a gain, because the
 library should be compiled with optimization on, but this method should be
 compiled with debugging. */
void SmiTreeNodeUnitTest();

/*TODO: split this class into two classes
 * 	1) SmiScenarioTree
 *  2) SmiLabelledTree
 */

template<class T>
class SmiScenarioTree {
	//friend void SmiScenarioTreeUnitTest();
public:

	//---------------------------------------------------------------------------
	/**@name Iterators */
	//@{


	/** begin */
	typename std::vector<T>::iterator treeBegin() {
		return node_data.begin();
	}
	/** end */
	typename std::vector<T>::iterator treeEnd() {
		return node_data.end();
	}
	/** whole tree */
	std::vector<T> &wholeTree() {
		return node_data;
	}

	/** scenario iterators
	 TODO: native code for these iterators that does not
	 depend on copying. */
	typename std::vector<T>::iterator scenBegin(int s) {
		getScenario(s);
		return scen_data.begin();
	}

	typename std::vector<T>::iterator scenEnd(int s) {
		getScenario(s);
		return scen_data.begin() + leaf_[s]->depth() + 1;
	}

	//---------------------------------------------------------------------------
	/**@name Query members */
	//@{

	/** Get root node.	*/
	SmiTreeNode<T> *getRoot() {
		return root_;
	}

	/** Get leaf node. */
	SmiTreeNode<T> *getLeaf(int scn) {
		return leaf_[scn];
	}

	/** Get node identified by scenario/stage.	*/
	SmiTreeNode<T> *find(unsigned int scenario, int stage) {
		assert (scenario < leaf_.size());
		SmiTreeNode<T> * n = leaf_[scenario];
		assert (stage < n->depth() + 1);
		while (stage < n->depth())
			n = n->getParent();
		return n;
	}

	/** get number of scenarios */
	int getNumScenarios() {
		return (int) leaf_.size();
	}

	/** Get node identified by longest match to array of labels */
	SmiTreeNode<T> *find(std::vector<int> &label) {

		assert(label.size()>0);
		SmiTreeNode<T> *n = root_, *next;
		unsigned int i = 0;
		while ((i < label.size()) && (next = n->getChildByLabel(label[i]))) {
			++i;
			n = next;
		}
		return n;
	}

	/** Get node identified by longest match to array of labels */
	SmiTreeNode<T> *find(int *label, unsigned int sz) {

		assert(sz>0);
		SmiTreeNode<T> *n = root_, *next;
		if (!n)
			return n;
		unsigned int i = 0;
		while ((i < sz) && (next = n->getChildByLabel(label[i]))) {
			++i;
			n = next;
		}
		return n;
	}

	/** Get vector of node data for given scenario */
	std::vector<T> &getScenario(int scenario) {
		assert (scenario < (int) leaf_.size());
		SmiTreeNode<T> * n = leaf_[scenario];

		//if ( n->getDataPtr()==scen_data[n->depth()] ) return scen_data;
		//Christian: Why is this necessary? Reason: Number of Stages places should be readily available in the vector, if this is not already the case, do it..
		//int ns = n->depth() + 1 - scen_data.size();
		//for (int j = 0; j < ns; j++)
		//	scen_data.push_back(n->getDataPtr());
	
		//Christian: TODO: Why not change this so, that it looks better? i.e. i-- and i= n->depth(); and i >= 0?
		//Christian: TODO: Think about changing container from vector to deque or list.. 
		int i = n->depth() + 1;
		scen_data.resize(i,n->getDataPtr()); //Maybe a call to capacity should do it too.. ?! TODO
		while (i > 0) {
			scen_data[--i] = n->getDataPtr();
			n = n->getParent();
		}
		return scen_data;
	}

	//@}

	//---------------------------------------------------------------------------
	/**@name Tree modification members */
	//@{
	/** Add new path from branching node to leaf.
	 The branching node is the one belonging to "brscenario" at depth "stage".
	 Length of incoming "pathdata" vector is leaf->depth() - stage.
	 Responsibility for memory management of SmiTreeNodeData elements
	 is assigned to SmiScenarioTree.
	 SmiTreeNodeData elements must be created with "new" operator.
	 */
	int addPathtoLeaf(int brscenario, int stage, std::vector<T> &pathdata,
			unsigned int start = 0) {
		SmiTreeNode<T> *parent = NULL;
		int scenario = static_cast<int>(leaf_.size());
		if (scenario)
			parent = find(brscenario, stage);

		parent = addNodesToTree(parent, scenario, pathdata, start);

		if (pathdata.size()) {
			leaf_.push_back(parent);
		}
		return static_cast<int>(leaf_.size()) - 1;

	}

	/** Set child labels */
	void setChildLabels(SmiTreeNode<T> *n, std::vector<int> labels) {
		int t = n->depth();
		while (n->hasChild()) {
			n->setLastChildLabel(labels[++t]);
			n = n->getChild();
		}
	}

	/** Add new path using labels to find branching node.
	 The length of the incoming path is leaf.depth().
	 Responsibility for memory management of SmiTreeNodeData elements
	 is assigned to SmiScenarioTree.
	 SmiTreeNodeData elements must be created with "new" operator.
	 */
	int addPathtoLeaf(std::vector<int> &labels, std::vector<T> &pathdata) {
		SmiTreeNode<T> *parent = NULL;
		int scenario = static_cast<int>(leaf_.size());
		if (scenario)
			parent = find(labels);

		unsigned int i = 0;
		if (parent)
			i = parent->depth() + 1;

		parent = addNodesToTree(parent, scenario, pathdata, i);
 
		if (pathdata.size()) {
			leaf_.push_back(parent);
		}
		return static_cast<int>(leaf_.size()) - 1;

	}

	SmiTreeNode<T> * addNodesToTree(SmiTreeNode<T> *parent, int scenario,
			std::vector<T> &pathdata, int start) {

		for (unsigned int i = start; i < pathdata.size(); ++i) {
			if (parent) {
				parent = parent->addChild(*&pathdata[i], scenario);
			} else {
				parent = root_ = new SmiTreeNode<T> (*&pathdata[0]);
				root_->setScenario(scenario);
			}
			// add data to full node_data array
			node_data.push_back(*&pathdata[i]);
		}

		return parent;
	}
	//@}
	//--------------------------------------------------------------------------
	/**@name Constructors, destructors and major modifying methods*/
	//@{
	/// Default Constructor creates an empty scenario tree
	SmiScenarioTree<T> () :
		leaf_(0), root_(NULL) {
			this->node_data.reserve(2);
	}

	/// Destructor
	virtual ~SmiScenarioTree<T> () {
		delete root_;
	}
	//@}

private:
	std::vector<T> node_data;
	std::vector<T> scen_data;
	std::vector<SmiTreeNode<T> *> leaf_;
	SmiTreeNode<T> *root_;
};

//#############################################################################
/** A function that tests the methods in the SmiScenarioTree class. The
 only reason for it not to be a member method is that this way it doesn't
 have to be compiled into the library. And that's a gain, because the
 library should be compiled with optimization on, but this method should be
 compiled with debugging. */
void SmiScenarioTreeUnitTest();

#endif //SmiScenarioTree_H
