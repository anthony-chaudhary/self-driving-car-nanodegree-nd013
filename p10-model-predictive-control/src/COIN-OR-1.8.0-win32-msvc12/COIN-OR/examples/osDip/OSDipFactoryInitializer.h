/* $Id: OSDipFactoryInitializer.cpp 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSDipFactoryInitializer.cpp
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
#include<map>
#include "OSDipBlockSolverFactory.h"
#include "OSDipBlockCoinSolver.h"
#include "OSDipBlockSlpSolver.h"
#include "OSDipBlockBearcatSolver.h"
//#include "MyBlockSolver.h"


#include <string>


class OSDipFactoryInitializer{
	
public:
	//the constructor 
	OSDipFactoryInitializer(){
		// user edit this, e.g
		//OSDipBlockSolverFactory::factories["MyBlockSolver"] = new MyBlockSolver::Factory;
		OSDipBlockSolverFactory::factories["OSDipBlockCoinSolver"] = new OSDipBlockCoinSolver::Factory;
		OSDipBlockSolverFactory::factories["OSDipBlockSlpSolver"] = new OSDipBlockSlpSolver::Factory;
		OSDipBlockSolverFactory::factories["OSDipBlockBearcatSolver"] = new OSDipBlockBearcatSolver::Factory;
	}
	
	
	//the destructor 
	~OSDipFactoryInitializer(){

		std::map<std::string, OSDipBlockSolverFactory*>::iterator mit ;
		for(mit = OSDipBlockSolverFactory::factories.begin(); 
			mit != OSDipBlockSolverFactory::factories.end(); mit++){
			delete mit->second;
			//std::cout << "DELETED A FACTORY " << std::endl;
		}		
	}//end destructor 
};

