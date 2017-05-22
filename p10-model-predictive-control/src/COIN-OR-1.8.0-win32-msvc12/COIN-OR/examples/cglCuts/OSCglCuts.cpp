/* $Id: OSAddCuts.cpp 2710 2009-06-10 21:13:43Z kmartin $ */
/** @file OSAddCuts.cpp
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


#include <cppad/cppad.hpp> 
#include "OSConfig.h"
#include "OSCoinSolver.h"
#include "OSResult.h" 
#include "OSiLReader.h"        
#include "OSiLWriter.h"   
#include "OSrLReader.h"          
#include "OSrLWriter.h"      
#include "OSInstance.h"  
#include "OSFileUtil.h"   
#include "OSDefaultSolver.h"  
#include "OSWSUtil.h" 
#include "OSSolverAgent.h"   
#include "OShL.h"     
#include "OSErrorClass.h"
#include "OSmps2osil.h"   
#include "OSBase64.h"
#include "OSErrorClass.h"
#include "CglGomory.hpp"
#include "CglSimpleRounding.hpp"
#include "CglKnapsackCover.hpp"
#include "OSMathUtil.h"
#include "CbcModel.hpp"

#include <iostream>
using std::cout;   
using std::endl;

int main( ){
	WindowsErrorPopupBlocker();
// test OS code samples here
	FileUtil *fileUtil = NULL; 
	fileUtil = new FileUtil();
	const char dirsep =  CoinFindDirSeparator();
 	// Set directory containing mps data files.
 	std::string dataDir;
   dataDir = dirsep == '/' ? "../data/" : "..\\data\\";
	cout << "Start Building the Model" << endl;
	try{
		// get the p0033 problem
		std::string osilFileName;
		osilFileName =  dataDir  +  "osilFiles" + dirsep +  "p0033.osil";
		std::cout << "Try to read a sample file" << std::endl;
		std::cout << "The file is: " ;
		std::cout <<  osilFileName << std::endl;
		std::string osil = fileUtil->getFileAsString( osilFileName.c_str() );
		OSiLReader *osilreader = NULL;
		osilreader = new OSiLReader(); 
		OSInstance *osinstance;
		osinstance = osilreader->readOSiL( osil);
		// done writing the model
		cout << "Done writing the Model" << endl;
		// now solve the model
		CoinSolver *solver  = NULL;
		solver = new CoinSolver();  
		solver->sSolverName ="cbc"; 
		solver->osinstance = osinstance;
		solver->buildSolverInstance();
		solver->osiSolver->setHintParam(OsiDoReducePrint, true, OsiHintTry);
		solver->osiSolver->initialSolve();
		cout << "Here is the initial objective value "  << solver->osiSolver->getObjValue()  << endl;
		
		CglKnapsackCover cover;
	    CglSimpleRounding round;  
		CglGomory gomory;
		CbcModel *model = new CbcModel( *solver->osiSolver);

		//model->setBestObjectiveValue(4000);
		model->setMaximumNodes(100000);
		//
		model->addCutGenerator(&gomory, 1, "Gomory");
		model->addCutGenerator(&cover, 1, "Cover");
		model->addCutGenerator(&round, 1, "Round");
		model->branchAndBound();	
		// now create a result object
		OSResult *osresult = new OSResult();
		// if we are throw an exception if the problem is nonlinear
		double *x = NULL;
		double *y = NULL;
		double *z = NULL;
		//int i = 0;
		std::string *rcost = NULL;
		// resultHeader infomration
		if(osresult->setServiceName("Solved with Coin Solver: " + solver->sSolverName) != true)
			throw ErrorClass("OSResult error: setServiceName");
		if(osresult->setInstanceName(  solver->osinstance->getInstanceName()) != true)
			throw ErrorClass("OSResult error: setInstanceName");
		if(osresult->setVariableNumber( solver->osinstance->getVariableNumber()) != true)
			throw ErrorClass("OSResult error: setVariableNumer");
		if(osresult->setObjectiveNumber( 1) != true)
			throw ErrorClass("OSResult error: setObjectiveNumber");
		if(osresult->setConstraintNumber( solver->osinstance->getConstraintNumber()) != true)
			throw ErrorClass("OSResult error: setConstraintNumber");
		if(osresult->setSolutionNumber(  1) != true)
			throw ErrorClass("OSResult error: setSolutionNumer");	
		int solIdx = 0;
		std::string description = "";
		osresult->setGeneralStatusType("success");
		std::cout << "PROVEN OPTIMAL " << model->isProvenOptimal() << std::endl;
		int i;
		if (model->isProvenOptimal() == 1){
			osresult->setSolutionStatus(solIdx, "optimal", description);
			/* Retrieve the solution */
			x = new double[solver->osinstance->getVariableNumber() ];
			y = new double[solver->osinstance->getConstraintNumber() ];
			z = new double[1];
			rcost = new std::string[ solver->osinstance->getVariableNumber()];
			//
			*(z + 0)  =  model->getObjValue();
			osresult->setObjectiveValuesDense(solIdx, z);
			for(i=0; i < solver->osinstance->getVariableNumber(); i++){
				*(x + i) = model->getColSolution()[i];
			}
			osresult->setPrimalVariableValuesDense(solIdx, x );
			//if( solver->sSolverName.find( "symphony") == std::string::npos){
			for(i=0; i <  solver->osinstance->getConstraintNumber(); i++){
				*(y + i) = model->getRowPrice()[ i];
			}
			osresult->setDualVariableValuesDense(solIdx, y);
			//
			// now put the reduced costs into the osrl
			int numberOfOtherVariableResult = 1;
			int otherIdx = 0;
			// first set the number of Other Variable Results
			osresult->setNumberOfOtherVariableResults(solIdx, numberOfOtherVariableResult);
			
			std::ostringstream outStr;
			int numberOfVar =  solver->osinstance->getVariableNumber();
			for(i=0; i < numberOfVar; i++){
				outStr << model->getReducedCost()[ i]; 
				rcost[ i] = outStr.str();
				outStr.str("");
			}
			osresult->setAnOtherVariableResultDense(solIdx, otherIdx, "reduced costs", "", "the variable reduced costs",  
				rcost);			
			// end of settiing reduced costs			
		}
		else{ 
			if(solver->osiSolver->isProvenPrimalInfeasible() == true) 
				osresult->setSolutionStatus(solIdx, "infeasible", description);
			else
				if(solver->osiSolver->isProvenDualInfeasible() == true) 
					osresult->setSolutionStatus(solIdx, "dualinfeasible", description);
				else
					osresult->setSolutionStatus(solIdx, "other", description);
		}
		OSrLWriter *osrlwriter = new OSrLWriter();
		std::cout <<  osrlwriter->writeOSrL( osresult) << std::endl;
		if(solver->osinstance->getVariableNumber() > 0){
			delete[] x;
			x = NULL;
		}
		if(solver->osinstance->getConstraintNumber()) delete[] y;
		y = NULL;
		delete[] z;	
		z = NULL;
		if(solver->osinstance->getVariableNumber() > 0){
			delete[] rcost;
			rcost = NULL;
		}
    	// do garbage collection
		delete osresult;
		osresult = NULL;
		delete osrlwriter;
		osrlwriter = NULL;
		delete solver;
		solver = NULL;
		delete osilreader;
		osilreader = NULL;
		delete fileUtil;
		fileUtil  = NULL;
		delete model;
		model = NULL;
		cout << "Done with garbage collection" << endl;
		cout << "Program terminates normally" << endl;
		return 0;
	}
	catch(const ErrorClass& eclass){
		delete fileUtil;
		std::cout << eclass.errormsg <<  std::endl;
		return 0;
	} 
}// end main

