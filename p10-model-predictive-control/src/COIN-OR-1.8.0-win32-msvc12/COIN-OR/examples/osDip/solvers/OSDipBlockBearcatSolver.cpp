/* $Id: OSBlockBearcatSolver.cpp 3038 2009-11-07 11:43:44Z kmartin $ */
/** @file OSBlockBearcatSolver.cpp
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
#include "OSDipBlockBearcatSolver.h"
#include "OSErrorClass.h" 
#include "OSDataStructures.h"

#include <sstream>
using std::ostringstream;




OSDipBlockBearcatSolver::OSDipBlockBearcatSolver():
	m_osinstance(NULL) {
	std::cout << "INSIDE OSDipBlockBearcatSolver CONSTRUCTOR" << std::endl;
}//end OSDipBlockBearcatSolver


OSDipBlockBearcatSolver::OSDipBlockBearcatSolver( OSInstance *osinstance,  OSOption *osoption) {
	  std::cout << "INSIDE OSDipBlockBearcatSolver CONSTRUCTOR" << std::endl;
	  std::cout << "whichBlock = " << m_whichBlock<< std::endl;
	try{
		m_osinstance = osinstance;
		m_numberOfVar = m_osinstance->getVariableNumber();
		
		m_osoption = osoption;
		
		m_demand = NULL;
		m_u = NULL;
		m_v = NULL;
		m_g = NULL;
		m_px = NULL;
		m_tx =NULL;
		m_varIdx = NULL;
		
		//now get data
		std::vector<SolverOption*> solverOptions;
		std::vector<SolverOption*>::iterator vit;
		std::vector<int >demand;

		
		solverOptions = m_osoption->getSolverOptions("Dip");
		//iterate over the vector
		
		int tmpVal;

		for (vit = solverOptions.begin(); vit != solverOptions.end(); vit++) {
			
			
			//std::cout << (*vit)->name << std::endl;
			
			//(*vit)->name.compare("initialCol") == 0
			//if(rowNames[ i3].find("routeCapacity(1)") == string::npos )
			
			if( (*vit)->name.find("numHubs") !=  std::string::npos){
				
				std::istringstream buffer( (*vit)->value);
				buffer >> m_numHubs;
				std::cout << "numHubs = " << m_numHubs <<  std::endl;
				
			}else{
				
				if((*vit)->name.find("numNodes") !=  std::string::npos){
					
					std::istringstream buffer( (*vit)->value);
					buffer >> m_numNodes;
					std::cout << "numNodes = " <<  m_numNodes <<  std::endl;
					
				}else{
					if((*vit)->name.find("totalDemand") !=  std::string::npos){
						
						std::istringstream buffer( (*vit)->value);
						buffer >> m_totalDemand;
						std::cout << "m_totalDemand = " << m_totalDemand <<  std::endl;
						
					}else{
						if((*vit)->name.find("minDemand") !=  std::string::npos){
							
							std::istringstream buffer( (*vit)->value);
							buffer >> m_minDemand;
							std::cout << "m_minDemand = " << m_minDemand <<  std::endl;
						
						}else{
							if( (*vit)->name.find("demand") !=  std::string::npos ){
								
								std::istringstream buffer( (*vit)->value);
								buffer >> tmpVal;
								demand.push_back( tmpVal);
								//std::cout << "demand = " << tmpVal <<  std::endl;
								
							}
						}
					}
				}
			}//end if on solver options
			
		}//end for loop on options
		
		//now fill in demand
		m_demand = new int[ m_numNodes];
		
		m_varIdx = new int[ m_numNodes];
		
		m_u = new double*[ m_numNodes];
		m_v = new double*[ m_numNodes];
		m_g = new double*[ m_numNodes];
		
		m_px = new int*[ m_numNodes];
		m_tx = new int*[ m_numNodes];
		
		if(m_numNodes != (int)demand.size( ) ) throw ErrorClass("inconsistent number of demand nodes");
		int i;
		for (i = 0; i < m_numNodes; i++) {
			
			m_demand[ i] = demand[i];
			m_u[ i] = new double[ m_totalDemand + 1];
			m_v[ i] = new double[ m_totalDemand + 1];
			m_g[ i] = new double[ m_numNodes];
			
			m_px[ i] = new int[ m_totalDemand + 1];
			m_tx[ i] = new int[ m_totalDemand + 1];
			
			
		}
		
		
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	

}//end OSDipBlockBearcatSolver

OSDipBlockBearcatSolver::~OSDipBlockBearcatSolver(){
	
	std::cout << "INSIDE ~OSDipBlockBearcatSolver()" << std::endl;


	std::vector<IndexValuePair*>::iterator  vit;
	
	for (vit = m_primalVals.begin(); vit != m_primalVals.end(); vit++) {
		
		delete *vit;
	}
	
	//delete data structures for arrays used in calculating minimum reduced cost
	int i;
	
	for(i = 0; i < m_numNodes; i++){

	    delete[] m_u[i];
		m_u[i] = NULL;
		
	    delete[] m_v[i];
		m_v[i] = NULL;
		
	    delete[] m_g[i];
		m_g[i] = NULL;
		
	    delete[] m_px[i];
		m_px[i] = NULL;
		
	    delete[] m_tx[i];
		m_tx[i] = NULL;
	}
	
	delete[] m_u;
	m_u= NULL;

	delete[] m_v;
	m_v= NULL;
	
	delete[] m_g;
	m_g= NULL;
	
	delete[] m_px;
	m_px= NULL;
	
	delete[] m_tx;
	m_tx= NULL;
	
	
	
	if(m_demand != NULL) delete[] m_demand;
	
	if(m_varIdx != NULL) delete[] m_varIdx;
	//if(m_osrlreader != NULL) delete m_osrlreader;
}//end ~OSDipBlockBearcatSolver

void OSDipBlockBearcatSolver::solve(double *cost, std::vector<IndexValuePair*> *solIndexValPair, double *optVal){

	try{
		
		int i;
		
		
		
		struct IndexValuePair *primalValPair;

		std::vector<IndexValuePair*>::iterator  vit;
		
		for (vit = m_primalVals.begin(); vit != m_primalVals.end(); vit++) {
			
			delete *vit;
		}
		m_primalVals.clear();
		
			
		//now, lets find the minimum reduced cost
		
		*optVal = OSDBL_MAX;
		
		//std::cout << "Number of Var " << m_numberOfVar  << std::endl;
	
		int l ;
		
		int bestl;
		int kountVar;
		double testVal;
		
		for(l = m_minDemand; l <= m_totalDemand; l++){
			
			testVal = qrouteCost(m_whichBlock,  l,  cost,  &kountVar);
			if( testVal < *optVal){
				*optVal = testVal;
				bestl = l;
			}
			
		
			
		}
		
		*optVal = qrouteCost(m_whichBlock,  bestl,  cost,  &kountVar);
		std::cout <<   "best reduced cost = " << *optVal << std::endl;
		
		std::map<int, int> indexCount;
		
		std::map<int, int>::iterator mit;
		
		
		for(i = 0; i < kountVar; i++){
			
			if( indexCount.find(  m_varIdx[ i]) == indexCount.end()  ){
				
				indexCount[ m_varIdx[ i]] = 1;
				
			}else{
				
				indexCount[ m_varIdx[ i]] += 1;
				
			}
				
				
		}
		
		for (mit = indexCount.begin(); mit != indexCount.end(); mit++){
			
			//std::cout << "Variable Index " << mit->first << " Count =  " << mit->second << std::endl;
			
			//std::cout << "Variable Name = " << m_osinstance->getVariableNames()[   mit->first]  << std::endl;
			
			// get unique indexes
			primalValPair = new IndexValuePair();
			
			primalValPair->value =  mit->second;
			primalValPair->idx =  mit->first;
	
			
			m_primalVals.push_back( primalValPair);
		}
		
		/*** old way could lead to duplicate indexes
		for (i = 0; i < kountVar; i++){
			
			//std::cout << "Variable Number = " << m_varIdx[ i] << std::endl;
			std::cout << "Variable Name = " << m_osinstance->getVariableNames()[  m_varIdx[ i]]  << std::endl;
			
			// get unique indexes
			primalValPair = new IndexValuePair();
			
			primalValPair->value = 1.0;
			primalValPair->idx = m_varIdx[ i];
	
			
			m_primalVals.push_back( primalValPair);
			
			
		}
	*/
		
		 *solIndexValPair = m_primalVals;

			
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	
	
}//end solve


