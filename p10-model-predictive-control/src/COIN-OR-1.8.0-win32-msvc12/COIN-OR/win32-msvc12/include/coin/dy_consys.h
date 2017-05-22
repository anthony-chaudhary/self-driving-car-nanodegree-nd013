/*
  This file is a portion of the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

#ifndef _CONSYS_H
#define _CONSYS_H

/*
  @(#)dy_consys.h	4.4	11/11/04
  svn/cvs: $Id: dy_consys.h 407 2010-12-31 20:48:48Z lou $

  This header file contains declarations for a constraint system data
  structure, tailored for LP-based branch-and-cut MILP algorithms (more
  generally, for any situation where dynamic change in the number of rows
  and/or columns is expected). The constraint system allows for arbitrary
  additions and deletions. Additional space is allocated as needed; there are
  no a priori limits. Allocated space is never reduced, however, short of
  destroying the constraint system.

  The coefficient matrix is implemented as a sparse structure, linked by row
  and by column. Access to the rows and columns of the coefficient matrix is
  via utility routines.
  
  To provide O(1) cost for row and column addition and deletion, while
  maintaining a dense index set, the hole left by deleting a row or column
  is filled by moving the last row or column to occupy the vacant index slot.

  The client can 'attach' row and column vectors to the constraint system;
  more precisely, vectors and multiple pointers to those vectors.  The
  vectors will be dynamically resized and pointers updated whenever rows
  or columns are added to the constraint system. Entries in attached vectors
  are automatically reordered when a row or column is moved to fill the hole
  left by a deletion.

  The user is expected to work directly with any attached vectors; there are
  no special access routines.

  Because rows and columns can move, clients must be careful when they keep
  records of row or column indices, and update these records as necessary.

  A special set of attached vectors are referred to as associated vectors.
  These are distinguished only by having pointers allocated in the constraint
  system header structure.

  NOTE: The package assumes that variables and constraints are indexed from
	1; this makes error checking a lot more reliable, particularly for
	errors of omission (whoops, forgot to set that index ... ). Vectors
	intended for expanded rows or columns must be sized accordingly.
	(I.e., if vector is to hold k values, the actual allocated space
	is k+1 entries, with vector[0] unused.) consys will do this
	automatically when asked to allocate space. Clients must remember
	this when allocating space for vectors that they will attach to a
	constraint system.

  NOTE: The constraint system is prepared to deal with both finite (most
	often DBL_MAX) and infinite (IEEE) infinity in the upper and lower
	bounds vectors for variables (vlb, vub). Explicit checks are
	necessary to maintain the value of a finite infinity when scaling the
	constraint system. The value of infinity must be supplied as a
	parameter to create_consys(). It's not a good idea to have infinity
	popping up elsewhere, but IEEE infinity should work (because no
	explicit checks are required for mathematical correctness).

  NOTE: At the infinitesimal end, any coefficient with absolute value less
	than 1.0e-20 will be dropped. Currently this is hardwired (see
	consys_create). It may change if there's a need.
*/

#include "dy_vector.h"



/*
  Constraint coefficient matrix

  This is a sparse-matrix data structure, linked by row and column.

  Note that rows and columns are << not >> sorted in index order. Insertions
  and deletions are done adjacent to the headers, for efficiency.
*/
/*
  Coefficients

  Field		Description
  -----		-----------
  rowhdr	The row header for this coefficient.
  colhdr	The column header for this coefficient.
  val		The value of this coefficient.
  rownxt	The next coefficient in this row.
  colnxt	The next coefficient in this column.
*/

typedef struct coeff_struct_tag
{ struct rowhdr_struct_tag *rowhdr ;
  struct colhdr_struct_tag *colhdr ;
  double val ;
  struct coeff_struct_tag *rownxt ;
  struct coeff_struct_tag *colnxt ; } coeff_struct ;

/*
  Column headers

  Field		Description
  -----		-----------
  ndx		The index of this column.
  len		The number of coefficients in the column.
  nme		The name of the variable associated with the column.
  coeffs	The coefficients of the column.
*/

