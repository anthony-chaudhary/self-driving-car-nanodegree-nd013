/*===========================================================================*
 * This file is part of the Branch, Constrain and Price Software (BiCePS)    *
 *                                                                           *
 * BiCePS is distributed under the Eclipse Public License as part of the     *
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
 * Copyright (C) 2001-2013, Lehigh University, Yan Xu, and Ted Ralphs.       *
 * All Rights Reserved.                                                      *
 *===========================================================================*/

#ifndef BcpsObjectPool_h_
#define BcpsObjectPool_h_

#include <vector>

#include "AlpsKnowledgePool.h"

#include "BcpsObject.h"

//#############################################################################
/** Object pool is used to store objects */
//#############################################################################

class BcpsObjectPool : public AlpsKnowledgePool {

 protected:

    std::vector<AlpsKnowledge *> objects_;

 public:

    /** Default construct. */
    BcpsObjectPool() {}
    virtual ~BcpsObjectPool() {
	if (! objects_.empty()) {
	    freeGuts();
	}
    }
    
    /** Free object pointers. */
    inline void freeGuts() {
	for (int i = static_cast<int> (objects_.size() - 1); i > -1; --i) {
	    delete objects_[i];
	}
        objects_.clear();
    }
    
    /** Reset to empty. Don't free memory. */
    inline void clear(){ objects_.clear(); }
    
    /** Add a knowledge to pool */
    virtual void addKnowledge(AlpsKnowledge * nk, double priority) { 
	objects_.push_back(nk);
    }
 
    /** Query how many knowledges are in the pool.*/
    virtual int getNumKnowledges() const {
	return static_cast<int>(objects_.size());
    }

    /** Query a knowledge, but doesn't remove it from the pool*/
    virtual std::pair<AlpsKnowledge*, double> getKnowledge() const {
	return std::make_pair(objects_[0], 0.0);
    }

    /** Check whether the pool has knowledge. */
    virtual bool hasKnowledge() const
        { return objects_.empty() ? false : true; }

    /** Delete object k from pool */
    void deleteObject(int k) { 
	assert(k > -1 && k < ((int)objects_.size()));
	
	AlpsKnowledge *objectK = getObject(k);
	std::vector<AlpsKnowledge *>::iterator pos;
	pos = objects_.begin() + k;
	objects_.erase(pos);
	
	// Free memory of object k.
	delete objectK;
    }

    /** Get all objects. */
    const std::vector<AlpsKnowledge *>& getObjects() const { return objects_; }

    /** Get a object. */
    AlpsKnowledge *getObject(int k) const { return objects_[k]; }
};

//#############################################################################

class BcpsConstraintPool : public BcpsObjectPool {
 public:
    BcpsConstraintPool() {}
    virtual ~BcpsConstraintPool() {}

    /** Add a constraint to pool */
    void addConstraint(BcpsConstraint * con) { objects_.push_back(con); }

    /** Delete constraint k from pool */
    void deleteConstraint(int k) { deleteObject(k); }
    
    /** Query how many constraints are in the pool.*/
    int getNumConstraints() const { return getNumKnowledges(); }
    
    /** Get the vector of constraints. */
    const std::vector<AlpsKnowledge *>& getConstraints() const {return objects_;}

    /** Get a constraints. */
    AlpsKnowledge *getConstraint(int k) const {return getObject(k);}
};

//#############################################################################

class BcpsVariablePool : public BcpsObjectPool {
 public:
    BcpsVariablePool() {}
    virtual ~BcpsVariablePool() {}

    /** Add a variable to pool */
    void addVariable(BcpsVariable * var) { objects_.push_back(var); }

    /** Delete variable k from pool */
    void deleteVariable(int k) { deleteObject(k); }

    /** Query how many variables are in the pool.*/
    int getNumVariables() const { return getNumKnowledges(); }

    /** Get the vector of variables. */
    const std::vector<AlpsKnowledge *>& getVariables() const {return objects_;}

    /** Get the vector of variables. */
    AlpsKnowledge *getVariable(int k) const {return getObject(k);}
};

//#############################################################################

#endif // End of file
