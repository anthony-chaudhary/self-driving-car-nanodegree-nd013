/* $Id: OSModificationDemo.cpp 2831 2009-07-21 07:34:44Z kmartin $ */
/** @file OSModificationDemo.cpp
 * 
 * @author  Horand Gassmann, Jun Ma, Kipp Martin, 
 *
 * \remarks
 * Copyright (C) 2008-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Dalhousie University, Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 */ 


#include "OSConfig.h"
#include "OSCoinSolver.h"
#include "OSIpoptSolver.h"
#include "OSResult.h" 
#include "OSiLReader.h"        
#include "OSiLWriter.h"   
#include "OSrLReader.h"          
#include "OSrLWriter.h"      
#include "OSInstance.h"  
#include "OSoLWriter.h"
#include "OSFileUtil.h"   
#include "OSDefaultSolver.h"  
//#include "OSWSUtil.h" 
//#include "OSSolverAgent.h"   
#include "OShL.h"     
#include "OSErrorClass.h"
#include "OSmps2osil.h"   
#include "OSBase64.h"
#include "OSErrorClass.h"
#include "OSMathUtil.h"

#include "OsiClpSolverInterface.hpp"
#include "OsiCbcSolverInterface.hpp"
#include "CbcModel.hpp"

#include<iostream> 
using std::cout;   
using std::endl;

