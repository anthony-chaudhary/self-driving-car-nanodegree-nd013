/* $Id: OS_lp.hpp 2904 2009-09-06 06:47:42Z kmartin $ */
// Last edit: 2/10/05
//
// Name:     OS_lp.hpp
// Author:   Francois Margot
//           Tepper School of Business
//           Carnegie Mellon University, Pittsburgh, PA 15213
//           email: fmargot@andrew.cmu.edu
// Date:     12/28/03
//-----------------------------------------------------------------------------
// Copyright (C) 2003, Francois Margot, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef _OS_LP_H
#define _OS_LP_H


#include "OS.hpp"
#include "BCP_lp_user.hpp"
#include "BCP_buffer.hpp"
#include "BCP_parameters.hpp"
#include "OS_var.hpp"
#include "OS_user_data.hpp"
#include "OS_packer.hpp"

  /** Class for operations at the node level */

class OS_lp : public BCP_lp_user {

    OsiSolverInterface* cg_lp;
    //BCP_parameter_set<MCF1_par> par;
   // OS_user_data data;

  /// Pointer on the description of the problem
  OS_prob *os_prob;     

  /// Pointer on the description of the user data
  MY_user_data *p_ud;  
  
  /// in_strong = 1 if and only if in strong branching phase
  int in_strong;       

  /// Hold the value of EPSILON of class OS_prob
  double EPS;          

  /// Hold pointers on generated algorithmic cuts
  BCP_vec<BCP_cut*> algo_cuts;    
  
  /// Hold pointers on generated variables
  BCP_vec<BCP_var*> algo_vars;   
  

 bool generated_vars; 
 

 bool generated_cuts; 

  /// Hold indices of violated indexed cuts
  std::vector<int> violated_cuts; 

public:





  /**@name Constructors and destructors */
  //@{
  /// Default constructor 
  OS_lp(){}
  
  /// Destructor
  virtual ~OS_lp() {}
  //@}

  /**@name Packing and unpacking methods */
  //@{
  /// Unpack data sent from the tree manager
  virtual void unpack_module_data(BCP_buffer & buf);

  /// Pack algorithmic cuts
 // virtual void pack_cut_algo(const BCP_cut_algo* cut, BCP_buffer& buf);

  /// Unpack algorithmic cuts
//  virtual BCP_cut_algo* unpack_cut_algo(BCP_buffer& buf);
  
  
//	virtual void pack_var_algo(const BCP_var_algo* var, BCP_buffer& buf) {
		
//		OS_pack_var(var, buf);
//    }
	
 //   virtual BCP_var_algo* unpack_var_algo(BCP_buffer& buf) {
//			return OS_unpack_var(buf);
 //   } 

  /// Pack user data
  //virtual void pack_user_data(const BCP_user_data* ud, BCP_buffer& buf);

  /// Unpack user data
  //virtual MY_user_data* unpack_user_data(BCP_buffer& buf);
  //@}  

  // Tell Bcp which solver to use and set parameters of solver.
  /// Called only once at the beginning, from the root node
  virtual OsiSolverInterface *initialize_solver_interface();
  
  /// Initialize data members at the start of processing a new subproblem
  virtual void initialize_new_search_tree_node(
                                 const BCP_vec<BCP_var*>& vars,
				 const BCP_vec<BCP_cut*>& cuts,
				 const BCP_vec<BCP_obj_status>& var_status,
				 const BCP_vec<BCP_obj_status>& cut_status,
				 BCP_vec<int>& var_changed_pos,
				 BCP_vec<double>& var_new_bd,
				 BCP_vec<int>& cut_changed_pos,
				 BCP_vec<double>& cut_new_bd);
  
  /// Modify the parameters of the LP solver.
  /// Called at the beginning of the optimization of a node when
  /// the node LP is not yet solved.
  virtual void modify_lp_parameters(OsiSolverInterface* lp, 
				    bool in_strong_branching);
  
  /// Test feasibility of the LP solution.
  /// Called after each node LP has been solved.
  /// Called even if the node LP was infeasible
  /// Called also during strong branching
 // virtual BCP_solution* test_feasibility(const BCP_lp_result& lp_result,
//					 const BCP_vec<BCP_var*>& vars,
//					 const BCP_vec<BCP_cut*>& cuts);
  

  
  /// Cut generation. 
  /// Send to BCP the cuts generated in test_feasibility.
  /// Use this function to generate standard cuts (Knapsack covers,
  /// Lift-and-Project, odd holes, ...).


