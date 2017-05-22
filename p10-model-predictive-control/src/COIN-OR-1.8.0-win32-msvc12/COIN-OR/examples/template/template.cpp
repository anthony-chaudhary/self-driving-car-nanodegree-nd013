/* $Id: template.cpp 2710 2009-06-10 21:13:43Z kmartin $ */
/** @file template.cpp
 * 
 * @author  Horand Gassmann, Jun Ma, Kipp Martin, 
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Dalhousie University, Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 */ 
 
//#include <cppad/cppad.hpp> 
#include "OSConfig.h"
#include "OSCoinSolver.h"
#include "OSIpoptSolver.h"
#include "OSResult.h" 
#include "OSiLReader.h"        
#include "OSiLWriter.h"   
#include "OSrLReader.h"          
#include "OSrLWriter.h"  
#include "OSoLWriter.h"  
#include "OSInstance.h"  
#include "OSFileUtil.h"   
#include "OSDefaultSolver.h"     
#include "OShL.h"     
#include "OSErrorClass.h"
#include "OSmps2osil.h"   
#include "OSBase64.h"
#include "OSErrorClass.h"
#include "OSMathUtil.h"
#include "CoinFinite.hpp"

#include "ClpSimplex.hpp"
#include "ClpSolve.hpp"
#include "CbcOrClpParam.hpp"
#include "ClpInterior.hpp"
#include "ClpCholeskyBase.hpp"
#include "ClpQuadraticObjective.hpp"

#include<iostream> 
#include <ostream>
#include <sstream>
#include <streambuf>

#include<map>
#include<stdio.h>
 
using std::cout;   
using std::endl;




