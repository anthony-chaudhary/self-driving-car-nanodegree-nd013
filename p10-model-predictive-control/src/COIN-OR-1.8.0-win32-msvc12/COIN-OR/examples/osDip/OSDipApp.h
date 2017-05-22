//===========================================================================//
// This file is part of the Decomp Solver Framework.                         //
//                                                                           //
// Decomp is distributed under the Common Public License as part of the      //
// COIN-OR repository (http://www.coin-or.org).                              //
//                                                                           //
// Author: Matthew Galati, Lehigh University                                 //
//                                                                           //
// Copyright (C) 2002-2008, Lehigh University, Matthew Galati, and Ted Ralphs//
// All Rights Reserved.                                                      //
//===========================================================================//

#ifndef OSDIPAPP_H
#define OSDIPAPP_H

// --------------------------------------------------------------------- //
#include "DecompApp.h"
#include "DecompParam.h"

// --------------------------------------------------------------------- //
#include "OSDipInterface.h"
#include "OSDipParam.h"
#include  "OSDipBlockSolver.h"
#include "OSInstance.h"
#include  "OSDipFactoryInitializer.h"

// --------------------------------------------------------------------- //
/*!
 * \class OSDipApp
 * A DecompApp for solving the 
 *     Bearcat Problem.
 * 
 * \see
 * DecompApp
 *
 */
// --------------------------------------------------------------------- //
class OSDipApp: public DecompApp {
public:
	
	OSDipFactoryInitializer *factoryInit;
	
	/** m_osDipBlockSolver is a vector OSDipBlockSolvers  */
	std::vector<OSDipBlockSolver* > m_osDipBlockSolver;
	
	/** m_blockOSInstances is a vector with an osinstance for each block */
	std::vector<OSInstance* > m_blockOSInstances;
	
	/** m_blockVars is a vector with the set of variables for each block */
	std::vector<std::set<int> >  m_blockVars;
	
	/** m_blockVarsAll is the set of all variables that appear in a block */
	std::set<int> m_blockVarsAll; 
	
	/** m_coreConstraintIndexes is the set core constraint indexes */
	std::set<int>  m_coreConstraintIndexes;	
	
	/** m_blockFactories is a vector solver factory for each block */
	std::vector<std::string >  m_blockFactories;
	
	/** Class id tag (for log / debugging). */
	const std::string m_classTag;

	/** the OS interface */
	OS_DipInterface m_osInterface;

	/** Application specific parameters. */
	OSDipParam m_appParam;

	/** The model objective coefficients (original space). */
	double *m_objective;

	/** The model constraint systems used for different algos. */
	DecompConstraintSet *m_modelC;
	std::map<int, DecompConstraintSet*> m_modelR;
	
	/** The model constraint system used master only vars */
	std::map<int, DecompConstraintSet*> m_modelMasterOnly;

	/** Initialize application. */
	void initializeApp(UtilParameters & utilParam);

	/** Create model parts. */
	void createModels();

	//this creates the coupling/core constraint set
	void createModelPart(DecompConstraintSet * model, const int nRowsPart,
			const int * rowsPart);

	/* @name Inherited (from virtual) methods. */
	int generateInitVars(DecompVarList & initVars);

	void createModelMasterOnlys2(std::vector<int> & masterOnlyCols);

	   /* @name Inherited (from virtual) methods. */
	   /** Solve the relaxed problem. */
       DecompSolverStatus solveRelaxed(const int          whichBlock,
                                       const double     * redCostX,
                                       const double       convexDual,
                                       std::list<DecompVar*> & vars);  
	   
	   
	   int generateCuts(const double* x, DecompCutList & newCuts);
	   
	   bool APPisUserFeasible(const double* x, const int n_cols,
			   const double tolZero);

	/** Default constructor. Takes an instance of UtilParameters */
	OSDipApp(UtilParameters & utilParam) :
		DecompApp(utilParam), m_classTag("OSDip-APP"), m_objective(NULL) {
		initializeApp(utilParam);
	}

	virtual ~OSDipApp() {
		std::cout << "INSIDE OSDip DECOMP DESTRUCTOR" << std::endl;
		std::vector<OSDipBlockSolver*>::iterator vit;
		UTIL_DELARR( m_objective);
		UTIL_DELPTR( m_modelC);
		UtilDeleteMapPtr( m_modelR);
		UtilDeleteMapPtr( m_modelMasterOnly);
	
		//garbage collection on the solvers
		for (vit = m_osDipBlockSolver.begin(); vit
				!= m_osDipBlockSolver.end(); vit++) {

				delete *vit;

		}
		//finally delete the factories
		
		delete factoryInit;
	}
	
	
	



};



#endif
