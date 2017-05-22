/* $Id: OSRemoteTest.cpp 3355 2010-03-28 08:06:32Z kmartin $ */
/** @file OSRemoteTest.cpp
 * 
 * @author  Horand Gassmann, Jun Ma, Kipp Martin, 
 *
 * \remarks
 * Copyright (C) 2008-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Dalhousie University, Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 */ 
 

#include <cppad/cppad.hpp> 
#include "OSConfig.h"
#include "OSCoinSolver.h"

#ifdef COIN_HAS_IPOPT  
	#ifndef COIN_HAS_ASL
		#include "OSIpoptSolver.h"
		#undef COIN_HAS_ASL
	#else
		#include "OSIpoptSolver.h"
	#endif
#endif

#include "OSResult.h" 
#include "OSiLReader.h"        
#include "OSiLWriter.h"   
#include "OSrLReader.h"          
#include "OSrLWriter.h"      
#include "OSInstance.h"  
#include "OSFileUtil.h"   
#include "OSDefaultSolver.h"  
#include "OSWSUtil.h" 
#include "OSSolverAgent.h"   
#include "OShL.h"     
#include "OSErrorClass.h"
#include "OSmps2osil.h"   
#include "OSBase64.h"
//#include "OSCommonUtil.h"
#include "OSErrorClass.h"
#include "OSMathUtil.h"

#include<iostream> 
using std::cout;   
using std::endl;

