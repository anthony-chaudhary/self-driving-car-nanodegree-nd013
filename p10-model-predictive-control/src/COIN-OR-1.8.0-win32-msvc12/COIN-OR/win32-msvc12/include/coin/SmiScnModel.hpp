// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef SmiScnModel_HPP
#define SmiScnModel_HPP

// Include files
#include "CoinPragma.hpp"
#include "SmiDiscreteDistribution.hpp"
#include "SmiScenarioTree.hpp"
#include "SmiScnData.hpp"
#include "OsiSolverInterface.hpp"
#include "CoinPackedVector.hpp"
#include "SmiMessage.hpp"
#include "ClpModel.hpp"


// STL declarations
#include <vector>

// forward declaration of SmiScnNode
class SmiScnNode;


//#############################################################################

/** SmiScnModel: COIN-SMI Scenario Model Class

Concrete class for generating scenario stochastic linear programs.

This class implements the Scenarios format of the Stochastic MPS
modeling system (TODO: web pointer).  Core data and Scenarios data
can be passed using COIN/OSI structures, or can be read from SMPS
formatted files.

Typical driver fragment looks like this
\code
SmiScnModel smi;
smi.readSmps("app0110R");
smi.setOsiSolverHandle(new OsiClpSolverInterface());
OsiSolverInterface *osiStoch = smi.loadOsiSolverData();
osiStoch->initialSolve();
\endcode

The setOsiSolverHandle method allows the user to pass in any OSI
compatible solver.

*/
class SmiScnModel
{
    friend void SmiScnModelDiscreteUnitTest();
    friend void DecompUnitTest();
public:

    /**@name Read SMPS files.

    There should be three files: {name}.[core, time, stoch].
    If you have different extension conventions, then you can
    hack the method yourself.
    The files can be compressed. The object that reads the files is
    derived from CoinMpsIO.

    The optional argument SmiCoreCombineRule allows user to pass in
    a class to override the default methods to combine core and stochastic data.

    */
    int readSmps(const char *name,
        SmiCoreCombineRule *r=NULL);

    /**@name Writes SMPS files.
    
    This method generates three files {name}.[core, time, stoch] or {name}.[cor, tim, sto] (see second parameter).
    
    @param name The name for the model and the written files
    
    @param winFileExtensions optional; false by default, so extensions will be [core, time, stoch]. With this parameter set to true, it will be [cor, tim, sto].
    
    @param strictFormat optional, true by default. Set to false if SMPS files should be written in free format.
    
    @return -1 in case of no existing SMI model, otherwise 0
    
    */
    int writeSmps(const char *name, bool winFileExtensions = false, bool strictFormat = true);

    SmiCoreData * getCore() {return core_;}

    /**@name Direct methods.

    Direct methods require the user to create instances of
    Core data and Scenario data.
    Currently, the dimension of the core nodes determines the
    dimension of the scenario nodes, but this is something that
    could easily be changed.
    */

    //@{



    /// generate scenarios from discrete distribution
    void processDiscreteDistributionIntoScenarios(SmiDiscreteDistribution *s, bool test=false);

    void setModelProb(double p) {totalProb_=p; }

    int addNodeToSubmodel(SmiScnNode * smiScnNode);

    /** generate scenario with ancestor/branch node identification

    Core argument must be supplied.
    Data values combine with corresponding core values,
    if found, or creates them if not.

    Scenario nodes need to have same dimensions as core nodes.

    Data field arguments can be NULL, or empty.

    branch, anc, arguments must be supplied.  These
    identify the branching node according to the Stochastic MPS
    standard.

    prob is unconditional probability of scenario

    */
    SmiScenarioIndex generateScenario(SmiCoreData *core,
        CoinPackedMatrix *matrix,
        CoinPackedVector *dclo, CoinPackedVector *dcup,
        CoinPackedVector *dobj,
        CoinPackedVector *drlo, CoinPackedVector *drup,
        SmiStageIndex branch, SmiScenarioIndex anc, double prob,
        SmiCoreCombineRule *r = SmiCoreCombineReplace::Instance());

