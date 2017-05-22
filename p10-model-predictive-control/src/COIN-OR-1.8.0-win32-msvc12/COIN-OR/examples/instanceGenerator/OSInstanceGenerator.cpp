/* $Id: OSInstanceGenerator.cpp 2698 2009-06-09 04:14:07Z kmartin $ */
/** @file OSInstanceGenerator.cpp
 * 
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin, 
 *
 * \remarks
 * Copyright (C) 2005-2011, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 * In this example we show how to use the <b>OSInstance</b> API
 * to generate an OSiL problem instance.
 * 
 * The objective of this example is to illustrate how to build a problem instance
 * using the OSInstance class API. In particular we illustrate a number of the set()
 * methods in the OSInstance class. As written, we need the LINDO nonlinear solver.
 *
 * <b>Model:</b> A nonlinear model with multiple local minimizers.
 * Taken from the Samples directory of the LINDO API
 *
 *           maximize  abs( x0 + 1) + .4 * x1;  <br />
 *           s.t.     x0           + x1       <= 4; <br />
 *                    x0 * x1      + x1       <= 6; <br />
 *                    x0 * x1                    <= 0; <br />
 *                    max(x1 + 1, x0)           >= 0; <br />
 *                    if(x1, 1, x1)              <= 0; <br />
 *                    (x1 * 2 * x1  -  x1) * x0  <= 0; <br />
 *                    -100  <=  x0  <=  100 <br />
 *                    x1 is binary <br />
 */

#include <iostream>
#include <vector>  

#include "CoinHelperFunctions.hpp"
#include "OSConfig.h" 
#include "OSInstance.h"
#include "OSiLWriter.h"
#include "OSParameters.h"
#include "OSnLNode.h"
#include "OSErrorClass.h"



 


 

