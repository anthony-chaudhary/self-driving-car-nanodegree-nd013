/*
  This file is a part of the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

#ifndef _DYLP_H
#define _DYLP_H

/*
  @(#)dylp.h	4.6	10/15/05
  svn/cvs: $Id: dylp.h 407 2010-12-31 20:48:48Z lou $

  This file contains definitions related to dylp, a subroutine library which
  implements a dynamic (primal-dual) linear programming algorithm based on
  the algorithm described by Padberg in Linear Optimisation & Extensions,
  Springer-Verlag, 1995.  dylp also owes a debt to previous and contemporary
  codes the author has worked with --- la05, bandbx, zoom/xmp, ylp, and glpk.

  At minimum, dylp requires only a constraint system. Since it manages a
  dynamically sized private copy of the constraint system while solving the
  LP, there's no point in having the client attach logical variables (they'd
  just get in the way, really).

  dylp will accept a basis specification. This takes the form of a status
  vector giving the status of all variables, and a basis vector specifying
  the active constraints and their associated basic variables. From this
  dylp will construct an initial active problem which consists of exactly
  the given constraints and basic variables, with the logicals for the
  constraints making up the nonbasic partition.

  dylp returns as a solution the simplex termination code, the objective
  value (or related value, appropriate for the termination code), status for
  all variables, the active constraints, and the associated primal and dual
  variables (put a little differently, a basis, the values of the basic
  variables, and the dual variables associated with the active constraints).

  The conditional compilation symbol DYLP_INTERNAL is used to delimit
  definitions that should be considered internal to dylp. Don't define this
  symbol in a client.
*/

#include "dylib_errs.h"
#include "dylib_io.h"
#include "dy_consys.h"

/*
  A few words on notation. Traditional matrix and vector notation for LP
  suffers a bit when limited to ascii text, but it's readable if you're
  familiar with the original. The notation in the comments and code is
  loosely based on Chvatal, "Linear Programming", W.H. Freeman, 1983, which
  the author happens to like.

  A matrix is represented with a capital letter, e.g., B. A vector is
  represented with a small letter, e.g., x. A subscript is given in angle
  brackets, e.g., x<j> for the jth coefficient of x. An individual element of
  a matrix has two subscripts, e.g., a<i,j> for the element in row i, column
  j. Column and row vectors are shown with one subscript, e.g., a<j> for the
  jth column (or row).  Whether the vector is supposed to be a column or a
  row should generally be clear from context. A capital letter in the
  subscript position indicates a set of elements, e.g., x<N> is the non-basic
  variables.

  The inverse of a matrix is denoted inv(*), e.g., the basis inverse,
  inv(B).  The dot product of two vectors is denoted dot(*,*), e.g.,
  dot(c,x), or sometimes just written directly, e.g., cx.

  The system of constraints is assumed to be Ax <= b, with m rows and n
  columns. Once the logical variables (aka slacks and artificials) have been
  added, it becomes Ax = b.  A is the constraint matrix, x is the vector of
  primal variables, and b is the right-hand-side (rhs).  NOTE that the
  convention for indices is NOT the usual one. Logical variables are assigned
  indices 1..m and architectural variables are assigned indices m+1..m+n. It
  makes for more efficient addition/deletion of variables; see dy_consys.h for a
  little more explanation.

  There is an objective function z = cx, where z is the objective value and c
  is the vector of coefficients.  dylp minimises the objective.
  
  The matrix A is partitioned into the set of basic columns B, and the set of
  non-basic columns N (sometimes A<N>). The corresponding partitions of c and
  x are c<B>, x<B>, and c<N>, x<N>.

  Premultiplication by the basis inverse (e.g., inv(B)a<j>) is referred to as
  an `ftran'; postmultiplication (e.g., c<B>inv(B)) as a `btran'.  Quantities
  that have been transformed using the basis inverse are often (but not
  always) renamed as 'barred' quantities.

  The basic primal variables are x<B> = inv(B)b.  The dual variables are y =
  c<B>inv(B).  The jth column of A, premultiplied by inv(B), is abar<j> =
  inv(B)a<j>. The reduced costs are cbar = c<N> - c<B>inv(B)N = c<N>-yN.

  The variable i is used as a row index, j as a column index. Often they will
  represent the entering primal variable (j) and the leaving primal variable
  (i). Be aware that comments are often written as if the leaving primal
  variable x<i> occupies row i of the basis. This simplifies the writing.
  But keep in mind that variable x<i> can occupy an arbitrary row k of the
  basis.
*/

/*
  Termination codes for dy_primal and dy_dual, the top level routines of the
  dylp simplex algorithms. Also used by various internal routines.  Codes
  marked with (*) will never be returned to the client unless dylp has
  failed.

  lpINV         The code is not valid (i.e., not set by an execution of
		dy_primal or dy_dual).

  lpOPTIMAL	The problem has an optimal solution.

  lpUNBOUNDED	The problem is unbounded.

  lpSWING(*)	The problem is pseudo-unbounded: Some primal variable grew
		excessively in a single pivot.

  lpINFEAS	The problem is infeasible.

  lpACCCHK	An accuracy check failed and dylp's internal recovery
		algorithms could not recover the situation.

  lpSTALLED	The problem has been abandoned due to stalling. (We could
		in fact actually be cycling, but that's too much trouble
		to prove.)

  lpITERLIM	The problem has been abandoned because it has exceeded an
		absolute iteration limit.

  lpNOSPACE     The problem has been abandoned because the basis package did
		not have sufficient space to maintain the basis.

  lpLOSTFEAS	Feasibility was lost during simplex execution.

  lpPUNT	The lp has punted because it ran into a pivoting problem.

		The next three codes indicate that we're in the middle of
		attempting a forced transition for error recovery purposes.

  lpFORCEDUAL(*) Force a primal to dual transition.

  lpFORCEPRIMAL(*) Force a dual to primal transition.

  lpFORCEFULL(*) Force all inactive constraints and variables to be loaded.

  lpFATAL	Fatal confusion or error of some sort; covers a multitude
		of sins.

  The dual simplex routine does not have a phase I routine equivalent to
  dy_primal1 for the primal simplex. (In the context of dylp, it expects
  to run after dy_primal has been used to find an initial optimal solution.)

  When using the dual simplex method, internal codes reflect the state of
  the dual problem, but dy_dual makes the usual translation back to the
  primal problem, as:

  Dual		Primal		Rationale
  ----		------		---------
  lpUNBOUNDED	lpINFEAS	Standard duality theorem.

  Note that lpSWING always refers to primal variables.
*/

typedef enum { lpFATAL = -1, lpINV = 0,
	       lpOPTIMAL, lpUNBOUNDED, lpSWING, lpINFEAS,
	       lpACCCHK,
	       lpSTALLED, lpITERLIM, lpNOSPACE,
	       lpLOSTFEAS, lpPUNT,
	       lpFORCEDUAL, lpFORCEPRIMAL, lpFORCEFULL } lpret_enum ;


/*
  Phase codes for dylp

  dyINV		Invalid phase
  dyINIT	Initialisation and setup, including establishing the
		initial set of constraints and variables and crashing the
		first basis.
  dyPRIMAL1	Primal simplex phase I
  dyPRIMAL2	Primal simplex phase II
  dyDUAL	Dual simplex
  dyPURGEVAR	Deactivation of variables.
  dyGENVAR	Generation of new variables (not part of original problem).
  dyADDVAR	Activation of variables.
  dyPURGECON	Deactivation of constraints.
  dyGENCON	Generation of new constraints (not part of original problem).
  dyADDCON	Activation of constraints.
  dyFORCEDUAL	Force dual feasibility (error recovery)
  dyFORCEPRIMAL	Force primal feasibility (error recovery)
  dyFORCEFULL	Force activation of the full system (error recovery)
  dyDONE	Execution is finished, for one reason or another.

  It's true that new variables will be added during dyGENCON -- at the least,
  each newly generated constraint will bring with it a logical variable.
  dyGENVAR differs in that it is augmenting some subset of the constraints
  with new variables (classic column generation, for example).

  The main loop states (dyPRIMAL1 -- dyFORCEFULL) must remain a contiguous
  block. dy_dumpstats counts on dyPRIMAL1 and dyFORCEPRIMAL being first and
  last, respectively, in the block.

  dyDONE must remain the last code --- it's used to dimension a statistics
  array that tracks the number of times the main loop states are entered.
*/

typedef enum { dyINV = 0, dyINIT,
	       dyPRIMAL1, dyPRIMAL2, dyDUAL,
	       dyPURGEVAR, dyGENVAR, dyADDVAR,
	       dyPURGECON, dyGENCON, dyADDCON,
	       dyFORCEDUAL, dyFORCEPRIMAL, dyFORCEFULL,
	       dyDONE } dyphase_enum ;
/*
  General return and error codes.
  
  Used by various routines in dylp.
  No routine
  uses all of these, but there's enough overlap to make one big enum
  convenient.

  dyrINV	Invalid code.

  dyrOK		Whatever it was that was being done was done without incident.
  dyrOPTIMAL	The problem is optimal.
  dyrUNBOUND	The problem is unbounded.
  dyrSWING	The problem is pseudo-unbounded: Some variable grew by an
		excessive amount in a single pivot.
  dyrINFEAS	The problem is infeasible.

  dyrREQCHK	Requests a refactor and accuracy check (triggered by various
		checks for bogus numbers).
  dyrACCCHK	An accuracy check has failed.
  dyrLOSTPFEAS	Primal feasibility has been lost.
  dyrLOSTDFEAS	Dual feasibility has been lost.

  dyrDEGEN	Degeneracy has been discovered, or a degenerate pivot has been
		taken.
  dyrRESELECT	Reselect an incoming variable (after an abortive pivot
		attempt).
  dyrMADPIV	The selected pivot coefficient was (numerically) unstable.
  dyrPUNT	In the context of the dual simplex: the dual simplex has
		decided to punt to the primal simplex phase I, for any of
		several reasons. Generally this is indicative of the relative
		lack of sophistication in the dual simplex.
		In the context of the primal simplex: this indicates that all
		candidates to enter the basis were flagged with a NOPIVOT
		qualifier.

  dyrPATCHED	The basis package managed to factor the basis after patching
		it.
  dyrSINGULAR	The basis package discovered the basis was singular. (Typically
		as a consequence of a pivot gone bad.)
  dyrNUMERIC	The basis package detected unacceptable growth in the basis
		coefficients.
  dyrBSPACE	The basis package ran out of space for the basis
		representation.	
  dyrSTALLED	The LP seems to have stalled (and could possibly be cycling,
		but that's too much trouble to prove); triggered by too many
		iterations with no change in the objective.
  dyrITERLIM	The iteration limit has been exceeded.

  dyrFATAL	Fatal confusion; covers a multitude of sins.

   The specific values assigned to some of the codes in the enum come from
   earlier use of yla05 as the basis package. It's gone, but there's no
   incentive to remove the values.
*/

