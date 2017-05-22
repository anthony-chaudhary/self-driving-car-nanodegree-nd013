#include "BcpsLicense.h"

class BcpsInterface {
   // *FIXME* : we'll need pointer to the master module class


   /**@name Informational methods for the user. */
   /*@{*/
      /** The best known upper bound (might be DBL_MAX) */
      double upper_bound() const;
      /** The phase the algorithm is in */
      int current_phase() const;
      /** The level of the search tree node being processed */
      int current_level() const;
      /** The internal index of the search tree node being processed */
      int current_index() const;
      /** The iteration count within the search tree node being processed */
      int current_iteration() const;
   /*@}*/


   /**@name Methods to get/set BCP parameters on the fly */
   /*@{*/
   /*@}*/

   /**@name Constructor, Destructor */
   /*@{*/
   /*@}*/

   //==========================================================================
   /** This method serves as hook for the user to do some preprocessing on a
       search tree node before the node is processed. Also, logical fixing
       results can be returned in the last four parameters. This might be very
       useful if the branching implies significant tightening.<br>
       Default: empty method. 
       @param vars       (IN) The variables in the current formulation 
       @param cuts       (IN) The cuts in the current formulation
       @param var_status (IN) The stati of the variables
       @param cut_status (IN) The stati of the cuts
       @param var_changed_pos (OUT) The positions of the variables whose
	                            bounds should be tightened
       @param var_new_bd      (OUT) The new lb/ub of those variables
       @param cut_changed_pos (OUT) The positions of the cuts whose bounds
       should be tightened
       @param cut_new_bd (OUT) The new lb/ub of those cuts
   */
   virtual void
   initialize_new_search_tree_node(const BcpsObject_p* objects,
				   const BCP_vec<BCP_cut*>& cuts,
				   const BCP_vec<BCP_obj_status>& var_status,
				   const BCP_vec<BCP_obj_status>& cut_status,
				   BCP_vec<int>& var_changed_pos,
				   BCP_vec<double>& var_new_bd,
				   BCP_vec<int>& cut_changed_pos,
				   BCP_vec<double>& cut_new_bd);
  /*@}*/
