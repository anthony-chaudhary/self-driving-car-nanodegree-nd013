/* $Id: OSResultDemo.cpp 2710 2009-06-10 21:13:43Z kmartin $ */
/** @file OSResultDemo.cpp
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
#include "OSOption.h"
#include "OSoLWriter.h"
#include "OSFileUtil.h"   
#include "OSDefaultSolver.h"  
#include "OShL.h"     
#include "OSErrorClass.h"
#include "OSmps2osil.h"   
#include "OSBase64.h"
#include "OSErrorClass.h"
#include "OSMathUtil.h"


#include "CoinError.hpp"
#include "CoinHelperFunctions.hpp"
#include<iostream> 


using std::string;
using std::cout;   
using std::endl;

//int main(int argC, char* argV[]){
int main( ){
	WindowsErrorPopupBlocker();
	FileUtil *fileUtil = NULL; 
	fileUtil = new FileUtil();
	cout << "Start Building the Model" << endl;
	try{


		const char dirsep =  CoinFindDirSeparator();
		// Set directory containing mps data files.
		std::string dataDir;
		std::string osilFileName;
		dataDir = dirsep == '/' ? "../data/" : "..\\data\\";
		
		osilFileName =  dataDir + "osilFiles" + dirsep +  "parincLinear.osil";
		std::cout << "Try to read a sample file" << std::endl;
		std::cout << "The file is: " ;
		std::cout <<  osilFileName << std::endl;
		std::string osil = fileUtil->getFileAsString( osilFileName.c_str() );
		//now create some options
		OSiLReader *osilreader = NULL;
		osilreader = new OSiLReader(); 
		OSInstance *osinstance;
		osinstance = osilreader->readOSiL( osil);
		CoinSolver *solver = new CoinSolver();
		solver->osinstance = osinstance;
		solver->sSolverName ="clp"; 
		solver->buildSolverInstance();		
		solver->solve();
		
		// we demonstrate the OSResult API
		// first read the result in OSResult object
		// creat an OSResult object from the string			
		OSrLReader *osrlreader = NULL;
		OSResult *osresult = NULL;
		osrlreader = new OSrLReader();
		osresult  = osrlreader->readOSrL( solver->osrl);
		
		//now use the OSResult API -- first make sure we got an optimal solution
		//get the status
		std::string solStatus;
		double optSolValue;
		// the argument is the solution index
		solStatus = osresult->getSolutionStatusType( 0 );
		// if solStatus is optimal get the optimal solution value
		if( solStatus.find("ptimal") != string::npos ){
		//first index is objIdx, second is solution index
			optSolValue = osresult->getOptimalObjValue( -1, 0);
			std::cout << "OPTIMAL SOLUTION VALUE  " <<  optSolValue << std::endl;
		}else{
			std::cout << "NO OPTIMAL SOLUTION FOUND " << std::endl;
		}
		
		int i;
		int j;
		int k;
		int vecSize;
		// now get the primal solution
		std::vector<IndexValuePair*> primalValPair;
		primalValPair = osresult->getOptimalPrimalVariableValues( 0);
		vecSize = primalValPair.size();
		for(i = 0; i < vecSize; i++){
			std::cout << "index = " <<  primalValPair[ i]->idx << std::endl;
			std::cout << "value = " <<  primalValPair[ i]->value << std::endl;
		}
		// now get the dual solution
		std::vector<IndexValuePair*> dualValPair;
		dualValPair = osresult->getOptimalDualVariableValues( 0);
		vecSize = dualValPair.size();
		for(i = 0; i < vecSize; i++){
			std::cout << "index = " <<  dualValPair[ i]->idx << std::endl;
			std::cout << "value = " <<  dualValPair[ i]->value << std::endl;
		}
		//the OSResult API is currently somewhat limited, but you can get at the
		//OSResult object directly -- let's get all the otherVar stuff
		int numSolutions;
		int numberOfOtherVariableResults;
		int numberOfOtherVar;
		numSolutions =osresult-> getSolutionNumber();
		for(i = 0; i < numSolutions; i++){
			numberOfOtherVariableResults = osresult->
			getNumberOfOtherVariableResults( i);
			for(j = 0; j < numberOfOtherVariableResults; j++){
				std::cout << "Other Name = " << osresult->optimization->solution[ i]->variables->other[ j]->name << std::endl;
				std::cout << "Other Description = " << osresult->optimization->solution[ i]->variables->other[ j]->description << std::endl;
				std::cout << "Other NumberOfVar = " << osresult->optimization->solution[ i]->variables->other[ j]->numberOfVar <<  std::endl;
				numberOfOtherVar = osresult->optimization->solution[ i]->variables->other[ j]->numberOfVar;
				for(k = 0; k < numberOfOtherVar; k++){
					std::cout << "Other Variable index = "   <<  osresult->optimization->solution[ i]->variables->other[ j]->var[ k]->idx << std::endl;
					std::cout << "Other Variable value = "   <<  osresult->optimization->solution[ i]->variables->other[ j]->var[ k]->value << std::endl;
				}
			}
		}		
		//garbage collection
		delete solver;
		solver = NULL;
		delete fileUtil;
		fileUtil = NULL;
		delete osrlreader;
		osrlreader = NULL;
		delete osilreader;
		osilreader = NULL;
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