typedef enum { dyrFATAL = -10, dyrITERLIM, dyrSTALLED,
	       dyrBSPACE = -7, dyrSINGULAR = -6, dyrNUMERIC = -5,
	       dyrLOSTPFEAS, dyrLOSTDFEAS, dyrDEGEN, dyrMADPIV,
	       dyrINV = 0, dyrOK = 1, dyrPATCHED = 2,
	       dyrRESELECT, dyrREQCHK, dyrACCCHK, dyrPUNT,
	       dyrOPTIMAL, dyrUNBOUND, dyrSWING, dyrINFEAS } dyret_enum ;

/*
  Requests and results for checks and recalculations

  Some symbolic names for requesting and reporting on factoring, accuracy
  checks and primal and dual variable calculations. These originated with la
  Duenna, hence the lad prefix. Interpretation varies subtly from routine to
  routine, so check the parameter notes.

  ladPRIMALCHK	(i) set to request primal accuracy check, Bx<B> = b - Nx<N>,
		(o) set to indicate failure of check
  ladDUALCHK	(i) set to to request dual accuracy check, yB = c<B>
		(o) set to indicate failure of check
  ladPRIMFEAS	(i) set to request primal feasibility check (primal variables
		    within bounds)
		(o) set to indicate loss of primal feasibility
  ladDUALFEAS	(i) set to request dual feasibility check (reduced costs of
		    proper sign)
		(o) set to indicate loss of dual feasibility
  ladPFQUIET	(i) set to suppress warnings about variables which are not
		    primal feasible
  ladDFQUIET	(i) set to suppress warnings about variables which are not
		    dual feasible
  ladDUALS	(i) set to request calculation of the dual variables and
		    gradient vector
		(o) set to indicate calculation of the dual variables and
		    gradient vector
  ladPRIMALS	(i) set to request calculation of the primal variables
		(o) set to indicate calculation of the primal variables
  ladFACTOR	(i) set to indicate the basis should be refactored
		(o) set to indicate the basis has been factored
  ladEXPAND	(i) set to force expansion of the space allocated for the
		    basis representation
		(o) set to indicate the space allocated for the basis was
		    increased
*/

#define ladPRIMFEAS	1<<0
#define ladPRIMALCHK	1<<1
#define ladPFQUIET	1<<2
#define ladDUALFEAS	1<<3
#define ladDUALCHK	1<<4
#define ladDFQUIET	1<<5
#define ladDUALS	1<<6
#define ladPRIMALS	1<<7
#define ladFACTOR	1<<8
#define ladEXPAND	1<<9



/*
  Variable status codes

  dylp keeps explicit status for both basic and nonbasic variables. These are
  set up as flags so that it's easy to test when more than one status will do
  for a particular action.

  vstatBFX	basic, fixed
  vstatBUUB	basic, above upper bound
  vstatBUB	basic, at upper bound
  vstatB	basic, strictly between bounds (a well-behaved basic variable)
  vstatBLB	basic, at lower bound
  vstatBLLB	basic, below lower bound
  vstatBFR	basic, free (unbounded)

  vstatNBFX	nonbasic, fixed
  vstatNBUB	nonbasic at upper bound
  vstatNBLB	nonbasic at lower bound
  vstatNBFR	nonbasic free

  vstatSB	superbasic, within bounds

  dylp ensures that superbasic variables are, in fact, always strictly within
  bounds.

  Inactive NBFR variables can be created at startup if dylp is working with a
  partial system and there are free variables that are not selected to be in
  the initial basis. If the client is forcing a full system, these will be
  active NBFR variables.  Error recovery may also create active NBFR
  variables. By convention, NBFR variables always have a value of zero.

  Inactive SB variables should not occur. SB status occurs only as the result
  of error recovery and is only valid in primal simplex.

  The value of SB variables is lost when they are reported out as part of a
  solution. This will only happen if dylp could not find an optimal solution.

  The following qualifiers can be added to the status:

  vstatNOPIVOT	Prevents the variable from being considered as a candidate
		for pivoting; used by the pivot rejection machinery.
  vstatNOPER	Prevents the variable from being perturbed during the
		formation of a restricted subproblem.
  vstatNOLOAD	Prevents the variable from being considered for activation;
		used by startup and variable activation/deactivation routines.
*/

#define vstatINV     0
#define vstatBFX  1<<0
#define vstatBUB  1<<1
#define vstatB    1<<2
#define vstatBLB  1<<3
#define vstatBFR  1<<4
#define vstatNBFX 1<<5
#define vstatNBUB 1<<6
#define vstatNBLB 1<<7
#define vstatNBFR 1<<8
#define vstatSB   1<<9
#define vstatBUUB  1<<10
#define vstatBLLB  1<<11

/*
  TAKE NOTE: Do not use the msb as a qualifier! The status value, with or
  without qualifiers, must be a positive value when cast to a signed integer.
*/

#define vstatNOPIVOT ((flags) 1<<(sizeof(flags)*8-2))
#define vstatNOPER ((flags) 1<<(sizeof(flags)*8-3))
#define vstatNOLOAD ((flags) 1<<(sizeof(flags)*8-4))

#define vstatBASIC \
	(vstatBFX|vstatBUUB|vstatBUB|vstatB|vstatBLB|vstatBLLB|vstatBFR)
#define vstatNONBASIC (vstatNBFX|vstatNBUB|vstatNBLB)
#define vstatEXOTIC (vstatSB|vstatNBFR)

#define vstatSTATUS (vstatBASIC|vstatNONBASIC|vstatEXOTIC)
#define vstatQUALS (vstatNOPIVOT|vstatNOPER|vstatNOLOAD)

/*
  This macro checks (in a simplistic way) that its parameter encodes one and
  only one status.  It's intended for mild error checking.  See
  dylp_utils:dy_chkstatus if you're really feeling paranoid.
*/

#define VALID_STATUS(zz_status_zz) \
  (zz_status_zz == vstatBFX || zz_status_zz == vstatBUB || \
   zz_status_zz == vstatB   || zz_status_zz == vstatBLB || \
   zz_status_zz == vstatBFR || \
   zz_status_zz == vstatNBFX || zz_status_zz == vstatNBUB || \
   zz_status_zz == vstatNBLB || zz_status_zz == vstatNBFR || \
   zz_status_zz == vstatSB)



/*
  Interface structures: lpprob_struct, lptols_struct, lpopts_struct
*/

/*
  basis_struct

  This structure is used to describe a basis to dylp, and to return the
  final basis at termination. The size of the basis depends on the
  number of active constraints, which will be a subset of the constraints
  in the system.

  The constraint system as supplied to dylp should not have logical variables
  (dylp will create them automatically). This presents a problem if the final
  basis contains basic logical variables. In this case, vndx is set to the
  negative of the index of the constraint which spawned the logical. This
  same technique can be used on input to, for example, specify the traditional
  all-logical starting basis.

  Field		Definition
  -----		----------
  len		The number of rows in the basis.
  el.cndx	Index of the constraint in this basis position.
  el.vndx	Index of the variable in this basis position.

*/

typedef struct { int cndx ; int vndx ; } basisel_struct ;

typedef struct
{ int len ;
  basisel_struct *el ; } basis_struct ;


/*
  LP problem control and status flags

  lpctlNOFREE		(i) Prevents dylp from freeing the problem structures,
			    in anticipation of a subsequent hot start. If dylp
			    exits with a state that is not suitable for hot
			    start, this flag is ignored and the problem data
			    structures are released.
  lpctlONLYFREE		(i) In conjunction with an initial phase of dyDONE,
			    causes dylp to do nothing except free the problem
			    data structure and return.
  lpctlUBNDCHG		(i) Indicates that the variable upper bounds (vub) have
			    been changed.
  lpctlLBNDCHG		(i) Indicates that the variable lower bounds (lub) have
			    been changed.
  lpctlRHSCHG		(i) Indicates that the right-hand side (rhs) has been
			    changed. Includes the rhslow vector (if it exists).
  lpctlOBJCHG		(i) Indicates that the objective (obj) has been changed.
  lpctlACTVARSIN	(i) Indicates that a valid active variable vector has
			    been supplied.
  lpctlINITACTVAR	(i) Forces dylp to perform variable activation before
			    beginning simplex iterations.
  lpctlINITACTCON	(i) Forces dylp to perform constraint activation before
			    beginning simplex iterations. (If variable
			    activation is also requested, constraint activation
			    occurs first.)

  lpctlACTVARSOUT	(i) Indicates that an active variable vector is to be
			    returned.
			(o) Indicates that a valid active variable vector has
			    been returned.

  lpctlDYVALID		(o) Indicates that dylp exited in a state which can
			    be restarted with a hot start.

*/

