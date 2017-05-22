//===========================================================================//
// This file is part of the Decomp Solver Framework.                         //
//                                                                           //
// Decomp is distributed under the Common Public License as part of the      //
// COIN-OR repository (http://www.coin-or.org).                              //
//                                                                           //
// Author: Matthew Galati, Lehigh University  
// Heavily edited by Kipp Martin, University of Chicago
//                                                                           //
// Copyright (C) 2002-2009, Lehigh University, Matthew Galati, and Ted Ralphs//
// All Rights Reserved.                                                      //
//===========================================================================//

//===========================================================================//
#include "OSDipApp.h"
//===========================================================================//
#include "DecompVar.h"
#include  "OSDipBlockSolver.h"
#include  "OSDipBlockCoinSolver.h"



void OSDipApp::initializeApp(UtilParameters & utilParam) {

	UtilPrintFuncBegin(m_osLog, m_classTag, "initializeApp()",
			m_appParam.LogLevel, 2);

	//---
	//--- get application parameters
	//---
	m_appParam.getSettings(utilParam);
	if (m_appParam.LogLevel >= 1)
		m_appParam.dumpSettings(m_osLog);

	try {

		//---
		//--- read OSiL instance
		//
		if (m_appParam.OSiLFile.size() <= 1)
			throw ErrorClass("An OSiL file not specified in the parameter file");
		std::string osilFile = m_appParam.DataDir + UtilDirSlash()
				+ m_appParam.OSiLFile;
		m_osInterface.readOSiL( osilFile);

		//---
		//--- read OSoL instance --  it is not necessary, if not there
		//--  all constraints become block constraints
		//
		//if(m_appParam.OSoLFile.size() <=  1) throw ErrorClass("An OSoL file not specified in the parameter file");
		if (m_appParam.OSoLFile.size() > 0) {
			std::string osolFile = m_appParam.DataDir + UtilDirSlash()
					+ m_appParam.OSoLFile;
			m_osInterface.readOSoL( osolFile);
		}

		//get relevant sets and vectors
		//first get a set of core constraint indexes 
		m_coreConstraintIndexes = m_osInterface.getCoreConstraintIndexes();

		// next get a vector of sets, where each set is the
		// variable indexes for a block
		m_blockVars = m_osInterface.getBlockVarIndexes();
		
		//get the solver factory for each block
		 m_blockFactories = m_osInterface.getBlockFactories();

		//finally get an osinstance for each block
		m_blockOSInstances = m_osInterface.getBlockOSInstances();
		
		

		//loop over the instances and generate a solver for each block
		std::vector<OSInstance*>::iterator vit1;
		
		std::string solverFactory;
		int whichBlock = 0;

		OSDipBlockSolver *solver = NULL;
		factoryInit  = new OSDipFactoryInitializer();
		
		for (vit1 = m_blockOSInstances.begin(); vit1
				!= m_blockOSInstances.end(); vit1++) {
			
			//kipp check for a valid name here -- 
			if( m_blockFactories[ whichBlock].size()  > 0){
				
				solverFactory =  m_blockFactories[ whichBlock];
				
			}else{
				//give it the default factory in the parameter file
				solverFactory =  m_appParam.solverFactory;
				
			}
			
			
			
			
			OSDipBlockSolverFactory::factories[ solverFactory]->osinstance = *vit1;
			OSDipBlockSolverFactory::factories[ solverFactory]->osoption = m_osInterface.m_osoption;
			solver = OSDipBlockSolverFactory::factories[ solverFactory]->create();
			solver->m_whichBlock  = whichBlock;
			m_osDipBlockSolver.push_back( solver);
			
			whichBlock++;

		}

		//get a list of all variables in block
		std::vector<std::set<int> >::iterator vit;
		std::set<int>::iterator sit;
		std::set<int> blockVar;

		for (vit = m_blockVars.begin(); vit != m_blockVars.end(); vit++) {

			blockVar = *vit;

			for (sit = blockVar.begin(); sit != blockVar.end(); sit++) {

				if (m_blockVarsAll.find(*sit) == m_blockVarsAll.end()) {
					m_blockVarsAll.insert(*sit);
				}

			}

		}

		blockVar.clear();

		//---
		//--- create models
		//---
		createModels();

		//some testing
		/**
		 std::vector<std::set<int> >::iterator vit2;

		 std::vector<IndexValuePair*> solIndexValPair;
		 std::vector<IndexValuePair*>::iterator vit3;
		 double optVal;

		 double *cost = NULL;
		 int index;
		 int whichBlock;

		 whichBlock = 0;

		 for (vit2 = m_blockVars.begin(); vit2 != m_blockVars.end(); vit2++) {

		 blockVar = *vit2;
		 

		 cost = new double[blockVar.size()];

		 index = 0;

		 for (sit = blockVar.begin(); sit != blockVar.end(); sit++) {

		 cost[index] = m_objective[*sit];
		 cost[index] = -1;
		 index++;

		 }
		 solIndexValPair.clear();
		 m_osDipBlockSolver[whichBlock++]->solve(cost, &solIndexValPair,
		 &optVal);

		 std::cout << "OPTIMAL VALUE  = " << optVal  << std::endl;

		 std::cout << "solIndexValPair SIZE 2 = " << solIndexValPair.size()  << std::endl;

		 for (vit3 = solIndexValPair.begin(); vit3 != solIndexValPair.end(); vit3++) {

		 std::cout << "IDEXXXXX =  " << (*vit3)->idx << std::endl;
		 std::cout << "VALUEEEEE =  " << (*vit3)->value << std::endl;
		 }

		 
		 delete[] cost;

		 }
		*/ 
		
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}

	UtilPrintFuncEnd(m_osLog, m_classTag, "initializeApp()",
			m_appParam.LogLevel, 2);

}//end initializeApp

