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

#ifndef BcpsNodeDesc_h_
#define BcpsNodeDesc_h_

#include "BcpsModel.h"

#include "Alps.h"
#include "AlpsNodeDesc.h"

#include "BcpsObject.h"


//#############################################################################

/** This class contains modifications for a single
    <code>std::vector<T></code> object. */

template <class T> struct BcpsFieldListMod {
    /** How the modification is stored, explicit means complete 
	replacement, relative means relative to some other explicit 
	object vector ("explicit" is a key word so we can't 
	name the field that.) */
    bool relative;

    /** The number of entries to be modified. */
    int  numModify;

    /** The positions to be modified. */
    int* posModify;
    
    /** Values*/
    T*   entries;
};


//#############################################################################

    
/** Here is the set of <code>vectorMod_</code> objects that represent the
    list of objects of a particular type (either in relative or explicit
    form). If <code>numRemove</code> is positive, then
    <code>posRemove</code> contains the positions of the objects that are
    to be deleted from all the lists (that are stored relatively). */

struct BcpsObjectListMod {

    /** The number of entries to be deleted. If this is -1, then all 
	objects should be deleted, i.e., we have an explicit list. */
    int  numRemove;

    /** The positions of the entries to be deleted. */
    int* posRemove;
    
    /** The number of objects that are to added. */
    int  numAdd;
    /** The objects to be added. */
    BcpsObject **objects;
  
    /** These are the data structures that store the changes in the
	individual fields. */
    /** @{ */
    BcpsFieldListMod<double> lbHard;
    BcpsFieldListMod<double> ubHard;
    BcpsFieldListMod<double> lbSoft;
    BcpsFieldListMod<double> ubSoft;
    BcpsFieldListMod<int> status;
    /** @} */
};


//#############################################################################

/**
   For a given type, the objectVecStorage_ structure holds the
   description. This description is explicit if the \c numRemoved member is
   -1. In this case, the \c objects member holds the full list of objects of
   the given type. If an explicit description of the parent is given together
   with a relative description of the current node then the process of
   reconstructing the explicit description of the current node is as follows:
   <ol>
   <li> initialize the explicit list <b>L</b> to be the parent's list;
   <li> remove the objects on the indicated positions from <b>L</b>;
   <li> modify the appropriate members of the objects in <b>L</b> according to
   the vectorMod_ members of the objectVecStorage_ structure;
   <li> append the objects to be added to <b>L</b>.
   </ol>

   If the numRemoved field is -1, that means that the current description is
   explicit. In this case, the contents of the vectorMod_ members are largely
   irrelevant, except for the \c relative fields which indicate whether the
   appropriate \c vectorMod_ member can ever be expressed as a relative
   description or not.

   If the \c numRemoved field is >=0, then the current description is
   considered relative even if every vectorMod_ member contains explicit data.
*/

//#############################################################################

/** The description of a node can be either explicit or relative to its
    parent. In the node there are a number of object types and for each object
    type the explicit/relative description is considered separately.

    If the information on an object type is relative, it means that at least
    one of the fields (lbHard, ubHard, etc.) has a relative description.
*/
class BcpsNodeDesc : public AlpsNodeDesc {

 protected: 

    /** Variable objects. */
    BcpsObjectListMod *vars_;

    /** Constraint objects. */
    BcpsObjectListMod *cons_;
  
 public:

    /** Default constructor. */
    BcpsNodeDesc() { initToNull(); }
    
    /** Useful constructor. */
    BcpsNodeDesc(BcpsModel* m) 
        :
        AlpsNodeDesc(m) { initToNull(); }
    
    /** Destructor. */
    virtual ~BcpsNodeDesc();
    
    /** Initialize member data. */
    void initToNull();
  
    /** Set variable objects. */
    void setVars(int numRem,
                 const int    *posRem,
                 int numAdd,
                 const BcpsObject **objects,
                 bool relvlh,
                 int numvlh, 
                 const int    *vlhp,
                 const double *vlhe,
                 bool relvuh,
                 int numvuh,
                 const int    *vuhp,
                 const double *vuhe,
                 bool relvls,
                 int numvls,
                 const int    *vlsp,
                 const double *vlse,
                 bool relvus,
                 int numvus,
                 const int    *vusp,
                 const double *vuse);
    
    /** Assign variable objects. Take over memory ownership. */
    void assignVars(int numRem,
                    int    *&posRem,
                    int numAdd,
                    BcpsObject **&objects,
                    bool relvlh,
                    int numvlh, 
                    int    *&vlhp,
                    double *&vlhe,
                    bool relvuh,
                    int numvuh,
                    int    *&vuhp,
                    double *&vuhe,
                    bool relvls,
                    int numvls,
                    int    *&vlsp,
                    double *&vlse,
                    bool relvus,
                    int numvus,
                    int    *&vusp,
                    double *&vuse);

    /** Set constraint objects. */    
    void setCons(int numRem,
                 const int    *posRem,
                 int numAdd,
                 const BcpsObject **objects,
                 bool relclh,
                 int numclh,
                 const int    *clhp,
                 const double *clhe,
                 bool relcuh,
                 int numcuh,
                 const int    *cuhp,
                 const double *cuhe,
                 bool relcls,
                 int numcls,
                 const int    *clsp,
                 const double *clse,
                 bool relcus,
                 int numcus,
                 const int    *cusp,
                 const double *cuse);
    
    /** Assign constraint objects. Take over memory ownership. */
    void assignCons(int numRem,
                    int    *&posRem,
                    int numAdd,
                    BcpsObject **&objects,
                    bool relclh,
                    int numclh,
                    int    *&clhp,
                    double *&clhe,
                    bool relcuh,
                    int numcuh,
                    int    *&cuhp,
                    double *&cuhe,
                    bool relcls,
                    int numcls,
                    int    *&clsp,
                    double *&clse,
                    bool relcus,
                    int numcus,
                    int    *&cusp,
                    double *&cuse);
    