#define lpctlNOFREE	1<<0
#define lpctlONLYFREE	1<<1
#define lpctlUBNDCHG	1<<2
#define lpctlLBNDCHG	1<<3
#define lpctlRHSCHG	1<<4
#define lpctlOBJCHG	1<<5
#define lpctlACTVARSIN	1<<6
#define lpctlINITACTVAR	1<<7
#define lpctlINITACTCON	1<<8

#define lpctlACTVARSOUT	1<<10

#define lpctlDYVALID	1<<11

/*
  lpprob_struct

  This structure is used to pass an LP problem into dylp and convey the
  results back to the client. The allocated size indicated in colsze and
  rowsze is assumed to be accurate.  If basis, status, x, or y are NULL, they
  will be allocated as needed. If they are non-NULL, dylp will reallocate
  them if necessary (i.e., when the actual size of the lp exceeds the
  allocated size of the vectors).

  The status vector has the following coding:
    * for nonbasic variables, the normal dylp status flags are used;
    * for basic variables, the negative of the basis index is used.
  
  There is one unavoidable problem with this scheme -- the status vector
  provides the only information about the value of nonbasic variables. This
  is adequate for all but superbasic variables and nonbasic free variables
  which are not at zero. Both of these cases are transient anomalies, created
  only when the basis package is forced to patch a singular basis, and they
  should not persist in the final solution when an optimal solution is found
  or when the problem is infeasible.  They may, however, occur in the
  solution reported for an unbounded problem if the unbounded condition is
  discovered before the nonbasic free or superbasic variable is chosen for
  pivoting.  On input, nonbasic free variables are assumed to take the value
  0, and specifying a superbasic variable is illegal.

  Field		Definition
  -----		----------
  owner		ID of the owner of this problem.
  ctlopts	Control and status flags.		
  phase		(i) If set to dyDONE, dylp will free any retained data
		    structures and return. Any other value is ignored.
		(o) Termination phase of the dynamic simplex algorithm;
		    should be dyDONE unless something screws up, in which
		    case it'll be dyINV.
  lpret		Return code from the simplex routine.
  obj		For lpOPTIMAL, the value of the objective function.
		For lpINFEAS, the total infeasibility.
		For lpUNBOUNDED, the index of the unbounded variable, negated
		  if the variable can decrease without bound, positive if it
		  can increase without bound. The logical for constraint i
		  is represented as n+i.
		Otherwise, undefined.
  iters		The number of simplex iterations.
  consys	The constraint system.
  fullsys	True if the active constraint system is the full system, false
  		otherwise. 
  basis		(i) Initial basis.
  		(o) Final basis.
  status	(i) Initial status vector.
		(o) Final status vector.
  x		(i) No values used, but a vector can be supplied.
		(o) The values of the basic variables (indexed by basis
		    position).
  y		(i) No values used, but a vector can be supplied.
		(o) The values of the dual variables (indexed by basis
		    position).
  actvars	There is one entry for each variable, coded TRUE if the
		variable is active, FALSE otherwise. The vector supplied on
		input will be overwritten on output.
		(i) Variables to be activated at startup. Used only for a
		    warm start. Validity is indicated by the lpctlACTVARSIN
		    flag. A vector can be supplied strictly for output use.
		(o) The current active variables. Will be returned only if
		    requested by the lpctlACTVARSOUT flag. If the vector is
		    valid on return, lpctlACTVARSOUT will remain set,
		    otherwise it will be reset.

  colsze        Allocated column capacity (length of status vector).
  rowsze        Allocated row capacity (length of basis, x, and y vectors).

  
  Note that dylp will reallocate status, basis->el, actvars, x, and y, if the
  vectors supplied at startup are too small to report the solution. Don't set
  colsze or rowsze to nonzero values without actually allocating space.
*/

typedef struct
{ void *owner ;
  flags ctlopts ;
  dyphase_enum phase ;
  lpret_enum lpret ;
  double obj ;
  int iters ;
  consys_struct *consys ;
  bool fullsys ;
  basis_struct *basis ;
  flags *status ;
  double *x ;
  double *y ;
  bool *actvars ;
  int colsze ;
  int rowsze ; } lpprob_struct ;



/*
  lptols_struct

  This structure contains phase and tolerance information for the lp algorithm.

  The philosophy with respect to the separate zero and feasibility tolerances
  for primal and dual variables is that dylp uses the zero tolerance when
  calculating primal or dual variables, and the feasibility tolerance when
  checking for feasibility. This allows us to keep small values for accuracy
  in computation, but not be so fussy when it comes to feasibility.

  Field		Definition
  -----		----------
  inf		Infinity. dylp uses IEEE FP infinity, but be careful not to
		pass it to the basis package.
  zero          Zero tolerance for primal variables, and also the generic
		zero tolerance for constraint coefficients, right-hand-side
		values, etc.
  pchk		Primal accuracy check tolerance.
  pfeas		Primal feasibility check tolerance; dynamically scaled from
		zero in proportion to the 1-norm of the primal basic variables.
  pfeas_scale	Primal feasibility check tolerance multiplier. This provides
		some user-controllable decoupling of zero and pfeas.
  cost          Base zero tolerance for checks involving objective function
		coefficients, reduced costs, and related values.
  dchk		Dual accuracy check tolerance. Also used by dy_duenna to
		test for improvement in the objective.
  dfeas         Dual feasbility check tolerance; dynamically scaled from cost
		in proportion to the 1-norm of the dual variables. Acts as the
		zero tolerance for reduced costs.
  dfeas_scale	Dual feasibility check tolerance multiplier. This provides
		some user-controllable decoupling of cost and dfeas.
  pivot         Simplex pivot selection tolerance, expressed as a multiplier
		for the pivot selection tolerance used by the basis package
		when factoring the basis. (I.e., the actual pivot selection
		criteria will be to accept a simplex pivot a<i,j> if
		|a<i,j>| > lptols.pivot*basis.pivot*MAX{i}|a<i,j>|.)
  bogus		Multiplier used to identify 'bogus' values, in the range
		tol < |val| < bogus*tol for the appropriate tolerance.
  swing		Ratio used to identify excessive growth in primal variables
		(pseudo-unboundedness).
  toobig	Absolute value of primal variables which will cause dual
		multipivoting to consider primal infeasibility when selecting
		a flip/pivot sequence.
  purge		Percentage change in objective function required before
		constraint or variable purging is attempted.
  purgevar	Percentage of maximum reduced cost used to determine the
		variable purge threshold; nonbasic architectural variables
		at their optimum bound whose reduced cost exceeds
		purgevar*MAX{j}cbar<j> are purged.

  reframe	Multiplier used to trigger a reference framework reset in
		PSE pricing; reset occurs if
		  |gamma<j> - ||abar<j>||^2| > reframe*||abar<j>||^2.
		The check is made in pseupdate.
		Also used to trigger recalculation of the basis inverse row
		norms used in DSE pricing; reset occurs if
		  |rho<i> - ||beta<i>||^2| > reframe*||beta<i>||^2.
		The check is made in dseupdate.
*/

typedef struct
{ double inf ;
  double zero ;
  double pchk ;
  double pfeas ;
  double pfeas_scale ;
  double cost ;
  double dchk ;
  double dfeas ;
  double dfeas_scale ;
  double pivot ;
  double bogus ;
  double swing ;
  double toobig ;
  double purge ;
  double purgevar ;
  double reframe ; } lptols_struct ;

#if defined(DYLP_INTERNAL) || defined(BONSAIG)
/*
  A few handy macros for testing values against tolerances.
*/
#ifdef DYLP_INTERNAL
#  ifdef BND_TOLER
#    undef BND_TOLER
#  endif
#  define BND_TOLER dy_tols->pfeas
#  ifdef INF_TOLER
#    undef INF_TOLER
#  endif
#  define INF_TOLER dy_tols->inf
#endif

#define withintol(zz_val_zz,zz_tgt_zz,zz_tol_zz) \
  (fabs((zz_val_zz)-(zz_tgt_zz)) <= zz_tol_zz)

#define setcleanzero(zz_val_zz,zz_tol_zz) \
  if (fabs(zz_val_zz) < zz_tol_zz) zz_val_zz = 0

#define atbnd(zz_val_zz,zz_bnd_zz) \
  ((fabs(zz_bnd_zz) < INF_TOLER) && \
   (fabs((zz_val_zz)-(zz_bnd_zz)) < BND_TOLER*(1.0+fabs(zz_bnd_zz))))

#define belowbnd(zz_val_zz,zz_bnd_zz) \
  ((fabs(zz_bnd_zz) < INF_TOLER) ? \
   (((zz_bnd_zz)-(zz_val_zz)) > BND_TOLER*(1.0+fabs(zz_bnd_zz))) : \
   (zz_val_zz < zz_bnd_zz))

#define abovebnd(zz_val_zz,zz_bnd_zz) \
  ((fabs(zz_bnd_zz) < INF_TOLER) ? \
   (((zz_val_zz)-(zz_bnd_zz)) > BND_TOLER*(1.0+fabs(zz_bnd_zz))) : \
   (zz_val_zz > zz_bnd_zz))

#define withinbnds(zz_lb_zz,zz_val_zz,zz_ub_zz) \
  (!abovebnd(zz_val_zz,zz_ub_zz) && !belowbnd(zz_val_zz,zz_lb_zz))

#endif /* DYLP_INTERNAL || BONSAIG */

#ifdef DYLP_INTERNAL
/*
  Finally, a macro to decide if we should snap to a value. The notion here is
  that the accuracy with which one can hit a target value depends on both the
  magnitude of the target and the distance travelled to get there. On a
  64-bit machine, IEEE FP has about 15 decimal digits of accuracy. For
  example, if we're travelling 1.0e7 and trying to hit zero, we only have 8
  decimal places of accuracy remaining. If we're within 1.0e-8, might as well
  snap to 0. In practice, there's already a bit of roundoff in any nontrivial
  calculation, so we start with the zero tolerance and scale from there.
  
  In some cases, we only know the target, so the best we can do is
  scale to it.

  The utility of this idea is highly questionable.
*/

