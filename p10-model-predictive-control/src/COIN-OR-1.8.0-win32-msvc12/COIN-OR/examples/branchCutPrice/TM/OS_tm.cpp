/* $Id: OS_tm.cpp 2904 2009-09-06 06:47:42Z kmartin $ */
// Last edit: 1/6/07
//
// Name:     OS_tm.cpp
// Author:   Francois Margot
//           Tepper School of Business
//           Carnegie Mellon University, Pittsburgh, PA 15213
//           email: fmargot@andrew.cmu.edu
// Date:     12/28/03
//-----------------------------------------------------------------------------
// Copyright (C) 2003, Francois Margot, International Business Machines
// Corporation and others.  All Rights Reserved.

#include<iomanip>

//OS Stuff
#include "OSConfig.h"
#include "OSResult.h" 
#include "OSiLReader.h"        
#include "OSiLWriter.h"   
#include "OSrLReader.h"          
#include "OSrLWriter.h"      
#include "OSFileUtil.h"      
#include "OSErrorClass.h"
#include "OSErrorClass.h"
//end OS Stuff

#include <CoinHelperFunctions.hpp>
#include <CoinFileIO.hpp>
#include <OsiClpSolverInterface.hpp>
#include <cassert>

#include "OS_init.hpp"
#include "BCP_tm.hpp"
#include "OS_tm.hpp"
#include "OS_user_data.hpp"
#include "OS.hpp"

#include "BCP_math.hpp"

using namespace std;

/*************************************************************************/

int main(int argc, char* argv[]) {
	WindowsErrorPopupBlocker();
	OS_init os_init;
	int retCode = -1;
	std::cout << "Call bcp_main  "  << std::endl;
	retCode =  bcp_main(argc, argv, &os_init);
	std::cout << "Return from call to bcp_main " << retCode << std::endl;
	//delete os_init.tm;
	std::cout << "OSBranchCutPrice terminates normally" << std::endl;
	return retCode;
}

/*************************************************************************/



//kipp where is the constructon


void OS_tm::readInput(const char* filename){



   // Create OSInstance and feed into class OS_prob
	
	// get the file
	FileUtil *fileUtil = NULL; 
	fileUtil = new FileUtil();
	const char dirsep =  CoinFindDirSeparator();
 	// Set directory containing mps data files.
 	std::string dataDir;
 	dataDir = dirsep == '/' ? "../data/" : "..\\data\\";
	cout << "Start Building the Model" << endl;	
	
	try{
		// get the parincLinear problem
		std::string osilFileName;
		//osilFileName =  dataDir  +   "parincLinear.osil";
		osilFileName =  dataDir + "p0033.osil";
		std::cout << "Try to read a sample file" << std::endl;
		std::cout << "The file is: " << osilFileName << std::endl <<std::endl ;
		std::string osil = fileUtil->getFileAsString( osilFileName.c_str() );
		//if(filename == NULL) throw ErrorClass("put file name on command line");
		//osil = fileUtil->getFileAsString( osilFileName);
		os_prob.osilreader = new OSiLReader();
		os_prob.osinstance = os_prob.osilreader->readOSiL( osil);
		delete fileUtil;
	}// end the try

	
	catch(const ErrorClass& eclass){
		delete fileUtil;
		std::cout << eclass.errormsg <<  std::endl;
		return ;
	} 
	
	
	
}

/*************************************************************************/
void OS_tm::pack_module_data(BCP_buffer& buf, BCP_process_t ptype) {
  // possible process types looked up in BCP_enum_process_t.hpp
  
  switch (ptype) {
  case BCP_ProcessType_LP:
    {
      // Pack a pointer; does not work for parallel machines
      // this make make the os_prob object available to the os_lp object
      buf.pack(&os_prob); 
    }
    break;
  default:
    abort();
  }
}



