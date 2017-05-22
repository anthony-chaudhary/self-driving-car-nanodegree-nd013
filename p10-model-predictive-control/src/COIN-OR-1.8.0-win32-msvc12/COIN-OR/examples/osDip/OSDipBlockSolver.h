/* $Id: OSDipBlockSolver.h 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSDipBlockSolver.h
 * 
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin, 
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Dalhousie University,  Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 */

#ifndef OSDIPBLOCKSOLVER_H
#define OSDIPBLOCKSOLVER_H

// --------------------------------------------------------------------- //
#include "OSInstance.h"
#include "OSResult.h"
#include "OSDataStructures.h"
#include "OSErrorClass.h"
#include <vector>
#include <string>
#include <map>




// --------------------------------------------------------------------- //
/*!
 * \class 
 * OSBDiplockSolver is a virtual class for solving the 
 *    Block subproblem
 * 
 *
 */
// --------------------------------------------------------------------- //
class OSDipBlockSolver{
public:
	

	
	/** m_cost is the objective function
	 * of (reduced) coefficients
	 */
	double *m_cost;
	
	/**  m_sparseVec is the column found by solving the block
	 * optimization problem
	 */
	SparseVector *m_sparseVec;
	
	/** m_optVal is the optimal objective function
	 * value of the block optimization problem
	 */
	double m_optVal;
	
	
	/** m_whichBlock is the index of the subproblem
	 * we are working with
	 */
	int m_whichBlock;
		
	/**
	 * <p>
	 * Calculate the function value given the current variable values.
	 * This is an abstract method which is required to be implemented by the concrete
	 * operator nodes that derive or extend from this OSnLNode class.
	 * </p>
	 *
	 * @param cost -- an input value: pointer to the objective function (reduced) coefficients.
	 * @param sv -- a return value: sparse vector with the optimal column
	 * @param optVal -- a return value: the optimal solution value of the subproblem
	 */
	virtual void solve(double *cost, std::vector<IndexValuePair*> *solIndexValPair, double *optVal) = 0;
	
	
	/**
	 * <p>
	 * Calculate the function value given the current variable values.
	 * This is an abstract method which is required to be implemented by the concrete
	 * operator nodes that derive or extend from this OSnLNode class.
	 * </p>
	 *
	 * @param cost -- an input value: pointer to the objective function (reduced) coefficients.
	 * @param osrl -- a return value: an osrl string
	 */
	virtual void solve(double *cost, std::string *osrl) = 0 ;
	
	
	/**
	 *
	 * Default Constructor. 
	 */	
	OSDipBlockSolver();
	
	/**
	 *
	 * Default destructor. 
	 */	
	virtual ~OSDipBlockSolver() = 0;	
	
	
	//
};//end class OSDipBlockSolver



#endif