int main(int argC, char* argV[]){
	WindowsErrorPopupBlocker();
	FileUtil *fileUtil = NULL; 
	fileUtil = new FileUtil();
	
	
	//demonstrate use of Clp
	ClpSolve solveOptions;
	ClpSolve::SolveType method;
	method = ClpSolve::useBarrier;
	//or primal simplex
	method = ClpSolve::usePrimal;
	solveOptions.setSolveType( method);

	ClpSimplex*  qpClpModel;
    //ClpInterior*  qpClpModel;
	qpClpModel = NULL;
	// template -- add your code here -- //
	std::cout << "Hello World" << std::endl;
	
	
	/////////
	
    CbcOrClpParam parameters[CBCMAXPARAMETERS];
    int numberParameters ;
    establishParams(numberParameters, parameters) ;
    std::cout << "NUMBER OF PARAMETERS = " << numberParameters<< std::endl;

    /*
    parameters[whichParam(CLP_PARAM_ACTION_BASISIN, numberParameters, parameters)].setStringValue(importBasisFile);
    parameters[whichParam(CLP_PARAM_ACTION_BASISOUT, numberParameters, parameters)].setStringValue(exportBasisFile);
    parameters[whichParam(CLP_PARAM_ACTION_PRINTMASK, numberParameters, parameters)].setStringValue(printMask);
    parameters[whichParam(CLP_PARAM_ACTION_DIRECTORY, numberParameters, parameters)].setStringValue(directory);
    parameters[whichParam(CLP_PARAM_ACTION_DIRSAMPLE, numberParameters, parameters)].setStringValue(dirSample);
    parameters[whichParam(CLP_PARAM_ACTION_DIRNETLIB, numberParameters, parameters)].setStringValue(dirNetlib);
    parameters[whichParam(CBC_PARAM_ACTION_DIRMIPLIB, numberParameters, parameters)].setStringValue(dirMiplib);
    parameters[whichParam(CLP_PARAM_DBL_DUALBOUND, numberParameters, parameters)].setDoubleValue(models->dualBound());
    parameters[whichParam(CLP_PARAM_DBL_DUALTOLERANCE, numberParameters, parameters)].setDoubleValue(models->dualTolerance());
    parameters[whichParam(CLP_PARAM_ACTION_EXPORT, numberParameters, parameters)].setStringValue(exportFile);
    parameters[whichParam(CLP_PARAM_INT_IDIOT, numberParameters, parameters)].setIntValue(doIdiot);
    parameters[whichParam(CLP_PARAM_ACTION_IMPORT, numberParameters, parameters)].setStringValue(importFile);
    parameters[whichParam(CLP_PARAM_INT_SOLVERLOGLEVEL, numberParameters, parameters)].setIntValue(models->logLevel());
    parameters[whichParam(CLP_PARAM_INT_MAXFACTOR, numberParameters, parameters)].setIntValue(models->factorizationFrequency());
    parameters[whichParam(CLP_PARAM_INT_MAXITERATION, numberParameters, parameters)].setIntValue(models->maximumIterations());
    parameters[whichParam(CLP_PARAM_INT_OUTPUTFORMAT, numberParameters, parameters)].setIntValue(outputFormat);
    parameters[whichParam(CLP_PARAM_INT_PRESOLVEPASS, numberParameters, parameters)].setIntValue(preSolve);
    parameters[whichParam(CLP_PARAM_INT_PERTVALUE, numberParameters, parameters)].setIntValue(models->perturbation());
    parameters[whichParam(CLP_PARAM_DBL_PRIMALTOLERANCE, numberParameters, parameters)].setDoubleValue(models->primalTolerance());
    parameters[whichParam(CLP_PARAM_DBL_PRIMALWEIGHT, numberParameters, parameters)].setDoubleValue(models->infeasibilityCost());
    parameters[whichParam(CLP_PARAM_ACTION_RESTORE, numberParameters, parameters)].setStringValue(restoreFile);
    parameters[whichParam(CLP_PARAM_ACTION_SAVE, numberParameters, parameters)].setStringValue(saveFile);
    parameters[whichParam(CLP_PARAM_DBL_TIMELIMIT, numberParameters, parameters)].setDoubleValue(models->maximumSeconds());
    parameters[whichParam(CLP_PARAM_ACTION_SOLUTION, numberParameters, parameters)].setStringValue(solutionFile);
    parameters[whichParam(CLP_PARAM_ACTION_SAVESOL, numberParameters, parameters)].setStringValue(solutionSaveFile);
    parameters[whichParam(CLP_PARAM_INT_SPRINT, numberParameters, parameters)].setIntValue(doSprint);
    parameters[whichParam(CLP_PARAM_INT_SUBSTITUTION, numberParameters, parameters)].setIntValue(substitution);
    parameters[whichParam(CLP_PARAM_INT_DUALIZE, numberParameters, parameters)].setIntValue(dualize);
    parameters[whichParam(CLP_PARAM_DBL_PRESOLVETOLERANCE, numberParameters, parameters)].setDoubleValue(1.0e-8);
 */

	///////
	

	qpClpModel = new ClpSimplex();
    //qpClpModel = new  ClpInterior();
	
	
	//set a parameter
	

	qpClpModel->setIntParam(ClpMaxNumIteration, 100) ;
	
	
	//parameters[whichParam(CLP_PARAM_INT_SOLVERLOGLEVEL, numberParameters, parameters)].setIntValue(qpClpModel->logLevel());
	
	std::cout << "Parameter number for:  CLP_PARAM_INT_SOLVERLOGLEVEL  " <<  whichParam(CLP_PARAM_INT_SOLVERLOGLEVEL, numberParameters, parameters) << std::endl;
	std::cout << "Parameter number for:  CLP_PARAM_INT_MAXITERATION  " <<  whichParam(CLP_PARAM_INT_MAXITERATION, numberParameters, parameters) << std::endl;
	parameters[ whichParam(CLP_PARAM_INT_SOLVERLOGLEVEL, numberParameters, parameters)].setIntParameter(qpClpModel, 0);
	parameters[ whichParam(CLP_PARAM_INT_MAXITERATION, numberParameters, parameters)].setIntParameter(qpClpModel, 10);
	
	std::cout << "CLP_PARAM_INT_SOLVERLOGLEVEL = "  <<  parameters[whichParam(CLP_PARAM_INT_SOLVERLOGLEVEL, numberParameters, parameters)].intValue() << std::endl;
	
	try{

        int i;
 

		const char dirsep =  CoinFindDirSeparator();
		std::string osil;
		/** Set directory containing mps data files.
		 *  Note that the file location is hardwired into the program.
		 *  If you move the executable or the data, 
		 *  make sure to update dataDir appropriately
		 */
		std::string dataDir;
		std::string osilFileName;
		dataDir = dirsep == '/' ? "../data/" : "..\\data\\";

		// first declare a generic solver
		DefaultSolver *solver  = NULL;
		
		OSiLReader *osilreader = NULL;
		OSInstance *osinstance = NULL;
		OSoLWriter *osolwriter = NULL;
		OSOption* osoption = NULL;
		

		
		/******************** Start Clp Example *************************/
		std::cout << std::endl << std::endl;
		std::cout << "CLP EXAMPLE" << std::endl;
		
		/******************** STEP 1 ************************
		* Get an instance in mps format, and create an OSInstance object
		*/
		std::string qpFileName;
		qpFileName =  dataDir  +  "parincQuadratic.osil";
		// convert to the OS native format
		osil = fileUtil->getFileAsString( qpFileName.c_str() );
		osilreader = new OSiLReader(); 
		osinstance = osilreader->readOSiL( osil);
		
		
		/******************** STEP 2 ************************
		* Create an OSOption object and give the solver options
		*/		
		osoption = new OSOption();
		/** 
		 *  here is the format for setting options:
		 *	bool setAnotherSolverOption(std::string name, std::string value, std::string solver, 
		 *	std::string category, std::string type, std::string description);
		 */
		 // normally most output is turned off, here we turn it back on
		osoption->setAnotherSolverOption("OsiHintTry","","osi","","OsiHintStrength","");
		osoption->setAnotherSolverOption("OsiDoReducePrint","false","osi","","OsiHintParam","");
		osolwriter = new OSoLWriter();
		std::cout << osolwriter-> writeOSoL( osoption);
		
		
		/******************** STEP 3 ************************
		* Create the solver object -- for a CoinSolver we must specify
		* which solver to use
		*/
		//solver = new CoinSolver();
		//solver->sSolverName ="clp"; 
		
		
	    CoinPackedMatrix* matrix;
	    bool columnMajor = true;
	    double maxGap = 0;
		matrix = new CoinPackedMatrix(
		columnMajor, //Column or Row Major
		columnMajor? osinstance->getConstraintNumber() : osinstance->getVariableNumber(), //Minor Dimension
		columnMajor? osinstance->getVariableNumber() : osinstance->getConstraintNumber(), //Major Dimension
		osinstance->getLinearConstraintCoefficientNumber(), //Number of nonzeroes
		columnMajor? osinstance->getLinearConstraintCoefficientsInColumnMajor()->values : osinstance->getLinearConstraintCoefficientsInRowMajor()->values, //Pointer to matrix nonzeroes
		columnMajor? osinstance->getLinearConstraintCoefficientsInColumnMajor()->indexes : osinstance->getLinearConstraintCoefficientsInRowMajor()->indexes, //Pointer to start of minor dimension indexes -- change to allow for row storage
		columnMajor? osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts : osinstance->getLinearConstraintCoefficientsInRowMajor()->starts, //Pointers to start of columns.
		0,   0, maxGap ); 
		
		qpClpModel->setOptimizationDirection( -1);
		qpClpModel->loadProblem( *matrix, osinstance->getVariableLowerBounds(), 
				osinstance->getVariableUpperBounds(),  
				osinstance->getDenseObjectiveCoefficients()[0], 
				osinstance->getConstraintLowerBounds(), osinstance->getConstraintUpperBounds()
		);
		
		
        //now for the quadratic part
		//get the quadratic terms
		std::map<int, std::map<int, double> > varIndexMap;
		std::map<int, std::map<int, double> >::iterator mit1;
		std::map<int, double> tmpMap;
		std::map<int, double>::iterator mit2;
		
		int numNonz;
		numNonz = 0;
		
        QuadraticTerms* quadTerms = NULL;
        quadTerms = osinstance->getQuadraticTerms();
        int i1;
        int j1;
        int tmpint;
          
        for(i = 0; i < osinstance->getNumberOfQuadraticTerms(); i++){
        	
        	if( quadTerms->rowIndexes[ i] == -1){
        		
        		i1 = quadTerms->varOneIndexes[ i] ;
        		j1 = quadTerms->varTwoIndexes[ i] ;
        		
        		if(i1 == j1) quadTerms->coefficients[i] = 2*quadTerms->coefficients[i];
        		
        		//does Clp expect j1 >= i1?????
        		//try this
        		
        		if(j1 < i1){ //switch
        			
        			tmpint = j1;
        			j1 = i1;
        			i1 = tmpint;
        			
        			
        		}
        		
        		mit1 = varIndexMap.find( i1 );
        		
        		if( mit1 == varIndexMap.end() ){ //add new index
        			
        			tmpMap.insert( std::pair<int, double>( j1, 
							quadTerms->coefficients[i]) );
        			numNonz++;
        			
        			varIndexMap[ i1 ] = tmpMap;
        			
        			tmpMap.clear();
        		}else{ 
        			
        			//map index already exists
        			//insert second index if not alreay there
        			mit2 = mit1->second.find( j1) ;
        			
        			if( mit2 == mit1->second.end() ){
        				//add the new index and coefficient
        				mit1->second.insert(  std::pair<int, double>( j1, 
    							quadTerms->coefficients[i])  );
        				numNonz++;
        				
        			}else{
        				
        				mit2->second += quadTerms->coefficients[i];
        				
        			}
        			
        		}       	
        	
        	}//end if on test for objective function index
        	
        }//end loop over quadratic terms
        	
        std::cout << "numNonz = " << numNonz << std::endl;
		
        
        int *start = NULL;
        int *idx = NULL;
        double *nonz = NULL;

        start = new int[ varIndexMap.size() + 1];
        idx = new int[ numNonz]; //index the columns
        nonz = new double[ numNonz];
        
        int kount;
        kount = 0;
        numNonz = 0;
        start[ kount++] = numNonz;

        for( mit1 = varIndexMap.begin(); mit1 != varIndexMap.end();  mit1++){
        	
        	std::cout  << std::endl;
        	std::cout << "FIRST INDEX = " << mit1->first << std::endl;
        	for( mit2 = mit1->second.begin(); mit2 != mit1->second.end();  mit2++){
        		std::cout << "SECOND INDEX = " << mit2->first << std::endl;
        		std::cout << "COEFFICIENT = " << mit2->second << std::endl;
        		idx[ numNonz] = mit2->first;
        		//we multiply by 2 for Clp
        		nonz[ numNonz++ ] = mit2->second;	
        	}
        	start[ kount++] = numNonz ;
        	
        }
        
       /**
        start[ 1] = 2;
        start[ 2] = 4;
        
        idx[0] = 0;
        idx[1] = 1;
        
        idx[2] = 1;
        idx[3] = 0;
        
        nonz[ 0] = -.06666666*2;
        nonz[ 1] = .4*2;
        nonz[ 2] = -.2*2;
        nonz[ 3] = -.3*2;
        */
       

        qpClpModel->loadQuadraticObjective( qpClpModel->numberColumns(), start, idx, nonz);
        
        qpClpModel->writeMps("quad.mps");
        
        
        //call solver
		//qpClpModel->primal();
        ClpCholeskyBase * cholesky = new ClpCholeskyBase();
        cholesky->setKKT(true);
       // qpClpModel->setCholesky(cholesky);
       // qpClpModel->primalDual();
        
        qpClpModel->initialSolve( solveOptions);
		
        double *primal;
        double *dual;
        primal = qpClpModel->primalColumnSolution();
        dual = qpClpModel->dualRowSolution();
	
		
        int numberColumns = qpClpModel->numberColumns();
        int numberRows = qpClpModel->numberRows();
        for (i = 0; i < numberColumns; i++) {
             if (fabs(primal[i]) > 1.0e-8)
                  printf("%d primal %g\n", i, primal[i]);
        }
        for (i = 0; i < numberRows; i++) {
             if (fabs(dual[i]) > 1.0e-8)
                  printf("%d dual %g\n", i, dual[i]);
        }
	 
        
        std::cout << osinstance->printModel();
        
        
		/******************** STEP 4 ************************
		* Give the solver the instance and options and solve
		*/	
		//solver->osinstance = osinstance;
		//solver->osoption = osoption;	
		//solver->solve();
		
		
		/******************** STEP 5 ************************
		* Create a result object and get the optimal objective
		* and primal variable values
		*/	
		//getOSResult( solver->osrl);
		
		//OSResult *osr = solver->osresult;
		//int numOfBasisVar = osr->getNumberOfBasisVar(0);
		//std::cout << "NUMBER OF BASIS VARS = " << numOfBasisVar << std::endl;
		
		
		
		//do garbage collection
		delete osilreader;
		osilreader = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		delete osolwriter;
		osolwriter = NULL;
		delete qpClpModel;
		qpClpModel = NULL;
		//finish garbage collection

		
		/******************** End Clp Example *************************/	
	
	
	
	delete fileUtil;
	fileUtil = NULL;	
	std::cout << "Program terminates normally" << std::endl;
	return 0;
	
	}
	catch(const ErrorClass& eclass){
		delete fileUtil;
		fileUtil = NULL;
		delete qpClpModel;
		qpClpModel = NULL;
		std::cout << eclass.errormsg <<  std::endl;
		return 0;
	} 

}// end main

