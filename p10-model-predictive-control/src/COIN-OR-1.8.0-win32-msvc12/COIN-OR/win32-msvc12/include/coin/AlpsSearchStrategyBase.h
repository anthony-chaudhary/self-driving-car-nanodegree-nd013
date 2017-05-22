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

#ifndef AlpsSearchStrategyBase_h_
#define AlpsSearchStrategyBase_h_

#include "Alps.h"

class AlpsModel;
class AlpsSubTree;
class AlpsTreeNode;

//#############################################################################

/** Subtree and node selection rule, which is uese when
    1) comparing subtrees or nodes when storing them in knowledge pools.
    2) selecting the next subtee or node to be processed. 

    The steps required to define a new search strategy are:
    1) derive two subclasses from \code AlpsSearchStrategy, one for subtree 
       and one for node. (see AlpsSearchStrategyActual.h),
    2) override the \code compare() member function, and
    3) set it to knowledge broker.
*/
template<class T> 
class AlpsSearchStrategy
{
protected:
    /** Used to change search behavior. */
    double weight_;
    
    /** Which type of strategy: best-first, etc. */
    int type_;
    
public:
    /** Default Constructor. */
    AlpsSearchStrategy() : weight_(-1.0), type_(AlpsSearchTypeBestFirst){}
        
    /** Default destructor. */
    virtual ~AlpsSearchStrategy() {}
        
    /** Compare the preference of x and y. Return true if prefer y; 
        return false if prefer x. The function is used when:
        1) comparing subtrees or nodes when storing them in knowledge pools.
        2) selecting the next subtee or node to be processed. 
    */
    virtual bool compare(T x, T y) = 0;
    
    bool operator() (T x, T y) {
        return compare(x, y);
    }
    
    /** @name Get/set weight
     *
     */
    //@{
    inline double getWeight() const { return weight_; }
    inline void setWeight(double nw) { weight_ = nw; }
    //@}
    
    /** @name Node selection
     *
     */
    //@{
    /** Selection next node to be processed. */
    virtual AlpsTreeNode* selectNextNode(AlpsSubTree *subTree) 
    { AlpsTreeNode *temp = 0; return temp; }

    /** Create new nodes from a pregnant node and store them in node pool. */
    virtual void createNewNodes(AlpsSubTree *subTree, AlpsTreeNode *node)
    { }
    //@}

    /** Get type of strategy */
    int getType(){ return type_; }
    
    /** Set type of strategy */
    void setType(int t) { type_ = t; }
};

//#############################################################################

/** Compare function for priority queue. */
template<class T>
class AlpsCompare 
{
public:
    AlpsSearchStrategy<T>* strategy_;
    
public:
    /** Default Constructor */
    AlpsCompare () : strategy_(0) {}
    virtual ~AlpsCompare() {}

    void setComareBase(AlpsSearchStrategy<T>* c) { 
        strategy_ = c;
    }
    
    bool operator() (T x, T y) {
        return strategy_->compare(x, y);
    }
};

//#############################################################################

#endif
