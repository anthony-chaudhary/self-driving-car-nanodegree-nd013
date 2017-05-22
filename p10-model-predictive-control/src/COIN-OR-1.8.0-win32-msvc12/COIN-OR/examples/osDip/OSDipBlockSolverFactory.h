/* $Id: OSDipBlockSolverFactory.h 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSDipBlockSolverFactory.h
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

#ifndef OSDIPBLOCKSOLVERFACTORY_H
#define OSDIPBLOCKSOLVERFACTORY_H

// --------------------------------------------------------------------- //
#include "OSInstance.h"
#include "OSOption.h"
#include "OSResult.h"
#include "OSDataStructures.h"
#include "OSErrorClass.h"
#include <vector>
#include <string>
#include <map>
#include "OSDipBlockSolver.h"

// --------------------------------------------------------------------- //
/*!
 * \class 
 * OSBDiplockSolverFactory is a virtual class for a solver factory
 * 
 *
 */
// --------------------------------------------------------------------- //



class OSDipBlockSolverFactory{
	
public:
	OSInstance *osinstance;
	OSOption *osoption;
	
	virtual  OSDipBlockSolver* create() = 0;
	static  std::map<std::string, OSDipBlockSolverFactory*> factories;
	static 	OSDipBlockSolver* createOSDipBlockSolver(const std::string &solverName) throw(ErrorClass);

	/**
	 *
	 * Default Constructor. 
	 */	
	OSDipBlockSolverFactory();
	
	/**
	 *
	 * Default destructor. 
	 */	
	virtual ~OSDipBlockSolverFactory() = 0;
	
};

#endif