    /** generate scenario with labels information

    Core argument must be supplied.
    Data values combine with corresponding core values,
    if found, or creates them if not.

    Scenario nodes need to have same dimensions as core nodes.

    Data field arguments can be NULL, or empty.

    Labels are passed as vector<int> array.
    Adds new path using labels to find branching node.
    The depth (root to leaf) of new path is labels.size().

    */
    SmiScenarioIndex generateScenario(SmiCoreData *core,
        CoinPackedMatrix *matrix,
        CoinPackedVector *dclo, CoinPackedVector *dcup,
        CoinPackedVector *dobj,
        CoinPackedVector *drlo, CoinPackedVector *drup,
        std::vector<int>labels, double prob,
        SmiCoreCombineRule *r = SmiCoreCombineReplace::Instance());

	    /** generate scenario with ancestor/branch node identification

    Core argument must be supplied.
    Data values combine with corresponding core values,
    if found, or creates them if not.

    Scenario nodes need to have same dimensions as core nodes.

    Data field arguments can be NULL, or empty.

    branch, anc, arguments must be supplied.  These
    identify the branching node according to the Stochastic MPS
    standard.

    prob is unconditional probability of scenario

    */
    SmiScenarioIndex generateScenario(
        CoinPackedMatrix *matrix,
        CoinPackedVector *dclo, CoinPackedVector *dcup,
        CoinPackedVector *dobj,
        CoinPackedVector *drlo, CoinPackedVector *drup,
        SmiStageIndex branch, SmiScenarioIndex anc, double prob,
        SmiCoreCombineRule *r = SmiCoreCombineReplace::Instance());

    /** generate scenario with labels information

    Core argument must be supplied.
    Data values combine with corresponding core values,
    if found, or creates them if not.

    Scenario nodes need to have same dimensions as core nodes.

    Data field arguments can be NULL, or empty.

    Labels are passed as vector<int> array.
    Adds new path using labels to find branching node.
    The depth (root to leaf) of new path is labels.size().

    */
    SmiScenarioIndex generateScenario(
        CoinPackedMatrix *matrix,
        CoinPackedVector *dclo, CoinPackedVector *dcup,
        CoinPackedVector *dobj,
        CoinPackedVector *drlo, CoinPackedVector *drup,
        std::vector<int>labels, double prob,
        SmiCoreCombineRule *r = SmiCoreCombineReplace::Instance());

	SmiScenarioIndex generateScenarioFromCore(SmiCoreData *core, double prob, 
        SmiCoreCombineRule *r = SmiCoreCombineReplace::Instance());
	SmiScenarioIndex generateScenarioFromCore(double prob,
        SmiCoreCombineRule *r = SmiCoreCombineReplace::Instance());

	/**@name loadOsiSolverData

    Loads deterministic equivalent model into internal osi data structures
    and return handle.

    Note: this uses a callback class SmiCoreCombineRule to decide how to combine
    the core and stochastic data. The user can override the default
    methods when the scenario is generated (see SmiScnModel::generateScenario)
    or when SMPS files are processed (see SmiScnModel::readSmps).

    */
    OsiSolverInterface * loadOsiSolverData();
    OsiSolverInterface * loadOsiSolverDataForSubproblem(int stage, int scenStart);

    std::vector< std::pair<double,double> > solveWS(OsiSolverInterface *osiSolver, double objSense); //Returns value of Wait-And-See solution, with objSense of 1 (= minimization) for default
    std::pair<double,double*> solveEV(OsiSolverInterface *osiSolver, double objSense);
    double solveEEV(OsiSolverInterface *osiSolver, double objSense);

    double getWSValue(OsiSolverInterface *osiSolver, double objSense);
    double getEVValue(OsiSolverInterface* osiSolver, double objSense);
    double getEEVValue(OsiSolverInterface* osiSolver, double objSense);

    //Let SmiScnModel own core data
    void setCore(SmiCoreData * val) { core_ = val; }

