/* $Id: OSBlockSlpSolver.cpp 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSBlockSlpSolver.cpp
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
#include "OSDipBlockSlpSolver.h"
#include "OSErrorClass.h" 
#include "OSDataStructures.h"



OSDipBlockSlpSolver::OSDipBlockSlpSolver():
	m_osinstance(NULL) {
	std::cout << "INSIDE OSDipBlockCoinSolver CONSTRUCTOR" << std::endl;
}//end OSDipBlockSlpSolver


OSDipBlockSlpSolver::OSDipBlockSlpSolver( OSInstance *osinstance) {
	
	try{
		m_osinstance = osinstance;
		m_numberOfVar = m_osinstance->getVariableNumber();
	
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	

}//end OSDipBlockSlpSolver

OSDipBlockSlpSolver::~OSDipBlockSlpSolver(){
	
	std::cout << "INSIDED ~OSDipBlockSlpSolver()" << std::endl;


	std::vector<IndexValuePair*>::iterator  vit;
	
	for (vit = m_primalVals.begin(); vit != m_primalVals.end(); vit++) {
		
		delete *vit;
	}
	//if(m_osrlreader != NULL) delete m_osrlreader;
}//end ~OSDipBlockSlpSolver

void OSDipBlockSlpSolver::solve(double *cost, std::vector<IndexValuePair*> *solIndexValPair, double *optVal){

	try{
		struct IndexValuePair *primalValPair;

		std::vector<IndexValuePair*>::iterator  vit;
		
		for (vit = m_primalVals.begin(); vit != m_primalVals.end(); vit++) {
			
			delete *vit;
		}
		m_primalVals.clear();
		//sum up negative coefficient
		// -- critical -- we assume last variable is the y variable
		// this is just for illustration
		*optVal = 0;
		for(int i = 0 ; i < m_numberOfVar - 1; i++){
			
			m_osinstance->instanceData->objectives->obj[0]->coef[i]->value 
					= cost[ i];
			
			
			primalValPair = new IndexValuePair();
			
			primalValPair->value = 0.0;
			primalValPair->idx = i;
			
			if (cost[ i] < 0){
				
				*optVal = *optVal + cost[ i];
				 primalValPair->value = 1.0;
			}
			
			m_primalVals.push_back( primalValPair);
			
			
		}//end for
		
		//now set the y variable to 1 if optVal is negative
		//otherwise 0
		primalValPair = new IndexValuePair();
		primalValPair->idx = m_numberOfVar - 1;
		
		if(*optVal < 0){
			
			primalValPair->value = 1.0;
			*optVal = *optVal + cost[ m_numberOfVar - 1];
			
		}else{
			
			primalValPair->value = 0.0;	
		}
		
		m_primalVals.push_back( primalValPair);
		
		
		m_osinstance->bObjectivesModified = true;
		std::cout << m_osinstance->printModel( ) << std::endl;
		//
		
		std::cout << "NUMBER OF VARIABLES m_primalVals = " <<  m_primalVals.size() << std::endl;
		
		for (vit = m_primalVals.begin(); vit != m_primalVals.end(); vit++) {
			
			//solIndexValPair.push_back( *vit);
			
		}
		
		 *solIndexValPair = m_primalVals;
		// the argument is the solution index
		
		
		//solStatus = m_osresult->getSolutionStatusType( 0 );
		
		//std::cout << "SOLUTION STATUS " << solStatus << std::endl;
		// if solStatus is optimal get the optimal solution value
		//if( solStatus.find("ptimal") != string::npos ){
		//first index is objIdx, second is solution index
		//	*optVal = m_osresult->getOptimalObjValue( -1, 0);
		//	*solIndexValPair = m_osresult->getOptimalPrimalVariableValues( 0);			
		//}else{
		//	throw ErrorClass("problem -- did not optimize a subproblem");
		//}	
			
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	
	
}//end solve


void OSDipBlockSlpSolver::solve(double *cost, std::string *osrl){

	
	try{
		//not implemented
			
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	
	
}//end solve

