/* $Id: OSDipInterface.cpp 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSDipInterface.cpp
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

//===========================================================================//


#include "OSDipInterface.h"
#include "UtilMacrosDecomp.h"

//===========================================================================//
void OS_DipInterface::readOSiL(std::string & fileName) {

	FileUtil *fileUtil = NULL;
	fileUtil = new FileUtil();

	try {

		std::string osil;
		osil = fileUtil->getFileAsString(fileName.c_str());
		m_osilreader = new OSiLReader();
		m_osinstance = m_osilreader->readOSiL(osil);

		bool columnMajor = false;
		double maxGap = 0;

		m_coinpm
				= new CoinPackedMatrix(
						columnMajor, //Column or Row Major
						columnMajor ? m_osinstance->getConstraintNumber()
								: m_osinstance->getVariableNumber(), //Minor Dimension
						columnMajor ? m_osinstance->getVariableNumber()
								: m_osinstance->getConstraintNumber(), //Major Dimension
						m_osinstance->getLinearConstraintCoefficientNumber(), //Number of nonzeroes
						columnMajor ? m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->values
								: m_osinstance->getLinearConstraintCoefficientsInRowMajor()->values, //Pointer to matrix nonzeroes
						columnMajor ? m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->indexes
								: m_osinstance->getLinearConstraintCoefficientsInRowMajor()->indexes, //Pointer to start of minor dimension indexes -- change to allow for row storage
						columnMajor ? m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts
								: m_osinstance->getLinearConstraintCoefficientsInRowMajor()->starts, //Pointers to start of columns.
						0, 0, maxGap);

		delete fileUtil;
		fileUtil = NULL;
		//kipp  -- worry about garbage collection here -- do I delete m_osinstance
		//delete  osilreader;
		//osilreader = NULL;


	} catch (const ErrorClass& eclass) {
		std::cout << "PROBLEM READING OSiL File" << std::endl;
		delete fileUtil;
		std::cout << eclass.errormsg << std::endl;
		fileUtil = NULL;
		throw ErrorClass(eclass.errormsg);

	}

}//end readOSiL


//===========================================================================//
void OS_DipInterface::readOSoL(std::string & fileName) {

	FileUtil *fileUtil = NULL;
	fileUtil = new FileUtil();
	try {

		std::string osol;
		osol = fileUtil->getFileAsString(fileName.c_str());
		m_osolreader = new OSoLReader();
		m_osoption = m_osolreader->readOSoL(osol);

		delete fileUtil;
		fileUtil = NULL;
		//kipp  -- worry about garbage collection here -- do I delete m_osinstance
		//delete  osilreader;
		//osilreader = NULL;


	} catch (const ErrorClass& eclass) {
		std::cout << "Problem reading OSoL File" << std::endl;
		delete fileUtil;
		std::cout << eclass.errormsg << std::endl;
		fileUtil = NULL;
		throw ErrorClass(eclass.errormsg);

	}

}// end readOSoL

CoinPackedMatrix * OS_DipInterface::getCoinPackedMatrix() {

	return m_coinpm;
}//end getCoinPackedMatrix

CoinPackedVector * OS_DipInterface::getRow(int i) {

	m_row = new CoinPackedVector();

	int k;

	SparseMatrix* sm;

	sm = m_osinstance->getLinearConstraintCoefficientsInRowMajor();

	for (k = sm->starts[i]; k < sm->starts[i + 1]; k++) {

		m_row->insert(sm->indexes[k], sm->values[k]);

	}

	return m_row;
}//end getRow

const char* OS_DipInterface::getIntegerColumns() {

	int numVars = getVariableNumber();
	m_integerVars = new char[numVars];
	int i;

	for (i = 0; i < numVars; i++) {

		if (m_osinstance->getVariableTypes()[i] == 'B'
				|| m_osinstance->getVariableTypes()[i] == 'I')
			m_integerVars[i] = '1';
		else
			m_integerVars[i] = '0';

	}
	return m_integerVars;

}// end getIntegerColumns


double OS_DipInterface::getObjectiveOffset() {

	double offset = 0;
	//assume only one objective function for now

	if (getObjectiveConstants() != NULL) {
		offset = getObjectiveConstants()[0];
	}

	return offset;
}// end getObjectiveOffset


std::vector<std::string> OS_DipInterface::getBlockFactories() {

	//get the factory solver for each block in the model

	if (m_blockFactoriesProcessed == true)
		return m_blockFactories;

	try {
		if (m_osoption == NULL)
			throw ErrorClass("we have a null osoption");

		std::vector<OtherVariableOption*> otherVariableOptions;
		std::vector<OtherVariableOption*>::iterator vit;

		if (m_osoption != NULL && m_osoption->getNumberOfOtherVariableOptions()
				> 0) {

			otherVariableOptions = m_osoption->getOtherVariableOptions("Dip");
			//iterate over the vector of variable options
			for (vit = otherVariableOptions.begin(); vit
					!= otherVariableOptions.end(); vit++) {

				// right now we assume blocks are ordered  -- we ignor value
				if ((*vit)->name.compare("variableBlockSet") == 0) {

					if ((*vit)->value.size() > 0) {

						m_blockFactories.push_back((*vit)->value);

					} else {

						m_blockFactories.push_back("");
					}

				}

			}
		}

	}//end try

	catch (const ErrorClass& eclass) {

		std::cout << eclass.errormsg << std::endl;
		throw ErrorClass(eclass.errormsg);

	}
	m_blockFactoriesProcessed = true;
	return m_blockFactories;
}//end getBlockVarIndexes

std::vector<std::set<int> > OS_DipInterface::getBlockVarIndexes() {

	//get the variable indexes for each block in the model
	std::set<int> varSet; //variables indexes in a specific block
	if (m_blockVariableIndexesProcessed == true)
		return m_blockVariableIndexes;

	try {
		if (m_osoption == NULL)
			throw ErrorClass("we have a null osoption");

		int numVar;
		int i;
		std::vector<OtherVariableOption*> otherVariableOptions;
		std::vector<OtherVariableOption*>::iterator vit;

		if (m_osoption != NULL && m_osoption->getNumberOfOtherVariableOptions()
				> 0) {

			otherVariableOptions = m_osoption->getOtherVariableOptions("Dip");
			//iterate over the vector of variable options
			for (vit = otherVariableOptions.begin(); vit
					!= otherVariableOptions.end(); vit++) {

				// right now we assume blocks are ordered  -- we ignor value
				if ((*vit)->name.compare("variableBlockSet") == 0) {

					// see if we have a set of block variables
					// if so we insert into our vector of sets
					varSet.clear();

					numVar = (*vit)->numberOfVar;

					for (i = 0; i < numVar; i++) {

						if ((*vit)->var[i]->idx
								>= m_osinstance->getVariableNumber())
							throw ErrorClass(
									"found an invalid varaible index in OSoL file");

						varSet.insert((*vit)->var[i]->idx);

					}//end for on variables in this block

					m_blockVariableIndexes.push_back(varSet);
				}// end of if 


			}//end for over constraint options
		}// if on ospton null


		if (m_blockVariableIndexes.size() <= 0)
			throw ErrorClass("someting wrong -- no variables in the blocks");

	} //end try

	catch (const ErrorClass& eclass) {

		std::cout << eclass.errormsg << std::endl;
		throw ErrorClass(eclass.errormsg);

	}
	m_blockVariableIndexesProcessed = true;
	return m_blockVariableIndexes;
}//end getBlockVarIndexes


std::vector<std::map<int, int> > OS_DipInterface::getBlockConstraintIndexes() {

	if (m_blockConstraintIndexesProcessed == true)
		return m_blockConstraintIndexes;
	//get the variable indexes for each block in the model
	std::map<int, int> conMap; //constraint indexes in a specific block
	std::set<int> varSet; //constraint indexes in a specific block
	std::set<int> coreConstraintIndexes;
	std::vector<std::set<int> > blockVariableIndexes;

	int *starts = NULL;
	int *indexes = NULL;
	int kount;

	try {

		// first get the block variable indexes, these are necessary
		blockVariableIndexes = getBlockVarIndexes();
		// get the core constraint indexes
		coreConstraintIndexes = getCoreConstraintIndexes();

		if (blockVariableIndexes.size() <= 0)
			throw ErrorClass("someting wrong -- no variables in the blocks");

		std::vector<std::set<int> >::iterator vit;
		std::set<int>::iterator sit;
		int i;

		for (vit = blockVariableIndexes.begin(); vit
				!= blockVariableIndexes.end(); vit++) {

			varSet.clear();
			conMap.clear();

			varSet = *vit;
			//now get the nonzeros for the variables in
			//varSet and see which nonzeros are in non-core
			//constraints
			kount = 0;
			for (sit = varSet.begin(); sit != varSet.end(); sit++) {

				starts
						= m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts;
				indexes
						= m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->indexes;

				for (i = starts[*sit]; i < starts[*sit + 1]; i++) {

					//add the row index if not in a core constraint
					//don't count same index twice
					if (coreConstraintIndexes.find(indexes[i])
							== coreConstraintIndexes.end() && 
							(conMap.find(indexes[i] ) == conMap.end()  )
					) {
						//we have a new constraint index

						conMap.insert(std::make_pair(indexes[i], kount));
						kount++;
					}
				}
			}

			m_blockConstraintIndexes.push_back(conMap);

		}//end for iterator


	} //end try

	catch (const ErrorClass& eclass) {

		std::cout << eclass.errormsg << std::endl;
		throw ErrorClass(eclass.errormsg);

	}
	m_blockConstraintIndexesProcessed = true;
	return m_blockConstraintIndexes;
}//end getBlockConstraintIndexes


std::vector<OSInstance*> OS_DipInterface::getBlockOSInstances() {
	//get the OSInstance for each block
	if (m_blockOSInstancesProcessed == true)
		return m_blockOSInstances;

	std::map<int, int> conMap; //constraint indexes in a specific block
	std::set<int> varSet; //constraint indexes in a specific block
	std::vector<std::map<int, int> > blockConstraintIndexes;
	std::set<int> coreConstraintIndexes;
	std::vector<std::set<int> > blockVariableIndexes;

	int *starts = NULL;
	int *indexes = NULL;
	double *values = NULL;

	OSInstance *osinstance;
	std::vector<std::set<int> >::iterator vit;
	std::set<int>::iterator sit;
	std::map<int, int>::iterator mit;
	int i;
	int kount;

	int whichBlock;
	int numNonz;
	int k1, k2;

	//variable stuff
	int numberVar;
	std::string* varNames = NULL;
	char* varTypes = NULL;
	double* varLowerBounds = NULL;
	double* varUpperBounds = NULL;

	//constraint stuff
	int numberCon;
	std::string* conNames = NULL;
	double* conLowerBounds = NULL;
	double* conUpperBounds = NULL;
	double* conConstants = NULL;

	int idx;

	try {

		// first get the block variable indexes, these are necessary
		blockVariableIndexes = getBlockVarIndexes();
		// get the core constraint indexes

		coreConstraintIndexes = getCoreConstraintIndexes();
		// get the block constraint indexes

		blockConstraintIndexes = getBlockConstraintIndexes();

		if (blockVariableIndexes.size() <= 0)
			throw ErrorClass(
					"someting wrong in getBlockOSInstances() -- no variables in the blocks");

		//
		//loop over each block
		//
		whichBlock = 0;
		for (vit = blockVariableIndexes.begin(); vit
				!= blockVariableIndexes.end(); vit++) {

			varSet.clear();
			conMap.clear();
			varSet = *vit;

			osinstance = new OSInstance();
			//define variable arrays
			numberVar = varSet.size();
			varTypes = new char[numberVar];
			varNames = new std::string[numberVar];
			varLowerBounds = new double[numberVar];
			varUpperBounds = new double[numberVar];

			//now get the nonzeros for the variables in
			//varSet and see which nonzeros are in non-core
			//constraints
			kount = 0;

			osinstance->setVariableNumber(numberVar);

			SparseVector *objcoeff;
			objcoeff = new SparseVector(numberVar);

			//need to count the number  of nonzero elements
			//in the block constraints
			numNonz = 0;

			for (sit = varSet.begin(); sit != varSet.end(); sit++) {

				varTypes[kount] = m_osinstance->getVariableTypes()[*sit];
				varLowerBounds[kount]
						= m_osinstance->getVariableLowerBounds()[*sit];
				varUpperBounds[kount]
						= m_osinstance->getVariableUpperBounds()[*sit];

				varNames[kount] = m_osinstance->getVariableNames()[*sit];

				objcoeff->indexes[kount] = kount;
				//objcoeff->values[kount] = 0.0;
				objcoeff->values[kount] =  m_osinstance->getDenseObjectiveCoefficients()[0][ *sit];
				numNonz
						+= m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts[*sit + 1]
								- m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts[*sit];

				kount++;

			}//end of loop over the variables in this block

			osinstance->setVariables(numberVar, varNames, varLowerBounds,
					varUpperBounds, varTypes);

			// now the objective function
			osinstance->setObjectiveNumber(1);
			// now the coefficient
			osinstance->addObjective(-1, "objfunction",
					m_osinstance->getObjectiveMaxOrMins()[0], 0.0, 1.0,
					objcoeff);

			conMap = blockConstraintIndexes[whichBlock];
			numberCon = conMap.size();

			if (numberCon > 0) {

				starts = new int[numberVar + 1];
				indexes = new int[numNonz];
				values = new double[numNonz];
				kount = 0;
				starts[kount] = 0;

				numNonz = 0;
				for (sit = varSet.begin(); sit != varSet.end(); sit++) {

					k2
							= m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts[*sit
									+ 1];
					k1
							= m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->starts[*sit];

					for (i = k1; i < k2; i++) {

						idx
								= m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->indexes[i];

						//check to make sure we are not in a core constraint
						if (coreConstraintIndexes.find(idx)
								== coreConstraintIndexes.end()) {

							indexes[numNonz] = conMap[idx];
							values[numNonz]
									= m_osinstance->getLinearConstraintCoefficientsInColumnMajor()->values[i];
							numNonz++;
						}

					}
					starts[kount + 1] = numNonz;

					kount++;

				}

				//now the constraints


				osinstance->setConstraintNumber(numberCon);
				conLowerBounds = new double[numberCon];
				conUpperBounds = new double[numberCon];
				conConstants = new double[numberCon];

				for (mit = conMap.begin(); mit != conMap.end(); mit++) {

					conLowerBounds[mit->second]
							= m_osinstance->getConstraintLowerBounds()[mit->first];
					conUpperBounds[mit->second]
							= m_osinstance->getConstraintUpperBounds()[mit->first];
					conConstants[mit->second] = 1.0;



				}

				std::cout << "Call setConstraints " << numberCon << std::endl;

				osinstance->setConstraints(numberCon, conNames, conLowerBounds,
						conUpperBounds, conConstants);

				osinstance->setLinearConstraintCoefficients(numNonz, true,
						values, 0, numNonz - 1, indexes, 0, numNonz - 1,
						starts, 0, numberVar);

			}
			
			//numNonz--;


			//add the osinstance
			m_blockOSInstances.push_back(osinstance);

			//see what this puppy looks like

			//std::cout << osinstance->printModel( ) << std::endl;

			objcoeff->bDeleteArrays = true;
			delete objcoeff;
			if (varLowerBounds != NULL)
				delete[] varLowerBounds;
			if (varUpperBounds != NULL)
				delete[] varUpperBounds;
			if (varTypes != NULL)
				delete[] varTypes;
			if (varNames != NULL)
				delete[] varNames;

			//delete []starts;
			//delete []indexes;
			//delete []values;

			if (conLowerBounds != NULL)
				delete[] conLowerBounds;
			if (conUpperBounds != NULL)
				delete[] conUpperBounds;
			if (conConstants != NULL)
				delete[] conConstants;

			whichBlock++;

		}//end for iterator for the blocks


	} //end try

	catch (const ErrorClass& eclass) {

		std::cout << eclass.errormsg << std::endl;
		throw ErrorClass(eclass.errormsg);

	}
	m_blockOSInstancesProcessed = true;
	return m_blockOSInstances;

}//end getBlockOSInstances()

std::set<int> OS_DipInterface::getCoreConstraintIndexes() {

	//get the indexes of the core constraints

	if (m_coreConstraintIndexesProcessed == true)
		return m_coreConstraintIndexes;
	try {
		if (m_osoption == NULL)
			throw ErrorClass("we have a null osoption");

		int numCoreCon;
		int i;
		std::vector<OtherConstraintOption*> otherConstraintOptions;
		std::vector<OtherConstraintOption*>::iterator vit;

		if (m_osoption != NULL
				&& m_osoption->getNumberOfOtherConstraintOptions() > 0) {

			otherConstraintOptions = m_osoption->getOtherConstraintOptions(
					"Dip");
			//iterate over the vector of contraint options
			for (vit = otherConstraintOptions.begin(); vit
					!= otherConstraintOptions.end(); vit++) {

				// get the core constraints
				// get the indexes of these constraints
				if (((*vit)->name.compare("constraintSet") == 0)
						&& ((*vit)->type.compare("Core") == 0)) {

					numCoreCon = (*vit)->numberOfCon;

					for (i = 0; i < numCoreCon; i++) {

						if ((*vit)->con[i]->idx
								>= m_osinstance->getConstraintNumber())
							throw ErrorClass(
									"found an invalid constraint index in OSoL file");

						m_coreConstraintIndexes.insert((*vit)->con[i]->idx);

					}//end for on variables in this block

				}//end iff on core constraints


			}// end of for on constraint options


		}//end of if on osptio null

		if (m_coreConstraintIndexes.size() <= 0)
			throw ErrorClass(
					"there were no core constraints listed in the option file");
	}// end of try


	catch (const ErrorClass& eclass) {

		std::cout << eclass.errormsg << std::endl;
		throw ErrorClass(eclass.errormsg);

	}
	m_coreConstraintIndexesProcessed = true;
	return m_coreConstraintIndexes;

}//end getCoreConstraintIndexes


double* OS_DipInterface::getObjectiveFunctionCoeff() {

	//Dip has only one objective function 

	return m_osinstance->getDenseObjectiveCoefficients()[0];
}// end getObjectiveFunctionCoeff()


/** Default constructor. */
OS_DipInterface::OS_DipInterface() :

	m_isProvenOptimal(false), m_bestKnownLB(-1.e20), m_bestKnownUB(1.e20),
			m_osinstance(NULL),
			m_osoption(NULL),	
			m_osilreader(NULL),
			m_osolreader(NULL),
			m_integerVars(NULL),
			m_coinpm(NULL),
			m_row(NULL),
			m_blockVariableIndexesProcessed(false),
			m_coreConstraintIndexesProcessed(false),
			m_blockConstraintIndexesProcessed(false),
			m_blockOSInstancesProcessed(false),
			m_blockFactoriesProcessed(false) 
{

}

