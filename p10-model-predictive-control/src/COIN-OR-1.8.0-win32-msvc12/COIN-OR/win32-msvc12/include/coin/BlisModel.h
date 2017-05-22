/*===========================================================================*
 * This file is part of the BiCePS Linear Integer Solver (BLIS).             *
 *                                                                           *
 * BLIS is distributed under the Eclipse Public License as part of the       *
 * COIN-OR repository (http://www.coin-or.org).                              *
 *                                                                           *
 * Authors:                                                                  *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *                                                                           *
 * Conceptual Design:                                                        *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *          Laszlo Ladanyi, IBM T.J. Watson Research Center                  *
 *          Matthew Saltzman, Clemson University                             *
 *                                                                           * 
 *                                                                           *
 * Copyright (C) 2001-2013, Lehigh University, Yan Xu, and Ted Ralphs.       *
 * All Rights Reserved.                                                      *
 *===========================================================================*/

//#############################################################################

#ifndef BlisModel_h_
#define BlisModel_h_

//#############################################################################

#include <vector>

#include "CoinMpsIO.hpp"
#include "CoinLpIO.hpp"
#include "CoinPackedMatrix.hpp"

#include "CglCutGenerator.hpp"

#include "OsiCuts.hpp"
#include "OsiSolverInterface.hpp"

#include "AlpsEnumProcessT.h"
#include "AlpsParams.h"
#include "AlpsTreeNode.h"

#include "BcpsBranchStrategy.h"
#include "BcpsObject.h"
#include "BcpsObjectPool.h"
#include "BcpsModel.h"

#include "Blis.h"
#include "BlisConGenerator.h"
#include "BlisHeuristic.h"
#include "BlisMessage.h"
#include "BlisParams.h"
#include "BlisPseudo.h"
#include "BlisPresolve.h"

//#############################################################################

class BlisConstraint;
class BlisSolution;
class BcpsVariable;
class BlisVariable;

//#############################################################################

/* Declare a BLIS model */
class BlisModel : public BcpsModel {

protected:
    
    //------------------------------------------------------
    // LP SOLVER.
    //------------------------------------------------------
    
    /** Input by user. */
    OsiSolverInterface *origLpSolver_;
    /** Presolved. */
    OsiSolverInterface *presolvedLpSolver_;
    /** Actually used. If using presolve, then it is presolved;
	otherwise it is the original. */
    OsiSolverInterface *lpSolver_;
    
    //------------------------------------------------------
    // PROBLEM DATA. Populate when loadProblem(),
    //------------------------------------------------------
    
    /** Column majored matrix.(For MPS file, etc.) */
    CoinPackedMatrix *colMatrix_;    

    /** Variable and constraint bounds. */
    //@{
    double *varLB_;
    double *varUB_;
    double *conLB_;
    double *conUB_;
    //@}

    /** Number of columns/rows/elements */
    //@{
    int numCols_;
    int numRows_;
    int numElems_;
    //@}

    /** Objective function. */
    //@{
    double objSense_;
    double *objCoef_;
    //@}

    /** Column types. */
    //@{
    int numIntObjects_;
    int *intColIndices_;  // size of numIntObjects_
    //@}

    /** User's input objects. */
    //@{
    std::vector<BcpsVariable *> inputVar_;
    std::vector<BcpsConstraint *> inputCon_;
    //@}

    //------------------------------------------------------
    // PRESOLVE
    //------------------------------------------------------

    BlisPresolve *presolve_;
    // AT - Begin
    bool presolved;
    bool problemSetup;
    // AT - End
    
    //------------------------------------------------------
    // SOLUTION.
    //------------------------------------------------------
    
    int numSolutions_;
    int numHeurSolutions_;

    /** Incumbent objective value. */
    double incObjValue_;

    /** Incumbent */
    double *incumbent_;

    /** Cutoff in lp solver. */
    double cutoff_;

    /** Cutoff increment. */
    double cutoffInc_;
    
    //------------------------------------------------------
    // SEARCHING.
    //------------------------------------------------------

    int *intObjIndices_; // size of numCols_
    char *colType_;

    /** Starting var/con bounds for processing each node */
    //@{
    double *startVarLB_;
    double *startVarUB_;
    double *startConLB_;
    double *startConUB_;
    //@}

    /** Variable selection function. */
    BcpsBranchStrategy * branchStrategy_;
    BcpsBranchStrategy * rampUpBranchStrategy_;

