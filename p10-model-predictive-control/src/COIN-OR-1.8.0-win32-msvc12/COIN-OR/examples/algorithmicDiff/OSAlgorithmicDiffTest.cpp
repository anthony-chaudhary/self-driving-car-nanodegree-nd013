/* $Id: OSAlgorithmicDiffTest.cpp 2698 2009-06-09 04:14:07Z kmartin $ */
/** @file OSAlgorithmicDiffTest.cpp
 * 
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2011, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, Dalhousie University and the University of Chicago.
 * All Rights Reserved. 
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 * This example illustrates the algorithmic differentiation features
 * of the OS library
 * 
 * The test problem CppADTestLag.osil:
 * 
 * min x0^2 + 9*x1   -- w[0]  <br />
 * s.t.  <br />
 * 33 - 105 + 1.37*x1 + 2*x3  + 5*x1 <= 10  -- y[0] <br />
 * ln(x0*x3) + 7*x2 >= 10  -- y[1] <br />
 * Note: in the first constraint 33 is a constant term and 105  <br />
 * is part of the nl node <br />
 * the Jacobian is: <br />
 * 
 * 2*x0   9       0      0  <br />
 * 0      6.37    0      2  <br />
 * 1/x0   0       7      1/x3  <br />
 * 
 * now set x0 = 1, x1 = 5,  x2 = 10, x3 = 5
 * 
 * the Jacobian is
 * 
 * 2   9       0    0 <br />
 * 0   6.37    0    2 <br />
 * 1   0       7   .2 <br />
 * 
 * Now form a Lagrangian with multipliers of w on the objective
 * z0 the multiplier on the first constraint and z1 on the second
 * the Lagrangian is then:
 * 
 * 	L = w*(x0^2 + 9*x1) + z0*(1 + 1.37*x1 + 2*x3 + 5*x1) + z1*(log(x0*x3) + 7*x2)
 * 
 * the partial with respect to x0 <br />
 * L_0 = 2 * w * x0  + z1 / x0
 * 
 * the partial with respect to x1 <br />
 * L_1 = w * 9 + z0*1.37 + z0*5
 * 
 * the partial with respect to x2 <br />
 * L_2 = z1*7 
 * 
 * the partial with respect to x3 <br />
 * L_3 = z0*2 + z1/x3 
 * 
 * in the Hessian there are only two nonzero terms 
 * however the code will think there may be three since it
 * does not know log(x0*x3) = log(x0) + log(x3)<br />
 * L_00 = 2 * w - z1 / ( x0 * x0 ) <br />
 * L_33 = - z1 / (x3 * x3)
 * 
 * <b>IMPORTANT:</b> the forwardAD and reverseAD calls ONLY apply
 * to the nonlinear part of the problem. The 9*x1 term
 * in the objective is not part of the AD
 * calculation nor are any terms in <linearConstraintCoefficients>
 * that <b>DO NOT</b> appear in any nl nodes, for example the 7*x2 term
 * in constraint with index 1. Note also, that there are only three
 * variables that appear in nl nodes, x2 does not appear in an nl node
 * 
 */


#include <cstddef>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <stack>
#include <cppad/cppad.hpp>
#include <iostream>

// CoinHelperFunctions <has cmath>
#include "CoinHelperFunctions.hpp"
#include "OSInstance.h"
#include "OSiLWriter.h"
#include "OSParameters.h"
#include "OSnLNode.h"
#include "OSErrorClass.h"
#include "OSFileUtil.h"  
#include "OSiLReader.h"
#include "OSInstance.h"
#include "OSExpressionTree.h"
#include "OSnLNode.h"
#include "OSDataStructures.h"



#include <vector>  
#include <map> 
#include <string>
 

