/* $Id: OSBlockSolver.cpp 3038 2009-11-07 11:43:44Z Gassmann $ */
/** @file OSBlockSolver.cpp
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
#include "OSDipBlockSolver.h"
#include "OSDipBlockSolverFactory.h"


OSDipBlockSolver::OSDipBlockSolver():
	m_cost(NULL),
	m_sparseVec( NULL),
	m_optVal( 0) {
}//end OSDipBlockSolver

OSDipBlockSolver::~OSDipBlockSolver(){

	//cout << "inside OSDipBlockSolver destructor" << endl;

}//end ~OSDipBlockSolver