    // Hotstart strategy 0 = off, 
    // 1 = branch if incorrect,
    // 2 = branch even if correct, ....
    BlisHotStartStrategy hotstartStrategy_;
    
    /** Number of objects. */
    int numObjects_;

    /** The set of objects. */
    BcpsObject **objects_;

    /** The objects that can be shared. */
    char *sharedObjectMark_;

    /** Priorities of integer object. */
    int *priority_;

    /** Active node. */
    AlpsTreeNode *activeNode_;

    /** Number of strong branching. */
    int numStrong_;

    // Not used. 
    double nodeWeight_;

    /** Maximum number of resolve during branching. */
    int numBranchResolve_;
    
    //------------------------------------------------------
    // HEURISTICS.
    //------------------------------------------------------

    /** Number of heuristics. */
    int numHeuristics_;

    /** The list of heuristics. */
    BlisHeuristic **heuristics_;

    //------------------------------------------------------
    // CONSTRAINTS.
    //------------------------------------------------------

    /** If use cut generators. */
    BlisCutStrategy cutStrategy_; 
    
    /** Frequency of cut generation */
    int cutGenerationFrequency_; 
    
    /** Number of cut generators used. */
    int numCutGenerators_;
    
    /** Number of cuts can be generators. */
    int maxNumCons_;

    /** The list of cut generators used. */
    BlisConGenerator **generators_;

    /** Store all the cuts. */
    BcpsConstraintPool *constraintPool_;
    
    /** Temporary store old cuts at a node when installing a node. */
    BlisConstraint **oldConstraints_;
    
    /** The memory size allocated for oldConstraints_. */
    int oldConstraintsSize_;
    
    /** Number of old constraints. */
    int numOldConstraints_;

    /** Random keys. */
    double *conRandoms_;
    
    /** Dense constraint cutoff */
    int denseConCutoff_;
    
    //------------------------------------------------------
    // PARAMETERS, STATISTICS, and MESSAGE
    //------------------------------------------------------

    /** Blis parameters. */
    BlisParams *BlisPar_;
    
    /** Message handler. */
    CoinMessageHandler *blisMessageHandler_;

    /** Blis messages. */
    CoinMessages blisMessages_;

    /** Number of processed nodes. */
    int numNodes_;
    
    /** Number of lp(Simplex) iterations. */
    int numIterations_;

    /** Average number of lp iterations to solve a subproblem. */
    int aveIterations_;
    
    //------------------------------------------------------
    // TEMPORARY STORAGE
    //------------------------------------------------------
    
    /** Tempory storage for var/con indices. */
    //@{
    int *tempVarLBPos_;
    int *tempVarUBPos_;
    int *tempConLBPos_;
    int *tempConUBPos_;
    //@}

    //------------------------------------------------------
    // Knowledge shared
    //------------------------------------------------------

    /** Constraints that can be sent/broadcasted to other processes. */
    BcpsConstraintPool *constraintPoolSend_;
    
    /** Constraints that are received from other processses. */
    BcpsConstraintPool *constraintPoolReceive_;

 public:

    /** If root node. */
    bool isRoot_;

    /** The number of passes during bounding procedure.*/
    int boundingPass_;

    /** Integer tolerance. */
    double integerTol_;

    /** Input relative optimal gap. */
    double optimalRelGap_;

    /** Input absolute optimal gap. */
    double optimalAbsGap_;

    /** Current relative optimal gap. */
    double currRelGap_;

    /** Current absolute optimal gap. */
    double currAbsGap_;

    /** If use heuristics. */
    BlisHeurStrategy heurStrategy_;
    
    /** Frequency of using heuristics. */
    int heurCallFrequency_;
    
    /** Store new cuts in each pass. */
    OsiCuts newCutPool_;
    
    /** Record the path from leaf to root. */
    std::vector<AlpsTreeNode *> leafToRootPath;    

 protected:

    /** Intialize member data */
    void init();

    /** Create variables and constraints. */
    void createObjects();
    
 public:

    /** Default construtor. */
    BlisModel() 
    { 
	init();
    }

    /** Destructor. */
    virtual ~BlisModel();
    
    /** Actual destructor. */
    void gutsOfDestructor();
    
    //------------------------------------------------------
    // SETUP, LP SOLVER
    //------------------------------------------------------

    /** Pass a matrix in */
    void setColMatrix(CoinPackedMatrix *mat){ colMatrix_ = mat; }
    
    /** Pass column upper bounds */
    void setNumCons(int num){ numRows_ = num; }
    
    /** Pass column upper bounds */
    void setNumVars(int num){ numCols_ = num; }
    