typedef struct colhdr_struct_tag
{ int ndx ;
  int len ;
  const char *nme ;
  coeff_struct *coeffs ; } colhdr_struct ;

/*
  Row headers

  Field		Description
  -----		-----------
  ndx		The index of this row.
  len		The number of coefficients in the row.
  nme		The name of the variable associated with the row.
  coeffs	The coefficients of the row.
*/

typedef struct rowhdr_struct_tag
{ int ndx ;
  int len ;
  const char *nme ;
  coeff_struct *coeffs ; } rowhdr_struct ;

/*
  Coefficient matrix header

  Field		Definition
  -----		---------
  coeffcnt	The number of coefficients in the matrix.
  cols		Array of pointers to column headers.
  rows		Array of pointers to row headers.
*/

typedef struct
{ int coeffcnt ;
  colhdr_struct **cols ;
  rowhdr_struct **rows ; } conmtx_struct ;



/*
  Attached Vectors:

  As mentioned at the top, attached vectors are automatically resized whenever
  the constraint system is resized, and reorderd to track the movement of rows
  and columns due to deletions.

  A particular subset of attached vectors are designated as associated
  vectors; a system has at most one of each type of associated vector.
  Their only distinguishing characteristic is that they occupy the dedicated
  pointers in the constraint system header:
   * objective function
   * variable type <3>
   * variable upper & lower bounds
   * right-hand-side (a.k.a. rhs or b) and rhsl (a.k.a. rhslow or blow) <1>
   * constraint type
   * upper & lower bounds for constraint left-hand-sides <2>
   * row and column scaling vectors (more accurately, the diagonal elements
     of row and column scaling matrices)
   
   <1> rhsl is created when range constraints (blow <= ax <= b) are present
       in the constraint system.
   <2> These are calculated using the upper and lower bounds on the variables
       in the constraint, and are used in arc consistency calculations. See
       further explanation below.
   <3> Arguably the variable type vector only needs to cover the architectural
       variables, but it'd be a pain to distinguish a resize involving the
       architectural columns from a resize involving the logical columns.
       Easier to waste a little space.

  The flags given below are used in attached vector headers to indicate
  how a vector should be handled, and in the consys_struct.parts field to
  indicate which components of the constraint system are present. Their
  most important function is to specify whether a vector is a row vector
  or a column vector. Beyond that, they serve as weak consistency and type
  checks.
  
  Notes:
  * MTX, ROWHDR, and COLHDR cannot be allocated/deallocated, but having
    codes for them makes the interface to some of the utility routines a
    bit more uniform.
  * COL and ROW should be used for generic column and row vectors,
    respectively.
  * VUB is initialised to +infinity.
  * RSCALE and CSCALE are initialised to 1.0
  * VTYP and CTYP are tied to the vartyp_enum and contyp_enum types.
*/

#define CONSYS_MTX	((flags) 1<<0)
#define CONSYS_ROW	((flags) 1<<1)
#define CONSYS_COL	((flags) 1<<2)
#define CONSYS_OBJ	((flags) 1<<3)
#define CONSYS_VUB	((flags) 1<<4)
#define CONSYS_VLB	((flags) 1<<5)
#define CONSYS_RHS	((flags) 1<<6)
#define CONSYS_CUB	((flags) 1<<7)
#define CONSYS_CLB	((flags) 1<<8)
#define CONSYS_RHSLOW	((flags) 1<<9)
#define CONSYS_VTYP	((flags) 1<<10)
#define CONSYS_CTYP	((flags) 1<<11)
#define CONSYS_COLHDR	((flags) 1<<12)
#define CONSYS_ROWHDR	((flags) 1<<13)
#define CONSYS_RSCALE	((flags) 1<<14)
#define CONSYS_CSCALE	((flags) 1<<15)