#define snaptol1(zz_tgt_zz) (dy_tols->zero*(1.0+(zz_tgt_zz)))

#define snaptol2(zz_tgt_zz,zz_dst_zz) \
  (dy_tols->zero*(1.0+maxx((zz_tgt_zz),(zz_dst_zz))))

#define snaptol3(zz_tol_zz,zz_tgt_zz,zz_dst_zz) \
  ((zz_tol_zz)*(1.0+maxx((zz_tgt_zz),(zz_dst_zz))))

#endif /* DYLP_INTERNAL */



/*
  Enum for initial basis type.

  This determines the criteria used to select the initial set of basic
  variables during a cold start.

  ibINV		invalid
  ibLOGICAL	Use only logical (slack and artificial) variables
  ibSLACK	Use slack variables for inequalities. Prefer architectural
		over artificial variables for equalities.
  ibARCH	Prefer architectural variables over logical variables.
*/

typedef enum { ibINV = 0, ibLOGICAL, ibSLACK, ibARCH } ibtype_enum ;

/*
  Enum for calling context.

  As dylp evolves, it has proven useful to know the context of the
  call. Consider this a work in progress. The default context is INITIALLP.

  cxINV		invalid (context is unknown)
  cxLOAD	This call is only to (re)load data structures. Returns after
		one iteration of dual or primal simplex, but shows problem
		status rather than lpITERLIM.
  cxUNLOAD	This call is solely for the purpose of freeing the problem
		data structures. (Replaces dyDONE/lpctlONLYFREE hack.)
  cxSINGLELP	This is a one-off call to solve a single LP from scratch.
  cxINITIALLP	This is a call to solve a single LP from scratch, but will
		likely be followed by calls to reoptimise.
  cxBANDC	This call is made in the context of a branch-and-cut
		algorithm.
  cxUSERPIV	This call is in the context of user control of pivoting.
*/

typedef enum { cxINV = 0, cxLOAD, cxUNLOAD,
	       cxSINGLELP, cxINITIALLP, cxBANDC, cxUSERPIV } cxtype_enum ;


/*
  lpopts_struct

  This structure is used to pass option settings to dylp. Default values are
  declared at the beginning of dy_setup.c.

  Field		Definition
  -----		----------
  context	The context in which dylp is being called. See comments
		above for cxtype_enum.
  forcecold	TRUE to force a cold start, FALSE otherwise. If set to TRUE,
		dominates warm and hot start.
  forcewarm	TRUE to force a warm start, FALSE otherwise. If set to TRUE,
		dominates hot start.
  fullsys	Forces the use of the full constraint system at all times. The
		full constraint system is loaded on startup, and all constraint
		and variable deactivation/activation is skipped. (But see the
		finpurge option below.) (Also, this will not prevent dylp
		from resorting to forced phase transitions, which typically
		involve deactivation of constraints or variables. Arguably
		this is a bad thing, and may change in the future.)
  active	Used to estimate the initial size of the dylp constraint
		system relative to the original system.
    vars	  Fraction of original variables expected to be active at
		  any time.
    cons	  Fraction of original inequalities expected to be active at
		  any time.
  initcons	Specifies how inequalities will be selected to initialize the
		active system. See extensive comments in dy_coldstart.c.
    frac	  Fraction of inequalities to be used.
    i1l		  Lower bound on angle to objective, first interval
    i1lopen	  TRUE if the bound is open.
    i1u		  Upper bound on angle to objective, first interval
    i1uopen	  TRUE if the bound is open.
    i2valid	  TRUE if the second interval is specified
    i2l		  Lower bound on angle to objective, second interval
    i2lopen	  TRUE if the bound is open.
    i2u		  Upper bound on angle to objective, second interval
    i2uopen	  TRUE if the bound is open.
  coldbasis	Code specifying the kind of basis built for a cold start. See
		comments for ibtype_enum and comments in dy_coldstart.c
  finpurge	Controls whether dylp does a final deactivation of constraints
		and/or variables. This will occur only an optimal solution is
		found, and is not suppressed by fullsys.
    cons	  TRUE to purge constraints
    vars	  TRUE to purge variables
  heroics	Controls behaviour during forced primal <-> dual transitions
    d2p		  TRUE to allow deactivation of basic architecturals, FALSE
		  to disallow. FALSE is recommended, and the default.
    p2d		  TRUE to allow deactivation of tight constraints, FALSE
		  to disallow. FALSE is recommended, and the default.
    flip	  TRUE to allow flips to regain dual feasibility, FALSE to
		  disallow. Tends to cycle; default is false.
  coldvars	If the number of active variables exceeds this value after
		a cold start, dylp will attempt to purge variables prior to
		the initial primal simplex run.
  con		Options related to constraint activation/deactivation
    actlvl	  The constraint activation strategy
		    0: (strict) activate violated constraints,
		       lhs < rhslow or lhs > rhs
		    1: (tight) activate tight or violated constraints,
		       lhs <= rhslow or lhs >= rhs
    actlim	  If non-zero, limits the number of constraints that can be
		  activated in any one call to a constraint activation
		  routine.
    deactlvl	The constraint deactivation strategy:
		  0: (normal) deactivate only inequalities i which are
		     strictly loose (i.e., slk<i> basic, not at bound).
		  1: (aggressive) normal plus inequalities which are tight
		     with y<i> = 0.
		  2: (fanatic) aggressive plus equalities with y<i> = 0
  usedual	TRUE if dual phase II is to be used to regain feasibility after
		adding constraints, FALSE to force use of primal phase I.
  addvar	If non-zero, at most this many variables will be added in
		any one pass through phase dyADDVAR.
  dualadd	Controls the types of activation allowed when adding variables
		during dual simplex.
		  0:	variable activation is disallowed
		  1:	type 1 activation (variables that will be dual feasible
			when activated into the nonbasic partition)
		  2:	type 2 activation (variables which can be activated
			if immediately pivoted into the basis)
		  3:	type 3 activation (activate with bound-to-bound pivot)
		See dy_dualaddvars for more extensive comments.
  scan		Partial pricing parameter. Controls the number of columns to
		be scanned for a new candidate entering variable when the
		candidate selected during PSE update is rejected.
  iterlim	The per phase pivot limit for the code; if set to 0, no
		limit is enforced.
  idlelim	The number of iterations without change in the objective
		function before the code declares the problem is stalled or
		cycling.
  dpsel		Options to control dual pivoting. Selection of the leaving
		variable is always handled by DSE.
    strat:	  The strategy used to select the entering variable:
		    0: standard ratio test; may use anti-degen lite
		    1: multipivoting, selecting for maximum dual objective
		       improvement.
		    2: multipivoting, select for minimum predicted
		       infeasibility.
		    3: multipivoting, select infeasibility reduction if
		       possible, otherwise maximum dual objective improvement.
    flex	  If TRUE, dylp will switch between strategies 1 and 3, using
		  strategy 1 unless primal magnitudes become too large.
    allownopiv	  If TRUE, sequences of flips with no finishing pivot will be
		  allowed. Defaults to false, very prone to cycling.
  ppsel		Options to control primal pivoting. Selection of the entering
		variable is always handled by PSE.
    strat:	  The strategy used to select the leaving variable:
		    0: standard ratio test; may use anti-degen lite
		    1: multipivoting
  factor	The LP basis will be refactored every factor iterations, in
		the absence of some compelling reason (typically error
		recovery) that forces it to occur sooner.
  check		An accuracy check will be forced every check iterations, in
		the absence of some compelling reason to do it earlier.
  groom		Controls the action taken by the basis grooming routine
		when it makes a nontrivial status correction:
		  0:	catatonic
		  1:	issue a warning
		  2:	issue an error message and force an abort
		Numeric codes should match keywords in dy_options.c:dy_ctlopt.
  degen		TRUE to allow creation of restricted subproblems to deal with
		degeneracy, FALSE to disallow it.
  degenpivlim	The number of successive degenerate pivots required before
		creating a restricted subproblem.
  degenlite	Controls secondary antidegeneracy --- `antidegen lite'
		  0: (pivotabort) break ties using |abar<kj>| and abort when
		     delta<j> = 0
		  1: (pivot) break ties using |abar<kj>| but always scan the
		     full basis
		  2: (alignobj) break ties by examining the alignment of the
		     hyperplane which will become tight on the pivot; choose
		     so that movement in the direction of the objective most
		     nearly lies in the hyperplane
		  3: (alignedge) break ties by examining the alignment of the
		     hyperplane which will become tight on the pivot; choose
		     so that the direction of motion defined by the entering
		     variable most nearly lies in the hyperplane.
		  4: (perpobj) break ties by examining the alignment of the
		     hyperplane which will become tight on the pivot; choose
		     so that the normal of the hyperplane is most nearly
		     aligned with the normal of the objective
		  5: (perpedge) break ties by examining the alignment of the
		     hyperplane which will become tight on the pivot; choose
		     so that the normal of the hyperplane is most nearly
		     aligned with the direction of motion
		Numeric codes should match keywords in dy_options.c:dy_ctlopt.
  patch		TRUE to allow the code to patch a singular basis, FALSE to
		prevent patching.
  copyorigsys	Controls whether dylp makes a local copy of the original
		system. If set to TRUE, dylp will always make a local copy.
		If set to FALSE, a copy will be made only if necessary.
		Scaling will trigger a local copy.
  scaling	Controls whether dylp attempts to scale the original constraint
		system for numeric stability.
		  0: scaling is forbidden
		  1: scale the original constraint system using numeric
		     scaling vectors attached to the system
		  2: evaluate the original constraint system and scale it if
		     necessary
		Note that even if scaling = 0, dylp may install +/-1.0 scaling
		vectors in order to flip >= constraints to <= constraints. See
		comments in dy_scaling.c
  print		Substructure for picky printing control. For all print options,
		a value of 0 suppresses all information messages.
    major	  Controls printing of major phase information.
		  1: a message at each phase transition.
    scaling	  Controls print level during initial evaluation and scaling of
		  the original constraint system.
		  1: start and finish messages
		  2: stability measures for original and scaled matrices;
		     adjustments to tolerances.
    setup	  Controls print level while creating the initial constraint
		  system for dylp.
		  1: start and finish messages.
		  2: summary information about activated constraints
		  3: messages about each constraint included in the initial
		     system.
		  4: messages about each constraint processed for the initial
		     system
		  5: messages about each variable included in the initial
		     system.
		  6: lists value and status of inactive variables with
		     nonzero values
    crash	  Controls print level while crashing the basis.
		  1: start & finish messages
		  2: summary counts for logicals, architecturals, artificials
		  3: a dump of the completed basis
		  4: detailed info on the selection of each architectural
		     and artificial variable
    pricing	  Controls print level for pricing of columns (rows) in primal
		  (dual) simplex.
		  1: summary messages
		  2: lists candidate list and primal variable selected for
		     entry (exit) at each pivot
		  3: lists each variable as it's added to the candidate list
		     and again when reconsidered for pivoting
    pivoting	  Controls print level for selection of the leaving (entering)
		  primal variable in primal (dual) simplex and updating of
		  variables.
		  1: prints result of leaving (entering) variable selection
		  2: information about the selection of the leaving (entering)
		     variable.
		  3: more information about the selection of the leaving
		     (entering) variable.
		  4: prints the pivot column (row) before and after
		     multiplication by the basis inverse, and yet more
		     pivot selection information.
		  5: prints a line for every candidate evaluated
    pivreject	  Controls print level for information related to the operation
		  of the pivot rejection mechanism.
		  1: Prints a message for each row/column added to the pivot
		     rejection list, plus other major events.
		  2: Prints a message for each row/column removed from the
		     pivot rejection list.
    degen	  Controls print level for information related to the operation
		  of the antidegeneracy mechanism.
		  1: prints a message each time the antidegeneracy level
		     changes
		  2: prints a message when a true degenerate pivot is taken
		     under duress
		  3: prints a message when a degenerate pivot is taken
		  4: prints anomalies as each degenerate set is formed and
		     backed out
		  5: prints details of each degenerate set as it's formed and
		     backed out
    phase1	  Controls general print level for phase 1 of primal simplex.
		  1: messages about extraordinary events -- problem pivots, etc.
		  2: messages about 'routine' but infrequent events --
		     termination conditions, refactoring, unboundedness, etc.
		  3: messages with additional details of problems encountered
		  4: a one line log message is printed for each pivot
		  5: summary information about infeasible variables and phase
		     I objective coefficients; information about primal
		     variables updated at each pivot.
		  6: prints the primal variables after each pivot; prints
		     infeasible variables during phase I objective construction
		  7: prints the dual variables after each pivot; prints
		     infeasible variables during phase I objective modification
    phase2	  Controls general print level for phase 1 of primal simplex.
		  1: messages about extraordinary events -- problem pivots, etc.
		  2: messages about 'routine' but infrequent events --
		     termination conditions, refactoring, unboundedness, etc.
		  3: messages with additional details of problems encountered
		  4: a one line log message is printed for each pivot
		  5: prints the updated basic primal variables after each pivot
		  6: prints all basic primal variables after each pivot
		  7: prints the dual variables after each pivot.
    dual	  Controls general print level for the dual simplex. As
		  phase2.
    basis	  Controls print level in routines working with the basis.
		  1: summary warnings about problems: empty rows, singularity,
		     numerical instability, etc.
		  2: information about factoring failures and recovery actions
		  3: warnings about individual empty rows, details of column
		     replacement when patching a singular basis, pivot
		     tolerance adjustments; information about pivoting failures
		     and recovery actions
		  4: basis stability after factoring
		  5: basis stability after pivoting
    conmgmt	  Controls print level while dylp is in the purge/generate/
		  activate constraint phases.
		  1: prints the number of constraints purged, generated,
		     & activated, and new size of constraint system.
		  2: prints a message for each constraint purged or activated.
		     (The cut generation routine is responsible for
		     handling this function when it generates cuts.)
		  3: additional details about refactoring and new values
		     of primal and dual variables.
		  4: prints a message about any variables affected as a side
		     effect of constraint changes, constraints processed
		     but not activated, and information about direction of
		     recession and relative angle of constraints when adding
		     constraints to an unbounded problem.
		  5: prints a running commentary on constraint and variable
		     shifts, inactive variables.
    varmgmt	  Controls print level while dylp is in the purge/generate/
		  activate variable phases.
		  1: prints the number of variables purged, generated,
		     & activated, and new size of constraint system.
		  2: prints a message for each variable purged & activated.
		     (The column generation routine is responsible for
		     handling this function when it generates new variables).
		  3: prints a message about any constraints affected as a side
		     effect of variable changes, variables processed but not
		     activated, and information about direction of recession
		     and relative angle of dual constraints when adding
		     variables to an unbounded dual.
		  4: prints a running commentary on constraint and variable
		     shifts.
    force	  Controls print level when dylp is attempting to force a
		  transition (primal -> dual, dual -> primal) or force the
		  use of the full constraint system.
		  1: prints a summary message giving the result of the
		     transition attempt
		  2: prints messages about actions taken for individual
		     constraints and variables.
		  3: additional information about variables and constraints
		     examined.
    tableau	  Controls print level for routines that generate tableau
		  vectors (beta<i>, beta<j>, abar<i>, abar<j>) for use by
		  external clients.
		  1: prints summary messages about the circumstances
		  4: prints nonzeros in the final vector.
		  5: prints nonzeros in intermediate vectors and (dy_betaj,
		     dy_abarj only) inactive rows
		  6: prints nonzeros of active portion in internal reference
		     frame (dy_betaj only)
    rays	  Controls print level for routines that generate primal
		  and dual rays for use by external clients.
		  1: prints summary messages about vectors found.
		  3: print information about columns / rows examined.
		  4: print information about why a column or row was rejected.
		  5: print nonzeros for each ray
    soln	  Controls print level for routines that generate primal and
		  dual solutions for use by external clients.
		  1: prints summary messages about the circumstances
		  3: prints nonzeros in the final vector
		  4: prints nonzeros in intermediate vectors
*/

