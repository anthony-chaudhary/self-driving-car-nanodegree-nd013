/* $Id: OS_lp.cpp 2907 2009-09-06 07:26:05Z kmartin $ */
// Last edit: 6/23/06
//
// Name:     OS_lp.cpp
// Author:   Francois Margot
//           Tepper School of Business
//           Carnegie Mellon University, Pittsburgh, PA 15213
//           email: fmargot@andrew.cmu.edu
// Date:     12/28/03
//-----------------------------------------------------------------------------
// Copyright (C) 2003, Francois Margot, International Business Machines
// Corporation and others.  All Rights Reserved.

#undef HEUR_SOL

#include <vector>

#include "OS_lp.hpp"
#include "OS_cut.hpp"
#include "OS_var.hpp"
#include "OsiClpSolverInterface.hpp"
#include "CglSimpleRounding.hpp"
#include "CglKnapsackCover.hpp"

#include "BCP_math.hpp"
#include "BCP_lp.hpp"
#include "BCP_problem_core.hpp"
#include "BCP_lp_param.hpp"

using namespace std;

/************************************************************************/ 
void
OS_lp::unpack_module_data(BCP_buffer& buf)
{
  buf.unpack( os_prob);
  EPS = os_prob->EPSILON; 
  /**
   * we also have access to the osinstance object that was created in OS_tm
   * the OS_tm tree manager sent us a pointer to this -- the osinstance is
   * actually a memeber of OS_prob -- put in this class information we want access to
   */
  
  // just a simple test
  
  std::cout << "number of variables " << os_prob->osinstance->getVariableNumber() << std::endl;


  
}



/************************************************************************/
OsiSolverInterface * OS_lp::initialize_solver_interface(){

	OsiClpSolverInterface * clp = new OsiClpSolverInterface;
	clp->messageHandler()->setLogLevel(0);
	return clp;
}

/************************************************************************/
void OS_lp::initialize_new_search_tree_node(const BCP_vec<BCP_var*>& vars,
				       const BCP_vec<BCP_cut*>& cuts,
				       const BCP_vec<BCP_obj_status>& vstatus,
				       const BCP_vec<BCP_obj_status>& cstatus,
				       BCP_vec<int>& var_changed_pos,
				       BCP_vec<double>& var_new_bd,
				       BCP_vec<int>& cut_changed_pos,
				       BCP_vec<double>& cut_new_bd)

  // Called at the beginning of the optimization of a node. 
  // The node LP is not yet solved.

{
  in_strong = 0;
  
  	//BCP_lp_param.hpp for a list of parameters
  	//lets get a parameter -- should be true by default
  	bool relSol = BCP_lp_user::get_param(BCP_lp_par::LpVerb_RelaxedSolution) ;
  	std::cout  << relSol << std::endl;
  	
  	// now reset the value
  	if(relSol == true){
  		BCP_lp_user::set_param( BCP_lp_par::LpVerb_RelaxedSolution, false);
  	}else{
  		BCP_lp_user::set_param( BCP_lp_par::LpVerb_RelaxedSolution, true);
  	}
  	
  	// see if it got reset
  	relSol = BCP_lp_user::get_param(BCP_lp_par::LpVerb_RelaxedSolution) ;
  	std::cout  <<  relSol << std::endl;
  	

  	//set another way
  	// number of cuts to keep in the pool
	//this->getLpProblemPointer()->par.set_entry(BCP_lp_par::IneffectiveBeforeDelete, 10000); 
  	
  	// turn off strong branching
  	//this->getLpProblemPointer()->par.set_entry(BCP_lp_par::MaxPresolveIter, -1);
  	

#ifdef USER_DATA
  MY_user_data *curr_ud = dynamic_cast<MY_user_data*> (get_user_data());
  curr_ud->is_processed = 1;
#endif

}

/************************************************************************/
void OS_lp::modify_lp_parameters(OsiSolverInterface* lp, bool in_strong_branching){

  // Called each time the node LP is solved
	
	
	/****
	 * 
	 *  BCP_lp_param.hpp for param doc. 
	 *  BCP_lp_param.cpp for default values.
	 *  to change a param in your user class: getLpProblemPointer()->par.set_entry(LpVerb_RelaxedSolution, false);
	 *  to change a param in bb.par:  BCP_LpVerb_RelaxedSolution 0
	 * 
	 */
	
	

  // if (in_strong_branching) {
  //   in_strong = 1;
  //   lp->setIntParam(OsiMaxNumIterationHotStart, 50);
  // }

   // write the current LP in file lpnode.lp
   //lp->writeLp("lpnode", "lp");
   //cout << "LP node written in file lpnode.lp" << endl;

   // to write the current LP file in file lpnode.mps use the following:
   // lp->writeMps("lpnode", "mps", 0.0);
   // cout << "LP node written in file lpnode.mps" << endl;

}