//===========================================================================//
void OSDipApp::createModelPart(DecompConstraintSet * model,
		const int nRowsPart, const int * rowsPart) {

	const int nCols = m_osInterface.getVariableNumber();
	const double * rowLB = m_osInterface.getRowLower();
	const double * rowUB = m_osInterface.getRowUpper();
	const double * colLB = m_osInterface.getColLower();
	const double * colUB = m_osInterface.getColUpper();
	const char * integerVars = m_osInterface.getIntegerColumns();

	std::cout << "STARTING createModelPart" << std::endl;

	model->M = new CoinPackedMatrix(false, 0.0, 0.0);

	if (!model->M)
		throw UtilExceptionMemory("createModels", "OSDipApp");
	model->reserve(nRowsPart, nCols);
	model->M->submatrixOf(*m_osInterface.m_coinpm, nRowsPart, rowsPart);

	//---
	//--- set the row upper and lower bounds
	//--- set the col upper and lower bounds
	//---
	m_appParam.UseNames = true;
	int i, r;
	for (i = 0; i < nRowsPart; i++) {
		r = rowsPart[i];
		if (m_appParam.UseNames) {
			const char * rowName =
					m_osInterface.getConstraintNames()[r].c_str();
			//		std::cout << "Row Name = " << m_osInterface.getConstraintNames()[r] << std::endl;
			if (rowName)
				model->rowNames.push_back(rowName);

			//std::cout << "Row Name = " << m_osInterface.getConstraintNames()[r] << std::endl;
		}
		model->rowLB.push_back(rowLB[r]);
		model->rowUB.push_back(rowUB[r]);
	}
	copy(colLB, colLB + nCols, back_inserter(model->colLB));
	copy(colUB, colUB + nCols, back_inserter(model->colUB));

	//---
	//--- big fat hack... we don't deal with dual rays yet,
	//---  so, we assume subproblems are bounded
	//---
	//--- NOTE: might also need to tighten LBs
	//---
	//--- Too small - ATM infeasible!
	//--- Too big   - round off issues with big coeffs in 
	//---             master-only vars
	//---
	//--- TODO: need extreme rays or bounded subproblems from user
	//---


	if (m_appParam.ColumnUB < 1.0e15) {
		for (i = 0; i < nCols; i++) {
			if (colUB[i] > 1.0e15) {
				model->colUB[i] = m_appParam.ColumnUB;
			}
		}
	}
	if (m_appParam.ColumnLB > -1.0e15) {
		for (i = 0; i < nCols; i++) {
			if (colLB[i] < -1.0e15) {
				model->colLB[i] = m_appParam.ColumnLB;
			}
		}
	}

	//---
	//--- set the indices of the integer variables of modelRelax
	//---  also set the column names, if they exist
	//---
	for (i = 0; i < nCols; i++) {
		if (m_appParam.UseNames) {
			//const char * colName =  m_osInterface.columnName(i);
			const char * colName = m_osInterface.getVariableNames()[i].c_str();

			if (colName)
				model->colNames.push_back(colName);
		}

		if ((integerVars != NULL) && integerVars[i] == '1') {
			//std::cout  << "WE HAVE AN INTEGER VARIABLE " << std::endl;
			model->integerVars.push_back(i);
		}
	}

	//free local memory
	UTIL_DELARR(integerVars);

}

