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

#ifndef AlpsSolution_h_
#define AlpsSolution_h_

#include <iosfwd>
#include <map>
#include <vector>

#include "AlpsKnowledge.h"
#include "AlpsTreeNode.h" // to get AlpsNodeIndex_t

//#############################################################################

class AlpsSolution : public AlpsKnowledge { 

 private:
    /** Diable copy constructor and assignment. */
    AlpsSolution(const AlpsSolution&);
    AlpsSolution& operator=(const AlpsSolution&);

    /** The index of the node where the solution was found. */ 
    int index_;
    
    /** The depth of the node where the solution was found. */ 
    int depth_;

 public:
    
    /** Default constructor. */
    AlpsSolution() :
       index_(-1),
       depth_(-1)
    {
       setType(AlpsKnowledgeTypeSolution);
    }

    /** Constructor to set index and depth. */
    AlpsSolution(const AlpsNodeIndex_t i, const int d) :
       index_(i),
       depth_(d)
    {
       setType(AlpsKnowledgeTypeSolution);
    }

    /** Destructor. */
    virtual ~AlpsSolution() {}

    /** Get index where solution was found */
    AlpsNodeIndex_t getIndex() { return index_; }
    
    /** Set index where solution was found */
    void setIndex(const AlpsNodeIndex_t i) { index_ = i; }
    
    /** Get depth where solution was found */
    int getDepth() { return depth_; }
    
    /** Set depth where solution was found */
    void setDepth(const int d) { depth_ = d; }
    
    /** Print out the solution.*/
    virtual void print(std::ostream& os) const{
	os << "WARNING: No solution print function is defined." << std::endl;
    }
};

#endif