    // get scenario problem data
    SmiScenarioIndex getNumScenarios(){ return smiTree_.getNumScenarios();}
    double getScenarioProb(SmiScenarioIndex ns);
    SmiScnNode * getLeafNode(SmiScenarioIndex i){ return smiTree_.getLeaf(i)->getDataPtr(); }
    SmiScnNode * getRootNode(){ return smiTree_.getRoot()->getDataPtr(); }

    int* getIntegerInd(){ return core_->getIntegerIndices();}
    int getIntegerLen(){return core_->getIntegerLength(); }
    int* getBinaryInd(){return core_->getBinaryIndices();}
    int getBinaryLen(){return core_->getBinaryLength();}

    inline std::vector<int> getIntIndices() {
        return intIndices;
    }
    inline void addIntIndice(int indice) {
        intIndices.push_back(indice);
    }

    // getXXX by scenario
    double getObjectiveValue(SmiScenarioIndex ns);
    double *getColSolution(SmiScenarioIndex ns, int *length);
    double *getRowSolution(SmiScenarioIndex ns, int *length);
	double *getRowDuals(SmiScenarioIndex ns, int *length);
    double getColSolution(SmiScenarioIndex ns, int stage, int colIndex);
    double getRowSolution(SmiScenarioIndex ns, int stage, int rowIndex);
	double getRowDuals(SmiScenarioIndex ns, int stage, int rowIndex);

	// base classes for getting values from solved SmiScnModel
	double *getColValue(const double *d, SmiScenarioIndex ns, int*length);
	double  getColValue(const double *d, SmiScenarioIndex ns, int stage, int rowIndex);
	double *getRowValue(const double *d, SmiScenarioIndex ns, int*length, bool isDual);
	double  getRowValue(const double *d, SmiScenarioIndex ns, int stage, int rowIndex, bool isDual);

    // OsiSolverInterface
    void setOsiSolverHandle(OsiSolverInterface &osi)
    {
        osiStoch_ = osi.clone(false);
    }
    void setOsiSolverHandle(OsiSolverInterface *osi)
    {
        osiStoch_ = osi->clone(false);
    }
    OsiSolverInterface * getOsiSolverInterface();
    // If user wants to delete SmiScnModel but keep OsiSolverInterface
    inline void releaseSolver() {osiStoch_=NULL;}
    inline void releaseCore() { core_=NULL; }

	//Quadratic
	void setQuadraticSolver(ClpModel *clp){ clp_=clp; }
	ClpModel * getQuadraticSolver() {return clp_;}
	ClpModel * loadQuadraticSolverData();


    // constructor: Lesson from Effective C++: Initialize values in the same order as declared in .hpp file.
    SmiScnModel():
    handler_(NULL),messages_(NULL),osiStoch_(NULL), nrow_(0), ncol_(0), nels_(0),nels_max(0),
        drlo_(NULL), drup_(NULL), dobj_(NULL), dclo_(NULL), dcup_(NULL), matrix_(NULL),
        dels_(NULL),indx_(NULL),rstrt_(NULL),minrow_(0),
        solve_synch_(false),totalProb_(0),core_(NULL),smiTree_(),integerInd(NULL),integerLen(0),binaryInd(NULL),binaryLen(0),intIndices(),maxNelsPerScenInStage(NULL)
    {
		nqels_=0;
	}

    // destructor
    ~SmiScnModel();
    //Christian: this method adds data of current node to big det. eq. problem
    void addNode(SmiScnNode *node, bool notDetEq = false);
    void deleteNode(SmiScnNode *tnode);

    void addNode(SmiNodeData *node);
    inline SmiScenarioTree<SmiScnNode *> * getSmiTree() { return &smiTree_; }
private:
    CoinMessageHandler *handler_;
    SmiMessage *messages_;

    // internal clone of user declared OSI
    OsiSolverInterface * osiStoch_;
    // model statistics useful for predefining size of structures
    int nrow_;
    int ncol_;
    int nels_;
    int nels_max;
    // data pointers used in AddNode and loadOsiSolverData aka generateDetEq
    double *drlo_;
    double *drup_;
    double *dobj_;
    double *dclo_;
    double *dcup_;
    CoinPackedMatrix *matrix_;
    double *dels_;
    int    *indx_;
    int    *rstrt_;
	//Quadratic
	int nqels_;
	int *qstart_;
	int *qindx_;
	double *qdels_;
	ClpModel *clp_;