//===========================================================================//
void OSDipApp::createModels() {

	UtilPrintFuncBegin(m_osLog, m_classTag, "createModels()",
			m_appParam.LogLevel, 2);

	const int nCols = m_osInterface.getVariableNumber();
	int nRowsCore;
	int *rowsCore = NULL;
	int coreRowIndex;
	int i;
	int numBlocks;
	std::set<int>::iterator sit;
	std::string modelName;

	try {

		//First the define the objective function over the entire variable space
		//Create the memory for the objective  function
		m_objective = new double[nCols];
		for (i = 0; i < nCols; i++) {
			m_objective[i] = m_osInterface.getObjectiveFunctionCoeff()[i];
			//std::cout << "obj coeff = " << m_objective[i] << std::endl;
		}
		setModelObjective( m_objective);

		//define the blocks
		numBlocks = static_cast<int> (m_blockVars.size());
		for (i = 0; i < numBlocks; i++) {

			modelName = "Block" + UtilIntToStr(i);
			setModelRelax(NULL, modelName, i);
		}

		//define the core constraints -- constraints NOT in a block

		nRowsCore = m_coreConstraintIndexes.size();
		if (nRowsCore <= 0)
			throw ErrorClass("We need at least one coupling constraint");

		rowsCore = new int[nRowsCore];

		//std::set<int>::iterator sit;	
		coreRowIndex = 0;
		for (sit = m_coreConstraintIndexes.begin(); sit
				!= m_coreConstraintIndexes.end(); sit++) {

			rowsCore[coreRowIndex++] = *sit;

		}

		if (coreRowIndex != nRowsCore)
			throw ErrorClass("There was an error counting coupling constraints");

		DecompConstraintSet * modelCore = new DecompConstraintSet();
		createModelPart(modelCore, nRowsCore, rowsCore);

		setModelCore(modelCore, "core");
		//---
		//--- save a pointer so we can delete it later
		//---
		m_modelC = modelCore;

		// done generating the core

		//get the master only variables
		//modelCore->masterOnlyCols.push_back(i);


		for (i = 0; i < nCols; i++) {

			if (m_blockVarsAll.find(i) == m_blockVarsAll.end()) {
				modelCore->masterOnlyCols.push_back(i);
				//std::cout << "MASTER ONLY VARIABLE " << i << std::endl;
			}
		}

		//---
		//--- create an extra "empty" block for the master-only vars
		//---   since I don't know what OSI will do with empty problem
		//---   we will make column bounds explicity rows
		//---
		int nMasterOnlyCols =
				static_cast<int> (modelCore->masterOnlyCols.size());
		if (nMasterOnlyCols) {
			if (m_appParam.LogLevel >= 1)
				(*m_osLog) << "Create model part Master-Only." << std::endl;

			createModelMasterOnlys2(modelCore->masterOnlyCols);

		}

		UtilPrintFuncBegin(m_osLog, m_classTag, "printCurrentProblem()",
				m_appParam.LogLevel, 2);

		//free local memory

		UTIL_DELARR(rowsCore);

	}//end try

	catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}

}// end createModels()


//===========================================================================//

void OSDipApp::createModelMasterOnlys2(std::vector<int> & masterOnlyCols) {

	int nBlocks = static_cast<int> (m_blockVars.size());
	const int nCols = m_osInterface.getVariableNumber();
	const double * colLB = m_osInterface.getColLower();
	const double * colUB = m_osInterface.getColUpper();
	const char * integerVars = m_osInterface.getIntegerColumns();
	int nMasterOnlyCols = static_cast<int> (masterOnlyCols.size());

	if (m_appParam.LogLevel >= 1) {
		(*m_osLog) << "nCols = " << nCols << std::endl;
		(*m_osLog) << "nMasterOnlyCols = " << nMasterOnlyCols << std::endl;
	}

	if (nMasterOnlyCols == 0)
		return;

	int i;
	std::vector<int>::iterator vit;
	for (vit = masterOnlyCols.begin(); vit != masterOnlyCols.end(); vit++) {
		i = *vit;

		//THINK:
		//  what-if master-only var is integer and bound is not at integer?

		DecompConstraintSet * model = new DecompConstraintSet();
		model->m_masterOnly = true;
		model->m_masterOnlyIndex = i;
		model->m_masterOnlyLB = colLB[i];
		//std::cout << "MASTER ONLY LB =  " << model->m_masterOnlyLB << std::endl;
		model->m_masterOnlyUB = colUB[i];
		//std::cout << "MASTER ONLY UB =  " << model->m_masterOnlyUB << std::endl;
		//0=cont, 1=integer
		if (integerVars[i] == '1')
			model->m_masterOnlyIsInt = true;
		//model->m_masterOnlyIsInt = integerVars[i] ? true : false;
		if (m_appParam.ColumnUB < 1.0e15)
			if (colUB[i] > 1.0e15)
				model->m_masterOnlyUB = m_appParam.ColumnUB;
		if (m_appParam.ColumnLB > -1.0e15)
			if (colLB[i] < -1.0e15)
				model->m_masterOnlyLB = m_appParam.ColumnLB;

		m_modelMasterOnly.insert(std::make_pair(i, model)); //keep track for garbage collection
		setModelRelax(model, "master_only" + UtilIntToStr(i), nBlocks);
		nBlocks++;
	}
	//free local memory
	UTIL_DELARR(integerVars);
	return;
}//end createModelMasterOnlys2


