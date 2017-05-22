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

#ifndef BlisPseudo_h_
#define BlisPseudo_h_

#include "CoinError.hpp"
#include "AlpsKnowledge.h"

//#############################################################################

class BlisPseudocost : public AlpsKnowledge
{
private:
    /** Use to calculate score. */
    double weight_;
    
    /** Average object change when branching up. */
    double upCost_;
    
    /** How many times being branched up. */
    int upCount_;
    
    /** Average object change when branching down. */
    double downCost_;
    
    /** How many times being branched down. */
    int downCount_;
    
    /** The estimated importance. 
        Score = weight * MIN(downCost_, upCost_) +
        (1.0 - weight) * MAX(downCost_, upCost_)
    */
    double score_;
    
public:
    /** Default constructor. */
    BlisPseudocost() : 
        weight_(1.0),
        upCost_(0.0), 
        upCount_(0),
        downCost_(0.0),
        downCount_(0), 
        score_(0.0)
        {}
	
        /** Useful constructor. */
	BlisPseudocost(double uc, 
		       int un,
		       double dc, 
                   int dn,
                   double s)
        :
        weight_(1.0),
        upCost_(uc), 
        upCount_(un),
        downCost_(dc),
        downCount_(dn),
        score_(s) 
        {}
	
	/** Copy constructor */
	BlisPseudocost(const BlisPseudocost& cost) {
	    weight_ = cost.weight_;
	    upCost_ = cost.upCost_;
	    upCount_ = cost.upCount_;
	    downCost_ = cost.downCost_;
	    downCount_ = cost.downCount_;
	    score_ = cost.score_;
	}
	
	/** Overload operator = */
	BlisPseudocost& operator=(const BlisPseudocost& cost) {
	    weight_ = cost.weight_;
	    upCost_ = cost.upCost_;
	    upCount_ = cost.upCount_;
	    downCost_ = cost.downCost_;
	    downCount_ = cost.downCount_;
	    score_ = cost.score_;
	    return *this;
	}
	
    /** Set weigth. */
    void setWeight(double w) { 
        if (w < 0.0 || w > 1.0) {   
            throw CoinError("weight is not in range [0,1]", "setWeight", 
                            "BlisPseudo");
        }
        weight_= w; 
    }

    /** Update pseudocost. */
    void update(const int dir,
                const double parentObjValue,
                const double objValue,
                const double solValue);

    /** Update pseudocost. */
    void update(const int dir,
                const double objDiff,
                const double solValue);

    /** Update pseudocost. */
    void update(double upCost,
                int upCount,
                double downCost,
                int downCount);
    
    /** Get up branching count. */
    int getUpCount() { return upCount_; }

    /** Get up branching cost. */
    double getUpCost() { return upCost_; }
    
    /** Get down branching count. */
    int getDownCount() { return downCount_; }
    
    /** Get down branching cost. */
    double getDownCost() { return downCost_; } 

    /** Get importance. */
    double getScore() { return score_; }

    /** Set importance. */
    void setScore(double s) { score_ = s; }

    /** Pack pseudocost to the given object. */
    AlpsReturnStatus encodeTo(AlpsEncoded *encoded) const;
    
    /** Unpack pseudocost from the given encode object. */
    AlpsReturnStatus decodeFrom(AlpsEncoded &encoded);

    using AlpsKnowledge::encode ;
    /** Encode this node for message passing. */
    virtual AlpsEncoded* encode() const;

    /** Decode a node from an encoded object. */
    virtual AlpsKnowledge* decode(AlpsEncoded&) const;
};

#endif