/************************************************************************/
BCP_solution* OS_lp::generate_heuristic_solution(const BCP_lp_result& lpres,
			    const BCP_vec<BCP_var*>& vars,
			    const BCP_vec<BCP_cut*>& cuts)  {
	//not implemented
	return NULL;
}


/********************************************************************/
BCP_branching_decision OS_lp::select_branching_candidates(const BCP_lp_result& lpres,
                                 const BCP_vec<BCP_var*>& vars,
                                   const BCP_vec<BCP_cut*>& cuts,
                                   const BCP_lp_var_pool& local_var_pool,
                                   const BCP_lp_cut_pool& local_cut_pool,
                                   BCP_vec<BCP_lp_branching_object*>& cands,
                                   bool force_branch) {
	
	
	  // Called at the end of each iteration. Possible return values are:
	  // BCP_DoNotBranch_Fathomed : The node should be fathomed without branching
	  // BCP_DoNotBranch :          BCP should continue to work on this node
	  // BCP_DoBranch :             Branching must be done. In this case the 
	  //                            method returns the branching object candidates 
	  //                            in one of the arguments	

	std::cout << "INSIDE BRANCHING DECISION: TESTING VARIABLE TYPES " << std::endl;
	std::cout << "SIZE OF VARS  =  " <<  vars.size()  << std::endl;
	std::cout << "SIZE OF LOCAL VAR POOL  =  " << local_var_pool.size()   << std::endl;
	std::cout << "CANDS SIZE  =  " << cands.size()   << std::endl;

	
	const double intTol = BCP_lp_user::get_param(BCP_lp_par::IntegerTolerance) ;
	//const double intTol = 1e-5;
	int num_vars = vars.size();
	
	//for(int i = 0; i < num_vars; i++){
	//	std::cout << "lower = " << vars[ i]->lb() << std::endl;
	//	std::cout << "upper = " << vars[ i]->ub() << std::endl;
		
	//}

	// if no colums were generated use default branching 
	//if(local_var_pool.size() == 0) return BCP_lp_user::select_branching_candidates( lpres,
	//	vars, cuts, local_var_pool, local_cut_pool, cands, force_branch);
	

	// Don't branch if cutting planes have been generated

	if( local_cut_pool.size() > 0 ) {
    
		cout << "select_branching_candidates() returns BCP_DoNotBranch"  << endl;
    
		return(BCP_DoNotBranch);
  }
  else {
    // Branch on the first fractional non-continous variable
    int i;
    const double *x = lpres.x();
    BCP_vec<int> select_pos;

    for(i = 0; i < num_vars; i++) {  
    	
    	//std::cout << "TESTING VARIABLE  " << i  << "  FOR BRANCHING" << std::endl;
    	//std::cout << "VALUE =   " << x[i]  << std::endl;
    	//std::cout << "x[i] -floor( x[i])  =   " << x[i] -floor( x[i])  << std::endl;
    	//std::cout << "x[i] -floor( x[i])  =   " << ceil( x[i]) - x[i]  << std::endl;
    
    	if( vars[ i]->var_type() < 2)	{
    		if((x[i] -floor( x[i])  > intTol) && (ceil( x[i]) - x[i] > intTol)) {
    			select_pos.push_back(i);

    			// put in cands all variables whose index are in select_pos

    			append_branching_vars( lpres.x(), vars, select_pos, cands);

    			cout << "Branching on variable: " << i << " (" << x[i] << ")   depth: " << current_level() << endl;
    			break;
    		}
    	}
    }
  
    if (cands.size() == 0 && local_var_pool.size() == 0) {
    	throw BCP_fatal_error("select_banching_candidate() : No cut in pool but couldn't select branching variable.\n");
    	//return BCP_DoNotBranch_Fathomed;
    }
    
    if (cands.size() == 0 && local_var_pool.size() > 0) {// none of the current variables are fractional so don't branch
    	
    	return BCP_DoNotBranch;
    }

    cout << "select_branching_candidates() returns BCP_DoBranch" << endl;

    return BCP_DoBranch;
  }

}