OS_DipInterface::~OS_DipInterface() {
	std::cout << "INSIDE OS DIP INTERFACE DESTRUCTOR" << std::endl;
	if (m_osilreader != NULL)
		delete m_osilreader;
	if (m_osolreader != NULL)
		delete m_osolreader;
	delete m_coinpm;

	std::vector<OSInstance*>::iterator vit;

	if (m_blockOSInstances.size() > 0) {
		for (vit = m_blockOSInstances.begin(); vit != m_blockOSInstances.end(); vit++) {

			delete *vit;
		}
	}
}



std::map<int, std::vector< int> > OS_DipInterface::generateInitialMaster(){
	
	
	// define the classes
	FileUtil *fileUtil = NULL;
	OSiLReader *osilreader = NULL;
	DefaultSolver *solver  = NULL;
	OSInstance *osinstance = NULL;
	OSOption *osoption = NULL;

	// end classes    

	std::string testFileName;
	std::string osil;
	
	
	std::map<int, std::vector< int> > indexMap;
	std::vector< int> indexes;
	fileUtil = new FileUtil();


	try {
		testFileName = "../data/restrictedMaster5.osil";
		
		osil = fileUtil->getFileAsString(testFileName.c_str());

#ifdef MY_DEBUG
		std::cout << "Done reading the file" << std::endl;
#endif

		osilreader = new OSiLReader();
		
		//create the osption 
		osoption = new OSOption();

#ifdef MY_DEBUG
		parsingTestResult << "Reading files successfully" << std::endl;
#endif

		osinstance = osilreader->readOSiL(osil);
		
		solver = new CoinSolver();
		solver->sSolverName ="cbc"; 
		solver->osinstance = osinstance;
		solver->osoption = osoption;	
		std::cout << "CALLING SOLVE FOR RESTRICTED MASTER" << std::endl;
		solver->solve();
		int i;
		int j;
		int k;
		int vecSize;
		// now get the primal solution
		std::vector<IndexValuePair*> primalValPair;
		primalValPair = solver->osresult->getOptimalPrimalVariableValues( 0);
		vecSize = primalValPair.size();
		
		
		int numNodes = 5;
		int numHubs = 1;
		int totalDemand = 4;
		
		
		
		int* routeDemand = NULL;
		int startPnt;
		
		routeDemand = new int[ numHubs];
		int kount;
		
		std::string* varNames;
		varNames =  osinstance->getVariableNames();
		
		//get route demands
		kount = 0;
		for(k = 0; k < numHubs; k++){
			
			routeDemand[k ] = (int)primalValPair[ kount]->value;
			
			std::cout << "Route Demand = " << routeDemand[k] << std::endl;
			kount++;
		
		}
		
		//now get x variable values
		
		for(k = 0; k < numHubs; k++){
			
			startPnt = k*totalDemand*(numNodes*numNodes - numNodes) + (routeDemand[ k] - 1)*(numNodes*numNodes - numNodes);
			
			for(i = 0; i < numNodes; i++){
				
				
				for(j = 0; j < i; j++){
					
					if( primalValPair[ kount]->value > .1){
						
						std::cout << varNames[ kount] << std::endl;
						indexes.push_back(startPnt +  i*(numNodes-1) + j    );
						
						
					}
					
					kount++;
					
				}
				
				for(j = i + 1; j < numNodes; j++){
					
					
					if( primalValPair[ kount]->value > .1){
						std::cout << varNames[ kount] << std::endl;
						indexes.push_back(startPnt +  i*(numNodes-1) + j -1   );
						
						
					}
					
					
					kount++;
					
				}
				
			}
			
			//create the map
			
			indexMap.insert(make_pair(k, indexes));
			
		}//end loop on k -- hubs
		
		
		std::cout << "DONE CALLING SOLVE FOR RESTRICTED MASTER" << std::endl;

		
		
		delete[] routeDemand;
		
		
		delete osilreader;
		osilreader = NULL;
		delete solver;
		solver = NULL;
		delete osoption;
		osoption = NULL;
		

	} catch (const ErrorClass& eclass) {
		std::cout << std::endl << std::endl << std::endl;
		if (osilreader != NULL)
			delete osilreader;
		if (solver != NULL)
			delete solver;
		if (osoption != NULL)
			delete osoption;
		//  Problem with the parser
		throw ErrorClass(eclass.errormsg);
	}

	delete fileUtil;
	fileUtil = NULL;
	return indexMap;
	
}//end generateInitialMaster
