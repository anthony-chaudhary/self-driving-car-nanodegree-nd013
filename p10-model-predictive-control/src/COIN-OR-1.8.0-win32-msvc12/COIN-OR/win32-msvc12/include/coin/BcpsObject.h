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

#ifndef BcpsObject_h_
#define BcpsObject_h_

#include "Alps.h"
#include "AlpsKnowledge.h"

#include "Bcps.h"

class BcpsModel;
class BcpsBranchObject;

//#############################################################################

#define    BCPS_NONREMOVALBE 0x0001
#define    BCPS_BRACNEDON    0x0010
#define    BCPS_SENDABLE     0x0100

//#############################################################################

/** typedef's for various integral types. They are defined here so it's easy
    to allocate more (or less) bits for them as needed. */
/**@{*/
typedef char BcpsIntegral_t;
/**@}*/

//#############################################################################
/** The possible types of an object: primal or dual (i.e., variable or
    constraint */
//#############################################################################

enum BcpsObject_t {
    BCPS_PRIMAL_OBJECT = 0,
    BCPS_VAR_OBJECT = 0,
    BCPS_DUAL_OBJECT = 1,
    BCPS_CON_OBJECT = 1
};

//#############################################################################
/** The possible representations types of an object */
//#############################################################################

enum BcpsObjRep_t {
    BCPS_CORE = 0,
    BCPS_INDEXED = 1,
    BCPS_ALGO = 2
};

//#############################################################################
/** A class for describing the objects that comprise a BCPS subproblem. At the
    BCPS level, all that is assumed about an object is that it has bounds and
    that it has an integrality type. The concept that an object */
//#############################################################################

class BcpsObject : public AlpsKnowledge {

 protected:

    /** Global index of this object */
    int objectIndex_;
    
    /** Core, indexed, or algorithmic */
    BcpsObjRep_t repType_;
    
    /** The integrality type of the object, i.e., what values it can 
	take up between the specified bounds. (Possible options: 'C' 
	for continuous, 'I' for general integer, 'B' for binary and 
	'S' for semicontinuous) */
    BcpsIntegral_t intType_;

    /** Valid in the whole tree or only the subtree rooted at the node that
	generate this object. */
    BcpsValidRegion validRegion_;

    /** The status of the object */
    // branched on? non-removable ? can it be sent to the pool?
    // do we need it at all?
    int status_;
    
    /** The lower bound of the object when it was first created */
    double lbHard_;
    
    /** The upper bound of the object when it was first created */
    double ubHard_;
    
    /** The current lower bound of the object */
    double lbSoft_;
    
    /** The current upper bound of the object */
    double ubSoft_;
    
    /** The hash value of this object. */
    double hashValue_;

    /** Number of inactive when in formulation. */
    int numInactive_;

    /** Effectiveness: nonnegative value. */
    double effectiveness_;
    
 public:

    BcpsObject()
	:
        objectIndex_(-1),
	repType_(BCPS_CORE),
	intType_('C'),
	validRegion_(BcpsValidLocal),
	status_(0),
	lbHard_(0.0),
	ubHard_(0.0),
	lbSoft_(0.0),
	ubSoft_(0.0),
	hashValue_(0.0),
	numInactive_(0),
	effectiveness_(-ALPS_DBL_MAX)
	{}
    
    BcpsObject(double lbh, double ubh, double lbs, double ubs) 
	:
        objectIndex_(-1),
	repType_(BCPS_CORE),
	intType_('C'),
 	validRegion_(BcpsValidLocal),
	status_(0),
	lbHard_(lbh),
	ubHard_(ubh),
	lbSoft_(lbs),
	ubSoft_(ubs),
	hashValue_(0.0),
	numInactive_(0),
	effectiveness_(-ALPS_DBL_MAX)
	{}

    virtual ~BcpsObject() {}

    /** Copy constructor. */
    BcpsObject (const BcpsObject & rhs) {
        objectIndex_ = rhs.objectIndex_;
	repType_ = rhs.repType_;
	intType_ = rhs.intType_;
	validRegion_ = rhs.validRegion_,
	status_ = rhs.status_;
	lbHard_ = rhs.lbHard_;
	ubHard_ = rhs.ubHard_;
	lbSoft_ = rhs.lbSoft_;
	ubSoft_ = rhs.ubSoft_;
        hashValue_ = rhs.hashValue_;
	numInactive_ = rhs.numInactive_;
	effectiveness_ = rhs.effectiveness_;
    }