/*
  Macros to identify row and column vectors.
*/
#define CONSYS_ROWVEC \
  (CONSYS_OBJ|CONSYS_VUB|CONSYS_VLB|CONSYS_VTYP|CONSYS_CSCALE| \
   CONSYS_COLHDR|CONSYS_ROW)

#define CONSYS_COLVEC \
  (CONSYS_RHS|CONSYS_RHSLOW|CONSYS_CUB|CONSYS_CLB|CONSYS_CTYP|CONSYS_RSCALE| \
   CONSYS_ROWHDR|CONSYS_COL)

/*
  A macro to check for a valid vector type.
*/

#define VALID_ATTVTYPE(zz_vectype_zz) \
  (zz_vectype_zz == CONSYS_OBJ || \
   zz_vectype_zz == CONSYS_VUB || zz_vectype_zz == CONSYS_VLB || \
   zz_vectype_zz == CONSYS_RHS || zz_vectype_zz == CONSYS_RHSLOW || \
   zz_vectype_zz == CONSYS_CUB || zz_vectype_zz == CONSYS_CUB || \
   zz_vectype_zz == CONSYS_VTYP || zz_vectype_zz == CONSYS_CTYP || \
   zz_vectype_zz == CONSYS_RSCALE || zz_vectype_zz == CONSYS_CSCALE || \
   zz_vectype_zz == CONSYS_ROW || zz_vectype_zz == CONSYS_COL)


/*
  Attached vector header

  This structure is used in the list of attached vectors that should be
  checked and resized with the constraint system.

  Field		Definition
  -----		----------
  nxt		List link.
  what		The type of vector (coded with the flag values for
		attached and associated vectors)
  elsze		The size of an element in the vector.
  vec		The address of the vector.
  pveclst	A list of addresses which hold pointers to vec. If vec is
		moved as a result of a resize, these are rewritten.
*/

typedef struct attvhdr_struct_tag { struct attvhdr_struct_tag *nxt ;
			            flags what ;
				    int elsze ;
			            void *vec ;
				    lnk_struct *pveclst ; } attvhdr_struct ;

/*
  Constraint bounds

  Constraint bounds are upper and lower bounds on the value of the
  left-hand-side of a constraint, calculated using the upper and lower bounds
  on variables. In the case where all variables have finite bounds, the
  constraint bound will also be finite, and things are straightforward. But
  there's a complication --- we'll want to be able to efficiently handle the
  case where all variables have finite bounds except one, x<t>. In this case
  we can calculate a finite bound for the free variable, using the bounds on
  the other variables. Bottom line is we need a structure that keeps count of
  the number of infinities, as well as the finite portion of the bound. See
  consistency.c for more about the mechanics of particular cases.

  The interpretation of an entry is as follows:
    -varcnt <= inf < 0	inf is the negative of the index of the single
			remaining variable contributing an infinity; bnd is
			the finite lhs bound calculated from the other
			variables of the constraint
    inf >= 0		inf is the number of variables contributing infinity
			to the bound; bnd is the value of the finite portion
			of the lhs bound. If inf == 0, the lhs bound is finite.

    inf < -varcnt and inf = 1 are invalid. A value which exceeds the number of
    variables in the constraint is also bogus.

  This encoding means that it's impossible to distinguish +inf and -inf just
  by looking at the bound. But, in the case of constraint bounds, this hasn't
  been a problem in practice. Lower bounds go to -inf, and upper bounds go to
  +inf, and context has been sufficient.

  The revs field is used to keep track of the number of times the bound has
  been revised. See milp.h:mipopts_struct for the recalculation frequency.
*/

typedef struct { int revs ;
		 int inf ;
		 double bnd ; } conbnd_struct ;



