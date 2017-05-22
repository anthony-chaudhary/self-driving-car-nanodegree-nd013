/* $Id: OSSolverDemo.cpp 3300 2010-03-17 07:32:54Z kmartin $ */
/** @file OSSolverDemo.cpp
 * 
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
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
#include "OSnl2osil.h"   
#include "OSBase64.h"
#include "OSErrorClass.h"
#include "OSMathUtil.h"
#include "CoinError.hpp"
#include "CoinHelperFunctions.hpp"
#include <iostream> 



#ifdef OS_HAS_ASL
#include "OSnl2osil.h"
#endif

#ifdef OS_HAS_BONMIN   
#include "OSBonminSolver.h"
#endif

#ifdef OS_HAS_COUENNE    
#include "OSCouenneSolver.h"
#endif

#ifdef OS_HAS_IPOPT    
#include "OSIpoptSolver.h"
#endif


using std::string;
using std::cout;   
using std::endl;

void getOSResult(std::string osrl);

//int main(int argC, char* argV[]){
int main( ){
	WindowsErrorPopupBlocker();
	FileUtil *fileUtil = NULL; 
	fileUtil = new FileUtil();
	cout << "Start Building the Model" << endl;
	//int i;
	try{


		const char dirsep =  CoinFindDirSeparator();
		std::string osil;
		// Set directory containing mps data files.
		std::string dataDir;
		std::string osilFileName;
		//dataDir = dirsep == '/' ? "../../data/" : "..\\..\\data\\";
		dataDir = dirsep == '/' ? "../data/" : "..\\data\\";
		// first declare a generic solver
		DefaultSolver *solver  = NULL;
		
		OSiLReader *osilreader = NULL;
		OSInstance *osinstance = NULL;
		OSoLWriter *osolwriter = NULL;
		OSOption* osoption = NULL;
		
		// set initial/starting values of 0 for the variables
		//double* xinitial = NULL;
		int numVar;
		double *xinitial;
		int i;
		

		
		/******************** Start Clp Example *************************/
		std::cout << std::endl << std::endl;
		std::cout << "CLP EXAMPLE" << std::endl;
		
		/******************** STEP 1 ************************
		* Get an instance in mps format, and create an OSInstance object
		*/
		std::string mpsFileName;
		mpsFileName =  dataDir  + "mpsFiles" + dirsep +  "parinc.mps";
		// convert to the OS native format
		OSmps2osil *mps2osil = NULL;
		mps2osil = new OSmps2osil( mpsFileName);
		// create the first in-memory OSInstance
		mps2osil->createOSInstance() ;
		osinstance =  mps2osil->osinstance;
		
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
		solver = new CoinSolver();
		solver->sSolverName ="clp"; 
		
		/******************** STEP 4 ************************
		* Give the solver the instance and options and solve
		*/	
		solver->osinstance = osinstance;
		solver->osoption = osoption;	
		solver->solve();
		
		
		/******************** STEP 5 ************************
		* Create a result object and get the optimal objective
		* and primal variable values
		*/	
		//getOSResult( solver->osrl);
		
		//OSResult *osr = solver->osresult;
		//int numOfBasisVar = osr->getNumberOfBasisVar(0);
		//std::cout << "NUMBER OF BASIS VARS = " << numOfBasisVar << std::endl;
		

		
		
		//do garbage collection
		delete mps2osil;
		mps2osil = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		delete osolwriter;
		osolwriter = NULL;
		//finish garbage collection

		
		/******************** End Clp Example *************************/
		
		
	
		
		/******************** Start Cbc Example *************************/
		std::cout << std::endl << std::endl;
		std::cout << "CBC EXAMPLE" << std::endl;

		/******************** STEP 1 ************************
		* Get an instance in native OSiL format and create an OSInstance object
		*/
		osilFileName =   dataDir + "osilFiles" + dirsep +  "p0033.osil";
		osil = fileUtil->getFileAsString( osilFileName.c_str() );
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
		// tell Cbc limit the number of nodes in the branch and bound tree
		osoption->setAnotherSolverOption("maxN","1000","cbc","","integer","");
		// tell Cbc limit the number of seconds
		osoption->setAnotherSolverOption("sec",".01","cbc","","integer","");
		// tell Cbc not to use cutting planes
		osoption->setAnotherSolverOption("cuts","off","cbc","","string","");
		//set a high-level of log reporting
		osoption->setAnotherSolverOption("log","10","cbc","","integer","");
		osolwriter = new OSoLWriter();
		std::cout << osolwriter-> writeOSoL( osoption);
		
		/******************** STEP 3 ************************
		* Create the solver object -- for a CoinSolver we must specify
		* which solver to use
		*/
		solver = new CoinSolver();
		solver->sSolverName ="cbc";
		
		/******************** STEP 4 ************************
		* Give the solver the instance and options and solve
		*/			
		solver->osinstance = osinstance;
		solver->osoption = osoption;	
		solver->solve();
		
		/******************** STEP 5 ************************
		* Create a result object and get the optimal objective
		* and primal variable values
		*/	
		getOSResult( solver->osrl);
		
		
		// start garbage collection
		delete osilreader;
		osilreader = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		delete osolwriter;
		osolwriter = NULL;
		// finish garbage collection
		
		//return 0;
		
		/******************** End Cbc Example *************************/
		
		
	#if 1	