    /** Get variable objects. */
    BcpsObjectListMod *getVars() const { return vars_; }
  
    /** Get constraint objects. */
    BcpsObjectListMod *getCons() const { return cons_; }
    
    /** Accesss varaibles. */
    BcpsObjectListMod *vars() { return vars_; }
    
    /** Accesss constraints. */
    BcpsObjectListMod *cons() { return cons_; }

    /** Set variable soft bounds. Take ownerships of arraies. */
    void assignVarSoftBound(int numModSoftVarLB, 
			    int *&varLBi,
			    double *&varLBv,
			    int numModSoftVarUB,
			    int *&varUBi,
			    double *&varUBv);
 
    /** Set variable soft bounds. Don't take ownerships of arraies. */
    void setVarSoftBound(int numModSoftVarLB, 
                         const int *varLBi,
                         const double *varLBv,
                         int numModSoftVarUB,
                         const int *varUBi,
                         const double *varUBv);
  
    /** Set variable hard bounds. Take ownerships of arraies. */
    void assignVarHardBound(int numModHardVarLB, 
			    int *&varLBi,
			    double *&varLBv,
			    int numModHardVarUB,
			    int *&varUBi,
			    double *&varUBv);
 
    /** Set constraint soft bounds.  Don't take ownerships of arraies. */
    void setConSoftBound(int numModSoftConLB, 
                         const int *conLBi,
                         const double *conLBv,
                         int numModSoftConUB,
                         const int *conUBi,
                         const double *conUBv);
    
    /** Set variable hard bounds. Don't take ownerships of arraies. */
    void setVarHardBound(int numModHardVarLB, 
                         const int *varLBi,
                         const double *varLBv,
                         int numModHardVarUB,
                         const int *varUBi,
                         const double *varUBv);
    
    /** Set constraint hard bounds. Don't take ownerships of arraies. */  
    void setConHardBound(int numModHardConLB, 
                         const int *conLBi,
                         const double *conLBv,
                         int numModHardConUB,
                         const int *conUBi,
                         const double *conUBv);

    /** Recode the added constraints. Take over the memory ownship of 
	aguments. Append to previous constraints. */
    void appendAddedConstraints(int numAdd, BcpsObject **addCons){
	int numAll = numAdd + cons_->numAdd;
	BcpsObject ** allAdd = new BcpsObject* [numAll];
	int k;
	
	for (k = 0; k < cons_->numAdd; ++k) {
	    allAdd[k] = cons_->objects[k];
	}
	for (k = 0; k < numAdd; ++k) {
	    allAdd[(k+cons_->numAdd)] = addCons[k];
	}

	delete [] cons_->objects;
	cons_->numAdd = numAll;
        cons_->objects = allAdd;
    }

    /** Recode the added constraints. Take over the memory ownship of 
	aguments. Delete already added constraints. */
    void setAddedConstraints(int numAdd, BcpsObject **addCons){
	// NOTE: make sure objects can be safely deleted.
	if (cons_->objects) {
	    for (int k = 0; k < cons_->numAdd; ++k) {
		delete cons_->objects[k];
	    }
	    delete [] cons_->objects;
	}
        cons_->numAdd = numAdd;
        cons_->objects = addCons;
    }

    /** Record the constraints are deleted. Take over the memory 
	ownship of arguments. */
    void delConstraints(int numDel, int *indices){
	if ( cons_->posRemove) delete []  cons_->posRemove;
        cons_->numRemove = numDel;
        cons_->posRemove = indices;
    }

    /** Record added variables. Take over the memory ownship of arguments. */
    void addVariables(int numAdd, BcpsObject **addVars){
	if (vars_->objects) {
	    for (int k = 0; k < vars_->numAdd; ++k) {
		delete vars_->objects[k];
	    }
	    delete [] vars_->objects;
	}
	
        vars_->numAdd = numAdd;
        vars_->objects = addVars;
    }
    
    /** Record deleted variables. Take over the memory ownship of arguemnts. */
    void delVariables(int numDel, int *indices){
	if (vars_->posRemove) delete []  vars_->posRemove;
        vars_->numRemove = numDel;
        vars_->posRemove = indices;
    }

 protected:

    /** Pack a double field into an encoded object. */
    AlpsReturnStatus encodeDblFieldMods(AlpsEncoded *encoded,
				      BcpsFieldListMod<double> * field) const;

    /** Pack a integer field into an encoded object. */
    AlpsReturnStatus encodeIntFieldMods(AlpsEncoded *encoded,
				      BcpsFieldListMod<int> * field) const;
    
    /** Pack object modifications to an encoded object. */
    AlpsReturnStatus encodeObjectMods(AlpsEncoded *encoded,
				    BcpsObjectListMod *objMod) const;

    /** Unpack a double field from an encoded object. */
    AlpsReturnStatus decodeDblFieldMods(AlpsEncoded &encoded,
				      BcpsFieldListMod<double>*field);

    /** Unpack a integer field from an encoded object. */
    AlpsReturnStatus decodeIntFieldMods(AlpsEncoded &encoded,
				      BcpsFieldListMod<int> *field);

    /** Unpack object modifications to an encoded object. */
    AlpsReturnStatus decodeObjectMods(AlpsEncoded &encoded,
				    BcpsObjectListMod *objMod);
    
 public:

    /** Pack bcps node description into an encoded. */
    AlpsReturnStatus encodeBcps(AlpsEncoded *encoded) const;

    /** Unpack bcps node description into an encoded. */
    AlpsReturnStatus decodeBcps(AlpsEncoded &encoded);
    
};

#endif /* End of file */