/**************************************************************************/
void OS_lp::set_user_data_for_children(BCP_presolved_lp_brobj* best, 
                                  const int selected)

  // Given the branching decision (parameter "best"; "selected" is the
  // index of the chosen branching decision in the candidate list), 
  // set the user data for the children.

{
#ifdef USER_DATA
  BCP_lp_branching_object *cand = best->candidate();
  MY_user_data *curr_ud = dynamic_cast<MY_user_data*> (get_user_data());
  real_user_data *curr_rud = curr_ud->p_rud;

  for(int i=0; i<cand->child_num; i++) {
    MY_user_data *ud = new MY_user_data(curr_rud->max_card_set_zero);
    real_user_data *rud = ud->p_rud;

    rud->card_set_zero = curr_rud->card_set_zero;

    for(int j=0; j<curr_rud->card_set_zero; j++) {
      rud->set_zero[j] = curr_rud->set_zero[j];
    }

    int ind_br = (*(cand->forced_var_pos))[0];

    if((*(cand->forced_var_bd))[2*i + 1] < EPS) {
      rud->set_zero[curr_rud->card_set_zero] = ind_br;
      (rud->card_set_zero)++;
    }
    best->user_data()[i] = ud;
  }
#endif /* USER_DATA */
} /* set_user_data_for_children */





/************************************************************************/


void  OS_lp::cuts_to_rows(const BCP_vec<BCP_var*>& vars, // on what to expand
                    BCP_vec<BCP_cut*>& cuts,       // what to expand
                    BCP_vec<BCP_row*>& rows,       // the expanded rows
                    // things that the user can use for lifting cuts if allowed
                    const BCP_lp_result& lpres,
                    BCP_object_origin origin, bool allow_multiple) {

  // Required function when indexed or algorithmic cuts are used.
  // Describes how to get a row of the matrix from the representation of the
  // cut.
	std::cout << "Execute cuts_to_rows" << std::endl;
	const int cutnum = cuts.size();
	for (int i=0; i<cutnum; ++i) {
		const OsiRowCut* bcut = dynamic_cast<const OS_cut*>(cuts[i]);
		if (bcut) {
    
			rows.push_back(new BCP_row(bcut->row(), bcut->lb(), bcut->ub()));
			continue;
		}
		throw BCP_fatal_error("Unknown cut type in cuts_to_rows.\n");
	}
}

/************************************************************************/

void OS_lp::vars_to_cols(const BCP_vec<BCP_cut*>& cuts,
		     BCP_vec<BCP_var*>& vars,
		     BCP_vec<BCP_col*>& cols,
		     const BCP_lp_result& lpres,
		     BCP_object_origin origin, bool allow_multiple)
{

	std::cout << "EXECUTE vars_to_cols  **************" << std::endl;
    static const CoinPackedVector emptyVector(false);
    const int numvars = vars.size();
    int i;
    for (i = 0; i < numvars; ++i) {
	const OS_var* v = dynamic_cast<const OS_var*>(vars[i]);
	    // Since we do not generate cuts, we can just disregard the "cuts"
	    // argument, since the column corresponding to the var is exactly
	    // the flow (plus the entry in the appropriate convexity
	    // constraint)
	    BCP_col* col = new BCP_col(v->coinPackedVec, v->weight, 0.0, 1.0);
	    //col->insert(data.numarcs + v->commodity, 1.0);
	    
	    cols.push_back( col);
	    // Excercise: if we had generated cuts, then the coefficients for
	    // those rows would be appended to the end of each column
    }
}