  //virtual void generate_cuts_in_lp(const BCP_lp_result& lpres,
//		      const BCP_vec<BCP_var*>& vars,
//		      const BCP_vec<BCP_cut*>& cuts,
//		      BCP_vec<BCP_cut*>& new_cuts,
//		      BCP_vec<BCP_row*>& new_rows);

  
 virtual  void display_lp_solution(const BCP_lp_result& lpres,
  				 const BCP_vec<BCP_var*>& vars,
  				 const BCP_vec<BCP_cut*>& cuts,
  				 const bool final_lp_solution);

  /// Simple rounding heuristic 
  virtual BCP_solution* generate_heuristic_solution(
                               const BCP_lp_result& lpres,
			       const BCP_vec<BCP_var*>& vars,
			       const BCP_vec<BCP_cut*>& cuts);

  /// Describes how to get a row of the matrix from the representation of the
  /// cut.
  /// Required method when indexed or algorithmic cuts are used.
  virtual void
  cuts_to_rows(const BCP_vec<BCP_var*>& vars, // on what to expand
	       BCP_vec<BCP_cut*>& cuts,       // what to expand
	       BCP_vec<BCP_row*>& rows,       // the expanded rows
	       const BCP_lp_result& lpres,
	       BCP_object_origin origin, bool allow_multiple);

  /// Called at the end of each iteration. Possible return values are:
  /// BCP_DoNotBranch_Fathomed : The node should be fathomed without branching;
  /// BCP_DoNotBranch :         BCP should continue to work on this node;
  /// BCP_DoBranch :            Branching must be done. In this case the 
  ///                           method returns the branching object candidates
  ///                           in one of the arguments.


  virtual BCP_branching_decision
  select_branching_candidates(const BCP_lp_result& lpres,
			      const BCP_vec<BCP_var*>& vars,
			      const BCP_vec<BCP_cut*>& cuts,
			      const BCP_lp_var_pool& local_var_pool,
			      const BCP_lp_cut_pool& local_cut_pool,
			      BCP_vec<BCP_lp_branching_object*>& cands,
			      bool force_branch = false);

  /// Set up the user data for the children according to the chosen
  /// branching object
  virtual void set_user_data_for_children(BCP_presolved_lp_brobj* best, 
					  const int selected);
					  



//	virtual double  compute_lower_bound(const double old_lower_bound,
//		const BCP_lp_result& lpres, const BCP_vec<BCP_var*>& vars,
//		const BCP_vec<BCP_cut*>& cuts);
			
			
//	virtual void generate_vars_in_lp(const BCP_lp_result& lpres,
//		const BCP_vec<BCP_var*>& vars, const BCP_vec<BCP_cut*>& cuts,
//		const bool before_fathom, BCP_vec<BCP_var*>& new_vars,
//		BCP_vec<BCP_col*>& new_cols);
			
			
 virtual void vars_to_cols(const BCP_vec<BCP_cut*>& cuts,
		 BCP_vec<BCP_var*>& vars, BCP_vec<BCP_col*>& cols,
		const BCP_lp_result& lpres,
		BCP_object_origin origin, bool allow_multiple);
    
    
    //===========================================================================
      /** Process the result of an iteration. This includes:
  	- computing a true lower bound on the subproblem. <br>
  	  In case column generation is done the lower bound for the subproblem
  	  might not be the same as the objective value of the current LP
  	  relaxation. Here the user has an option to return a true lower
  	  bound.
  	- test feasibility of the solution (or generate a heuristic solution)
  	- generating cuts and/or variables
  	.
  	The reason for the existence of this method is that (especially when
  	column generation is done) these tasks are so intertwined that it is
  	much easier to execute them in one method instead of in several
  	separate methods.

  	The default behavior is to do nothing and invoke the individual
  	methods one-by-one.

  	@param lp_result the result of the most recent LP optimization (IN)
  	@param vars      variables currently in the formulation (IN)
  	@param cuts      variables currently in the formulation (IN)
  	@param old_lower_bound the previously known best lower bound (IN)
          @param new_cuts  the vector of generated cuts (OUT)
          @param new_rows  the correspontding rows(OUT)
          @param new_vars      the vector of generated variables (OUT)
          @param new_cols the correspontding columns(OUT)
      */
    virtual void process_lp_result(const BCP_lp_result& lpres,
  		      const BCP_vec<BCP_var*>& vars,
  		      const BCP_vec<BCP_cut*>& cuts,
  		      const double old_lower_bound,
  		      double& true_lower_bound,
  		      BCP_solution*& sol,
  		      BCP_vec<BCP_cut*>& new_cuts,
  		      BCP_vec<BCP_row*>& new_rows,
  		      BCP_vec<BCP_var*>& new_vars,
  		      BCP_vec<BCP_col*>& new_cols);



};

#endif