    /** Pass column upper bounds */
    void setNumElems(int num){ numElems_ = num; }
    
    /** Pass column upper bounds */
    void setConLb(double *cl){ conLB_ = cl; }
    
    /** Pass column lower bounds */
    void setConUb(double *cu){ conUB_ = cu; }
    
    /** Pass variable upper bounds */
    void setVarLb(double *lb){ varLB_ = lb; }
    
    /** Pass variable lower bounds */
    void setVarUb(double *ub){ varUB_ = ub; }
    
    /** Pass variable types */
    void setColType(char *colType){
        colType_ = colType;
    }
    
    /** Pass objective coefficients */
    void setObjCoef(double *obj){ objCoef_ = obj; }
    
    /** For parallel code, only the master calls this function.
     *  1) Read in the instance data
     *  2) Set colMatrix_, varLB_, varUB_, conLB_, conUB
     *     numCols_, numRows_
     *  3) Set objCoef_ and objSense_
     *  4) Set colType_ ('C', 'I', or 'B')
     *  5) Create variables and constraints
     *  6) Set numCoreVariables_ and numCoreConstraints_
     */
    virtual void readInstance(const char* dataFile);

    /** For parallel code, only the master calls this function.
     *  Import model from vars and cons.
     *  1) Set colMatrix_, varLB_, varUB_, conLB_, conUB
     *     numCols_, numRows_
     *  2) Set objCoef_ (Assume minimization)
     *  3) Set colType_ ('C', 'I', or 'B')
     *  4) Set variables_ and constraints_
     *  5) Set numCoreVariables_ and numCoreConstraints_
     *  NOTE: Blis takes over the memory ownship of vars and cons, which 
     *        means users must NOT free vars or cons.
     */
    virtual void importModel(std::vector<BlisVariable *> vars,
                             std::vector<BlisConstraint *> cons);
    
    /** Read in Alps, Blis parameters. */
    virtual void readParameters(const int argnum, const char * const *arglist);

    /** Write out parameters. */
    virtual void writeParameters(std::ostream& outstream) const;

    /** For parallel code, only the master calls this function.
     *  Create the root node based on model. 
     */
    virtual AlpsTreeNode * createRoot();
    
    /** All processes call this function.
     *  Do necessary work to make model usable. Return success or not. 
     *  1) Set numIntObjects_, intColIndices_, intObjectIndices_
     *	2) Load problem to LP solver.
     *	3) Create integer objects (must after load to lp since using lp info)
     *	4) Set branch strategy
     *	5) Add heuristics
     *	6) Add Cgl cut generators
     */
    virtual bool setupSelf();

    /** Preprocessing the model. */
    virtual void preprocess();

    /** Postprocessing the searching results. */
    virtual void postprocess();
    
    /** Set lp solver. */
    virtual void setSolver(OsiSolverInterface *si) { origLpSolver_ = si; }
    
    /** Get lp solver. */
    virtual OsiSolverInterface *getSolver() { return origLpSolver_; }

    /** Get lp solver. */
    virtual OsiSolverInterface *solver() { return lpSolver_; }

    /** Resolving a lp. */
    bool resolve();

    /** Set active node. */
    void setActiveNode(AlpsTreeNode *node) { activeNode_ = node; }

    /** Set the solution estimate of the active node. */
    void setSolEstimate(double est) { activeNode_->setSolEstimate(est); }
    
    /** Get number of strong branchings. */
    int getNumStrong() { return numStrong_; }
    
    /** Add num to number of strong branchings. */
    void addNumStrong(int num=1) { numStrong_ += num; }

    /** Get the maximum number of resolve during branching. */
    int getNumBranchResolve() { return numBranchResolve_; }
    
    /** Set the maximum number of resolve during branching. */ 
    void setNumBranchResolve(int num) { numBranchResolve_ = num; }

    //------------------------------------------------------
    // PROBLEM DATA
    //------------------------------------------------------

    /** Get objective coefficients. */
    double* getObjCoef() const { return objCoef_; }

    /** Get column lower bound. */
    const double * getColLower() { return lpSolver_->getColLower(); }

    /** Get column upper bound. */
    const double * getColUpper() { return lpSolver_->getColUpper(); }

    /** Get number of columns. */
    int getNumCols() { return lpSolver_->getNumCols(); }

    /** Get number of rows. */
    int getNumRows() { return lpSolver_->getNumRows(); }

    /** Get variable bounds arrary. */
    double *varLB() { return varLB_; }
    double *varUB() { return varUB_; }