int  main(){	
	WindowsErrorPopupBlocker();
	using std::cout;
	using std::endl;
	using CppAD::AD;
	//using CppAD::NearEqual;
	using CppAD::vector;
	std::cout.precision(12);
	// error checking functions
	bool CheckFunctionValues( double *conVals, double objValue,
		double x0, double x1, double x2, double x3, double y0, double y1, double z );
	bool CheckHessianUpper( SparseHessianMatrix *sparseHessian, 
		double x0, double x1, double x2, double x3, double y0, double y1, double z );
	bool CheckGradientValues( SparseJacobianMatrix *sparseJac, double *objGrad,
		double x0, double x1, double x2, double x3, double y0, double y1, double z );
	bool ok = true;
	int k, idx;
	//
	// get the problem data
	//
 	FileUtil *fileUtil = NULL; 
	std::string osilFileName;
	std::string osil;
	// get the input file
	const char dirsep =  CoinFindDirSeparator();
  	// Set directory containing mps data files.
  	std::string dataDir;
    dataDir = dirsep == '/' ? "../data/" : "..\\data\\";
	//osilFileName =  dataDir + "osilFiles" + dirsep + "HS071_NLP.osil";
	osilFileName =  dataDir   + "osilFiles" + dirsep + "CppADTestLag.osil";
	std::cout  << "osilFileName  =  " << osilFileName << std::endl;
	fileUtil = new FileUtil();
	osil = fileUtil->getFileAsString( &osilFileName[0]);	
	//
	// create OSReader and OSInstance objects
	OSiLReader *osilreader = NULL;
	OSInstance *osinstance = NULL;
	// create reader, parse the OSiL, and generate the OSInstance object
	try{	
		// a counter
		int kjl;
		osilreader = new OSiLReader();
		osinstance = osilreader->readOSiL( osil);
		std::vector<double> funVals(3);
		std::vector<double> dfunVals(6);
		double *conVals = NULL;
		//conVals = new double[ 2];
		double *objVals = NULL;
		//objVals = new double[ 1];

		/**
		 * 
		 * create function with domain the variables and range
		 * objective function plus constraint values
		 * IMPORTANT: the forwardAD and reverseAD calls ONLY apply
		 * to the nonlinear part of the problem. The 9*x1 term
		 * in the objective is not part of the AD calculation
		 * nor are any terms in <linearConstraintCoefficients>
		 * that DO NOT appear in any nl nodes, for example the 7*x2 term
		 * in constraint with index 1. Note also, that there are only three
		 * variables that appear in nl nodes; x2 does not
		 * 
		 */
		//
		// first initialize the nonlinear structures for call backs
		std::cout << "Initialize Nonlinear Structures" << std::endl;
		osinstance->initForAlgDiff( );
		//osinstance->getJacobianSparsityPattern( );


		
		/**
		 * get an index map of the nonlinear variables
		 * and see which variable are in <nonlinearExpressions>
		 * element
		 */	
		std::map<int, int> varIndexMap;
		std::map<int, int>::iterator posVarIndexMap;
		varIndexMap = osinstance->getAllNonlinearVariablesIndexMap( );
		/**
		 * iterate through and get an index of all variables that
		 * are in <nonlinearExpressions> element
		 */		
		for(posVarIndexMap = varIndexMap.begin(); posVarIndexMap != varIndexMap.end(); ++posVarIndexMap){
				std::cout <<  "Variable Index = "   << posVarIndexMap->first  << std::endl ;
		}
		std::cout << "Number of nonlinear variables =  " << varIndexMap.size() << std::endl;
		
		//
		
		// get the number of nonlinear terms
		
		int mm = osinstance->getNumberOfNonlinearExpressionTreeModIndexes();
		
		int jj;
		
		for(jj = 0; jj < mm; jj++){
			std::cout << osinstance->getNonlinearExpressionTreeModIndexes()[ jj] << std::endl;
		}
		
		std::cout << "Number of unique nonlinear terms =  " <<  mm << std::endl;
		//return 0;
		
		
		
		// domain space vector
		size_t n  = varIndexMap.size(); // three variables
		// range space vector
		size_t m = 3; // Lagrangian has an objective and two constraints

		std::vector<double> x0( n);
		/** Now start to calculate function values and derivatives
		 * there are three nonlinear variables and we put
		 * their values into the vector x0
		 */
		x0[0] = 1; // the value for variable x0
		x0[1] = 5; // the value for variable x1
		x0[2] = 5; // the value for variable x3				
		std::cout << "CALL forward" << std::endl;
		funVals = osinstance->forwardAD(0, x0);
		for( kjl = 0; kjl < 3; kjl++){
			std::cout << "forward 0 " << funVals[ kjl] << std::endl;
		}
		// get the third column of the Jacobian from a forward sweep
		std::vector<double> x1( n);
		x1[0] = 0;
		x1[1] = 0;
		x1[2] = 1;
		std::cout << "Now get the third column of the Jacobian forwardAD(1, x1)"  << std::endl;
		funVals = osinstance->forwardAD(1, x1);
		for( kjl = 0; kjl < 3; kjl++){
			std::cout << "forward 1 " << funVals[ kjl] << std::endl;
		}
		
		/** now use forward to get the mixed second partials with
		 * respect to variable x0 and x3
		 * of every function in the range space
		 * 
		 * Since we want the mixed partials of x0 and x3 we take
		 * the sum of the appropriate unit vectors and do a forward
		 * order 1 again
		 */
		x1[0] = 1;
		x1[1] = 0;
		x1[2] = 1;
		// recalculate the forward sweep with the new x1 vector
		funVals = osinstance->forwardAD(1, x1);
		std::vector<double> x2( n);
		x2[0] = 0;
		x2[1] = 0;
		x2[2] = 0;
		std::cout << "Now calcuate forwardAD(2, x2)"  << std::endl;
		funVals = osinstance->forwardAD(2, x2);
		for( kjl = 0; kjl < 3; kjl++){
			std::cout << "forward 2 " << funVals[ kjl] << std::endl;
		}		 
		
		/** function values cannot be calculated with the reverse sweep,
		 * in order to do a reverse sweep, a forward sweep with at least 
		 * p = 0 must be complete
		 * 
		 * in the example below we get the third row of the Jacobian
		 * using a reverse sweep.
		 */
		std::vector<double> vlambda(3);
		vlambda[0] = 0;
		vlambda[1] = 0;
		vlambda[2] = 1;
		// reverse sweep to get third row of Jacobian 
		std::cout << "Now get the third row of the Jacobian reverseAD(1, vlambda)"  << std::endl;
		osinstance->forwardAD(0, x0);
		funVals = osinstance->reverseAD(1, vlambda);
		for( kjl = 0; kjl < 3; kjl++){
			std::cout << "reverse 1 " << funVals[ kjl] << std::endl;
		}
		// now get the Hessian of the Lagrangian of objective and 
		// with the following multipliers
		vlambda[0] = 1;
		vlambda[1] = 2;
		vlambda[2] = 1;
		/** for everything below to work out we must go back and 
		 * do a forward sweep with respect to third unit vector,
		 * i.e. we do a  forwardAD(1, e^{3})
		 */
		 
		x1[0] = 0;
		x1[1] = 0;
		x1[2] = 1;
		funVals = osinstance->forwardAD(1, x1);		 
		 
		/**
		 * 
		 * reverseAD(2, vlambda) will produce a vector
		 * of  size  2*n, the terms 0, 2, 4,  2*n - 2 will be
		 * the first partials of the Lagrangian formed using vlambda
		 * the terms 1, 3, ..., 2*n - 1 will be column (row) row i
		 * of the Hessian of Lagrangian assuming we did a forwardAD(1, e^{i})
		 * where e^{i} is the i'th unit vector
		 * 
		 */		 
		dfunVals = osinstance->reverseAD(2, vlambda);
		// get the first partials of the Lagrangian
		std::cout << "Here are the first partials of the Lagrangain" << std::endl;
		for(int kjl = 0; kjl <= 4; kjl+=2){
			std::cout << dfunVals[ kjl] << std::endl;
		}
		/**
		 * get the third row (column) of the Lagrangian
		 * of the Hessian matrix, it the third row because
		 * we did a forwardAD(1, e^{3})
		 */
		std::cout << "Here is the third row (column) of Hessian of Lagrangian" << std::endl;
		for(int kjl = 1; kjl <= 5; kjl+=2){
			std::cout << dfunVals[ kjl] << std::endl;
		}
		/**
		 * most solver APIs work with pointers and not vectors so
		 * the OS API works with pointer here the values of the 
		 * primal and Lagrange multipliers that we use
		 */
		double* x = new double[4]; //primal variables
		double* z = new double[2]; //Lagrange multipliers on constraints
		double* w = new double[1]; //Lagrange multiplier on objective
		x[ 0] = 1;    // primal variable 0
		x[ 1] = 5;    // primal variable 1
		x[ 2] = 10;   // primal variable 2
		x[ 3] = 5;    // primal variable 3
		z[ 0] = 2;    // Lagrange multiplier on constraint 0
		z[ 1] = 1;    // Lagrange multiplier on constraint 1
		w[ 0] = 1;    // Lagrange multiplier on the objective function
		
		/**  
		 * first we show different calls to get constraint and objective 
		 * function values, many of this function calls are overloaded, 
		 * for example, to evaluate all of the constraints at the
		 * current value of x use
		 * 
		 * calculateAllConstraintFunctionValues(double* x, double *objLambda, 
		 * double *conLambda, bool new_x, int highestOrder)
		 * 
		 * if you want the functions values calculated using the algorithmic
		 * differentiation package. This may be desirable if you are also going 
		 * to calculate derivatives at the same time. However, use
		 * 
		 * calculateAllConstraintFunctionValues(double* x, bool new_x)
		 * 
		 * if you are going to do a lot of function evaluations without derivative
		 * calculations, this way you do not need to use operator overloading
		 * 
		 * Similar remarks apply to the objective functions
		 * 
		 * if you want to get a function value by index here that signature
		 * 
		 * double calculateFunctionValue(int idx, double* x, bool new_x);
		 * 
		 * for example, use calculateFunctionValue(-1, x,  true)
		 * to get the first objective function value
		 * 
		 */
		 
		osinstance->bUseExpTreeForFunEval = false;
		std::cout << "Calculate objective, idx = -1"  << std::endl;			
		std::cout << "obj value = " << osinstance->calculateFunctionValue(-1, x,  true) << std::endl;
		
		std::cout << "Calculate  first constraint, idx = 0"  << std::endl;			
		std::cout << "constraint index 0 value = " << osinstance->calculateFunctionValue(0, x,  true) << std::endl;
		 
		std::cout << "Now use calculateAllConstraintFunctionValues"  << std::endl;			
		conVals = osinstance->calculateAllConstraintFunctionValues(x, true);
		for( idx = 0; idx < osinstance->getConstraintNumber(); idx++){
			std::cout << "CONSTRAINT FUNCTION INDEX = " <<  idx << "  CONSTRAINT FUNCTION VALUE =  "  << *(conVals + idx) << std::endl;
		}
		//
		std::cout << "Now use calculateAllObjectiveFunctionValues"  << std::endl;	
		objVals = osinstance->calculateAllObjectiveFunctionValues( x, NULL, NULL, true, 0);
		for( idx = 0; idx < osinstance->getObjectiveNumber(); idx++){
			std::cout << "OBJECTIVE FUNCTION  INDEX = " << idx <<  "  OBJECTIVE FUNCTION VALUE = "  << *(objVals + idx) << std::endl;
		}
		ok = CheckFunctionValues( conVals, *objVals, x[ 0], x[1], x[2], x[3],  z[0], z[1], w[0] );
		if( ok == 0){
			std::cout << "FAILED CHECKING FUNCTION VALUES TEST" << std::endl;
			return 0;
		}
		else{
			std::cout << "PASSED CHECKING FUNCTION VALUES TEST" << std::endl;
		}
		
		/**  
		 * Most solvers that provide call backs want sparsity pattern. 
		 * Jacobian and Hessain sparsity patters are provided by the 
		 * OSInstance API
		 * 
		 */
		std::cout << "PLACE CALL TO JACOBIAN SPARSITY PATTERN"   << std::endl;
		SparseJacobianMatrix *sparseJac;
		sparseJac = osinstance->getJacobianSparsityPattern();		
		// print out just the sparsity pattern
		std::cout << "JACOBIAN SPARSITY PATTERN"   << std::endl;
		std::cout << "JACOBIAN START SIZE "   <<  sparseJac->startSize << std::endl;
		for(idx = 0; idx < osinstance->getConstraintNumber(); idx++){
			// some solvers (e.g. reduced gradient solvers) may want to know which values of the
			// Jacobian matrix are constant, i.e. linear, sparseJac->conVals is the number of constant
			// terms in the gradient for each rowt, the first conVals terms are constant, when getting
			std::cout << "number constant terms in constraint "   <<  idx << " is " 
			<< *(sparseJac->conVals + idx)  << std::endl;
			for(k = *(sparseJac->starts + idx); k < *(sparseJac->starts + idx + 1); k++){
				std::cout << "row idx = " << idx <<  "  col idx = "<< *(sparseJac->indexes + k) << std::endl;
			}
		}	
		
		SparseHessianMatrix *sparseHessian;
		// the Hessian test
		// get the sparsity pattern -- many solvers want to initialize with just the sparsity
		std::cout << "GET LAGRANGIAN HESSIAN SPARSITY PATTERN"   << std::endl;
		sparseHessian = osinstance->getLagrangianHessianSparsityPattern( );
		for(idx = 0; idx < sparseHessian->hessDimension; idx++){
			std::cout <<  "Row Index = " << *(sparseHessian->hessRowIdx + idx) ;
			std::cout <<  "  Column Index = " << *(sparseHessian->hessColIdx + idx) << std::endl;
		}		 
		
		/**  
		 * Most solvers that provide call backs want Jacobian calculation. 
		 * Constraint Jacobians and objective function gradients are 
		 * provided OSInstance API 
		 * the basic signature for these functions is 
		 * 
		 * calculateAllConstraintFunctionGradients(double* x, double *objLambda, 
		 *    double *conLambda, bool new_x, int highestOrder)
		 * 
		 * If we have already calculated function values using the AD routine, then
		 * highestOrder = 0, if we calculated function values using the OS expression 
		 * tree then highestOrder = -1. Then if new_x = false and highestOrder = 0
		 * and we make a gradient calcuation, then forwardAD(0, x) WILL NOT be called
		 * and there is a savings
		 */
		double *objGrad;
		std::cout << "OBJECTIVE FUNCTION GRADIENT"   << std::endl;
		// in our implementation the objective function is a dense gradient
		objGrad = osinstance->calculateObjectiveFunctionGradient( x, NULL, NULL,  -1, false, 1);
		for(idx = 0; idx < osinstance->getVariableNumber(); idx++){
			std::cout << "col idxx = " << idx << "  value =  " << *(objGrad + idx)  << std::endl;
		}
		std::cout << "CONSTRAINT JACOBIAN MATRIX"   << std::endl;
		// now make the gradient calculations and fill in the sparse Jacobian matrix
		sparseJac = osinstance->calculateAllConstraintFunctionGradients( x, NULL, NULL,  false, 1);
		for(idx = 0; idx < osinstance->getConstraintNumber(); idx++){
			for(k = *(sparseJac->starts + idx); k < *(sparseJac->starts + idx + 1); k++){
				std::cout << "row idx = " << idx <<  "  col idx = "<< *(sparseJac->indexes + k)
				<< " value = " << *(sparseJac->values + k) << std::endl;
			}
		}
		ok = CheckGradientValues( sparseJac, objGrad, x[ 0], x[1], x[2], x[3], z[0], z[1], w[0] );
		if( ok == 0){
			std::cout << "FAILED THE GRADIENT TEST" << std::endl;
			return 0;
		}
		else{
			std::cout << "PASSED THE GRADIENT TEST" << std::endl;
		}					  
		/**  
		 * Some solvers that provide call backs want the Hessian of 
		 * the Lagrangian function. This is provided by the OSInstance
		 * API
		 * 
		 * Since we have already made the gradient calcualations we set 
		 * new_x  = false. 
		 * 
		 */
		//first iteration 
		std::cout << "GET LAGRANGIAN HESSIAN FIRST TIME"   << std::endl;
		sparseHessian = osinstance->calculateLagrangianHessian( x, w,  z,  false, 2);
		for(idx = 0; idx < sparseHessian->hessDimension; idx++){
			std::cout << "row idx = " << *(sparseHessian->hessRowIdx + idx) <<  
			"  col idx = "<< *(sparseHessian->hessColIdx + idx)
			<< " value = " << *(sparseHessian->hessValues + idx) << std::endl;
		}
		ok = CheckHessianUpper( sparseHessian, x[0],  x[1], x[2],  x[3],  z[0], z[1], w[0]);
		if( ok == 0){
			std::cout << "FAILED THE FIRST HESSIAN TEST" << std::endl;
			return 0; 
		}
		else{
			std::cout << "PASSED THE FIRST HESSIAN TEST" << std::endl;
		}
		// now change an x value, we don't rebuild the tree, however new_x 
		// must be set to true
		x[0] = 5;
		
		
		
		std::cout << "NOW GET LAGRANGIAN HESSIAN SECOND TIME FOR x[0] = 5"   << std::endl;
		
		/** If the solver knows that at an iteration all of the derivative 
		 * information is required then calculating the function values, Jacobian, and
		 * Hessian at the same time is the most efficient thing to do. This is
		 * by setting highestOrder = 2. Regardless of whether the call is to calculate
		 * function values for object or constraint, Jacobians, or even Hessian, 
		 * setting highestOrder = 2 results in all derivative being calculated
		 */		
		osinstance->calculateAllObjectiveFunctionValues( x, w, z, true, 2);
		/** If we just wanted objective function values we could have used
		 * 
		 * osinstance->calculateAllObjectiveFunctionValues( x, w, z, true, 0)
		 * 
		 *     --OR --
		 * 
		 * calculateAllObjectiveFunctionValues(double* x, bool new_x)
		 * 
		 * however since we make a call with highestOrder = 2 all derivatives
		 * are calculated and no other calls are necessary 
		 * 
		 */
		
		for(idx = 0; idx < sparseHessian->hessDimension; idx++){
			std::cout << "row idx = " << *(sparseHessian->hessRowIdx + idx) <<  
			"  col idx = "<< *(sparseHessian->hessColIdx + idx)
			<< " value = " << *(sparseHessian->hessValues + idx) << std::endl;
		}
		ok = CheckHessianUpper( sparseHessian , x[0],  x[1], x[2], x[3],  z[0], z[1], w[0] );
		if( ok == 0){
			std::cout << "FAILED THE SECOND HESSIAN TEST" << std::endl;
			return 0;
		}
		else{
			std::cout << "PASSED THE SECOND HESSIAN TEST" << std::endl  << std::endl ;
		}
		/** Since we called calculateAllObjectiveFunctionValues() with
		 * highestOrder = 2 the first derivative information is available
		 */
		std::cout << "HERE IS ROW 1 OF JACOBIAN MATRIX" << std::endl; 
		idx = 1;
		for(k = *(sparseJac->starts + idx); k < *(sparseJac->starts + idx + 1); k++){
			std::cout << "row idx = " << idx <<  "  col idx = "<< *(sparseJac->indexes + k)
				<< " value = " << *(sparseJac->values + k) << std::endl;
		}
		std::cout << std::endl; 
		/** we can also calculate the Hessian of any row or objective function
		 * the function signature is
		 * SparseHessianMatrix *calculateHessian( double* x, int idx, bool new_x);
		 * Below we get the Hessian for row 1 , Note we set new_x to true since the Lagrange
		 * multipliers changed
		 */
		sparseHessian = osinstance->calculateHessian(x, 1, true);
		std::cout << "HERE IS ROW 1 HESSIAN MATRIX" << std::endl;
		for(idx = 0; idx < sparseHessian->hessDimension; idx++){
			std::cout << "row idx = " << *(sparseHessian->hessRowIdx + idx) <<  
			"  col idx = "<< *(sparseHessian->hessColIdx + idx)
			<< " value = " << *(sparseHessian->hessValues + idx) << std::endl;
		}
		//
		// adjust the Lagrange multipliers to correspond to finding Hessian of constraint 1
		z[ 0] = 0;  // Lagrange multiplier on constraint 0
		z[ 1] = 1;  // Lagrange multiplier on constraint 1
		w[ 0] = 0;  // Lagrange multiplier on the objective function
		ok = CheckHessianUpper( sparseHessian , x[0],  x[1], x[2], x[3], z[0], z[1], w[0] );
		if( ok == 0){
			std::cout << "FAILED THE THIRD HESSIAN TEST" << std::endl;
			return 0;
		}
		else{
			std::cout << "PASSED THE THIRD HESSIAN TEST" << std::endl  << std::endl ;
		}
		//set x[0] back to its original value of 1
		x[ 0] = 1;
		//return 0;	
		//
		//
		// now work directly with the CppAD package instead of OSInstance API
		//
		n = 4;
		m = 3;
		CppADvector< AD<double> > X(n);
		CppADvector< AD<double> > Y(m);
		X[0] = 5;
		X[1] = 5;
		X[2] = 0;
		X[3] = 1;
		// declare independent variables and start tape recording
		std::cout << "Start Taping" << std::endl;
		CppAD::Independent( X);
		// range space vector 
		// we include the constant terms in the CppAD functions
		Y[ 0] =  CppAD::pow(X[0], 2) + 9*X[1];
		Y[ 1] =  33 - 105 + 1.37*X[1] + 2*X[3] + 5*X[1] ;
		Y[ 2] =  log(X[0]*X[3]) + 7*X[2] ;
		// create f: x -> y and stop tape recording
		CppAD::ADFun<double> f(X, Y); 
		std::cout << "Stop Taping" << std::endl;
		// get function values
		std::vector<double> x_vec( n);
		x_vec[ 0] = x[ 0];
		x_vec[ 1] = x[ 1];
		x_vec[ 2] = x[ 2];
		x_vec[ 3] = x[ 3];
		funVals = f.Forward(0, x_vec);
		conVals[ 0] = funVals[ 1];
		std::cout << "conVals[ 0] = " << conVals[ 0] << std::endl;
		conVals[ 1] = funVals[ 2];
		std::cout << "conVals[ 1] = " << conVals[ 1] << std::endl;
		objVals[ 0] = funVals[ 0];
		std::cout << "objVals[ 0] = " << objVals[ 0] << std::endl;
		ok = CheckFunctionValues( conVals, funVals[ 0], x[ 0], x[1], x[2], x[3], z[0], z[1], w[0] );
		if( ok == 0){
			std::cout << "FAILED CHECKING FUNCTION VALUES TEST" << std::endl;
			return 0;
		}
		else{
			std::cout << "PASSED CHECKING FUNCTION VALUES TEST" << std::endl;
		}
		// now get gradient and Hessian
		// first define and initialze unit vector vector
		sparseJac = osinstance->getJacobianSparsityPattern();
		std::vector<double> unit_col_vec( n);
		std::vector<double> lagMultipliers( m);	
		std::vector<double> gradVals( m);
		lagMultipliers[ 0] = w[ 0];
		lagMultipliers[ 1] = z[ 0];
		lagMultipliers[ 2] = z[ 1];	
		unsigned int index, kj;
		//return 0;
		for(index = 0; index < n; index++){
			unit_col_vec[ index] = 0;
		}	
		for(index = 0; index < n; index++){
			unit_col_vec[ index] = 1;
			// calculate column i of the Jacobian matrix
			gradVals = f.Forward(1, unit_col_vec);
			unit_col_vec[ index] = 0;
			// get the nonzero gradient values in constraint k
			for(kj = 0; kj < m; kj++){
				std::cout << "variable " << index << "  row " << kj << "  gradient value" << std::endl;
				std::cout << "gradient value = " << gradVals[ kj] << std::endl;	
			}
			// get row i of the Lagrangian function!!!
			std::cout << "CALL f.Reverse -------" << std::endl;
			f.Reverse(2, lagMultipliers);
			std::cout << "FINISH CALL f.Reverse -------" << std::endl;
		}
		// done with CppAD test	
		// do garbage collection
		delete osilreader;
		osilreader = NULL;
		std::cout << "OSILREADER DELETED" << std::endl;	
		//delete[] conVals;
		//delete[] objVals;		
		delete[] x;
		delete[] z;
		delete[] w;
	}
	catch(const ErrorClass& eclass){
		std::cout << eclass.errormsg << std::endl;
	} 

	//
	{
		//checking CppAD power, another illustration of CppAD
		size_t n  = 2;
	 	double x0 = 4;
	 	double x1 = .5;
	   	CppADvector< AD<double> > x(n);
	    x[0]      = x0;
	    x[1]      = x1;
	     // declare independent variables and start tape recording
	     CppAD::Independent(x);
	     // range space vector 
	     size_t m = 1;
	     CppADvector< AD<double> > y(m);
	     y[0] = std::pow(x0, x1);
	     // create f: x -> y and stop tape recording
	     CppAD::ADFun<double> f(x, y); 
	     // check value 
	     double check = std::pow(x0, x1);
	     // forward computation of first partial w.r.t. x[0]
	     std::vector<double> dx(n);
	     std::vector<double> dy(m);
	     dx[0] = 4.;
	     dx[1] = 1/2.;
	     dy    = f.Forward(1, dx);
	     std::cout << "dy =  " <<  dy[ 0] << std::endl;
	     check = x1 * std::pow(x0, x1-1.);
	     //ok   &= NearEqual(dy[0], check, 1e-10, 1e-10);
	     ok = ( fabs(check - dy[0])/(fabs( check) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	}
	
	{
		
		//checking CppAD sparsity features
		// domain space vector
		size_t n = 3;
		CPPAD_TEST_VECTOR< AD<double> > X(n);
		X[0] = 0.;
		X[1] = 1.;
		X[2] = 2.;
		// declare independent variables and start recording
		CppAD::Independent(X);
		// range space vector
		size_t m = 2;
		CPPAD_TEST_VECTOR< AD<double> > Y(m);
		Y[0] = CppAD::pow(X[0], 2)  + CppAD::pow(X[2], 2);
		Y[1] = -CppAD::pow(X[0], 2) + CppAD::pow(X[1], 2);
		// create f: X -> Y and stop tape recording
		CppAD::ADFun<double> f(X, Y);
		
		// sparsity pattern for the identity matrix
		std::vector<bool> r(n * n);
		size_t i, j;
		for(i = 0; i < n; i++) { 
			for(j = 0; j < n; j++)
				r[ i * n + j ] = false;
				r[ i * n + i ] = true;
		}
		// compute sparsity pattern for J(x) = F^{(1)} (x)
		f.ForSparseJac(n, r);
		///
		///
		///
		//std::vector<bool> s(m * m);
		//for(i = 0; i < m; i++){    
		//	for(j = 0; j < m; j++)
		//		s[ i * m + j ] = false;
		//	s[ i * m + i ] = true;
	    // }
	    // sparsity pattern for F'(x)
	    // f.RevSparseJac(m, s);			
		///
		///
	    ///
		// compute sparsity pattern for H(x) = F_0^{(2)} (x)
		std::vector<bool> e( m);
		//Vector s(m);
		for(i = 0; i < m; i++)
		e[i] = false;
		e[ 0] = true;
		e[ 1] = false;
		std::vector<bool> h( n*n);
		//Vector h(n * n);
		std::cout << "Computing Sparse Hessian" << std::endl;
		h = f.RevSparseHes(n, e);
		for(i = 0; i < n; i++){
			std::cout << "Row " << i << "  of Hessian " << std::endl;
			for(j = 0; j < n; j++){
				std::cout << h[ i*n + j] <<  "  ";
			}
			std::cout << std::endl;
		}
	}
	delete fileUtil;
	std::cout << "\nTEST OF ALGORITHMIC DIFFERENTIATION CONCLUDED SUCCESSFULLY\n";
	return 0;
}// end main program

bool CheckFunctionValues( double *conVals, double objValue,
	double x0, double x1, double x2, double x3, double z0, double z1, double w ){
	using CppAD::NearEqual;
	bool ok  = true;
	double checkObj = x0*x0 + 9*x1;
	std::cout  << "checkObj = " << checkObj << std::endl;
	std::cout  << "objValue = " << objValue << std::endl;
	//ok &= NearEqual(objValue, checkObj, 1e-10, 1e-10); 
	ok = ( fabs(checkObj - objValue )/(fabs( checkObj) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	double checkCon0 = 33. - 105. + 1.37*x1 + 2*x3 + 5*x1;
	std::cout  << "checkCon0 = " << checkCon0 << std::endl;
	std::cout  << "conVals = " << *(conVals + 0) << std::endl;
	//ok &= NearEqual(*(conVals + 0), checkCon0, 1e-10, 1e-10);
	ok = ( fabs(checkCon0 - *(conVals + 0) )/(fabs( checkCon0) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	double checkCon1 = log(x0*x3) + 7*x2;
	std::cout  << "checkCon1 = " << checkCon1 << std::endl;
	std::cout  << "conVals = " << *(conVals + 1) << std::endl;
	//ok &= NearEqual( *(conVals + 1), checkCon1, 1e-10, 1e-10);
	ok = ( fabs(checkCon1 - *(conVals + 1) )/(fabs( checkCon1) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	return ok;
}//CheckFunctionValues
//
//
bool CheckGradientValues( SparseJacobianMatrix *sparseJac, double *objGrad,
	double x0, double x1, double x2, double x3, double y0, double y1, double w ){
	using CppAD::NearEqual;
	bool ok  = true;
	// first the objective function gradient
	double checkObjPartial0 = 2*x0;
	//ok &= NearEqual( *(objGrad + 0), checkObjPartial0, 1e-10, 1e-10); 
	ok = ( fabs(checkObjPartial0 - *(objGrad + 0) )/(fabs( checkObjPartial0) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	double checkObjPartial1 = 9;
	//ok &= NearEqual( *(objGrad + 1), checkObjPartial1, 1e-10, 1e-10); 
	ok = ( fabs(checkObjPartial1 - *(objGrad + 1) )/(fabs( checkObjPartial1) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	double checkObjPartial2 = 0;
	//ok &= NearEqual( *(objGrad + 2), checkObjPartial2, 1e-10, 1e-10); 
	ok = ( fabs(checkObjPartial2 - *(objGrad + 2) )/(fabs( checkObjPartial2) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	// get the constrating gradient
	// row 0 gradient -- there are nonzero partials for variables 1 and 2
	double checkCon0Partial1 = 1.37 + 5.0;
	//ok &= NearEqual( *(sparseJac->values + 0), checkCon0Partial1, 1e-10, 1e-10); 
	ok = ( fabs(checkCon0Partial1 - *(sparseJac->values + 0) )/(fabs( checkCon0Partial1) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	double checkCon0Partial3 = 2.;
	//ok &= NearEqual( *(sparseJac->values + 1), checkCon0Partial3, 1e-10, 1e-10); 
	ok = ( fabs(checkCon0Partial3 - *(sparseJac->values + 1) )/(fabs( checkCon0Partial3) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	// row 1 gradient -- there are nonzero partials for variables 0 and 2
	double checkCon1Partial2 = 7;
	//ok &= NearEqual( *(sparseJac->values + 2), checkCon1Partial2, 1e-10, 1e-10); 	
	ok = ( fabs(checkCon1Partial2 - *(sparseJac->values + 2) )/(fabs( checkCon1Partial2) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	double checkCon1Partial0 = 1./x0;
	//ok &= NearEqual( *(sparseJac->values + 3), checkCon1Partial0, 1e-10, 1e-10); 
	ok = ( fabs(checkCon1Partial0 - *(sparseJac->values + 3) )/(fabs( checkCon1Partial0) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	double checkCon1Partial3 = 1./x3;
	//ok &= NearEqual( *(sparseJac->values + 4), checkCon1Partial3, 1e-10, 1e-10); 
	ok = ( fabs(checkCon1Partial3 - *(sparseJac->values + 4) )/(fabs( checkCon1Partial3) + OS_NEAR_EQUAL) <= OS_NEAR_EQUAL) ? true : false;
	return ok;
}//CheckGradientValues
//
bool CheckHessianUpper( SparseHessianMatrix *sparseHessian , 
	double x0, double x1, double x2, double x3, double z0, double z1, double w ){
	using CppAD::NearEqual;
	bool ok  = true;
	int hessValuesIdx = 0;
	//assert( sparseHessian->hessDimension = n * (n + 1) /2)
	// L_00 = 2 * w - z1 / ( x0 * x0 )
	double check = 2. * w - z1 / (x0 * x0);
	ok &= NearEqual(*(sparseHessian->hessValues + hessValuesIdx++), check, 1e-10, 1e-10); 
	if(ok == false) std::cout << "FAILED ONE" << std::endl;
	ok &= NearEqual(*(sparseHessian->hessValues + hessValuesIdx++), 0., 1e-10, 1e-10);
	if(ok == false) std::cout << "FAILED TWO" << std::endl;
	// L_22 = - z1 / (x3 * x3)
	check = - z1 / (x3 * x3);
	ok &= NearEqual(*(sparseHessian->hessValues + hessValuesIdx++), check, 1e-10, 1e-1);
	if(ok == false) std::cout << "FAILED THREE" << std::endl;
	return ok;
}//CheckHessianUpper