#ifdef OS_HAS_COUENNE		
		/******************** Start Couenne Example *************************/
		
		std::cout << std::endl << std::endl;
		std::cout << "COUENNE EXAMPLE" << std::endl;
		
		/******************** STEP 1 ************************
		 * Get an instance in AMPL nl  format, and create an OSInstance object
		 */
		std::string nlFileName;
		nlFileName =  dataDir  +  "amplFiles" + dirsep +  "bonminEx1.nl";
		// convert to the OS native format
		OSnl2osil *nl2osil = NULL;
		nl2osil = new OSnl2osil( nlFileName);
		// create the first in-memory OSInstance
		nl2osil->createOSInstance() ;
		osinstance =  nl2osil->osinstance;
		
		/******************** STEP 2 ************************
		 * Create an OSOption object and give the solver options
		 */			
        osoption = new OSOption();
        /** 
         *  here is the format for setting options:
         *	bool setAnotherSolverOption(std::string name, std::string value, std::string solver, 
         *	std::string category, std::string type, std::string description);
         */

        // set Bonmin options through Couenne 
        // set a limit of 50000 nodes -- this is on Cbc
        osoption->setAnotherSolverOption("node_limit","50000","couenne","bonmin","integer","");
        // control some Bonmin output
        osoption->setAnotherSolverOption("bb_log_level","3","couenne","bonmin","integer","");
        osoption->setAnotherSolverOption("nlp_log_level","2","couenne","bonmin","integer","");
        //solve 3 times at each node and get best solution
        osoption->setAnotherSolverOption("num_resolve_at_node","3","couenne","bonmin","integer","");
        //solve 5 times at root node and get best solution
        //osoption->setAnotherSolverOption("num_resolve_at_root","5","couenne","bonmin","integer","");
        // set Ipopt options through Couenne
        osoption->setAnotherSolverOption("max_iter","100","couenne","ipopt","integer","");

        // set a Couenne time limit option -- this seems to have no effect
        osoption->setAnotherSolverOption("time_limit","100","couenne","","numeric","");


        numVar =osinstance->getVariableNumber();
        xinitial = new double[numVar];
        for(i = 0; i < numVar; i++){
            xinitial[ i] = 0.0;
        }
        osoption->setInitVarValuesDense(numVar, xinitial);
        osolwriter = new OSoLWriter();
        std::cout << osolwriter-> writeOSoL( osoption);
		
		/******************** STEP 3 ************************
		 * Create the solver object
		 */			
		solver = new CouenneSolver();
		
		
		/******************** STEP 4 ************************
		 * Give the solver the instance and options and solve
		 */	
		
		solver->osinstance = osinstance;
		//solver->osoption = osoption;	
		solver->osol = "";
		solver->buildSolverInstance();
		solver->setSolverOptions();
		solver->solve()	;
		
		
		
		/******************** STEP 5 ************************
		 * Create a result object and get the optimal objective
		 * and primal variable values
		 */	
		std::cout  << "call get osresult" << std::endl;
		std::cout << solver->osrl << std::endl;
		getOSResult( solver->osrl);
		
		// start garbage collection
		delete[] xinitial;
		xinitial = NULL;
		delete osilreader;
		osilreader = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		delete osolwriter;
		osolwriter = NULL;
		delete nl2osil;
		nl2osil = NULL;
		// finish garbage collection
		
		/******************** End Couenne Example *************************/
		