using std::cout;
using std::endl;
int  main(){	
	WindowsErrorPopupBlocker();
	cout << "Start Building the Model" << endl;
	try{
		OSInstance *osinstance;
		osinstance = new OSInstance();
		//
		// put in some of the OSInstance <instanceHeader> information
		osinstance->setInstanceSource("An example from the LINDO API samples directory");
		osinstance->setInstanceDescription("A good example of a hard nonlinear program");
		//
		// now put in the OSInstance <instanceData> information
		// 
		// first the variables
		osinstance->setVariableNumber( 2);   
		//addVariable(int index, string name, double lowerBound, double upperBound, char type, double init, string initString);
		// we could use setVariables() and add all the variable with one method call -- below is easier
		osinstance->addVariable(0, "x0", -100, 100, 'C');
		osinstance->addVariable(1, "x1", 0, 1, 'B');
		//
		// now add the objective function
		osinstance->setObjectiveNumber( 1);
		// now the single coefficient in the linear part
		SparseVector *objcoeff;
		objcoeff = new SparseVector(1);   
		objcoeff->indexes[ 0] = 1;
		objcoeff->values[ 0] = .4;
		//bool addObjective(int index, string name, string maxOrMin, double constant, double weight, SparseVector* objectiveCoefficients);
		osinstance->addObjective(-1, "objfunction", "max", 0.0, 1.0, objcoeff);
		objcoeff->bDeleteArrays = true;
		delete objcoeff;
		//
		// now the constraints
		osinstance->setConstraintNumber( 6); 
		//bool addConstraint(int index, string name, double lowerBound, double upperBound, double constant);
		// note: we could use setConstraints() and add all the constraints with one method call -- below is easier
		osinstance->addConstraint(0, "row0", -OSDBL_MAX, 4, 0); 
		osinstance->addConstraint(1, "row1", -OSDBL_MAX, 6, 0);
		osinstance->addConstraint(2, "row2", -OSDBL_MAX, 0, 0);
		osinstance->addConstraint(3, "row3", 0 , OSDBL_MAX, 0); 
		osinstance->addConstraint(4, "row4", -OSDBL_MAX, 0, 0);
		osinstance->addConstraint(5, "row5", -OSDBL_MAX, 0, 0);
		//
		//
		// now add the <linearConstraintCoefficients>
		//bool setLinearConstraintCoefficients(int numberOfValues, bool isColumnMajor, 
		//double* values, int valuesBegin, int valuesEnd, 
		//int* indexes, int indexesBegin, int indexesEnd,   			
		//int* starts, int startsBegin, int startsEnd);	
		double *values = new double[ 3];
		int *indexes = new int[ 3];
		int *starts = new int[ 3];  
		values[ 0] = 1.0;
		values[ 1] = 1.0;
		values[ 2] = 1.0;
		indexes[ 0] = 0;
		indexes[ 1] = 0;
		indexes[ 2] = 1;
		starts[ 0] = 0;
		starts[ 1] = 1;
		starts[ 2] = 3; 
		osinstance->setLinearConstraintCoefficients(3, true, values, 0, 2, 
			indexes, 0, 2, starts, 0, 2);	
		//
		// finally the nonlinear part, not as nice since we don't have any set() methods
		// yet, we must work directly with the data structures
		//
		// we have 6 nonlinear expressions
		osinstance->instanceData->nonlinearExpressions->numberOfNonlinearExpressions = 6;
		osinstance->instanceData->nonlinearExpressions->nl = new Nl*[ 6 ];
		// define the vectors
		OSnLNode *nlNodePoint;
		OSnLNodeVariable *nlNodeVariablePoint;
		OSnLNodeNumber *nlNodeNumberPoint;
		OSnLNodeMax *nlNodeMaxPoint;
		std::vector<OSnLNode*> nlNodeVec;
		//
		//
		// the objective function nonlinear term abs( x0 + 1)
		osinstance->instanceData->nonlinearExpressions->nl[ 0] = new Nl();
		osinstance->instanceData->nonlinearExpressions->nl[ 0]->idx = -1;
		osinstance->instanceData->nonlinearExpressions->nl[ 0]->osExpressionTree = new OSExpressionTree();
		// create a variable nl node for x0
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=0;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for number 1
		nlNodeNumberPoint = new OSnLNodeNumber(); 
		nlNodeNumberPoint->value = 1.0;
		nlNodeVec.push_back( nlNodeNumberPoint);
		// create the nl node for +
		nlNodePoint = new OSnLNodePlus();
		nlNodeVec.push_back( nlNodePoint);
		// create the nl node for max
		nlNodePoint = new OSnLNodeAbs();
		nlNodeVec.push_back( nlNodePoint);
		// the vectors are in postfix format
		// now the expression tree
		osinstance->instanceData->nonlinearExpressions->nl[ 0]->osExpressionTree->m_treeRoot =
			nlNodeVec[ 0]->createExpressionTreeFromPostfix( nlNodeVec);
		nlNodeVec.clear();
		//
		//
		// constraint 0 has no nonlinear terms
		// generate the x0*x1 term in constraint 1
		//
		osinstance->instanceData->nonlinearExpressions->nl[ 1] = new Nl();
		osinstance->instanceData->nonlinearExpressions->nl[ 1]->idx = 1;
		osinstance->instanceData->nonlinearExpressions->nl[ 1]->osExpressionTree = new OSExpressionTree();
		// create a variable nl node for x0
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=0;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for x1
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for *
		nlNodePoint = new OSnLNodeTimes();
		nlNodeVec.push_back( nlNodePoint);
		// the vectors are in postfix format
		// now the expression tree
		osinstance->instanceData->nonlinearExpressions->nl[ 1]->osExpressionTree->m_treeRoot =
			nlNodeVec[ 0]->createExpressionTreeFromPostfix( nlNodeVec);
		nlNodeVec.clear();
		// 
		//
		// generate the x0*x1 term in constraint 2
		osinstance->instanceData->nonlinearExpressions->nl[ 2] = new Nl();
		osinstance->instanceData->nonlinearExpressions->nl[ 2]->idx = 2;
		osinstance->instanceData->nonlinearExpressions->nl[ 2]->osExpressionTree = new OSExpressionTree();
		// create a variable nl node for x0
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=0;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for x0
		nlNodeVariablePoint = new OSnLNodeVariable(); 
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for *
		nlNodePoint = new OSnLNodeTimes();
		nlNodeVec.push_back( nlNodePoint);
		// the vectors are in postfix format
		// now the expression tree
		osinstance->instanceData->nonlinearExpressions->nl[ 2]->osExpressionTree->m_treeRoot =
			nlNodeVec[ 0]->createExpressionTreeFromPostfix( nlNodeVec);
		nlNodeVec.clear();
		//
		//
		//
		// generate the max(x0 , x1 + 1) term in constraint 3
		osinstance->instanceData->nonlinearExpressions->nl[ 3] = new Nl();
		osinstance->instanceData->nonlinearExpressions->nl[ 3]->idx = 3;
		osinstance->instanceData->nonlinearExpressions->nl[ 3]->osExpressionTree = new OSExpressionTree();
		// create a variable nl node for x1
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for number 1
		nlNodeNumberPoint = new OSnLNodeNumber(); 
		nlNodeNumberPoint->value = 1.0;
		nlNodeVec.push_back( nlNodeNumberPoint);
		// create the nl node for +
		nlNodePoint = new OSnLNodePlus();
		nlNodeVec.push_back( nlNodePoint);
		// now push x0 to the stack
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=0;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for max
		nlNodeMaxPoint = new OSnLNodeMax();
		nlNodeMaxPoint->inumberOfChildren = 2;
		nlNodeMaxPoint->m_mChildren = new OSnLNode*[ nlNodeMaxPoint->inumberOfChildren];
		nlNodeVec.push_back( nlNodeMaxPoint);
		// the vectors are in postfix format
		// now the expression tree
		osinstance->instanceData->nonlinearExpressions->nl[ 3]->osExpressionTree->m_treeRoot =
			nlNodeVec[ 0]->createExpressionTreeFromPostfix( nlNodeVec);
		nlNodeVec.clear();
		//
		//
		//
		// generate the if(x1, 1, x1) term in constraint 4
		osinstance->instanceData->nonlinearExpressions->nl[ 4] = new Nl();
		osinstance->instanceData->nonlinearExpressions->nl[ 4]->idx = 4;
		osinstance->instanceData->nonlinearExpressions->nl[ 4]->osExpressionTree = new OSExpressionTree();
		// create a variable nl node for x1
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for number 1
		nlNodeNumberPoint = new OSnLNodeNumber(); 
		nlNodeNumberPoint->value = 1.0;
		nlNodeVec.push_back( nlNodeNumberPoint);
		// now push x1 to the stack
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for If
		nlNodePoint = new OSnLNodeIf();
		nlNodeVec.push_back( nlNodePoint);
		// the vectors are in postfix format
		// now the expression tree
		osinstance->instanceData->nonlinearExpressions->nl[ 4]->osExpressionTree->m_treeRoot =
			nlNodeVec[ 0]->createExpressionTreeFromPostfix( nlNodeVec);
		nlNodeVec.clear();
		//
		//
		//
		// generate the (x1 * 2 * x1  -  x1) * x0 term in constraint 5
		osinstance->instanceData->nonlinearExpressions->nl[ 5] = new Nl();
		osinstance->instanceData->nonlinearExpressions->nl[ 5]->idx = 5;
		osinstance->instanceData->nonlinearExpressions->nl[ 5]->osExpressionTree = new OSExpressionTree();
		// create a variable nl node for x1
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create the nl node for number 1
		nlNodeNumberPoint = new OSnLNodeNumber(); 
		nlNodeNumberPoint->value = 2.0;
		nlNodeVec.push_back( nlNodeNumberPoint);
		// create an nl node for *
		nlNodePoint = new OSnLNodeTimes(); 
		nlNodeVec.push_back( nlNodePoint);
		// now push x1 to the stack
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create an nl node for *
		nlNodePoint = new OSnLNodeTimes(); 
		nlNodeVec.push_back( nlNodePoint);
		// create a variable nl node for x1
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=1;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create an nl node for -
		nlNodePoint = new OSnLNodeMinus(); 
		nlNodeVec.push_back( nlNodePoint);
		// create a variable nl node for x0
		nlNodeVariablePoint = new OSnLNodeVariable();
		nlNodeVariablePoint->idx=0;
		nlNodeVec.push_back( nlNodeVariablePoint);
		// create an nl node for *
		nlNodePoint = new OSnLNodeTimes(); 
		nlNodeVec.push_back( nlNodePoint);
		// the vectors are in postfix format
		// now the expression tree
		osinstance->instanceData->nonlinearExpressions->nl[ 5]->osExpressionTree->m_treeRoot =
			nlNodeVec[ 0]->createExpressionTreeFromPostfix( nlNodeVec);
		nlNodeVec.clear();
		//
		//
		// 
		cout << "End Building the Model: Here is What you built" << endl; 
		// Write out the model
		OSiLWriter *osilwriter;
		osilwriter = new OSiLWriter();
		cout << osilwriter->writeOSiL( osinstance);
		std::cout << osinstance->printModel( ) << std::endl;
		// done writing the model
		
		cout << "Done writing the Model" << endl;
		delete osinstance;
		osinstance = NULL;
		delete osilwriter;
		osilwriter = NULL;
		cout << "Done with garbage collection" << endl;
		cout << "Program terminates normally" << endl;
		return 0;
	}
	catch(const ErrorClass& eclass){
		cout << eclass.errormsg <<  endl;
	}		
}