//int main(int argC, char* argV[]){
int main( ){
		std::string osil;

// test OS code samples here
	cout << "Start Building the Model" << endl;
	try{
		OSInstance *osinstance = new OSInstance();

		// put in some of the OSInstance <instanceHeader> information
		osinstance->setInstanceSource("From Anderson, Sweeney, Williams, and Martin");
		osinstance->setInstanceDescription("The Par Inc. Problem");
		//
		// now put in the OSInstance <instanceData> information
		// 
		// first the variables
		osinstance->setVariableNumber( 2);   
		//addVariable(int index, string name, double lowerBound, double upperBound, char type, double init, string initString);
		// we could use setVariables() and add all the variable with one method call -- below is easier
		osinstance->addVariable(0, "x0", 0, OSDBL_MAX, 'C');
		osinstance->addVariable(1, "x1", 0, OSDBL_MAX, 'C');
		//
		// now add the objective function
		osinstance->setObjectiveNumber( 1);
		// now the coefficient
		SparseVector *objcoeff = new SparseVector(2);   
		objcoeff->indexes[ 0] = 0;
		objcoeff->values[ 0] = 10;
		objcoeff->indexes[ 1] = 1;
		objcoeff->values[ 1] = 9;
		//bool addObjective(int index, string name, string maxOrMin, double constant, double weight, SparseVector* objectiveCoefficients);
		osinstance->addObjective(-1, "objfunction", "max", 0.0, 1.0, objcoeff);
		objcoeff->bDeleteArrays = true;
		delete objcoeff;
		//
		// now the constraints
		osinstance->setConstraintNumber( 4); 
		//bool addConstraint(int index, string name, double lowerBound, double upperBound, double constant);
		// note: we could use setConstraints() and add all the constraints with one method call -- below is easier
		osinstance->addConstraint(0, "row0", -OSDBL_MAX, 630, 0); 
		osinstance->addConstraint(1, "row1", -OSDBL_MAX, 600, 0);
		osinstance->addConstraint(2, "row2", -OSDBL_MAX, 708, 0);
		osinstance->addConstraint(3, "row3", -OSDBL_MAX, 135, 0); 
		/*
		now add the <linearConstraintCoefficients> --- row major
		*/
		double *values = new double[ 8];
		int *indexes = new int[ 8];
		int *starts = new int[ 5];  
		values[ 0] = .7;
		values[ 1] = 1;
		values[ 2] = .5;
		values[ 3] = 5./6.;
		values[ 4] = 1.0;
		values[ 5] = 2./3.;
		values[ 6] = .1;
		values[ 7] = .25;
		indexes[ 0] = 0;
		indexes[ 1] = 1;
		indexes[ 2] = 0;
		indexes[ 3] = 1;
		indexes[ 4] = 0;
		indexes[ 5] = 1;
		indexes[ 6] = 0;  
		indexes[ 7] = 1;
		starts[ 0] = 0;
		starts[ 1] = 2;
		starts[ 2] = 4;
		starts[ 3] = 6;
		starts[ 4] = 8;
		cout << "Call setLinearConstraintCoefficients" << endl;
		osinstance->setLinearConstraintCoefficients(8, false, values, 0, 7, 
			indexes, 0, 7, starts, 0, 4);
		cout << "End Building the Model" << endl; 

		// Write out the model
		OSiLWriter *osilwriter; 
		osilwriter = new OSiLWriter();
		osil = osilwriter->writeOSiL( osinstance);
		


		// now solve the model synchronously
		OSSolverAgent* osagent = NULL;
		osagent = new OSSolverAgent("http://74.94.100.129:8080/OSServer/services/OSSolverService");
		

		std::string osol = "<osol></osol>";
		cout << "Solve the model synchronously" << endl;
		cout << "osil input:" << endl << endl;
		cout << osil  << endl << endl;
		std::string osrl ="" ;
		osrl = osagent->solve(osil, osol);
		cout << "osrl result from osagent:" << endl << endl;
		cout << osrl  << endl << endl;

		// now do an asynchronous call
		// get the jobID first
		cout << "Now solve the problem in asynchronous mode" << endl;
		cout << "get the jobID" << endl;
		std::string jobID = osagent->getJobID("");
		cout << jobID << endl << endl;


		// build the osol 
		osol = "<osol><general><jobID>" + jobID + "</jobID></general></osol>";
		cout << "submit the problem" << endl;
		bool sent = osagent->send(osil, osol);
		cout << "problem submitted; success?" << sent << endl << endl;
		std::string ospl = "<ospl><processHeader><request action=\"getAll\"/></processHeader><processData/></ospl>";
		cout << "Enquire about job status" << endl << endl;
		std::string osql = osagent->knock(ospl, osol);
		cout << "Result of knock {} method:" << endl << osql  << endl << endl;

		// parse the result --- only look for 'status = "finished  "'
		cout << "Parse the result" << endl << endl;
		bool status_finished = false;
		bool status_found    = false;

		std::string::size_type pos1;
		std::string::size_type pos2;
		pos1 = osql.find( "<response" );
		if (pos1 != std::string::npos){
			pos2 = osql.find( ">", pos1 + 1);
			if (pos2 != std::string::npos){
				pos1 = osql.find( "status", pos1 + 1);
				if (pos1 != std::string::npos){
					pos1 = osql.find("=", pos1 + 1);
					if (pos1 != std::string::npos){
						status_found = true;
						pos1 = osql.find("finished", pos1 + 1);
						if (pos1 != std::string::npos){
							if (pos1 < pos2){
								status_finished = true;
							};
						}
						else{
							status_finished = false;
						};
					};
				};
			};
		};

		if (status_found){
			if (status_finished){
				cout << "process finished; retrieve results" << endl;
				osrl = osagent->retrieve(osol);
				cout << osrl  << endl << endl;
			}
				else {
				cout << "process still running; kill it!" << endl;
				osrl = osagent->kill(osol);
				cout << osrl  << endl << endl;
			};
		}
			else {
			cout << "Improperly formed string" << endl;
		};

		// do garbage collection
		delete osinstance;
		osinstance = NULL;
		delete osilwriter;
		osilwriter = NULL;
		delete osagent;
		osagent = NULL;

		cout << "Done with garbage collection" << endl;
		cout << "Program terminates normally" << endl;
		return 0;
		//
	}
	catch(const ErrorClass& eclass){

		std::cout << eclass.errormsg <<  std::endl;
		return 0;
	} 
}// end main