    /** Get original constraint bounds arrary. */
    double *conLB() { return conLB_; }
    double *conUB() { return conUB_; }

    /** The starting variable bounds arrary of a subproblem (internal use). */
    double *startVarLB() { return startVarLB_; }
    double *startVarUB() { return startVarUB_; }

    /** The starting constraint bounds arrary of a subproblem (internal use).*/
    double *startConLB() { return startConLB_; }
    double *startConUB() { return startConUB_; }

    /** Temparory storage. */
    int *tempVarLBPos() { return tempVarLBPos_; }
    int *tempVarUBPos() { return tempVarUBPos_; }
    int *tempConLBPos() { return tempConLBPos_; }
    int *tempConUBPos() { return tempConUBPos_; }

    //------------------------------------------------------
    // LP SOLUTION
    //------------------------------------------------------

    /** Get current objective function value. */
    double getLpObjValue() const { return lpSolver_->getObjValue(); }

    /** Get active lp solution. */
    const double * getLpSolution() const { return lpSolver_->getColSolution();}
    
    //------------------------------------------------------
    // MILP SOLUTION
    //------------------------------------------------------

    /** Get number of solutions. */
    int getNumSolutions() const { return numSolutions_; }
    
    /** Get number of heuristic solutions. */
    int getNumHeurSolutions() const { return numHeurSolutions_;}  
    
    /** Return best ip solution found so far. */
    double * incumbent() { return incumbent_; }
    
    /** Record a new incumbent solution and update objectiveValue. */
    int storeSolution(BlisSolutionType how, BlisSolution* sol);
    
    /** Get cut off value. */
    inline double getCutoff() const { return cutoff_;  }

    /** Set cut off value. */
    inline void setCutoff(double co) { 
        double inc = BlisPar_->entry(BlisParams::cutoffInc);        
#if 0
        std::cout << "3. cutoff_ = "<< cutoff_ 
                  << "; inc = " << inc << std::endl;
#endif
        co += inc;
        if (co < cutoff_) {
            cutoff_ = co;
            lpSolver_->setDblParam(OsiDualObjectiveLimit, co);
        }
    }

    /** Test if a solution found by heuristic is feasible. */
    BlisSolution *feasibleSolutionHeur(const double *solution);

    /** Test the current LP solution for feasiblility.
	Scan all objects for indications of infeasibility. This is broken down
	into simple integer infeasibility (\p numIntegerInfs)
	and all other reports of infeasibility(\p numObjectInfs). */
    virtual BlisSolution *feasibleSolution(int & numIntegerInfs, 
                                           int & numObjectInfs);
    
   /** User's criteria for a feasible solution. 
    *  If user think the given solution is feasible then need
    *     1) set userFeasible to true, and
    *     2) return a non-null solution.
    *  If user think the solution is infeasible then need
    *     1) set userFeasible to false, and
    *     2) return a null.
    */
    virtual BlisSolution *userFeasibleSolution(const double * solution, 
                                               bool &feasible) { 
        BlisSolution *sol = NULL;
        feasible = true; // Feasible by default
        return sol; 
    }
    
    //------------------------------------------------------
    // BRANCHING
    //------------------------------------------------------

    /** \name Branching Strategys
	See the BcpsBranchStrategy class for additional information.
    */
    //@{
    /** Get the current branching strategy.*/
    inline BcpsBranchStrategy * branchStrategy() const
	{ return branchStrategy_; }

    /** Set the branching strategy. */
    inline void setBranchingMethod(BcpsBranchStrategy * method) {
        if (branchStrategy_) delete branchStrategy_;
	branchStrategy_ = method; 
    }

    /** Set the branching stratedy. \overload */
    inline void setBranchingMethod(BcpsBranchStrategy & method) { 
        if (branchStrategy_) delete branchStrategy_;
        branchStrategy_ = &method;
    }
    inline BcpsBranchStrategy * rampUpBranchStrategy() const
	{ return rampUpBranchStrategy_; }
    //@}

    /** \name Object manipulation routines
     */
    //@{
    /** Get the number of objects. */
    inline int numObjects() const { return numObjects_; }

    /** Set the number of objects. */
    inline void setNumObjects(int num) { numObjects_ = num; }

    /** Get the array of objects. */
    inline BcpsObject ** objects() { return objects_;}

    /** Get the specified object. */
    inline BcpsObject * objects(int which) { return objects_[which]; }