/*
  Constraint type codes

  These codes have their origin in the MPS input standard.

  contypINV	invalid
  contypNB	non-binding constraint <1>
  contypGE	>= inequality
  contypEQ	equality
  contypLE	<= inequality
  contypRNG	'range' constraint, lb <= ax <= ub (a sort of shorthand for
		a >= and a <= inequality)

  <1> Non-binding constraints are a bit odd. They are used in two ways.
      The first non-binding constraint in the MPS file is, by convention,
      the objective function. The other use for non-binding constraints is
      in rows of type Dx (x one of N, E, G, or L), which specify linear
      combinations of constraints.
  <2> Following OSL (a good lead to follow when they're going where I want to
      go  :-), bonsai doesn't accept Dx rows, and throws away all but the
      first non-binding constraint, which it keeps only if it needs it for
      the objective function.
*/

typedef enum { contypINV = 0, contypNB,
	       contypGE, contypEQ, contypLE, contypRNG } contyp_enum ;

#define VALID_CONTYPE(zz_ctyp_zz) \
  (zz_ctyp_zz == contypGE || zz_ctyp_zz == contypEQ || \
   zz_ctyp_zz == contypLE || zz_ctyp_zz == contypRNG)

/*
  Variable type codes

  vartypINV	invalid
  vartypCON	continuous variable
  vartypINT	general integer variable
  vartypBIN	binary variable
*/

typedef enum { vartypINV = 0, vartypCON,
	       vartypINT, vartypBIN } vartyp_enum ;

#define VALID_VARTYPE(zz_vtyp_zz) \
  (zz_vtyp_zz == vartypCON || \
   zz_vtyp_zz == vartypINT || \
   zz_vtyp_zz == vartypBIN)

#define INT_VARTYPE(zz_vtyp_zz) \
  (zz_vtyp_zz == vartypINT || \
   zz_vtyp_zz == vartypBIN)

/*
  Behavioural options

  These codes are used as flags in the opts field of the constraint system
  header.

  CONSYS_LVARS	Set to indicate that logical variables are present and should
		be automatically maintained during constraint system
		manipulations.
  CONSYS_WRNZERO Set to indicate that a warning should be issued when the
		constraint system utility routines encounter a zero-length
		column or row. Also causes a warning during row/column
		creation and basis initialisation if an explicit zero
		coefficient is encountered.
  CONSYS_WRNATT Set to indicate that a warning should be issued when a
		duplicate attach request is encountered (i.e., both the
		vector and the pointer to the vector are already on the
		attvecs list).
  CONSYS_FININF	`Finite infinity' --- the client is indulging in the common
		trick of using a large finite value (most often, DBL_MAX) as
		infinity.
  CONSYS_CORRUPT The constraint system is corrupt --- an error has occurred
		during construction or modification that caused an operation
		to abort. Currently set only for errors that occur outside of
		debug and paranoia (you're supposed to look at the error
		messages for paranoia and debug).
*/

#define CONSYS_LVARS	((flags) 1<<0)
#define CONSYS_WRNZERO	((flags) 1<<1)
#define CONSYS_WRNATT	((flags) 1<<2)
#define CONSYS_FININF	((flags) 1<<3)
#define CONSYS_CORRUPT	((flags) 1<<4)