int OSDipApp::generateInitVars(DecompVarList & initVars) {

	//---
	//--- generateInitVars is a virtual method and can be overriden
	//---   if the user has some idea how to initialize the list of 
	//---   initial variables (columns in the DW master)
	//---
	std::cout << "GENERATE INIT VARS" << std::endl;
	UtilPrintFuncBegin(m_osLog, m_classTag, "generateInitVars()",
			m_appParam.LogLevel, 2);

	//---
	//--- Get the initial solution from the OSOption object
	//--- we want the variable other option where name="initialCol"
	//---

	//std::vector<OtherVariableOption*> getOtherVariableOptions(std::string solver_name); 
	std::vector<OtherVariableOption*> otherVarOptions;
	std::vector<OtherVariableOption*>::iterator vit;
	int *index = NULL;
	double *value = NULL;
	int i;
	double objValue;
	int whichBlock;
	DecompVar *var;

	try {
		if (m_osInterface.m_osoption != NULL
				&& m_osInterface.m_osoption->getNumberOfOtherVariableOptions()
						> 0) {
			std::cout << "Number of other variable options = "
					<< m_osInterface.m_osoption->getNumberOfOtherVariableOptions()
					<< std::endl;
			otherVarOptions
					= m_osInterface.m_osoption->getOtherVariableOptions("Dip");
			//iterate over the vector

			for (vit = otherVarOptions.begin(); vit != otherVarOptions.end(); vit++) {

				// see if we have an initialCol option

				if ((*vit)->name.compare("initialCol") == 0) {

					index = new int[(*vit)->numberOfVar];
					value = new double[(*vit)->numberOfVar];

					objValue = 0.0;

					for (i = 0; i < (*vit)->numberOfVar; i++) {

						index[i] = (*vit)->var[i]->idx;

						//convert the string to integer
						value[i] = atoi((*vit)->var[i]->value.c_str());
						objValue += m_objective[index[i]];

					}

					whichBlock = atoi((*vit)->value.c_str());
					var = new DecompVar((*vit)->numberOfVar, index, value,
							objValue);
					var->setBlockId(whichBlock);
					initVars.push_back(var);

					//free local memory
					UTIL_DELARR(index);
					UTIL_DELARR(value);

				}

			}

		}
		
		//for bearcat
		/**
		std::map<int, std::vector< int> > indexMap;
		std::map<int, std::vector< int> >::iterator mit;
		std::vector<int>::iterator vit2;
		

		indexMap = m_osInterface.generateInitialMaster();
		

		int kount;
		std::string* varNames;
		varNames =  m_osInterface.m_osinstance->getVariableNames();
		
		
		for (mit = indexMap.begin(); mit != indexMap.end(); mit++) {
			
			kount = 0;
			whichBlock = 0;
			objValue = 0.0;
			
			index = new int[ mit->second.size() ];
			value = new double[ mit->second.size()];
			
			for (vit2 = mit->second.begin(); vit2 != mit->second.end(); vit2++) {
				index[ kount] = *vit2;
				value[ kount] = 1.0;
				std::cout << varNames[ *vit2]  << std::endl;
				objValue += m_objective[ *vit2];
				kount++;
			}
			
			
			var = new DecompVar(mit->second.size(), index, value, objValue);
			
			var->setBlockId(whichBlock);
			initVars.push_back(var);
			//free local memory
			UTIL_DELARR(index);
			UTIL_DELARR(value);
			
		}
			

		//exit( 1);
		*/

	}//end try
	catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}

	UtilPrintFuncEnd(m_osLog, m_classTag, "generateInitVars()",
			m_appParam.LogLevel, 2);
	return static_cast<int> (initVars.size());
}