typedef struct
{ cxtype_enum context ;
  int scan ;
  int iterlim ;
  int idlelim ;
  struct { int strat ;
	   bool flex ;
	   bool allownopiv ; } dpsel ;
  struct { int strat ; } ppsel ;
  int factor ;
  int check ;
  int groom ;
  struct { int actlvl ;
	   int actlim ;
	   int deactlvl ; } con ;
  int addvar ;
  int dualadd ;
  int coldvars ;
  bool forcecold ;
  bool forcewarm ;
  bool usedual ;
  bool degen ;
  int degenpivlim ;
  int degenlite ;
  bool patch ;
  bool fullsys ;
  bool copyorigsys ;
  int scaling ;
  struct { float vars ;
	   float cons ; } active ;
  struct { double frac ;
	   bool i1lopen ;
	   double i1l ;
	   bool i1uopen ;
	   double i1u ;
	   bool i2valid ;
	   bool i2lopen ;
	   double i2l ;
	   bool i2uopen ;
	   double i2u ; } initcons ;
  ibtype_enum coldbasis ;
  struct { bool cons ;
	   bool vars ; } finpurge ;
  struct { bool d2p ;
	   bool p2d ;
	   bool flips ; } heroics ;
  struct { int major ;
	   int scaling ;
	   int setup ;
	   int crash ;
	   int pricing ;
	   int pivoting ;
	   int pivreject ;
	   int degen ;
	   int phase1 ;
	   int phase2 ;
	   int dual ;
	   int basis ;
	   int conmgmt ;
	   int varmgmt ;
	   int force ;
	   int tableau ;
	   int rays ;
	   int soln ; } print ; } lpopts_struct ;


  

  /*
    Statistics structure, used to collect information about aspects of dylp
    operation beyond simple pivot counts. The data structure definition is
    always present, but to fill it you have to define DYLP_STATISTICS.

    Field		Definition
    -----		----------
    phasecnts[dyDONE] Array with counts for number of times each phase is
		  executed.
    ini_simplex	The initial simplex phase
    cons		A set of arrays with data about individual constraints.
      sze		  Allocated capacity of the arrays.
      angle	  Angle to the objective function.
      actcnt	  Number of times constraint is activated.
      deactcnt	  Number of times constraint is deactivated.
      init	  True if constraint is active in initial system.
      fin		  True if constraint is active in final system.
    vars		A set of arrays with data about individual variables.
      sze		  Allocated capacity of the arrays.
      actcnt	  Number of times variable is activated.
      deactcnt	  Number of times variable is deactivated.
    angle
      max		Maximum angle to the objective function over all constraints.
      min		Minimum angle to the objective function over all constraints.
      hist[*]	Histogram of angles of constraints to the objective function.
		  There are DYSTATS_HISTBINS bins. Currently, 37 bins: 36 bins
		  spanning 5 degrees of angle, and a dedicated 90 degree bin.
    
    factor	Tracks how well we're doing with respect to refactoring the
		  basis.
      cnt		Number of time the basis has been refactored.
      prevpiv	Pivot count at last refactorisation.
      avgpivs	Average number of pivots between basis refactorisations.
      maxpivs	Maximum number of pivots between basis refactorisations.

    pivrej	Statistics about the pivot rejection list and punts.
      max		  maximum number of entries on the pivot rejection list
      mad		  total number of entries attributed to mad pivots
      sing	  total number of entries attributed to singular pivots
      pivtol_red	  total number of times the pivot tolerance was reduced
      min_pivtol	  the minimum pivot tolerance used
      puntcall	  total number of calls to dealWithPunt
      puntret	  total number of dyrPUNT returns recommended

    dmulti	Tracks the dual multipivot algorithm. All fields except cnt are
		  totals; divide by cnt to get averages.
      flippable	  Number of flippable variables in the constraint system.
      cnt		  Total calls to dualmultiin
      cands	  Number of candidates queued for evaluation for entry
      promote	  Number of calls that resulted in promoting a sane pivot
		    over an unstable pivot.
      nontrivial	  Number of times that the initial scan and sort left
		    multiple candidates for further evaluation.
      evals	  Actual number of candidates evaluated (ftran'd column)
      flips	  Number of bound-to-bound flips performed
      pivrnk	  Index in the list of candidates of the candidate finally
		    selected for pivoting.
      maxrnk	  Maximum index selected for pivoting.

    pmulti	Tracks the primal multipivot algorithm.
      cnt		  Total calls to primalmultiin
      cands	  Number of candidates queued for evaluation to leave
      nontrivial	  Number of times that the candidate list was sorted
      promote	  Number of calls that resulted in promoting a sane pivot
		    over an unstable pivot.
    
    infeas	Statistics on resolution of infeasibility in primal phase I.
		  Basically, what we're interested in tracking is the number
		  of infeasible variables and the number of pivots between a
		  change in the number of infeasible variables. We're interested
		  in separating the case of 1 variable from 2 or more, because
		  the latter requires vastly more calculation. A little care
		  is required because phase I can run many times.

      prevpiv	The pivot count (tot.iters) at the previous change.
      maxcnt	The maximum number of infeasible variables encountered (this
		  is not strictly monotonic, as dylp may enter phase I many
		  times due to activating violated constraints).
      totpivs	The total number of pivots expended in phase I.
      maxpivs	The maximum number of pivots with no change in the number of
		  feasible variables.
      chgcnt1	The number of times that the number of infeasible variables
		  changed and reduced costs did not have to be recalculated
		  (specifically, exactly one variable became feasible, and it
		  left the basis as it did so).
      chgcnt2	The number of times that the number of infeasible variables
		  changed in such a way as to require recalculation of the
		  reduced costs.

    [dp]degen[*]	Array of stats for each restricted subproblem nesting level,
		  with separate arrays for dual (ddegen) and primal (pdegen).
		  degen[0].cnt is used to hold the maximum nesting level.
      cnt		Number of times this nesting level was entered.
      avgsiz	The average number of variables in a restricted subproblem.
		  Kept by iterative update, as avg<k+1> = (avg<k>*k+size)/(k+1).
		  Suffers from cumulative loss of accuracy, but it'll do for
		  our purposes.
      maxsiz	The maximum number of variables in a restricted subproblem.
      totpivs	Total number of pivots at or above this nesting level.
      avgpivs	Average number of pivots at or above this nesting level.
      maxpivs	Maximum number of pivots for any one instance at or above
		  this nesting level.

    tot, p1, p2, d2	Iteration and pivot counts, total and for each
			  individual phase. These are copied over from
			  dy_lp (lp_struct) at the end of the run, so that
			  they can be printed by dumpstats.
    
    DYSTATS_MAXDEGEN is the maximum number of levels of nesting accommodated by
    antidegeneracy statistics and debugging structures. The actual algorithm
    has no inherent limitation.

    DYSTATS_HISTBINS is the number of bins for constraint angles. It should be an
    odd number. Each bin will span 180/(DYSTATS_HISTBINS-1) degrees, with the
    final bin reserved for constraints at 90 degrees. For example, a value of 37
    gives 180/(37-1) = 5 degrees per bin.
  */

  #define DYSTATS_MAXDEGEN 25
  #define DYSTATS_HISTBINS 37

  typedef struct {
    int phasecnts[dyDONE+1] ;
    dyphase_enum ini_simplex ;
    struct { int sze ;
	     double *angle ;
	     int *actcnt ;
	     int *deactcnt ;
	     bool *init ;
	     bool *fin ; } cons ;
    struct { int sze ;
	     int *actcnt ;
	     int *deactcnt ; } vars ;
    struct { float max ;
	     float min ;
	     int hist[DYSTATS_HISTBINS] ; } angle ;
    struct { int cnt ;
	     int prevpiv ;
	     float avgpivs ;
	     int maxpivs ; } factor ;
    struct { int max ;
	     int mad ;
	     int sing ;
	     int pivtol_red ;
	     double min_pivtol ;
	     int puntcall ;
	     int puntret ; } pivrej ;
    struct { int flippable ;
	     int cnt ;
	     int cands ;
	     int promote ;
	     int nontrivial ;
	     int evals ;
	     int flips ;
	     int pivrnks ;
	     int maxrnk ; } dmulti ;
    struct { int cnt ;
	     int cands ;
	     int nontrivial ;
	     int promote ; } pmulti ;
    struct { int prevpiv ;
	     int maxcnt ;
	     int totpivs ;
	     int maxpivs ;
	     int chgcnt1 ;
	     int chgcnt2 ; } infeas ;
    struct { int cnt ;
	     float avgsiz ;
	     int maxsiz ;
	     int totpivs ;
	     float avgpivs ;
	     int maxpivs ; } pdegen[DYSTATS_MAXDEGEN] ;
    struct { int cnt ;
	     float avgsiz ;
	     int maxsiz ;
	     int totpivs ;
	     float avgpivs ;
	     int maxpivs ; } ddegen[DYSTATS_MAXDEGEN] ;
    struct { int iters ;
	     int pivs ; } tot ;
    struct { int iters ;
	     int pivs ; } p1 ;
    struct { int iters ;
	     int pivs ; } p2 ;
    struct { int iters ;
	     int pivs ; } d2 ; } lpstats_struct ;



