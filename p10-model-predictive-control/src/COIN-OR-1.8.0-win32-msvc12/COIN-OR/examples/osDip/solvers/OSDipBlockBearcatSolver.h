/* $Id: OSBlockBearcatSolver.h 3038 2009-11-07 11:43:44Z kmartin $ */
/** @file OSBlockBearcatSolver.h
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

#ifndef OSDIPBLOCKBEARCATSOLVER_H
#define OSDIPBLOCKBEARCATSOLVER_H

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
class OSDipBlockBearcatSolver : public OSDipBlockSolver {
public:
	

	OSInstance *m_osinstance;
	OSOption *m_osoption;
	OSResult *m_osresult;
	
	//variables for the dynamic programming solution
	int m_numberOfVar;
	int m_numHubs;
	int m_numNodes;
	int m_totalDemand;
	int m_minDemand;
	int* m_demand;
	
	double** m_u; 
	double** m_v;
	int** m_px;
	int** m_tx;
	double** m_g;
	
	int* m_varIdx;
	//end variables for the dynamic programming solution
	
	std::vector<IndexValuePair*>  m_primalVals;
	
	
	double qrouteCost(const int& k, const int& l, double* c, int* kountVar) ;

	
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
	OSDipBlockBearcatSolver();
	
	
	/**
	 *
	 * Create the solver with the instance. 
	 */	
	OSDipBlockBearcatSolver( OSInstance *osinstance, OSOption *osoption);
	
	/**
	 *
	 * Default destructor. 
	 */	
	~OSDipBlockBearcatSolver();	
	
	class Factory;
	class Factory : public OSDipBlockSolverFactory{
		
	public:

		Factory(){
			
		}
		
		~Factory(){
			
		}
		
		OSDipBlockSolver* create() {  return new OSDipBlockBearcatSolver(  osinstance, osoption); };
		
	};// end class OSDipBlockSolverFactory
	
	
	//
};//end class OSDipBlockBearcatSolver




#endif