#endif //end of  OS_HAS_COUENNE			
#endif //end #if 0/1		
		
		
		
		
		/******************** Start SYMPHONY Example *************************/
		std::cout << std::endl << std::endl;
		std::cout << "SYMPHONY EXAMPLE" << std::endl;
		
		
		/******************** STEP 1 ************************
		* Get an instance in native OSiL format and create an OSInstance object
		*/
		osilFileName =  dataDir + "osilFiles" + dirsep +  "p0033.osil";
		osil = fileUtil->getFileAsString( osilFileName.c_str() );
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
		 
		//turn on SYMPHONY output
		osoption->setAnotherSolverOption("verbosity","0","symphony","","","");
		osolwriter = new OSoLWriter();
		std::cout << osolwriter-> writeOSoL( osoption);
		
		/******************** STEP 3 ************************
		* Create the solver object -- for a CoinSolver we must specify
		* which solver to use
		*/	
		solver = new CoinSolver();
		solver->sSolverName ="symphony"; 

		/******************** STEP 4 ************************
		* Give the solver the instance and options and solve
		*/			
		solver->osinstance = osinstance;
		solver->osoption = osoption;	
		solver->solve();
		
		/******************** STEP 5 ************************
		* Create a result object and get the optimal objective
		* and primal variable values
		*/	
		getOSResult( solver->osrl);
		// start garbage collection
		delete osilreader;
		osilreader = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		delete osolwriter;
		osolwriter = NULL;
		//finish garbage collection
// 		
/******************** End SYMPHONY Example *************************/	
		
		
		
			

#ifdef OS_HAS_IPOPT		
		/******************** Start Ipopt Example *************************/
		
		std::cout << std::endl << std::endl;
		std::cout << "IPOPT EXAMPLE" << std::endl;
		
		/******************** STEP 1 ************************
		* Get an instance in OSiL  format, and create an OSiL string
		*/
		osilFileName =  dataDir  +  "osilFiles" + dirsep +  "rosenbrockmod.osil";
		//get an osil string
		osil = fileUtil->getFileAsString( osilFileName.c_str() );

		/******************** STEP 2 ************************
		* Create an OSOption object and give the solver options
		*/			
		osoption = new OSOption();
		/** 
		 *  here is the format for setting options:
		 *	bool setAnotherSolverOption(std::string name, std::string value, std::string solver, 
		 *	std::string category, std::string type, std::string description);
		 */
		 
		// set iteration limit
		osoption->setAnotherSolverOption("max_iter","100","ipopt","","integer","");
		osoption->setAnotherSolverOption("output_file","ipopt_out.txt","ipopt","","string","");
		
		
		// set initial/starting values of 0 for the variables
		numVar = 2; //rosenbrock mod has two variables 
		xinitial = new double[numVar];
		for(i = 0; i < numVar; i++){
			xinitial[ i] = 1.0;
		}
		osoption->setInitVarValuesDense(numVar, xinitial);
		osolwriter = new OSoLWriter();
		std::cout << osolwriter-> writeOSoL( osoption);
		
		/******************** STEP 3 ************************
		* Create the solver object
		*/			
		solver = new IpoptSolver();
		
		
		/******************** STEP 4 ************************
		* Give the solver the instance and options and solve
		*/	
		solver->osil = osil;
		solver->osoption = osoption;	
		solver->solve();
		
		
		/******************** STEP 5 ************************
		* Create a result object and get the optimal objective
		* and primal variable values
		*/	
		

		getOSResult( solver->osrl);

		
		// start garbage collection
		delete[] xinitial;
		xinitial = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		delete osolwriter;
		osolwriter = NULL;
		// finish garbage collection
		
		/******************** End Ipopt Example *************************/
	
		
		