/*
  Constraint system header

  The top-level 'handle' for the structure.

  Field		Definition
  -----		---------
  nme		The name assigned to this constraint system.
  parts		Flags indicating which components of the constraint system are
		supposed to be present.
  opts		Flags indicating various behavioural options.
  inf		The value of infinity.
  tiny		The value of the infinitesimal.
  varcnt	The total number of variables (and the column dimension).
  archvcnt	The number of architectural variables. The number of continuous
		architectural variables is archvcnt-(intvcnt+binvcnt).
  logvcnt	The number of logical variables.
  intvcnt	The number of general integer variables.
  binvcnt	The number of binary variables.
  maxcollen	The number of coefficients in the largest column.
  maxcolndx	The index of the largest column.
  concnt	The total number of constraints (and the row dimension).
  archccnt	The number of architectural constraints.
  cutccnt	The number of cut constraints.
  maxrowlen	The number of coefficients in the largest row.
  maxrowndx	The index of the largest row.
  colsze	The allocated column capacity for the constraint system.
  rowsze	The allocated row capacity for the constraint system.
  mtx		The constraint matrix header.

    The vectors rowscale and colscale are valid only after execution of one
    of the scaling routines consys_geomscale, consys_equiscale, or
    consys_applyscale. The fields maxaij and minaij are valid only after
    execution of consys_evalsys or any of the scaling routines.

  maxaij	max{i,j} |a<ij>| (valid only after scaling)
  minaij	min{i,j, a<ij> != 0} |a<ij>| (valid only after scaling)
  rowscale	The row scaling vector.
  colscale	The column scaling vector.

  objnme	The name of the objective function.
  objndx	Index of the objective function, if it's installed as a
		constraint cx - x<z> = 0.
  xzndx		Index of the variable x<z>.
  obj		The objective function.
  vtyp		The type of variable.
  vub		The upper bounds for variables.
  vlb		The lower bounds for variables.
  rhs		The right-hand-side vector.
  rhslow	blow for range constraints of form blow <= ax <= b.
  ctyp		The type of constraint (contyp_enum).
  cub		The upper bounds for constraint left-hand-sides.
  clb		The lower bounds for constraint left-hand sides.
  attvecs	The list of attached vectors.

  NOTE the distinction between dimension and size -- the allocated capacity
       of the constraint system is [rowsze x colsze], while the actual size
       of the constraint system is [concnt x varcnt].
*/

typedef struct
{ const char *nme ;
  flags parts ;
  flags opts ;
  double inf ;
  double tiny ;
  int varcnt ;
  int archvcnt ;
  int logvcnt ;
  int intvcnt ;
  int binvcnt ;
  int maxcollen ;
  int maxcolndx ;
  int concnt ;
  int archccnt ;
  int cutccnt ;
  int maxrowlen ;
  int maxrowndx ;
  int colsze ;
  int rowsze ;
  conmtx_struct mtx ;
  double maxaij ;
  double minaij ;
  double *rowscale ;
  double *colscale ;
  const char *objnme ;
  int objndx ;
  int xzndx ;
  double *obj ;
  vartyp_enum *vtyp ;
  double *vub ;
  double *vlb ;
  double *rhs ;
  double *rhslow ;
  contyp_enum *ctyp ;
  conbnd_struct *cub ;
  conbnd_struct *clb ;
  attvhdr_struct *attvecs ; } consys_struct ;



/*
  consys_utils.c
*/

extern consys_struct *consys_create(const char *nme, flags parts, flags opts,
				    int concnt, int varcnt, double infinity) ;
extern bool consys_dupsys(consys_struct *src, consys_struct **dst,
			  flags dstvecs) ;
extern void consys_free (consys_struct *consys) ;
extern bool consys_realloc(consys_struct *consys, char rowcol, int incr),
	    consys_attach(consys_struct *consys, flags what, int elsze,
			  void **pvec),
	    consys_update(consys_struct *consys, void *oldvec, void *newvec),
	    consys_detach(consys_struct *consys, void **pvec, bool all) ;

extern bool consys_addcol_pk(consys_struct *consys,
			     vartyp_enum vartyp, pkvec_struct *pkcol,
			     double obj, double vlb, double vub),
	    consys_addcol_ex(consys_struct *consys, vartyp_enum vartyp,
			     const char **nme, double *excol,
			     double obj, double vlb, double vub),
	    consys_addrow_pk(consys_struct *consys, char rowclass,
			     contyp_enum contyp, pkvec_struct *pkrow,
			     double rhs, double rhslow,
			     conbnd_struct *cub, conbnd_struct *clb),
	    consys_getcol_pk(consys_struct *consys, int colndx,
			     pkvec_struct **pkvec),
	    consys_getcol_ex(consys_struct *consys, int colndx, double **vec),
	    consys_getrow_pk(consys_struct *consys, int rowndx,
			     pkvec_struct **pkvec),
	    consys_getrow_ex(consys_struct *consys, int rowndx, double **vec),
	    consys_delcol(consys_struct *consys, int colndx),
            consys_delrow(consys_struct *consys, int rowndx),
	    consys_delrow_stable(consys_struct *consys, int rowndx) ;