    // number of scenarios
    //	int scen_;
    // not sure if this is used
    int minrow_;
    // not sure if this is used
    bool solve_synch_;
    // total probability of added scenarios; used to normalize probability to one
    double totalProb_;
    //core model --- used for discrete distributions

private:
    SmiCoreData * core_;

	///constructs LP and QP data arrays for Deterministic Equivalent
	void generateSolverArrays();

    // scenario tree 
    SmiScenarioTree<SmiScnNode *> smiTree_;

    int* integerInd;
    int integerLen;
    int* binaryInd;
    int binaryLen;

    std::vector<int> intIndices;
    int* maxNelsPerScenInStage;
};

class SmiScnNode
{
    friend class SmiScnModel;
    friend void DecompUnitTest();
public:
    int getCoreColIndex(int i);
    int getCoreRowIndex(int i);
    inline void setScenarioIndex(SmiScenarioIndex i){ scen_=i;}
    inline SmiScenarioIndex getScenarioIndex() {return scen_;}
    inline int  getColStart() {return coffset_;}
    inline int  getRowStart() {return roffset_;}
    inline int getNumCols(){ return node_->getCore()->getNumCols(node_->getStage());}
    inline int getNumRows(){ return node_->getCore()->getNumRows(node_->getStage());}
    //inline int getNumElements() { return node_->getCore()->getNumElements(node_>getStage());}
    inline double getModelProb(){return mdl_prob_;}
    inline SmiStageIndex getStage() { return node_->getStage(); }
    inline SmiScnNode * getParent(){ return parent_;}
    inline void setProb(double p){prob_=p;}
    inline double addProb(double prob){ return prob_+=prob;}
    inline double getProb() { return prob_; }
    inline void setParent(SmiScnNode * g) {parent_=g;}

    inline bool isVirtualNode() { return !include_; }
    // So can delete root node
    inline void zapNode() {node_=NULL;}


    inline SmiScnNode(SmiNodeData *node) 
    {
        node_=node;
        node->addPtr();
        prob_=0;
        condProb_ = 0;
        parent_=NULL;
        scen_=-1;
        include_=true;
    }
    ~SmiScnNode()
    {
        if (node_)
        {
            //Only delete non-core nodes. Core nodes get deleted via SmiCoreData..
            if (!node_->isCoreNode()){
                delete node_;
                node_= NULL;
            }
       	}
         
    }

private:
    inline void setRowOffset(int r) {roffset_ = r;}
    inline void setColOffset(int c) {coffset_ = c;}

    inline double getCondProb(){return condProb_;}
    inline void setCondProb(double p){condProb_=p;}
    inline void setModelProb(double p){mdl_prob_=p;}
    //Necessary to get access to the data node stored within the SmiScnNode
public:
    inline SmiNodeData *getNode() {return node_;}
private:
    inline void setIncludeOff() { include_=false; }
    inline void setIncludeOn()  { include_=true; }
    inline bool getInclude() { return include_; }



private:
    SmiNodeData *node_;
    SmiScnNode *parent_;
    double prob_;
    double mdl_prob_;
    double condProb_;
    int coffset_;
    int roffset_;
    SmiScenarioIndex scen_;
    bool include_;
};

// function object for addnode loop
class SmiScnModelAddNode{
public:
    void operator() (SmiScnNode *node)
    {
        s_->addNode(node);
    }

    SmiScnModelAddNode(SmiScnModel *s) { s_ = s;}
private:
    SmiScnModel *s_;


};

// function object for deleteNode loop
class SmiScnModelDeleteNode{
public:
    void operator() (SmiScnNode *node)
    {
        s_->deleteNode(node);
    }

    SmiScnModelDeleteNode(SmiScnModel *s) { s_ = s;}
private:
    SmiScnModel *s_;


};

#endif //#define SmiScnModel_HPP