#ifdef DYLP_INTERNAL

/*
  Macros to determine whether a constraint or variable is active, and whether
  it's eligible for activation. Coding is explained below for dy_origcons and
  dy_origvars. The main purpose served by these macros is to make it easy to
  find activiation/deactivation points in the code, should the conventions ever
  change.
*/

#define ACTIVE_CON(zz_cndx_zz) (dy_origcons[(zz_cndx_zz)] > 0)
#define INACTIVE_CON(zz_cndx_zz) (dy_origcons[(zz_cndx_zz)] <= 0)
#define LOADABLE_CON(zz_cndx_zz) (dy_origcons[(zz_cndx_zz)] == 0)
#define MARK_UNLOADABLE_CON(zz_cndx_zz) (dy_origcons[(zz_cndx_zz)] = -1)
#define MARK_INACTIVE_CON(zz_cndx_zz) (dy_origcons[(zz_cndx_zz)] = 0)

#define ACTIVE_VAR(zz_vndx_zz) (dy_origvars[(zz_vndx_zz)] > 0)
#define INACTIVE_VAR(zz_vndx_zz) (dy_origvars[(zz_vndx_zz)] <= 0)
#define LOADABLE_VAR(zz_vndx_zz) \
  ((dy_origvars[(zz_vndx_zz)] < 0) && \
   flgoff(((flags) -dy_origvars[(zz_vndx_zz)]),vstatNOLOAD|vstatNBFX))
#define MARK_INACTIVE_VAR(zz_vndx_zz,zz_val_zz) \
  (dy_origvars[(zz_vndx_zz)] = (zz_val_zz))


/*
  dy_logchn		i/o id for the execution log file
  dy_gtxecho		controls echoing of generated text to stdout
*/

extern ioid dy_logchn ;
extern bool dy_gtxecho ;


