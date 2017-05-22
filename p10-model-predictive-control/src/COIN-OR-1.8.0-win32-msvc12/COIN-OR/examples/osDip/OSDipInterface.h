/* $Id: OSDipInterface.h 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSDipInterface.h
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

#ifndef OSDIPINTERFACE_H
#define OSDIPINTERFACE_H

//===========================================================================//
#include "UtilMacros.h"
#include "OSInstance.h"
#include "OSCoinSolver.h"
#include "OSConfig.h"
#include "OSResult.h" 
#include "OSOption.h"
#include "OSiLReader.h"        
#include "OSiLWriter.h" 
#include "OSoLReader.h"        
#include "OSoLWriter.h" 
#include "OSrLReader.h"          
#include "OSrLWriter.h"      
#include "OSInstance.h"  
#include "OSFileUtil.h"  
#include "CoinError.hpp"
#include "OSDefaultSolver.h"  
#include "OSWSUtil.h"     
#include "OSErrorClass.h"
#include "OSMathUtil.h"
#include "CoinHelperFunctions.hpp"



#include <set>
#include <map>
#include <vector>

//===========================================================================//
/*!
 * \class OS_DipInterface
 * A generic class to interface with Optimization Services (OS) for use  
 *    in Dip (Decomposition in Integer Programming).
 *
 *
 */

//===========================================================================//
class OS_DipInterface {


public:

	/** Best known LB/UB */
	bool      m_isProvenOptimal;
	double    m_bestKnownLB;
	double    m_bestKnownUB;



	OSInstance *m_osinstance;
	OSOption *m_osoption;	
	OSiLReader *m_osilreader;
	OSoLReader *m_osolreader;
	char* m_integerVars;
	CoinPackedMatrix *m_coinpm;
	CoinPackedVector *m_row ;


	/** @name Helper Methods. */
	void readOSiL(std::string & filename);
	
	void readOSoL(std::string & filename);

	CoinPackedVector *getRow( int i);
	
	//return a CoinPackedMatrix
	CoinPackedMatrix *getCoinPackedMatrix( );
	
    /** Returns array[getNumCols()] specifying if a variable is integer.

	At present, simply coded as zero (continuous) and non-zero (integer)
	May be extended at a later date.
    */	
	const char* getIntegerColumns();
	
	//get the objective function constant
	double getObjectiveOffset()	;
	
	
	//get the set of variable indexes for each block in the model
	std::vector<std::set<int> > getBlockVarIndexes();
	std::vector<std::set<int> > m_blockVariableIndexes;
	bool m_blockVariableIndexesProcessed;
		
	//get the set of core constraint indexes
	std::set<int>  getCoreConstraintIndexes();
	std::set<int> m_coreConstraintIndexes;
	bool m_coreConstraintIndexesProcessed;
	
	//get a map of constraint indexes for each block in the model
	//the key is the index of the constraint in the original problem
	//the key points to the index number in the block
	std::vector<std::map<int, int> > getBlockConstraintIndexes();
	std::vector<std::map<int, int> > m_blockConstraintIndexes;
	bool m_blockConstraintIndexesProcessed;
	
	//get and osinstance that corresponds to each block in the model
	std::vector<OSInstance* > getBlockOSInstances();
	std::vector<OSInstance* > m_blockOSInstances;
	bool m_blockOSInstancesProcessed;
	
	
	//get factory solver for each block in the model
	std::vector<std::string > getBlockFactories();
	std::vector<std::string > m_blockFactories;
	bool m_blockFactoriesProcessed;
	
	
	//get the objective function coefficients
	double *getObjectiveFunctionCoeff();
	

	inline const double getBestKnownLB() const {return m_bestKnownLB;} //kipp can we get rid of this
	inline const double getBestKnownUB() const {return m_bestKnownUB;} // kipp can we get rid of this


	//some OS wrapper methods

	/**
	 * Get objective maxOrMins. One maxOrMin for each objective.
	 * 
	 * @return a std::string array of objective maxOrMins ("max" or "min"), null if no objectives. 
	 * @throws Exception if the elements in objectives are logically inconsistent. 
	 */
	inline const std::string* getObjectiveMaxOrMins() const {return m_osinstance->getObjectiveMaxOrMins();} 


	/**
	 * Get objective constants. One constant for each objective.
	 * 
	 * @return a double array of objective constants, null if no objectives. 
	 * @throws Exception if the elements in objectives are logically inconsistent. 
	 */
	inline const double* getObjectiveConstants() const {return m_osinstance->getObjectiveConstants();} 

	/**
	 * Get objective weights. One weight for each objective.
	 * 
	 * @return a double array of objective weights, null if no objectives. 
	 * @throws Exception if the elements in objectives are logically inconsistent. 
	 */
	inline const double* getObjectiveWeights() const {return m_osinstance->getObjectiveWeights();} 

	/**
	 * getNumberOfIntegerVariables
	 * @return an integer which is the number of general integer variables. 
	 */
	inline const int getNumberOfIntegerVariables() const {return m_osinstance->getNumberOfIntegerVariables();} 

	/**
	 * getNumberOfBinaryVariables
	 * @return an integer which is the number of binary variables. 
	 */
	inline const int getNumberOfBinaryVariables() const {return m_osinstance->getNumberOfBinaryVariables();}
	

	/**
	 * Get variable number. 
	 * 
	 * @return variable number. 
	 */
	inline const int getVariableNumber() const {return m_osinstance->getVariableNumber();} 

	/**
	 * Get constraint number. 
	 * 
	 * @return constraint number. 
	 */
	inline const int getConstraintNumber() const {return m_osinstance->getConstraintNumber();} 
	
	/**
	 * Get variable lower bounds. 
	 * 
	 * @return pointer to variable lower bounds. 
	 */
	inline const double* getColLower() const {return m_osinstance->getVariableLowerBounds();} 
	
		
	/**
	 * Get variable upper bounds. 
	 * 
	 * @return pointer to variable upper bounds. 
	 */
	inline const double* getColUpper() const {return m_osinstance->getVariableUpperBounds();} 
	
	
	/**
	 * Get variable names. 
	 * 
	 * @return array of variable names. 
	 */
	inline const std::string* getVariableNames() const {return m_osinstance->getVariableNames();} 
	
	
	/**
	 * Get constraint lower bounds. 
	 * 
	 * @return pointer to constraint lower bounds. 
	 */
	inline const double* getRowLower() const {return m_osinstance->getConstraintLowerBounds();} 
	

	/**
	 * Get constraint upper bounds. 
	 * 
	 * @return pointer to constraint upper bounds. 
	 */
	inline const double* getRowUpper() const {return m_osinstance->getConstraintUpperBounds();} 
	
	
	/**
	 * Get constraint names. 
	 * 
	 * @return array of constraint names. 
	 */
	inline const std::string* getConstraintNames() const {return m_osinstance->getConstraintNames();} 
	
	
	std::map<int, std::vector< int> > generateInitialMaster();
	
	//end wrapper methods

	/** class constructor */
	OS_DipInterface();
	
	/** class destructor */
	~OS_DipInterface();
};

#endif
