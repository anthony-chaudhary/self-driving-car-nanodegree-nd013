/* $Id: OSBlockSlpSolver.h 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSBlockSlpSolver.h
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

#ifndef OSDIPBLOCKSLPSOLVER_H
#define OSDIPBLOCKSLPSOLVER_H

// --------------------------------------------------------------------- //
#include "OSDipBlockSolver.h"
#include "OSDataStructures.h"
#include "OSDefaultSolver.h" 
#include "OSrLReader.h"

#include "OSDipBlockSolverFactory.h"

// --------------------------------------------------------------------- //
/*!
 * \class 
 * OSDipBlockCoinSolver solves the block subproblem 
 *	using a COIN-OR solver
 * 
 *
 */
// --------------------------------------------------------------------- //
class OSDipBlockSlpSolver : public OSDipBlockSolver {
public:
	

	OSInstance *m_osinstance;
	OSResult *m_osresult;
	int m_numberOfVar;
	
	std::vector<IndexValuePair*>  m_primalVals;

	
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
	virtual void solve(double *cost, std::vector<IndexValuePair*> *solIndexValPair, double *optVal) ;
	
	
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
	virtual void solve(double *cost, std::string *osrl) ;
		
	/**
	 *
	 * Default Constructor. 
	 */	
	OSDipBlockSlpSolver();
	
	
	/**
	 *
	 * Create the solver with the instance. 
	 */	
	OSDipBlockSlpSolver( OSInstance *osinstance);
	
	/**
	 *
	 * Default destructor. 
	 */	
	~OSDipBlockSlpSolver();	
	
	class Factory;
	class Factory : public OSDipBlockSolverFactory{
		
	public:

		Factory(){
			
		}
		
		~Factory(){
			
		}
		
		OSDipBlockSolver* create() {  return new OSDipBlockSlpSolver(  osinstance); };
		
	};// end class OSDipBlockSolverFactory
	
	
	//
};//end class OSDipBlockSlpSolver




#endif