//int main(int argC, char* argV[]){
int main( ){
// test OS code samples here

	FileUtil *fileUtil = NULL; 
	fileUtil = new FileUtil();
	cout << "Start Building the Model" << endl;
	try{
		OSInstance *osinstance = new OSInstance();

		// put in some of the OSInstance <instanceHeader> information
		osinstance->setInstanceSource("From Anderson, Sweeney, Williams, and Martin");
		osinstance->setInstanceDescription("The Par Inc. Problem");
		//
		// now put in the OSInstance <instanceData> information
		// 
		// first the variables
		osinstance->setVariableNumber( 2);   
		//addVariable(int index, string name, double lowerBound, double upperBound, char type, double init, string initString);
		// we could use setVariables() and add all the variable with one method call -- below is easier
		osinstance->addVariable(0, "x0", 0, OSDBL_MAX, 'C');
		osinstance->addVariable(1, "x1", 0, OSDBL_MAX, 'C');
		//
		// now add the objective function
		osinstance->setObjectiveNumber( 1);
		// now the coefficient
		SparseVector *objcoeff = new SparseVector(2);   
		objcoeff->indexes[ 0] = 0;
		objcoeff->values[ 0] = 10;
		objcoeff->indexes[ 1] = 1;
		objcoeff->values[ 1] = 9;
		//bool addObjective(int index, string name, string maxOrMin, double constant, double weight, SparseVector* objectiveCoefficients);
		osinstance->addObjective(-1, "objfunction", "max", 0.0, 1.0, objcoeff);
		objcoeff->bDeleteArrays = true;
		delete objcoeff;
		//
		// now the constraints
		osinstance->setConstraintNumber( 4); 
		//bool addConstraint(int index, string name, double lowerBound, double upperBound, double constant);
		// note: we could use setConstraints() and add all the constraints with one method call -- below is easier
		osinstance->addConstraint(0, "row0", -OSDBL_MAX, 630, 0); 
		osinstance->addConstraint(1, "row1", -OSDBL_MAX, 600, 0);
		osinstance->addConstraint(2, "row2", -OSDBL_MAX, 708, 0);
		osinstance->addConstraint(3, "row3", -OSDBL_MAX, 135, 0); 
		/*
		now add the <linearConstraintCoefficients>
		first do column major
		bool setLinearConstraintCoefficients(int numberOfValues, bool isColumnMajor, 
		double* values, int valuesBegin, int valuesEnd, 
		int* indexes, int indexesBegin, int indexesEnd,   			
		int* starts, int startsBegin, int startsEnd);	
		double *values = new double[ 8];
		int *indexes = new int[ 8];
		int *starts = new int[ 3];  
		values[ 0] = .7;
		values[ 1] = .5;
		values[ 2] = 1.0;
		values[ 3] = .1;
		values[ 4] = 1.0;
		values[ 5] = 5./6.;
		values[ 6] = 2./3.;
		values[ 7] = .25;
		indexes[ 0] = 0;
		indexes[ 1] = 1;
		indexes[ 2] = 2;
		indexes[ 3] = 3;
		indexes[ 4] = 0;
		indexes[ 5] = 1;
		indexes[ 6] = 2;  
		indexes[ 7] = 3;
		starts[ 0] = 0;
		starts[ 1] = 4;
		starts[ 2] = 8; 
		cout << "Call setLinearConstraintCoefficients" << endl;
		osinstance->setLinearConstraintCoefficients(8, true, values, 0, 7, 
			indexes, 0, 7, starts, 0, 2);	
		
		now row major
		*/
		double *values = new double[ 8];
		int *indexes = new int[ 8];
		int *starts = new int[ 5];  
		values[ 0] = .7;
		values[ 1] = 1;
		values[ 2] = .5;
		values[ 3] = 5./6.;
		values[ 4] = 1.0;
		values[ 5] = 2./3.;
		values[ 6] = .1;
		values[ 7] = .25;
		indexes[ 0] = 0;
		indexes[ 1] = 1;
		indexes[ 2] = 0;
		indexes[ 3] = 1;
		indexes[ 4] = 0;
		indexes[ 5] = 1;
		indexes[ 6] = 0;  
		indexes[ 7] = 1;
		starts[ 0] = 0;
		starts[ 1] = 2;
		starts[ 2] = 4;
		starts[ 3] = 6;
		starts[ 4] = 8;
		cout << "Call setLinearConstraintCoefficients" << endl;
		osinstance->setLinearConstraintCoefficients(8, false, values, 0, 7, 
			indexes, 0, 7, starts, 0, 4);
		cout << "End Building the Model" << endl; 
		// Write out the model
		OSiLWriter *osilwriter; 
		osilwriter = new OSiLWriter();
		cout << osilwriter->writeOSiL( osinstance);
		// done writing the model
		cout << "Done writing the Model" << endl;
		// now solve the model
		CoinSolver *solver = new CoinSolver();
		solver->osinstance = osinstance;
		solver->sSolverName ="clp"; 
		solver->buildSolverInstance();		
		solver->solve();
		std::cout << solver->osrl << std::endl;
		// write the answer to a file
		//fileUtil->writeFileFromString("../result.xml", solver->osrl);
		// work with the OSResult object
		//OSResult *result = NULL;
		//result = solver->osresult;
		//std::cout << result->resultData->optimization->solution[0]->objectives->values->obj[0]->value << std::endl;
		// do garbage collection
		
		// illustrate changing an objective function coefficient
		delete solver;
		osinstance->bObjectivesModified = true;
		osinstance->instanceData->objectives->obj[0]->coef[0]->value = 5;
		solver = new CoinSolver();
		solver->osinstance = osinstance;
		solver->sSolverName ="clp"; 
		solver->buildSolverInstance();
		solver->solve();
		std::cout << solver->osrl << std::endl;
		delete solver;
		// change objective function coefficient a second time
		osinstance->bObjectivesModified = true;
		osinstance->instanceData->objectives->obj[0]->coef[0]->value = 0;	
		// make the upper bound of the second variable 500
		// first print out the current value -- should be "infinity"
		std::cout << "Variable upper bound = "<< osinstance->instanceData->variables->var[1]->ub << std::endl;
		// now change it to 500;
		osinstance->bVariablesModified = true;
		osinstance->instanceData->variables->var[1]->ub = 500;
		std::cout << "Variable upper bound = "<< osinstance->instanceData->variables->var[1]->ub << std::endl;
		solver = new CoinSolver();
		solver->osinstance = osinstance;
		solver->sSolverName ="clp"; 	
		solver->buildSolverInstance();
		solver->solve();
		std::cout << solver->osrl << std::endl;		
		std::cout << "Obj value =  " << solver->osresult->optimization->solution[0]->objectives->values->obj[0]->value << endl; 
		std::cout << "Obj value =  " << solver->osresult->getOptimalObjValue( -1, 0) << endl;  // use a get		
		delete osinstance;
		osinstance = NULL;
		delete osilwriter;
		osilwriter = NULL;
		delete solver;
		solver = NULL;
		delete fileUtil;
		fileUtil = NULL;
		cout << "Done with garbage collection" << endl;
		cout << "Program terminates normally" << endl;
		return 0;
		//
	}
	catch(const ErrorClass& eclass){
		delete fileUtil;
		std::cout << eclass.errormsg <<  std::endl;
		return 0;
	} 
}// end main