/************************************************************************/
void OS_lp::process_lp_result(const BCP_lp_result& lpres,
		      const BCP_vec<BCP_var*>& vars,
		      const BCP_vec<BCP_cut*>& cuts,
		      const double old_lower_bound,
		      double& true_lower_bound,
		      BCP_solution*& sol,
		      BCP_vec<BCP_cut*>& new_cuts,
		      BCP_vec<BCP_row*>& new_rows,
		      BCP_vec<BCP_var*>& new_vars,
		      BCP_vec<BCP_col*>& new_cols){
	
	
	generated_cuts = false;
	generated_vars = false;
	

#if 1
	
	{/// Cgl cut addition scope

		OsiClpSolverInterface* my_lp_solver;
		
		my_lp_solver =  dynamic_cast<OsiClpSolverInterface *>(getLpProblemPointer()->lp_solver);
		
	
	    //try a simple knapsack cut
	    //CglSimpleRounding rounding;
		CglKnapsackCover  knapCover;
	
	    //CoinRelFltEq eq(0.0001);
	    OsiSolverInterface::ApplyCutsReturnCode acRc;
	    OsiCuts my_cuts;
	
		//my_lp_solver =  (getLpProblemPointer()->lp_solver);
	    double epsilon = .1;
	    int i, k;

		
		// generate the knapsack constraints
	    knapCover.generateCuts(*my_lp_solver, my_cuts);  
	    const double *x = lpres.x();
	    
		CoinPackedVector pv;
		int *pvIndexes = NULL;
		double *pvElements = NULL;
		int numElem;
		double lhs = 0;
    	OS_cut* cut;
	    int ncuts = my_cuts.sizeRowCuts();
	    const OsiRowCut **newRowCuts = new const OsiRowCut * [ncuts];
	    std::cout <<  "sizeRowCuts() =    " << ncuts  << std::endl;
	    for(i = 0; i < ncuts; i++) {
	      newRowCuts[i] = &my_cuts.rowCut(i); 
	      cut = new OS_cut( *newRowCuts[i]);
	      // do not add cut if not violated  -- kipp add a test
	      //cout   <<" rhs = " <<  newRowCuts[i]->rhs()  << endl;
	      cout   <<" sense = " <<  newRowCuts[i]->sense()  << endl;
	      pv = newRowCuts[i]->row();
	      pvIndexes = pv.getIndices();
	      pvElements =  pv.getElements();
	      numElem = pv.getNumElements();

	      lhs = 0;
	      for(k = 0; k < numElem; k++){
	    	  lhs += pvElements[ k]*x[ pvIndexes[k] ];
	    	  //std::cout << pvElements[ k] << std::endl;
	      }
	     
	      if( (newRowCuts[i]->sense() == 'L')  && (lhs > newRowCuts[i]->rhs() +epsilon )  ){
	    	  cout   <<" lhs - rhs = " <<  lhs -  newRowCuts[i]->rhs() << endl;
	    	  std::cout <<  "PUSHING BACK KNAPSACK COVER ROUNDING CUT"    << std::endl;
	    	  algo_cuts.push_back( cut);  
	      }
	    }	
		    
		generated_cuts = (algo_cuts.size() > 0);
		for(i = 0; i < ncuts; i++){
			//delete newRowCuts[ i];
		}
		delete[] newRowCuts;
	}// end Cgl cut generation scope
	
#endif
	
#if 0
	
	{/// user cut addition scope
		int i;
		int num_vars = vars.size();
		const double *x = lpres.x();
		OsiRowCut* rcut = new OsiRowCut();
		// the cut column indexes
		int *cutind = new int[ num_vars];
		//  number of nonzeros in the cut
		int cut_nz;
	
		//we are just playing around here -- say the sum of the rhs cannot exceed 503.77
		double  cutrhs = 503.77;
		//cutrhs = 7000;
		// the cut coefficients
		double *cutcoef = new double[ num_vars];
		double lhs = 0;
		for(i=0; i < num_vars; i++) {
			cutcoef[ i] = 1;
			cutind[ i] = i;
			cut_nz = num_vars;
			lhs += x[ i];
		}
		std::cout  <<  "The LHS IS EQUAL TO ===   " << lhs << std::endl;
		
	
	    if(lhs > cutrhs + EPS) {
	
	    // cut is violated -- add it
	    	rcut->setLb(-BCP_DBL_MAX);
	    	rcut->setUb( cutrhs);
	    	rcut->setRow( cut_nz, cutind, cutcoef);
	    	OS_cut* cut = new OS_cut( *rcut);
	    	algo_cuts.push_back( cut);
	    	std::cout  <<  "WE ARE ADDING A CUT!!!!!!!!   "   << std::endl;
	    }  
		delete rcut;
		delete[] cutind;
		delete[] cutcoef;
		generated_cuts = (algo_cuts.size() > 0);
	}// end user cut generation scope
	
#endif	
	
#if 0
	{ // variable generation scope
		const double* pi = lpres.pi();
		int numNonz = 4;
		int varIdx = vars.size();
	    int* ind = new int[ numNonz];
	    double* val = new double[ numNonz];
		double objVal = -13.0;
		double reducedCost  = 0.0;
		int i;
		
		ind[ 0] = 0;
		ind[ 1] = 1;
		ind[ 2] = 2;
		ind[ 3] = 3;
			
		val[ 0] = 1.0;
		val[ 1] = 1.0;
		val[ 2] = 1.0;
		val[ 3] = 1.0;
		
		for(i = 0 ; i < numNonz; i++){
			reducedCost += -pi[ i]*val[ i];
		}
		reducedCost += objVal;
		std::cout << "REDUCED COST =   **************   "  << reducedCost << std::endl;
		
		if(reducedCost < -.01  && vars.size() < 3) {
			std::cout << "CALL OS_var CONSTRUCTOR   "    << std::endl;
			algo_vars.push_back(new OS_var(varIdx, CoinPackedVector(numNonz, ind, val, true), objVal));	
			std::cout << "DONE WITH CALL OS_var CONSTRUCTOR   "    << std::endl;
		}
		
		delete[] val;
	    delete[] ind;
	
		
	    generated_vars = (algo_vars.size() > 0);
	}// end variable generation scope

#endif
    
    
    //end of generated variables
  
    
	// if no cuts or vars were found  call the old method   
    if( (generated_vars == false) && (generated_cuts == false) ){
    	BCP_lp_user::process_lp_result(lpres, vars, cuts, 
    			old_lower_bound, true_lower_bound, sol, new_cuts,
    			new_rows, new_vars, new_cols);    	
    	return;
    }
    
    int i;
    // add the cuts
    
	if (generated_cuts) {
	    new_cuts.append( algo_cuts);
	    
	    // now convert the generated cuts into rows
	    
	    int cutnum = algo_cuts.size();
	  
	    for (i = 0; i < cutnum; ++i) {
	        
	        
	        const OsiRowCut* bcut = dynamic_cast<const OS_cut*>(algo_cuts[i]);
	       
	        if (bcut) {
	        	new_rows.push_back(new BCP_row(bcut->row(), bcut->lb(), bcut->ub()));
	        	//delete algo_cuts[ i];
	        }
	        else{
	        	throw BCP_fatal_error("Unknown cut type in cuts_to_rows.\n");
	        }
	    }
	    
	    
	    //delete algo_cuts[0];
	    algo_cuts.clear();  
	}
    
    // add the vars
    
    
	if (generated_vars) {
	    new_vars.append( algo_vars);
	    
	    
	    
	    //convert the generated variables to columns
	    
	    static const CoinPackedVector emptyVector(false);
	    const int numvars = algo_vars.size();
	    for (i = 0; i < numvars; ++i) {
		const OS_var* v = dynamic_cast<const OS_var*>(algo_vars[i]);
		    // Since we do not generate cuts, we can just disregard the "cuts"
		    // argument, since the column corresponding to the var is exactly
		    // the flow (plus the entry in the appropriate convexity
		    // constraint)
		    BCP_col* col = new BCP_col(v->coinPackedVec, v->weight, 0.0, 1.0);
		    //col->insert(data.numarcs + v->commodity, 1.0);
		    
		    new_cols.push_back( col);
		    // Excercise: if we had generated cuts, then the coefficients for
		    // those rows would be appended to the end of each column
	    }
	    
	     
	    algo_vars.clear();
	
		std::cout << "GENERATED VARS IS TRUE **************   "  << std::endl;
		true_lower_bound = old_lower_bound;
    } else {
    	std::cout << "GENERATED VARS IS FALSE **************   "  << std::endl;
    	true_lower_bound = lpres.objval();
    }
	std::cout << "TRUE LOWER BOUND **************   "  << true_lower_bound << std::endl;
}



/************************************************************************/


	void  OS_lp::display_lp_solution(const BCP_lp_result& lpres,
 				 const BCP_vec<BCP_var*>& vars,
 				 const BCP_vec<BCP_cut*>& cuts,
 				 const bool final_lp_solution){
		
	
		BCP_lp_user::display_lp_solution( lpres, vars, cuts,
		 				  final_lp_solution);
		return;
	
		bool relSol = false;
		unsigned int i;
		double ietol;
		ietol = BCP_lp_user::get_param(BCP_lp_par::IntegerTolerance) ;
		relSol = BCP_lp_user::get_param(BCP_lp_par::LpVerb_RelaxedSolution) ;
	
		const double *x = lpres.x();
		if( relSol == true){
			for (i = 0; i < vars.size(); ++i) {
				if(x[ i] > ietol) std::cout << x[ i] << std::endl;
			}
		}	

// kipp write OSrL here?
// if final_lp_solution is true print out some osrl.
}