    /** Mark object to be shared. */
    void setSharedObjectMark(int i) { sharedObjectMark_[i] = 1; }
    
    /** Clear all the share mark. */
    void clearSharedObjectMark() {
        for (int k = 0; k < numIntObjects_; ++k) {
            sharedObjectMark_[k] = 0;
        }
    }
    
    /** Delete all object information. */
    void deleteObjects();

    /** Add in object information.
	Objects are cloned; the owner can delete the originals. */
    void addObjects(int numObjects, BcpsObject ** objects);
    //@}

    /** Identify integer variable. */
    void createIntgerObjects(bool startAgain);

    /** Get integers' object indices. */
    int* getIntObjIndices() const { return intObjIndices_; }

    /** Get number of integers. */
    int getNumIntObjects() const { return numIntObjects_; }

    /** Get integers' column indices. */
    int* getIntColIndices() const { return intColIndices_; }

    /** Check if a value is integer. */
    bool checkInteger(double value) const {
	double integerTolerance = 1.0e-5;
	double nearest = floor(value + 0.5);
	if (fabs(value - nearest) <= integerTolerance) {
            return true;
        }
	else {
            return false;
        }
    }

    void analyzeObjective();
    
    //------------------------------------------------------
    // HEURISTICS.
    //------------------------------------------------------

    /** Add a heuristic. */
    void addHeuristic(BlisHeuristic * heur);
    
    /** Get a specific heuristic. */
    BlisHeuristic * heuristics(int i) const { return heuristics_[i]; }

    /** Get the number of heuristics. */
    int numHeuristics() const { return numHeuristics_; }

    //------------------------------------------------------
    // CONSTRAINTS.
    //------------------------------------------------------

    /** Add a Blis cut generator. */
    void addCutGenerator(BlisConGenerator * generator);

    /** Add a Cgl cut generator. */
    void addCutGenerator(CglCutGenerator * generator,
			 const char * name = NULL,
			 BlisCutStrategy strategy = BlisCutStrategyAuto,
			 int cutGenerationFrequency = 1,
			 bool normal = true, 
			 bool atSolution = false,
			 bool whenInfeasible = false);
    
    /** Get a specific cut generator. */
    BlisConGenerator *cutGenerators(int i) const { return generators_[i]; }

    /** Get the number of cut generators. */    
    int numCutGenerators() const { return numCutGenerators_; }

    /** Get the max number of cuts can be generated. */    
    int getMaxNumCons() const { return maxNumCons_; }

    /** Set the max number of cuts can be generated. */    
    void setMaxNumCons(int m) { maxNumCons_ = m; }

    /** Access constraint pool. */
    BcpsConstraintPool *constraintPool() { return constraintPool_; }

    /** Access receive constraint pool. */
    BcpsConstraintPool *constraintPoolReceive() 
	{ return constraintPoolReceive_; }
    
    /** Access send constraint pool. */
    BcpsConstraintPool *constraintPoolSend() { return constraintPoolSend_; }
    
    //@{
    /** Get number of old constraints. */
    int getNumOldConstraints() const { return numOldConstraints_; }

    /** Set number of old constraints. */
    void setNumOldConstraints(int num) { numOldConstraints_ = num; }
    
    /** Get max number of old constraints. */
    int getOldConstraintsSize() const { return oldConstraintsSize_; }
    
    /** Set max number of old constraints. */
    void setOldConstraintsSize(int num) { oldConstraintsSize_ = num; }

    /** Access old constraints. */
    BlisConstraint **oldConstraints() { return oldConstraints_; }
    
    /** set old constraints. */
    void setOldConstraints(BlisConstraint **old) { oldConstraints_ = old; }

    /** Set max number of old constraints. */
    void delOldConstraints() {
        delete [] oldConstraints_; 
        oldConstraints_ = NULL; 
    }
    //@}
    
    /** Query constraint generation strategy. */
    BlisCutStrategy getCutStrategy() const {
        return cutStrategy_; 
    }

    /** Set constraint generation strategy. */
    void setCutStrategy(BlisCutStrategy u) { cutStrategy_ = u; }

    /** Query constraint generation frequency. */
    int getCutGenerationFrequency() const { return cutGenerationFrequency_; }

    /** Set constraint generation frequency. */
    void setCutStrategy(int f) { cutGenerationFrequency_ = f; }

    /** Get the thresheld to be considered as a dense constraint. */
    int getDenseConCutoff() const { return denseConCutoff_; }

    /** Set the thresheld to be considered as a dense constraint. */
    void setDenseConCutoff(int cutoff) { denseConCutoff_ = cutoff; }
    