#endif //end of  OS_HAS_IPOPT	


#ifdef OS_HAS_BONMIN	
		/******************** Start Bonmin Example *************************/
		
		std::cout << std::endl << std::endl;
		std::cout << "BONMIN EXAMPLE" << std::endl;
		
		/******************** STEP 1 ************************
		* Get an instance in OSiL  format, and create an OSInstance object
		*/
		osilFileName =  dataDir  + "osilFiles" + dirsep +   "wayneQuadratic.osil";
		osil = fileUtil->getFileAsString( osilFileName.c_str() );
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
		 
		// we are going to limit thenumber of nodes and terminate early
		// set a limit of 0 nodes 
		osoption->setAnotherSolverOption("node_limit","0","bonmin","","integer","");

		
		osolwriter = new OSoLWriter();
		std::cout << osolwriter-> writeOSoL( osoption);
		
		/******************** STEP 3 ************************
		* Create the solver object
		*/			
		solver = new BonminSolver();
		
		
		/******************** STEP 4 ************************
		* Give the solver the instance and options and solve
		*/	
		solver->osinstance = osinstance;
		solver->osoption = osoption;	
		solver->solve();
		
		
		/******************** STEP 5 ************************
		* Create a result object and get the optimal objective
		* and primal variable values
		*/	
		std::cout  << "call get osresult" << std::endl;
		std::cout << solver->osrl << std::endl;
		getOSResult( solver->osrl);
		std::cout  << "finish call get osresult" << std::endl;
		// start garbage collection
		delete osilreader;
		osilreader = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		delete osolwriter;
		osolwriter = NULL;
		// finish garbage collection
		
		/******************** End Bonmin Example *************************/
		
#endif //end of  OS_HAS_BONMIN
        
			
		delete fileUtil;
		fileUtil = NULL;	
		std::cout << "\nSolverDemo COMPLETED WITHOUT ERROR\n";
		return 0;
		//
	}
	catch(const ErrorClass& eclass){
		delete fileUtil;
		std::cout << eclass.errormsg <<  std::endl;
		std::cout << "\nSolverDemo COMPLETED, BUT THERE WERE ERRORS\n";
		return 0;
	} 
}// end main


//create result object and get solution information
void getOSResult(std::string osrl){
//see the example OSResultDemo for a more detailed example
	std::cout << std::endl << std::endl << std::endl;
	OSrLReader *osrlreader = NULL;
	OSResult *osresult = NULL;
	osrlreader = new OSrLReader();
	osresult  = osrlreader->readOSrL( osrl);
	
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
		std::cout << std::endl;
		std::cout << "We are not optimal!" << std::endl;
		std::cout << solStatus << std::endl;
	}
	
	int i;
	int vecSize;
	// now get the primal solution
	std::vector<IndexValuePair*> primalValPair;
	primalValPair = osresult->getOptimalPrimalVariableValues( 0);
	vecSize = primalValPair.size();
	for(i = 0; i < vecSize; i++){
		if(primalValPair[ i]->value > 0 || primalValPair[ i]->value < 0){
			std::cout << "index = " <<  primalValPair[ i]->idx ;
			std::cout << "    value = " <<  primalValPair[ i]->value << std::endl;
		}
	}
	// write a description of the solution status
	
	
	std::cout << osresult->getSolutionStatusDescription( 0)<< std::endl;
	delete osrlreader;
}// get OSResult