void OS_tm::initialize_core(BCP_vec<BCP_var_core*>& vars,
		       BCP_vec<BCP_cut_core*>& cuts,
		       BCP_lp_relax*& matrix){
  
// Transmit the core constraints and variables to BCP
	std::cout << "INITIALIZE THE CORE " << std::endl;
	int i;
	int num_vars = os_prob.osinstance->getVariableNumber();   	
	double objsign = 1;
	
	objsign = ( os_prob.osinstance->getObjectiveMaxOrMins()[0] == "min") ? 1 : -1;

	for (i = 0; i < num_vars; ++i) {
		os_prob.osinstance->getDenseObjectiveCoefficients()[0][i] = objsign*(os_prob.osinstance->getDenseObjectiveCoefficients()[0][i]);
		if ( os_prob.osinstance->getVariableTypes()[i] == 'B'    ||   os_prob.osinstance->getVariableTypes()[i] == 'I') {
			if ( os_prob.osinstance->getVariableTypes()[i] == 'B' ) {
				vars.push_back(new BCP_var_core(BCP_BinaryVar, os_prob.osinstance->getDenseObjectiveCoefficients()[0][i], 0, 1));
			}
			else {
				vars.push_back(new BCP_var_core(BCP_IntegerVar, os_prob.osinstance->getDenseObjectiveCoefficients()[0][i], 
						os_prob.osinstance->getVariableLowerBounds()[ i], 
						os_prob.osinstance->getVariableUpperBounds()[ i]) );
			}
		} 
		else {
			vars.push_back(new BCP_var_core(BCP_ContinuousVar, os_prob.osinstance->getDenseObjectiveCoefficients()[0][i], 
					os_prob.osinstance->getVariableLowerBounds()[ i], 
					os_prob.osinstance->getVariableUpperBounds()[ i] ));
		}
		
	}
	
	const int num_cons = os_prob.osinstance->getConstraintNumber();
	
    // core constraints
   
	for (i=0; i < num_cons; ++i) {
		cuts.push_back(new BCP_cut_core( os_prob.osinstance->getConstraintLowerBounds()[ i], 
				os_prob.osinstance->getConstraintUpperBounds()[ i]));
	}
  
	matrix = new BCP_lp_relax;
	bool columnMajor = os_prob.osinstance->getLinearConstraintCoefficientMajor();
	int maxGap = 0;
	CoinPackedMatrix*  core_matrix = new CoinPackedMatrix(
		columnMajor, //Column or Row Major
		columnMajor? os_prob.osinstance->getConstraintNumber() : os_prob.osinstance->getVariableNumber(), //Minor Dimension
		columnMajor? os_prob.osinstance->getVariableNumber() : os_prob.osinstance->getConstraintNumber(), //Major Dimension
		os_prob.osinstance->getLinearConstraintCoefficientNumber(), //Number of nonzeroes
		columnMajor? os_prob.osinstance->getLinearConstraintCoefficientsInColumnMajor()->values : 
			os_prob.osinstance->getLinearConstraintCoefficientsInRowMajor()->values, //Pointer to matrix nonzeroes
		columnMajor? os_prob.osinstance->getLinearConstraintCoefficientsInColumnMajor()->indexes : 
			os_prob.osinstance->getLinearConstraintCoefficientsInRowMajor()->indexes, //Pointer to start of minor dimension indexes -- change to allow for row storage
		columnMajor? os_prob.osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts : 
			os_prob.osinstance->getLinearConstraintCoefficientsInRowMajor()->starts, //Pointers to start of columns.
			0,   0, maxGap );
	
	//os_prob.core is a coin packed matrix
	matrix->copyOf(*core_matrix, os_prob.osinstance->getDenseObjectiveCoefficients()[0], os_prob.osinstance->getVariableLowerBounds(), 
			os_prob.osinstance->getVariableUpperBounds(), os_prob.osinstance->getConstraintLowerBounds(), 
			os_prob.osinstance->getConstraintUpperBounds());
	
	//do the garbage collection
	delete core_matrix;

}//end initialize_core

/**************************************************************************/
void  OS_tm::create_root(BCP_vec<BCP_var*>& added_vars,
		BCP_vec<BCP_cut*>& added_cuts, BCP_user_data*& user_data) {
  
#ifdef USER_DATA
  user_data = new MY_user_data(os_prob.colnum);
#endif

}

/*************************************************************************/
void  OS_tm::display_feasible_solution(const BCP_solution *soln) {
	
	//kipp:  -- later write the OSiL file from here
	std::cout << "Default BCP display of the feasible solution: ***************" << std::endl << std::endl;
	BCP_tm_user::display_feasible_solution(  soln);
	
}


/*************************************************************************/

OS_tm::OS_tm(){

}



/*************************************************************************/

OS_tm::~OS_tm(){
	std::cout << "Inside OS_tm destructor" << std::endl;
	delete os_prob.osilreader;
}