    /** Get randoms for check parallel constraints. */
    double *getConRandoms() const { return conRandoms_; }
    
    //------------------------------------------------------
    // PRIORITY AND WEITGHT.
    //------------------------------------------------------

    /** Pass in branching priorities.
	If ifClique then priorities are on cliques otherwise
	priorities are on integer variables.  
	Other type (if exists set to default)
	1 is highest priority. (well actually -INT_MAX is but that's ugly)
	If hotstart > 0 then branches are created to force
	the variable to the value given by best solution.  This enables a
	sort of hot start.  The node choice should be greatest depth
	and hotstart should normally be switched off after a solution.
	
	If ifNotSimpleIntegers true then appended to normal integers
	
	\internal Added for Kurt Spielberg.
    */
    void passInPriorities(const int * priorities, 
			  bool ifNotSimpleIntegers,
			  int defaultValue = 1000);
    
    /** Priorities. */
    inline const int * priority() const { return priority_; }
    
    /** Returns priority level for an object (or 1000 if no priorities exist)*/
    inline int priority(int sequence) const { 
	if (priority_) return priority_[sequence];
	else return 1000;
    }
    
    inline double getNodeWeight() const { return nodeWeight_; }

    inline void setNodeWeight(double nw) { nodeWeight_ = nw; }
    //@}

    //------------------------------------------------------
    // STATISTICS.
    //------------------------------------------------------

    /** Log of specific models. */
    virtual void modelLog();
    
    /** Get how many Nodes it took to solve the problem. */
    int getNumNodes() const { return numNodes_; }

    /** Get how many iterations it took to solve the problem. */
    int getNumIterations() const { return numIterations_; }

    /** Get the average iterations it took to solve a lp. */
    int getAveIterations() const { return aveIterations_; }

    /** Increment node count. */
    void addNumNodes(int newNodes = 1) { numNodes_ += newNodes; }

    /** Increment Iteration count. */
    void addNumIterations(int newIter) {
        numIterations_ += newIter; 
        aveIterations_ = numIterations_ / numNodes_;
    }

    /** Get the message handler. */
    CoinMessageHandler * blisMessageHandler() const 
    { return blisMessageHandler_; }
    
    /** Return messages. */
    CoinMessages blisMessages() { return blisMessages_; }
    
    /** Access parameters. */
    //@{
    BlisParams * BlisPar() { return BlisPar_; }
    //@}

    /** Node log. */
    virtual void nodeLog(AlpsTreeNode *node, bool force);

    /** Return true, if all nodes can be fathomed.*/
    virtual bool fathomAllNodes();

    //------------------------------------------------------
    // PARALLEL
    //------------------------------------------------------

 protected:
    
    /** Pack Blis portion of the model into an encoded object. */
    AlpsReturnStatus encodeBlis(AlpsEncoded *encoded) const;

    /** Unpack Blis portion of the model from an encoded object. */
    AlpsReturnStatus decodeBlis(AlpsEncoded &encoded);  

    /** Retrieve and pack shared pseudocost. */
    void packSharedPseudocost(AlpsEncoded *encoded, int numToShare);

    /** Unpack and store shared pseduocost */
    void unpackSharedPseudocost(AlpsEncoded &encoded);
    
    /** Retrieve and pack shared constraints. */
    void packSharedConstraints(AlpsEncoded *encoded);

    /** Unpack and store shared constraints. */
    void unpackSharedConstraints(AlpsEncoded &encoded);
 
    /** Retrieve and pack shared variables. */
    void packSharedVariables(AlpsEncoded *encoded);

    /** Unpack and store shared variables. */
    void unpackSharedVariables(AlpsEncoded &encoded);  

 public:

    /** Register knowledge. */

    virtual void registerKnowledge();  

    using AlpsKnowledge::encode ;
    /** The method that encodes the model into an encoded object. */
    virtual AlpsEncoded* encode() const;
    
    /** The method that decodes the model from an encoded object. */
    virtual void decodeToSelf(AlpsEncoded&);
    
    /** Pack knowledge to be shared with others into an encoded object. 
        Return NULL means that no knowledge can be shared. */
    virtual AlpsEncoded* packSharedKnowlege();
    
    /** Unpack and store shared knowledge from an encoded object. */
    virtual void unpackSharedKnowledge(AlpsEncoded&);

    //AT - Begin
    virtual void presolveForTheWholeTree();
    //AT - end
};

#endif /* End of file */
