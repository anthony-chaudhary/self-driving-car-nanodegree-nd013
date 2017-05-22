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

//#############################################################################
// This file is modified from from COIN/Cbc
//#############################################################################


#include "BcpsObject.h"
#include "BlisPseudo.h"

class BlisModel;

//#############################################################################

class BlisObjectInt : public BcpsObject {

protected:

    /** Column index in the lp model. */
    int columnIndex_;
    
    /** Original lower bound. */
    double originalLower_;
    
    /** Original upper bound. */
    double originalUpper_;
    
    /** Breakeven i.e. >= this preferred is up. */
    double breakEven_;
    
    /** Pseudo cost. */
    BlisPseudocost pseudocost_;
    
 public:

    /** Default Constructor. */
    BlisObjectInt();
    
    /** Useful constructor - passed integer index and model index. */
    BlisObjectInt(int objectIndex, 
                  int iColumn, 
                  double lb,
                  double ub,
                  double breakEven = 0.5);
    
    /** Destructor. */
    virtual ~BlisObjectInt() {}
  
    /** Copy constructor. */
    BlisObjectInt( const BlisObjectInt &);
    
    /** Clone an object. */
    virtual BcpsObject * clone() const {
        return new BlisObjectInt(*this);
    }
    
    /** Assignment operator. */
    BlisObjectInt & operator=( const BlisObjectInt& rhs);
    
    /** Infeasibility. Range is [0.0, 0.5]. 
        \param PreferredWay   the direction close to an integer. */
    virtual double infeasibility(BcpsModel *m, int & preferredWay) const;
    
    /** Set bounds to contain the current solution.
	More precisely, for the variable associated with this object, 
        take the value given in the current solution, force it within
        the current bounds if required, then set the bounds to fix 
        the variable at the integer nearest the solution value.
    */
    virtual void feasibleRegion(BcpsModel *m);

    /** Creates a branching object. */
    virtual BcpsBranchObject * createBranchObject(BcpsModel *m, 
                                                  int direction) const;
    
    /** \brief Given a valid solution (with reduced costs, etc.),
	return a branching object which would give a new feasible
	point in the good direction.

	The preferred branching object will force the variable to 
	be +/-1 from its current value, depending on the reduced cost 
	and objective sense.  If movement in the direction which 
	improves the objective is impossible due to bounds on the 
	variable, the branching object will move in the other
	direction.  If no movement is possible, the method returns NULL.
        
	Only the bounds on this variable are considered when 
	determining if the new point is feasible.
    */
    virtual BcpsBranchObject * preferredNewFeasible(BcpsModel *m) const;
  
    /** \brief Given a valid solution (with reduced costs, etc.),
	return a branching object which would give a new feasible
	point in a bad direction.
	
	As for preferredNewFeasible(), but the preferred branching object will
	force movement in a direction that degrades the objective.
    */
    virtual BcpsBranchObject * notPreferredNewFeasible(BcpsModel *m) const ;
  
    /** Reset original upper and lower bound values from the solver.
	Handy for updating bounds held in this object after bounds held in the
	solver have been tightened.
    */
    virtual void resetBounds(BcpsModel *m);
    
    /** Column number if single column object, otherwise. */
    virtual int columnIndex() const { return columnIndex_; }

    /** @name Get or set Original bounds. */
    //@{
    inline double originalLowerBound() const { return originalLower_; }
    inline void setOriginalLowerBound(double value) { originalLower_=value; }
    inline double originalUpperBound() const { return originalUpper_; }
    inline void setOriginalUpperBound(double value) { originalUpper_=value; }
    //@}

    /** Breakeven e.g 0.7 -> >= 0.7 go up first. */
    inline double breakEven() const { return breakEven_; }

    /** Set breakeven e.g 0.7 -> >= 0.7 go up first. */
    inline void setBreakEven(double value) { breakEven_ = value; }

    /** Access pseudocost. */
    BlisPseudocost & pseudocost() { return pseudocost_; }
};