    /** Assignment operator. */
    BcpsObject & operator = ( const BcpsObject& rhs);
    
    /** Clone an enity. */
    virtual BcpsObject * clone() const {
        BcpsObject * be = NULL;
        return be;
    }

    /** Return the value of the appropriate field */
    /**@{*/
    inline int getObjectIndex() const         { return objectIndex_; }
    inline BcpsObjRep_t getRepType() const    { return repType_; }
    inline BcpsIntegral_t getIntType() const  { return intType_; }
    inline BcpsValidRegion getValidRegion() const  { return validRegion_; }
    inline int getStatus() const              { return status_; }
    inline double getLbHard() const           { return lbHard_; }
    inline double getUbHard() const           { return ubHard_; }
    inline double getLbSoft() const           { return lbSoft_; }
    inline double getUbSoft() const           { return ubSoft_; }
    inline int getNumInactive() const         { return numInactive_; }
    inline double getEffectiveness() const    { return effectiveness_; }
    /**@}*/

    /** Set the appropriate property */
    /**@{*/
    inline void setObjectIndex(int ind)       { objectIndex_ = ind; }    
    inline void setRepType(BcpsObjRep_t rt)   { repType_ = rt; }
    inline void setIntType(BcpsIntegral_t it) { intType_ = it; }
    inline void setValidRegion(BcpsValidRegion val) { validRegion_ = val;} 
    inline void setStatus(int st)             { status_ |= st; }
    inline void setLbHard(double lb)          { lbHard_ = lb; }
    inline void setUbHard(double ub)          { ubHard_ = ub; }
    inline void setLbSoft(double lb)          { lbSoft_ = lb; }
    inline void setUbSoft(double ub)          { ubSoft_ = ub; }
    inline void setNumInactive(int num)       { numInactive_ = num; }
    inline void setEffectiveness(double eff)  { effectiveness_= eff; }
    /**@}*/
    
    /** Hashing */
    /**@{*/
    virtual void hashing(BcpsModel *model=NULL)    { hashValue_ = 0.0; }
    double hashValue() const  {  return hashValue_; }
    /**@}*/

    /** Infeasibility of the object
	This is some measure of the infeasibility of the object. It should be
	scaled to be in the range [0.0, 0.5], with 0.0 indicating the object
	is satisfied.
	
	The preferred branching direction is returned in preferredWay,
	
	This is used to prepare for strong branching but should also think of
	case when no strong branching
	
	The object may also compute an estimate of cost of going "up" or 
	"down". This will probably be based on pseudo-cost ideas. */
    virtual double infeasibility(BcpsModel *m, int &preferredWay) const {
        return 0.0; 
    }
    
    /** Look at the current solution and set bounds to match the solution.*/
    virtual void feasibleRegion(BcpsModel *m) {}
    
    /** Create a branching object and indicate which way to branch first.
	The branching object has to know how to create branches (fix
	variables, etc.) */
    virtual BcpsBranchObject * createBranchObject(BcpsModel *m, int way) const{
        BcpsBranchObject *be = NULL;
        return be;
    }
    
    /** \brief Given a valid solution (with reduced costs, etc.),
	return a branching object which would give a new feasible
	point in a good direction. 
	If the method cannot generate a feasible point (because there aren't
	any, or because it isn't bright enough to find one), it should
	return null. */
    virtual BcpsBranchObject * preferredNewFeasible(BcpsModel *m) const {
        BcpsBranchObject * temp = NULL; 
        return temp; 
    }
  
    /** \brief Given a valid solution (with reduced costs, etc.),
	return a branching object which would give a new feasible
	point in a bad direction.
	If the method cannot generate a feasible point (because there aren't
	any, or because it isn't bright enough to find one), it should
	return null. */
    virtual BcpsBranchObject * notPreferredNewFeasible(BcpsModel *m) const { 
        BcpsBranchObject * temp = NULL; 
        return temp; 
    }
    