/*
  lp_struct

  This structure is the control structure for an LP problem within dylp.

  Field		Definition
  -----		----------
  phase		Current phase of the dynamic simplex algorithm.
  lpret		Return code from the most recent simplex execution.

  z		Value of the objective function (includes inactzcorr).
  inactzcorr	Correction to the objective function due to inactive variables
		with non-zero values.

  simplex	Simplex algorithm status and control
    active	  currently active or most recently completed
    next	  currently active or to be started
    init_pse	  TRUE if the PSE structures need to be reinitialised,
		  FALSE otherwise
    init_dse	  TRUE if the DSE structures need to be reinitialised,
		  FALSE otherwise
		These fields are used to determine when to update or
		reinitialise the PSE and DSE data structures. Active and next
		must be valid during the purge/gen/add variable/constraint
		cycles.

		A word on infeas and infeascnt: They are guaranteed accurate
		only immediately after initialisation and following a primal
		feasibility check.

  infeas	Total infeasibility = SUM{j} max(0,x<j>-ub<j>,lb<j>-x<j>)
  infeascnt	The number of infeasible variables; refreshed when dy_accchk
		is asked to do a primal feasibility check.

  ubnd		Substructure for information on unbounded or pseudo-unbounded
		problems.
    ndx		  The index of the variable fingered for causing unboundedness
		  or pseudo-unboundedness (swing).
    ratio	  The growth ratio.

  p1obj		The following fields relate to handling of the phase I
		objective function.
    installed	TRUE if the phase I objective is currently installed
    infcnt	Tracks the number of variables incorporated in p1obj which
		remain infeasible.
    infvars_sze	Allocated size of the infvars vector.
    infvars	Vector of indices of infeasible variables incorporated in the
		phase I objective.
    p1obj	Pointer to the phase I objective (temporary storage while
		the phase II objective is installed).
    p2obj	Pointer to the phase II objective (temporary storage while
		the phase I objective is installed).
  
		A word on pivot and iteration counts: Iteration counts tally
		iterations of the pivoting loop, successful or not. Pivot
		counts tally successful bound-to-bound or change-of-basis
		pivots. Pretty much all messages will give tot.iters, so that
		it's possible to track the progress of an LP. Iterf has an
		entirely different function -- it's tracking the accumulation
		of eta matrices in the basis representation.

  sys		Substructure for dynamic system modification status.
    forcedfull	  Set to TRUE if the full system has been forced in state
		  dyFORCEFULL. This should happen at most once, so if we
		  enter dyFORCEFULL and forcedfull == TRUE, it's fatal.
    cons
      loadable	  Count of constraints which could be activated
      unloadable  Count of constraints which are ineligible for activation
		  (empty constraints and nonbinding rows)
    vars
      loadable	  Count of variables which could be activated
      unloadable  Count of variables which are ineligible for activation
		  (nonbasic fixed)

  tot		Total simplex iterations and pivots, all phases
    iters
    pivs
  p1		Primal phase I iterations and pivots.
    iters
    pivs
  p2		Primal phase II iterations and pivots.
    iters
    pivs
  d2		Dual phase II iterations and pivots.
    iters
    pivs

  pivok		Set to TRUE in dy_{primal|dual}pivot if the current iteration
		is a  successful pivot. Cleared to FALSE at the head of
		dy_duenna.
  prev_pivok	Set to pivok at head of dy_duenna. Provides status of just
		completed pivot for post-Duenna decisions.

  basis		Various fields related to basis change, refactoring, etc.

    etas	The number of basis changes (hence eta matrices) since the
		the basis was last factored. Used to schedule periodic
		factoring of the basis. Reset to 0 each time the basis is
		factored.
    pivs        The number of basis pivots since entry into a primal or dual
		simplex phase (excludes bound-to-bound simplex `pivots').
		Used when deciding whether to remove variables from the pivot
		reject list, and whether to pop out of a simplex phase due to
		excessive swing.
    dinf	Number of successive refactors with dual infeasibility.
		Cleared at the start of a simplex phase.
		Incremented/cleared in dy_accchk iff a dual feasibility check
		is performed.

  degen		Activation level of antidegeneracy algorithm. Held at 0 when
		the antidegeneracy algorithm is not active. Incremented each
		time a restricted subproblem is formed, and decremented when
		the restriction is backed out. (Values > 1 indicate that
		degeneracy recurred while working on a restricted subproblem,
		resulting in further restriction.)
  degenpivcnt	The number of successive degenerate pivots.

  idlecnt	The number of cycles since the objective has changed.

  lastz		Previous objective value for various activities; used to
		detect and suppress loops.
    piv		  Objective at last pivot (detects stalling)
    cd		  Objective at last entry into constraint deactivation
		  (dyPURGECON) (detects constraint activate/deactivate loops)
    vd		  Objective at last entry into variable deactivation
		  (dyPURGEVAR) (detects variable activate/deactivate loops)
    fp		  Objective at last entry into force primal (dyFORCEPRIMAL)
		  (detects constraint activate/deactivate loops)
    fd		  Objective at last entry into force dual (dyFORCEDUAL)
		  (detects variable activate/deactivate loops)

  prim		Primal variable information
    norm1	  1-norm of basic primal variables inv(B)b
    norm2	  2-norm of basic primal variables
    max		  inf-norm (max) of basic primal variables
    maxndx	  index of max primal variable

  dual		Dual variable information
    norm1	  1-norm of dual variables c<B>inv(B)
    norm2	  2-norm of dual variables
    max		  inf-norm (max) of dual variables
    maxndx	  index of max dual variable

*/

typedef struct
{ dyphase_enum phase ;
  lpret_enum lpret ;
  double z ;
  double inactzcorr ;
  struct { dyphase_enum active ;
	   dyphase_enum next ;
	   bool init_pse ;
	   bool init_dse ; } simplex ;
  double infeas ;
  int infeascnt ;
  struct { int ndx ;
	   double ratio ; } ubnd ;
  struct { bool installed ;
	   int infcnt ;
	   int infvars_sze ;
	   int *infvars ;
	   double *p1obj ;
	   double *p2obj ; } p1obj ;
  struct { struct { int loadable ;
		    int unloadable ; } cons ;
	   struct { int loadable ;
		    int unloadable ; } vars ;
	   bool forcedfull ; } sys ;
  struct { int iters ;
	   int pivs ; } tot ;
  struct { int iters ;
	   int pivs ; } p1 ;
  struct { int iters ;
	   int pivs ; } p2 ;
  struct { int iters ;
	   int pivs ; } d2 ;
  bool pivok ;
  bool prev_pivok ;
  struct { int etas ;
	   int pivs ;
	   int dinf ; } basis ;
  int degen ;
  int degenpivcnt ;
  int idlecnt ;
  struct { double piv ;
	   double cd ;
	   double vd ;
	   double fp ;
	   double fd ; } lastz ;
  struct { double norm1 ;
	   double norm2 ;
	   double max ;
	   int maxndx ; } prim ;
  struct { double norm1 ;
	   double norm2 ;
	   double max ;
	   int maxndx ; } dual ;
  } lp_struct ;


/*
  Declarations global to the dylp implementation but not visible outside of
  it. With this we can avoid passing huge numbers of parameters and/or
  unpacking a structure on a regular basis. Unless otherwise indicated, indices
  are in the dy_sys (active system) frame of reference.

  dy_owner	Null if there's no active problem. Contains the ID of the
  		current owner if there's an active problem. Passed in as
		part of the lpprob_struct.

  Main structures
  ---------------
  dy_lp:	The lp control structure for dylp.
  dy_sys:	The active constraint system; initialised in dylp:startup
  dy_tols:	Tolerances in effect for dylp; initialised in
		dylp:dylp from orig_tols.
  dy_opts:	Options in effect for dylp; initialised in
		dylp:dylp to point to same structure as orig_opts.
  dy_stats	Statistics structure for dylp; initialised in dylp:dylp to
		point ot the same structure as orig_stats.

  Constraint & Variable Management
  --------------------------------
  dy_actvars:	The active variables. Indexed in dy_sys frame, contains
		indices in orig_sys frame. Attached to dy_sys.
		Entries for logical variables (1 <= j <= concnt) are
		meaningless.
  dy_actcons:	The active constraints. Indexed in dy_sys frame, contains
		indices in orig_sys frame. Attached to dy_sys.
  dy_origvars:  Status of the original architectural variables.
		 * A value of 0 indicates the entry hasn't been processed.
		   Should never happen.
		 * If the variable is active, the entry contains the index
		   of the variable in the dy_sys frame.
		 * If the variable is inactive, the coding is the negative of
		   the vstat flags, limited to the nonbasic status values
		   vstatNBFR, vstatNBFX, vstatNBLB, or vstatNBUB, and the
		   qualifier vstatNOLOAD.
		Indexed in orig_sys frame. Attached to orig_sys.
  dy_origcons:	Status of the original constraints.
		 * If the constraint is active, the entry contains the index
		   of the constraint in the dy_sys frame.
		 * If the constraint is inactive, contains 0.
		 * If the constraint cannot be activated, contains a negative
		   value.
		Indexed in orig_sys frame. Attached to orig_sys.

  Note that there are macros defined to test whether a variable or constraint
  is inactive, and whether it's eligible for activation. Use them!
  
  Basis Management
  ----------------
  dy_basis:	The basis vector. Indexed by basis position, attached to
		dy_sys.
  dy_var2basis:	Translates a variable index to a basis pos'n. Indexed by
		column, attached to dy_sys. Entries for nonbasic variables
		must be kept at 0.
  dy_status:	The status vector. Indexed by column, attached to dy_sys.
  
  Primal and Dual Variables
  -------------------------
  dy_x:         The values of all primal variables. Indexed by column,
		attached to dy_sys. Values of nonbasic variables must always
		be correct (to allow uniform handling of normal nonbasic
		variables and superbasic variables). Values of basic
		variables will retain unperturbed values while the
		antidegeneracy mechanism is active -- this allows the
		perturbation to be quickly backed out.
  dy_xbasic:	The values of the basic variables. Indexed by basis pos'n,
		attached to dy_sys.
  dy_y:         The dual variables. Indexed by basis pos'n, attached to
  		dy_sys.

  Projected Steepest Edge (PSE) Pricing
  -------------------------------------
  dy_cbar:	Iteratively updated vector of reduced costs. Indexed by column,
		attached to dy_sys.
  dy_gamma:	Iteratively updated vector of column norms ||abar<j>||^2.
		Indexed by column, attached to dy_sys.
  dy_frame:	Boolean vector which indicates if a given variable is in the
		current frame of reference. Indexed by column, attached to
		dy_sys.
  
  Dual Steepest Edge (DSE) Pricing
  --------------------------------
  dy_rho:	Iteratively updated vector of row norms ||beta<i>||^2.
		Indexed by basis position, attached to dy_sys.

  DSE pricing also makes use of dy_xbasic (the reduced costs of the dual
  variables), and maintains dy_cbar.
  
  Antidegeneracy
  --------------
  dy_brkout:    Specifies the direction a variable needs to move to escape
		from a degenerate vertex.  Indexed by basis pos'n, attached
		to dy_sys.
  dy_degenset:  Specifies the set of constraints (equivalently, basic
		variables) perturbed at each level of the antidegeneracy
		algorithm. Indexed by basis pos'n, attached to dy_sys.  The
		entry for a constraint is the highest level of degenerate set
		which includes the constraint. If the entry is 0, the
		constraint is not involved in degeneracy.
  dy_ddegenset:	Specifies the set of dual constraints (equivalently, reduced
		costs) perturbed at each level of the antidegeneracy
		algorithm. Indexed by variable index, attached to dy_sys.
		The entry for a dual constraint is the highest level of
		degenerate set which includes the constraint. If the entry is
		0, the constraint is not involved in degeneracy.
*/

extern void *dy_owner ;

extern lp_struct *dy_lp ;
extern consys_struct *dy_sys ;
extern lptols_struct *dy_tols ;
extern lpopts_struct *dy_opts ;
extern int *dy_actvars,*dy_actcons,*dy_origvars,*dy_origcons,
	   *dy_basis,*dy_var2basis,
	   *dy_brkout,*dy_degenset,*dy_ddegenset ;
extern flags *dy_status ;
extern double *dy_x,*dy_xbasic,*dy_y,*dy_cbar,*dy_gamma,*dy_rho ;
extern bool *dy_frame ;