void OSDipBlockBearcatSolver::solve(double *cost, std::string *osrl){

	
	try{
		//not implemented
			
	} catch (const ErrorClass& eclass) {

		throw ErrorClass(eclass.errormsg);

	}	
	
}//end solve



double OSDipBlockBearcatSolver::qrouteCost(const int& k, const int& l, double* c, int* kountVar){
	
	//critical -- nodes 0, ..., m_numNodes - 1 are the hub nodes
	// we are doing the calculation for hub k, k <= m_numNodes - 1
	//l  should be the total demand on the network -- we are NOT using 0 based counting
	double rcost;
	rcost = OSDBL_MAX;

	//start of the cost vector for hub k plus move to start of l demands
	// now move the pointer up
	//int startPnt = k*totalDemand*(m_numNodes*m_numNodes - m_numNodes) + (l - 1)*(m_numNodes*m_numNodes - m_numNodes);
	
	int startPnt = (l - 1)*(m_numNodes*m_numNodes - m_numNodes);
	c+=  startPnt ;

	*kountVar = 0;
	int bestLastNode = 0;
	int i;
	int j;
	int l1;
	int l2;
	//for(i = 0; i < 20; i++){
	//	std::cout << "i =  " << i  <<  " c[i] = " << *(c + i) << std::endl ;
	//}



	// l  is the total demand on this network
	//address of node (j, i) is j*(m_numNodes-1) + i when i < j
	//address of node (j, i) is j*(m_numNodes-1) + i - 1 when i > j
	
	//
	// initialize
	
	for(i = m_numHubs; i < m_numNodes; i++){
		
		
		for(l1 = m_minDemand; l1 <= l; l1++){  //l-1  is total demand on network
			
			m_u[i][l1] = OSDBL_MAX;
			m_v[i][l1] = OSDBL_MAX;
			
			if(l1 == *(m_demand + i) ){
				
				m_px[i][l1] = k;
				// want the cost for arc (k, i)
				// note: k < i so use that formula
				m_u[i][l1] = *(c + k*(m_numNodes - 1) + i - 1);  // put in correct cost

				
			}
		}	
	}
	//end initialize
	//
	
	//if l = minDemand we visit only one node, let's get the reduced cost in this case
	if(l == m_minDemand){
		
		for(i = m_numHubs; i < m_numNodes; i++){
			
			
			if(  m_u[i][l] + *(c + i*(m_numNodes-1) + k )  < rcost){
							
				rcost = m_u[i][l] + *(c + i*(m_numNodes-1) + k );
				
				//std::cout << " m_u[i][l2] = "  << m_u[i][l2] << std::endl;
				
				//std::cout << " *(c + i*(m_numNodes-1) + k ) = "  << *(c + i*(m_numNodes-1) + k ) << std::endl;
				//push node back
				bestLastNode = i;
			}
			
		}
		
		//go from node bestLastNode to node k		
		//node bestLastNode is a higher number than k
		*(m_varIdx + (*kountVar)++) = startPnt + bestLastNode*(m_numNodes - 1)  +  k ;
		*(m_varIdx + (*kountVar)++) = startPnt + k*(m_numNodes - 1)  + bestLastNode - 1;
		

		return rcost;	
	}//end if on l == minDemand
	
// now calculate values for demand 2 or greater 	
	//address of node (j, i) is j*(m_numNodes-1) + i when i < j
	//address of node (j, i) is j*(m_numNodes-1) + i - 1 when i > j
	// we start l2 at 2 since demand must be at least 1
	// change to min demand + 1
	int lowerVal = m_minDemand + 1;
	for(l2 = lowerVal; l2 <= l; l2++){// loop over possible demand values assuming we have already gone to at least one node
			
		for(i = m_numHubs; i < m_numNodes; i++) { //we are finding least cost to node i
			
			
			if( *(m_demand + i) < l2 ){ // kipp < OR <= ????
					
				for(j = m_numHubs; j < i; j++){ //we are coming from node j
					

						
					//calculate g  -- l2 - d[ i]  is the demand trough and including node j
					l1 = l2 - *(m_demand + i);
					
					if( m_px[j][ l1 ] != i ){//check to make sure we did not come into j from i
						
						
						m_g[j][i] = m_u[ j][ l1 ] + *(c + j*(m_numNodes-1) + i - 1) ;
						
						
						
						
					}else{
												
						m_g[j][i] = m_v[ j][ l1] + *(c + j*(m_numNodes-1) + i - 1) ;
						
						
						
					}
					
					// update u and the pointer for p
					
					if(m_g[j][i] < m_u[i][l2] ){
						
						m_u[i][l2] = m_g[j][i];
						m_px[i][l2] =  j;
						
					}


					
				}//end of first for loop on j, j < i
				
				
				for(j = i + 1; j < m_numNodes; j++){ //we are coming from node j
					
		
					//calculate g  -- l2 - d[ i]  is the demand trough and including node j
					l1 = l2 - *(m_demand + i);
					
					if( m_px[j][ l1 ] != i ){//check to make sure we did not come into j from i
						
						
						m_g[j][i] = m_u[ j][ l1 ] + *(c + j*(m_numNodes-1) + i ) ;
						
						
					}else{
												
						m_g[j][i] = m_v[ j][ l1] + *(c + j*(m_numNodes-1) + i ) ;
						
					}
					
					// update u and the pointer for p
					
					if(m_g[j][i] < m_u[i][l2] ){
						
						m_u[i][l2] = m_g[j][i];
						m_px[i][l2] =  j;
						
					}

					
				}//end of second for loop on j, j > i				
				
				
				//now calculate the second best solution and point
				//right now m_px[i][l2] points to the best j node
				
				for(j =m_numHubs; j < m_numNodes; j++){ //we are coming from node j
					
					if(j != i){
						
						if( (m_g[j][i] < m_v[i][l2] ) && (m_px[i][l2] != j)  ){ // kipp the && gives the second best
							
							//if(g(j, i) < v(i, l2),
							
							m_v[i][l2] = m_g[j][i];
							m_tx[i][l2] = j;	
							
							
						}
						
					}
					
					
				}//end second best calculation, anothe for loop on j
				
				//now if l2 = l  we are done
				if(l2 == l ){
					
					if(  m_u[i][l2] + *(c + i*(m_numNodes-1) + k )  < rcost){
						
						rcost = m_u[i][l2] + *(c + i*(m_numNodes-1) + k );

						bestLastNode = i;
					}
					
				}
				
				
			}//end if on demand less than l2
			
			
		}//i loop
		
		
	}//l2 loop

	
	//std::cout << "best Last Node = "  << bestLastNode << std::endl;
	
	// now get the path that gives the reduced cost
	
	int currentNode;
	int successorNode;
	int lvalue;
	
	//initialize
	// we work backwords from bestLastNode
	//in our recursion we recurse on the currentNode and assume
	//it is in the optimal path
	
	//node bestLastNode is a higher number than k
	*(m_varIdx + (*kountVar)++) = startPnt + bestLastNode*(m_numNodes - 1)  +  k ;
	
	
	

	//by successor, I mean node after current node in the path
	successorNode = k;
	currentNode = bestLastNode;
	//the lvalue is the demand through the currentNode
	lvalue = l ;

	
	while(currentNode != k){
		
		
		if( m_px[ currentNode][ lvalue ] != successorNode){
			

			
			//update nodes
			successorNode = currentNode;
			currentNode = m_px[ currentNode][ lvalue ];
			
			
			if(currentNode - successorNode > 0){
				 //below diagonal
				*(m_varIdx  + (*kountVar)++) = startPnt + currentNode*(m_numNodes - 1)  +  successorNode;
				
			}else{
				 //above diagonal
				*(m_varIdx + (*kountVar)++) = startPnt + currentNode*(m_numNodes - 1)  +  successorNode  - 1 ;
				
			}
 
			
		}else{ //take second best
			
			
			//update nodes
			successorNode = currentNode;
			currentNode = m_tx[ currentNode][ lvalue ];
			
			if(currentNode - successorNode > 0){
				 //below diagonal
				*(m_varIdx  + (*kountVar)++) = startPnt + currentNode*(m_numNodes - 1)  +  successorNode;
				
			}else{
				 //above diagonal
				*(m_varIdx + (*kountVar)++) = startPnt + currentNode*(m_numNodes - 1)  +  successorNode  - 1 ;
				
			}
			
		}
		
		//update lvalue
		lvalue = lvalue - *(m_demand + successorNode);
	
		

	}

	
	//go from node k to bestLastNode -- already done in loop above
	//*(m_varIdx + (*kountVar)++) = startPnt + k*(m_numNodes - 1)  +  currentNode - 1;
	
	
	

	return rcost;
	
}//end qroute