extern bool consys_setcoeff(consys_struct *consys,
			    int rowndx, int colndx, double val) ;
extern double consys_getcoeff(consys_struct *consys, int rowndx, int colndx) ;

extern bool consys_logicals(consys_struct *consys) ;

/*
  consys_mathutils.c
*/

extern int consys_gcdrow(consys_struct *consys, int rowndx) ;

extern double consys_dotcol(consys_struct *consys, int colndx, double *vec),
	      consys_dotrow(consys_struct *consys, int rowndx, double *vec) ;
extern double consys_1normrow(consys_struct *consys, int rowndx),
	      consys_ssqrow(consys_struct *consys, int rowndx),
	      consys_2normrow(consys_struct *consys, int rowndx),
	      consys_infnormrow(consys_struct *consys, int rowndx),
	      consys_1normcol(consys_struct *consys, int rowndx),
	      consys_ssqcol(consys_struct *consys, int rowndx),
	      consys_2normcol(consys_struct *consys, int rowndx),
	      consys_infnormcol(consys_struct *consys, int rowndx) ;

extern bool consys_mulrow(consys_struct *consys, int rowndx, double scalar) ;
extern bool consys_divrow(consys_struct *consys, int rowndx, double scalar) ;

extern bool consys_accumcol(consys_struct *consys, int colndx, double *vec) ;
extern bool consys_mulaccumcol(consys_struct *consys, int colndx,
			       double scalar, double *vec) ;

/*
  consys_scaling.c
*/

extern bool consys_evalsys(consys_struct *consys, double *scm, int *gecnt) ;
extern bool consys_geomscale(consys_struct *consys,
			     double **rowscale, double **colscale),
	    consys_equiscale(consys_struct *consys,
			     double **rowscale, double **colscale),
	    consys_applyscale(consys_struct *consys, bool convctyp,
			      double *rowscale, double *colscale) ;

/*
  consys_io.c
*/

extern const char *consys_prtvartyp(vartyp_enum vartyp),
		  *consys_prtcontyp(contyp_enum contyp) ;
extern char *consys_assocnme(consys_struct *consys, flags which),
	    *consys_conbndnme(char bndlett, int cndx, conbnd_struct *bnd),
	    *consys_conbndval(conbnd_struct *bnd) ;

#ifndef DYLP_NDEBUG

#include "dylib_io.h"
#include "dylib_std.h"

extern void consys_prtcon(ioid chn, bool echo,
			  consys_struct *consys, int i, const char *pfx) ;
#endif

/*
  A routine to set (change, really) the name of an existing constraint or
  variable.
*/

extern void consys_chgnme(consys_struct *consys, char cv,
			  int ndx, const char *newnme) ;

/*
  consys_nme returns a string containing the name of the specified constraint
  or variable. If the client supplies a buffer, that buffer is used to return
  the name. If a buffer isn't supplied, a little care is required.
    * If consys_nme can find a pointer to the name stored in the constraint
      matrix (i.e., in the row or column header) it will return the stored
      pointer. Successive calls will not interfere with one another.
    * If consys_nme has to build the name, it will use an internal buffer.
      Successive calls will reuse this buffer as required (overwriting the
      previous name).
  
  Names have to be built in two cases:
    * A fully prefixed name is requested (a prefix of 'consys->nme.' is
      added to the variable or constraint name).
    * The name of a logical variable is requested and logicals aren't enabled
      in the constraint system.
  
  A buffer of size CONSYS_MAXBUFLEN is guaranteed to be adequate.
*/

#define CONSYS_MAXBUFLEN 32
extern const char *consys_nme(consys_struct *consys,
			      char cv, int ndx, bool pfx, char *clientbuf) ;


#endif /* _CONSYS_H */