    /** Reset variable bounds to their original values.
        Bounds may be tightened, so it may be good to be able to reset them to
        their original values. */
    virtual void resetBounds(BcpsModel *m) {}
    
    /** Return true if branches created by object will modify 
        variable bounds.*/
    virtual bool boundBranch(BcpsModel *m) const { return true; }
    
    /** Returns floor and ceiling i.e. closest valid points. */
    virtual void floorCeiling(double &floorValue, 
			      double &ceilingValue, 
			      double value,
			      double tolerance) const;
    
    /** Return "up" estimate. Default: 1.0e-5. */
    virtual double upEstimate() const { return 1.0e-5; }
    
    /** Return "down" estimate. Default: 1.0e-5. */
    virtual double downEstimate() const { return 1.0e-5; }

    virtual void printDesc() {}
    
 protected:
    
    /** Pack Bcps part to a encode object. */
    AlpsReturnStatus encodeBcpsObject(AlpsEncoded *encoded) const {
	AlpsReturnStatus status = AlpsReturnStatusOk;
	encoded->writeRep(objectIndex_);
	encoded->writeRep(repType_);
	encoded->writeRep(intType_);
	encoded->writeRep(validRegion_);
	encoded->writeRep(status_);
	encoded->writeRep(lbHard_);
	encoded->writeRep(ubHard_);
	encoded->writeRep(lbSoft_);
	encoded->writeRep(ubSoft_);
	encoded->writeRep(hashValue_);
	return status;
    }

    /** Unpack Bcps part from a encode object. */
    AlpsReturnStatus decodeBcpsObject(AlpsEncoded &encoded){
	AlpsReturnStatus status = AlpsReturnStatusOk;
	encoded.readRep(objectIndex_);
	encoded.readRep(repType_);
	encoded.readRep(intType_);
	encoded.readRep(validRegion_);
	encoded.readRep(status_);
	encoded.readRep(lbHard_);
	encoded.readRep(ubHard_);
	encoded.readRep(lbSoft_);
	encoded.readRep(ubSoft_);
	encoded.readRep(hashValue_);
	return status;
    }

protected:
    
 public:

    using AlpsKnowledge::encode ;
    /** Pack into a encode object. */
    virtual AlpsReturnStatus encode(AlpsEncoded *encoded) {
	AlpsReturnStatus status = AlpsReturnStatusOk;
        assert(0);
	return status;
    }
   
    /** Decode a constraint from an encoded object. */
    virtual AlpsKnowledge* decode(AlpsEncoded& encoded) const {
        AlpsKnowledge* know = NULL;
        assert(0);
        return know;
    }
};


//#############################################################################


typedef BcpsObject*  BcpsObject_p;


//#############################################################################


class BcpsConstraint : public BcpsObject {

 public:
    /** Default constructor. */
    BcpsConstraint() : BcpsObject() { setType(BcpsKnowledgeTypeConstraint); }

    /** Useful constructor. */
    BcpsConstraint(double lbh, double ubh, double lbs, double ubs) 
	:
	BcpsObject(lbh, ubh, lbs, ubs)
	{ setType(BcpsKnowledgeTypeConstraint); }
        
    /** Desctructor constructor. */
    virtual ~BcpsConstraint() {}

    /** Copy constructor. */
    BcpsConstraint(const BcpsConstraint & rhs) 
	:
	BcpsObject(rhs)
	{}    
    
    inline virtual BcpsObject_t getObjType() const { return BCPS_CON_OBJECT; }
};


//#############################################################################


class BcpsVariable : public BcpsObject {
 public:
    /** Default constructor. */
    BcpsVariable() : BcpsObject() { setType(BcpsKnowledgeTypeVariable); }

    /** Useful constructor. */
    BcpsVariable(double lbh, double ubh, double lbs, double ubs) 
	:
	BcpsObject(lbh, ubh, lbs, ubs)
	{ setType(BcpsKnowledgeTypeVariable); }

    /** Destructor. */
    virtual ~BcpsVariable() {}

    /** Copy constructor. */
    BcpsVariable(const BcpsVariable & rhs) 
	:
	BcpsObject(rhs)
	{}

    inline virtual BcpsObject_t getObjType() const { return BCPS_VAR_OBJECT; }
};


//#############################################################################

#endif
