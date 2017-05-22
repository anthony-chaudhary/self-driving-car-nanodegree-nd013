/* $Id: OSBlockSolver.cpp 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSBlockSolver.cpp
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
#include "OSDipBlockCoinSolver.h"
#include "OSErrorClass.h"



OSDipBlockCoinSolver::OSDipBlockCoinSolver():
	m_osinstance(NULL) {
	std::cout << "INSIDE OSDipBlockCoinSolver CONSTRUCTOR" << std::endl;
}//end OSDipBlockCoinSolver


OSDipBlockCoinSolver::OSDipBlockCoinSolver( OSInstance *osinstance) {
	
	try{
		m_osinstance = osinstance;
		//initialize the solver
		m_solver = NULL;
		m_solver = new CoinSolver();
		//lets just start with Cbc
		m_solver->sSolverName ="cbc";		
		m_solver->osinstance = m_osinstance;
		m_solver->buildSolverInstance();
		//m_osrlreader = NULL;
		//m_osrlreader = new OSrLReader();
		m_numberOfVar = m_osinstance->getVariableNumber();
	
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	

}//end OSDipBlockCoinSolver

OSDipBlockCoinSolver::~OSDipBlockCoinSolver(){
	
	if(m_solver != NULL) delete m_solver;
	std::cout << "INSIDED ~OSDipBlockCoinSolver()" << std::endl;
	//if(m_osrlreader != NULL) delete m_osrlreader;
}//end ~OSDipBlockCoinSolver

void OSDipBlockCoinSolver::solve(double *cost, std::vector<IndexValuePair*> *solIndexValPair, double *optVal){

	try{
		//set the objective function
		//here we are using the Osi Interface
		//we have already built the model
		m_solver->osiSolver->setObjective( cost);
		
		//an echo check -- kipp put this inside a statement using a print log
		for(int i = 0 ; i < m_numberOfVar; i++){
			
			m_osinstance->instanceData->objectives->obj[0]->coef[i]->value 
					= cost[ i];
			
		}
		m_osinstance->bObjectivesModified = true;
		std::cout << m_osinstance->printModel( ) << std::endl;
		//
		
	
		//solve the model
		m_solver->solve();
		m_osresult = m_solver->osresult;
		std::string solStatus;
		// the argument is the solution index
		
		
		solStatus = m_osresult->getSolutionStatusType( 0 );
		
		std::cout << "SOLUTION STATUS " << solStatus << std::endl;
		// if solStatus is optimal get the optimal solution value
		if( solStatus.find("ptimal") != std::string::npos ){
		//first index is objIdx, second is solution index
			*optVal = m_osresult->getOptimalObjValue( -1, 0);
			*solIndexValPair = m_osresult->getOptimalPrimalVariableValues( 0);			
		}else{
			throw ErrorClass("problem -- did not optimize a subproblem");
		}	
			
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	
	
}//end solve


void OSDipBlockCoinSolver::solve(double *cost, std::string *osrl){

	
	try{
		//set the objective function
		//here we are using the Osi Interface
		//we have already built the model
		m_solver->osiSolver->setObjective( cost);
		
		//std::cout << m_osinstance->printModel( ) << std::endl;
		//solve the model
		m_solver->solve();
		//std::cout << "MODEL BEING SOLVED " << std::endl;
		//get the solution
		//m_osresult  = m_osrlreader->readOSrL( m_solver->osrl );
		m_osresult = m_solver->osresult;
		std::string solStatus;
		// the argument is the solution index
		
		
		solStatus = m_osresult->getSolutionStatusType( 0 );
		
		//std::cout << "SOLUTION STATUS " << solStatus << std::endl;
		// if solStatus is optimal get the optimal solution value
		if( solStatus.find("ptimal") != std::string::npos ){
		//first index is objIdx, second is solution index
			*osrl = m_solver->osrl;	
		}else{
			throw ErrorClass("problem -- did not optimize a subproblem");
		}	
			
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	
	
}//end solve