extern lpstats_struct *dy_stats ;

/*
  dy_scaling.c
*/

extern bool dy_initlclsystem(lpprob_struct *orig_lp, bool hotstart) ;
extern void dy_freelclsystem(lpprob_struct *orig_lp, bool freesys) ;
extern bool dy_isscaled(void) ;
extern void dy_scaling_vectors(const double **rscale, const double **cscale) ;
extern consys_struct *dy_scaled_origsys() ;

/*
  dy_coldstart.c
*/

extern dyret_enum dy_coldstart(consys_struct *orig_sys),
		  dy_crash(void) ;

/*
  dy_warmstart.c
*/

extern dyret_enum dy_warmstart(lpprob_struct *orig_lp) ;

/*
  dy_hotstart.c
*/

extern dyret_enum dy_hotstart(lpprob_struct *orig_lp) ;

/*
  dy_basis.c
*/

extern dyret_enum dy_factor(flags *calcflgs),
		  dy_pivot(int xipos, double abarij, double maxabarj) ;
extern double dy_chkpiv(double abarij, double maxabarj) ;
extern void dy_btran(double *col), dy_ftran(double *col, bool save) ;
extern bool dy_setpivparms(int curdelta, int mindelta) ;
extern char *dy_prtpivparms(int lvl) ;

/*
  dy_bound.c
*/

extern int dy_activateBndCons(consys_struct *orig_sys) ;
extern int dy_dualaddvars(consys_struct *orig_sys) ;

/*
  dy_conmgmt.c
*/

extern bool dy_loadcon(consys_struct *orig_sys, int orig_ndx,
		       bool genvars, int *inactndxs) ;
extern bool dy_deactNBLogPrimCon(consys_struct *orig_sys, int i),
	    dy_deactBLogPrimCon(consys_struct *orig_sys, int i),
	    dy_actBLogPrimCon(consys_struct *orig_sys, int i,
			      int *inactvars),
	    dy_actBLogPrimConList(consys_struct *orig_sys,
				  int cnt, int *ocndxs, int **inactvars) ;
extern int dy_deactivateCons(consys_struct *orig_sys),
	   dy_activateCons(consys_struct *orig_sys, bool with_vars) ;

/*
  dy_varmgmt.c
*/

extern bool dy_actNBPrimArch(consys_struct *orig_sys, int ovndx),
	    dy_actNBPrimArchList(consys_struct *orig_sys,
				 int cnt, int *ovndxs),
	    dy_deactBPrimArch(consys_struct *orig_sys, int ovndx),
	    dy_deactNBPrimArch(consys_struct *orig_sys, int ovndx) ;
extern int dy_deactivateVars(consys_struct *orig_sys),
	   dy_activateVars(consys_struct *orig_sys, int *candidates) ;

/*
  dy_primalpivot.c
*/

extern dyret_enum dy_primalin(int initcol, int scan, int *xjndx, int *nextcol),
		  dy_primalpivot(int xjndx, int indir, int *xindx, int *outdir,
				 double *abarij, double *delta, int *xjcand),
		  dy_degenout(int level) ;

/*
  dy_duenna.c
*/

extern dyret_enum dy_duenna(dyret_enum pivresult, int xjndx, int xindx,
			    int xjcand, int xicand),
		  dy_accchk(flags *checks) ;

/*
  dy_pivreject.c
*/

extern dyret_enum dy_addtopivrej(int xkndx, dyret_enum why,
				 double abarij, double maxabarij),
		  dy_dealWithPunt(void) ;
extern bool dy_clrpivrej(int *entries) ;
extern void dy_checkpivtol(void) ;
extern void dy_initpivrej(int sze), dy_freepivrej(void) ;


/*
  dy_primal.c
*/

extern lpret_enum dy_primal(void) ;
extern bool dy_initp1obj(void),dy_swapobjs(dyphase_enum phase) ; 

/*
  dy_dualpivot.c
*/

extern dyret_enum
  dy_dualout(int *xindx),
  dy_dualpivot(int xindx, int outdir,
	       int *p_xjndx, int *p_indir, double *p_cbarj,
	       double *p_abarij, double *p_delta, int *xicand),
  dy_dualdegenout(int level) ;

/*
  dy_dual.c
*/

extern lpret_enum dy_dual(void) ;

#endif /* DYLP_INTERNAL */

/*
  dy_setup.c
*/

extern void dy_defaults(lpopts_struct **opts, lptols_struct **tols),
	    dy_checkdefaults(consys_struct *sys,
			     lpopts_struct *opts, lptols_struct *tols),
	    dy_setprintopts(int lvl, lpopts_struct *opts) ;


/*
  dylp.c
*/

extern lpret_enum dylp(lpprob_struct *orig_lp, lpopts_struct *orig_opts,
		       lptols_struct *orig_tols, lpstats_struct *orig_stats) ;
extern void *dy_getOwner() ;

/*
  dylp_utils.c
*/

#ifdef DYLP_INTERNAL

extern lpret_enum dyret2lpret(dyret_enum dyret) ;
extern dyret_enum dy_updateprimals(int j, double deltaj, double *p_abarj) ;
extern bool dy_reducerhs(double *rhs, bool init),
	    dy_calcprimals(void),dy_calccbar(void) ;
extern void dy_calcduals(void),dy_setbasicstatus(void),
	    dy_dseinit(void),dy_pseinit(void) ;
extern double dy_calcobj(void),dy_calcdualobj(void),dy_calcpinfeas(void) ;
extern void dy_finishup(lpprob_struct *orig_lp, dyphase_enum phase) ;

#ifdef DYLP_PARANOIA

extern bool dy_chkstatus(int vndx),
            dy_chkdysys(consys_struct *orig_sys) ;
extern void dy_chkdual(int lvl) ;

#endif

#endif /* DYLP_INTERNAL */

extern bool dy_dupbasis(int dst_basissze, basis_struct **p_dst_basis,
			basis_struct *src_basis, int dst_statussze,
			flags **p_dst_status,
			int src_statuslen, flags *src_status) ;
extern void dy_freesoln(lpprob_struct *lpprob) ;

/*
  dy_penalty.c
*/

extern bool dy_pricenbvars(lpprob_struct *orig_lp, flags priceme,
			   double **p_ocbar, int *p_nbcnt, int **p_nbvars),
	    dy_pricedualpiv(lpprob_struct *orig_lp, int oxindx,
			    double nubi, double xi, double nlbi,
			    int nbcnt, int *nbvars,
			    double *cbar, double *p_upeni, double *p_dpeni) ;

/*
  dy_tableau.c
*/

extern bool dy_abarj(lpprob_struct *orig_lp, int tgt_j, double **p_abarj) ;
extern bool dy_betaj(lpprob_struct *orig_lp, int tgt_j, double **p_betaj) ;
extern bool dy_betak(lpprob_struct *orig_lp, int col_k, double **p_betaj) ;
extern bool dy_betai(lpprob_struct *orig_lp, int tgt_i, double **p_betai) ;
extern bool dy_abari(lpprob_struct *orig_lp, int tgt_i, double **p_abari,
		     double **p_betai) ;

/*
  dy_rays.c
*/

extern bool dy_primalRays(lpprob_struct *orig_lp,
			  int *p_numRays, double ***p_rays) ;
extern bool dy_dualRays(lpprob_struct *orig_lp, bool fullRay,
			int *p_numRays, double ***p_rays, bool trueDuals) ;

/*
  dy_solutions.c
*/

extern void dy_colDuals(lpprob_struct *orig_lp, double **p_cbar,
			bool trueDuals) ;
extern void dy_rowDuals(lpprob_struct *orig_lp, double **p_y,
			bool trueDuals) ;
extern void dy_rowDualsGivenC(lpprob_struct *orig_lp, double **p_y,
			      const double *c, bool trueDuals) ;

extern void dy_colPrimals(lpprob_struct *orig_lp, double **p_x) ;
extern void dy_rowPrimals(lpprob_struct *orig_lp,
			  double **p_xB, int **p_indB) ;
extern void dy_logPrimals(lpprob_struct *orig_lp, double **p_logx) ;

extern void dy_colStatus(lpprob_struct *orig_lp, flags **p_colstat) ;
extern void dy_logStatus(lpprob_struct *orig_lp, flags **p_logstat) ;

extern bool dy_expandxopt(lpprob_struct *lp, double **p_xopt) ;

/*
  dylp_io.c
*/

#include <dylib_io.h>

#ifdef DYLP_INTERNAL

extern void dy_logpivot(dyret_enum result, int xjndx, int indir, double cbarj,
			int xindx, int outdir, double abarij, double delta) ;
extern const char *dy_prtdyret(dyret_enum retcode) ;

#endif /* DYLP_INTERNAL */

extern const char *dy_prtlpret(lpret_enum lpret),
		  *dy_prtlpphase(dyphase_enum phase, bool abbrv) ;
extern char *dy_prtvstat(flags status) ;
extern bool dy_dumpcompact(ioid chn, bool echo, lpprob_struct *soln,
			   bool nbzeros) ;
extern void dy_setlogchn (ioid chn) ;
extern void dy_setgtxecho (bool echo) ;

/*
  dy_statistics.c

  These routines are compiled unconditionally, but note that the compile-time
  symbol DYLP_STATISTICS must be defined before dylp will actually take the
  time to collect detailed statistics. See dy_statistics.c for additional
  instructions.
*/

extern void dy_initstats(lpstats_struct **p_lpstats, consys_struct *orig_sys),
	    dy_dumpstats(ioid chn, bool echo, lpstats_struct *lpstats,
			 consys_struct *orig_sys),
	    dy_freestats(lpstats_struct **p_lpstats) ;

#ifdef DYLP_INTERNAL

extern void dy_finalstats(lpstats_struct *lpstats) ;

#endif /* DYLP_INTERNAL */


#endif /* _DYLP_H */