DecompSolverStatus OSDipApp::solveRelaxed(const int whichBlock,
		const double * redCostX, const double convexDual,
		std::list<DecompVar*> & vars) {
	
	UtilPrintFuncBegin(m_osLog, m_classTag, "solveRelaxed()",
			m_appParam.LogLevel, 2);

	std::vector<int> solInd;
	std::vector<double> solEls;
	double varRedCost = 0.0;
	double varOrigCost = 0.0;
	int kount;
	
	std::set<int> blockVar;
	
	
	blockVar = m_blockVars[ whichBlock];
	
	
	std::set<int>::iterator sit;
	std::vector<IndexValuePair*> solIndexValPair;
	std::vector<IndexValuePair*>::iterator vit;



	double *cost = NULL;
	int index;

	cost = new double[ blockVar.size()];

	index = 0;
	int* reverseMap;
	int reverseMapSize = blockVar.size();
	reverseMap = new int[ reverseMapSize];
	
	for (sit = blockVar.begin(); sit != blockVar.end(); sit++) {

		cost[index] = redCostX[*sit];
		reverseMap[ index] = *sit;
		//std::cout  << "cost[index] =  " << cost[index] << std::endl;
		index++;

	}
	
	try{
		
		
		m_osDipBlockSolver[whichBlock]->m_whichBlock;
		
	
		m_osDipBlockSolver[whichBlock]->solve(cost, &solIndexValPair, &varRedCost);
		kount = 0;	
		
		//std::cout << "NUMBER OF VARIABLES = " <<  solIndexValPair.size() << std::endl;
		//kipp -- change this!!! Pushing back even the zero variables -- crazy!!!
		/*
		for (sit = blockVar.begin(); sit != blockVar.end(); sit++) {
			//kipp be careful here -- the the dimension of the cost vector
			//is the same as the number of variable in the block -- NOT in the model
			if(solIndexValPair.size() != blockVar.size() ) throw ErrorClass("an inconsistent number of block variables"); 
		  solInd.push_back(  *sit ) ; //  again -- subproblem only sees variable in blockVar
		  //std::cout << "SOLUTION INDEX: = " << *sit << std::endl;
		  //std::cout << "  SOLUTION INDEX SUBPROBLEM = " << kount ;
		  //std::cout << "  VARIABLE VALUE  = " << solIndexValPair[ kount]->value << std::endl;		
		  solEls.push_back(  solIndexValPair[ kount]->value ) ;
		  varOrigCost +=  m_objective[ *sit]*solIndexValPair[ kount]->value;
		  kount++;
		 
		}
		*/

		for (vit = solIndexValPair.begin(); vit != solIndexValPair.end(); vit++) {
			
			//kipp -- check to make sure the variable indexed by (*vit)->idx is in the set blockVar
			solInd.push_back( reverseMap[ (*vit)->idx] ) ;
			solEls.push_back(  (*vit)->value ) ;
			
			 varOrigCost +=  m_objective[ reverseMap[ (*vit)->idx]]*(*vit)->value;
			
		}

		
		delete[] reverseMap;
	
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}

	


	UTIL_DEBUG(m_appParam.LogLevel, 2,
			std::cout << "WHICH BLOCK " << whichBlock << std::endl;
			std::cout << "Convex Dual = " << convexDual << std::endl;
			std::cout << "ORIGINAL COST =  = " <<  varOrigCost << std::endl;
			std::cout << "SUPROBLEM OPT VAL  = " <<  varRedCost << std::endl;
			printf("PUSH var with RC = %g\n", varRedCost - convexDual);
	);

	//exit( 1);
	DecompVar * var = new DecompVar(solInd, solEls, varRedCost - convexDual,
			varOrigCost);
	
	var->setBlockId( whichBlock);
	vars.push_back(var);

	UtilPrintFuncEnd(m_osLog, m_classTag, "APPsolveRelaxed()",
			m_appParam.LogLevel, 2);
			
	
	
	delete[] cost;	

	return DecompSolStatOptimal;
}//end solveRelaxed


int OSDipApp::generateCuts(const double  * x, 
			    DecompCutList & newCuts){
	
	std::cout  << "I AM INSIDE GENERATE CUTS, IT WAS CALLED" << std::endl;

	//exit( 1);
	return 0;
}//end generateCuts



bool OSDipApp::APPisUserFeasible(const double * x, 
				      const int      n_cols,
				      const double   tolZero){
	
	std::cout  << "I AM INSIDE APPIS USER FEASIBLE, IT WAS CALLED" << std::endl;
	//exit( 1);

	return true;	
}




