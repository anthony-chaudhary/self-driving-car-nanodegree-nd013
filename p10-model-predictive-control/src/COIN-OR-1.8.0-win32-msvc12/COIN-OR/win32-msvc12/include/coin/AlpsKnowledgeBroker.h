/*===========================================================================*
 * This file is part of the Abstract Library for Parallel Search (ALPS).     *
 *                                                                           *
 * ALPS is distributed under the Eclipse Public License as part of the       *
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
 *===========================================================================*/

#ifndef AlpsKnowledgeBroker_h_
#define AlpsKnowledgeBroker_h_

#include <cmath>
#include <iosfwd>
#include <map>
#include <string>

#include "CoinMessageHandler.hpp"

#include "AlpsSearchStrategy.h"
#include "AlpsEnumProcessT.h"
#include "AlpsKnowledge.h"
#include "AlpsKnowledgePool.h"
#include "AlpsMessage.h"
#include "AlpsParams.h"
#include "AlpsSolutionPool.h"
#include "AlpsSubTree.h"
#include "AlpsSubTreePool.h"
#include "AlpsModel.h"
#include "AlpsTime.h"

//#############################################################################

/** The base class of knowledge broker class. */
class AlpsKnowledgeBroker {

 private:

    AlpsKnowledgeBroker(const AlpsKnowledgeBroker&);
    AlpsKnowledgeBroker& operator=(const AlpsKnowledgeBroker&);

    /** Stores registered knowledge. */
    std::map<int, AlpsKnowledge*> decodeMap_;
    
 protected:

    /** The instance name. */
    std::string instanceName_;

    /** Pointer to model. */
    AlpsModel *model_;

    /** Alps phase. (RAMPUP, SEARCH, RAMPDOWN)*/
    AlpsPhase phase_;

    /** @name knowledge pools
     *
     */
    //@{
    /** A subtree pool holding a collection of subtrees. For serial version,
	there is only one subtree in the pool. */
    AlpsSubTreePool* subTreePool_;

    /** A solution pool containing the solutions found. */
    AlpsSolutionPool* solPool_;

    /** The collection of pools managed by the knowledge broker. */
    std::map<AlpsKnowledgeType, AlpsKnowledgePool*>* pools_;
    //@}

    /** @name Exploring subtree 
     *
     */
    //@{
    /** Point to the subtree that being explored. */
    AlpsSubTree* workingSubTree_;
    
    /** Indicate whether need a new subtree. */
    bool needWorkingSubTree_;

    /** The index to be assigned to a new search tree node. */
    AlpsNodeIndex_t nextIndex_;

    /** The maximum index can been assigned on this process. */
    AlpsNodeIndex_t maxIndex_;
    //@}                       

    /** @name Statistics
     *
     */
    //@{
    /***/
    /** Main timer. Do not touch. */
    AlpsTimer timer_;

    /** Subtree timer.  Do not touch.*/
    AlpsTimer subTreeTimer_;

    /** Secondary timer. */
    AlpsTimer tempTimer_;

    /** The number of solutions found. */
    int solNum_;

    /** The number of nodes that have been processed. */
    int nodeProcessedNum_;

    /** The number of nodes that have been branched. */
    int nodeBranchedNum_;

    /** The number of nodes that have been discarded before processing. */
    int nodeDiscardedNum_;

    /** The number of nodes that are pregnant. */
    int nodePartialNum_;

    /** To record how many nodes processed by the system
        (used in parallel code). */
    int systemNodeProcessed_;
    
    /** The number of nodes left. */
    int nodeLeftNum_;
    
    /** The depth of the tree. */
    int treeDepth_;

    /** The number of nodes pocessed to find the solution. */
    int bestSolNode_;

    /** Peak memory usage. */
    double peakMemory_;

    /** The status of search when terminated. */
    AlpsExitStatus exitStatus_;
    //@}
    
    /** @name Search strategy
     *
     */
    //@{
    /** Tree selection criterion. */
    AlpsSearchStrategy<AlpsSubTree*>* treeSelection_;

    /** Node selection criterion. */
    AlpsSearchStrategy<AlpsTreeNode*>* nodeSelection_;

    /** Node selection criterion. */
    AlpsSearchStrategy<AlpsTreeNode*>* rampUpNodeSelection_;
    //@}
    
    /** @name message handling
     *
     */
    //@{
    /** Message handler. */
    CoinMessageHandler * handler_;

    /** Alps messages. */
    CoinMessages messages_;

    /** The leve of printing message to screen of the master and general message. 
        (0: no; 1: basic; 2: moderate, 3: verbose) */
    int msgLevel_;

    /** The leve of printing message to screen of hubs. 
        (0: no; 1: basic; 2: moderate, 3: verbose) */
    int hubMsgLevel_;

    /** The leve of printing message to screen of workers. 
        (0: no; 1: basic; 2: moderate, 3: verbose) */
    int workerMsgLevel_;
    
    /** The degree of log file.
        (0: no; 1: basic; 2: moderate, 3: verbose) */
    int logFileLevel_;

    /** The log file. */
    std::string logfile_;
    //@}

    /** The approximate memory size (bytes) of a node with full description. */
    int nodeMemSize_;

    /** The approximately CPU time to process a node. */
    double nodeProcessingTime_;

    /** The size of largest message buffer can be sent or received. */
    int largeSize_;

    /** Has user input balance period */
    bool userBalancePeriod_;
    
    /** Times that node log is printed. */
    int numNodeLog_;
    
 public:

    /** Default constructor. */
    AlpsKnowledgeBroker();

    /** Destructor. */
    virtual ~AlpsKnowledgeBroker();

    //-------------------------------------------------------------------------
    /** @name Funcitons related to register knowledge. 
     *
     */
    //@{
    /** Every user derived knowledge class must register. 
	The register methods register the decode method of the class so that 
	later on we can decode objects from buffers. Invoking this 
	registration for class <code>foo</code> is a single line:<br>
	<code>foo().registerClass(name, userKnowledge)</code>.
        NOTE: take over user knowledge's memory ownership, user doesn't
        need free memory. 
    */
    void registerClass(int name, AlpsKnowledge* userKnowledge) {
        // Check if alread have one.
        std::map<int, AlpsKnowledge*>::iterator pos, pos1;
        pos = decodeMap_.find(name);
        pos1 = decodeMap_.end();
        
        if (pos != pos1) {
            AlpsKnowledge* kl = pos->second;
            decodeMap_.erase(pos);
            delete kl;
        }
        
        decodeMap_[name] = userKnowledge;
    }

    /** This method returns the pointer to an empty object of the registered
	class <code>name</code>. Then the <code>decode()</code> method of that
	object can be used to decode a new object of the same type from the
	buffer. This method will be invoked as follows to decode an object
	whose type is <code>name</code>:<br>
	<code>obj = AlpsKnowledge::decoderObject(name)->decode(buf) </code> 
    */

    const AlpsKnowledge* decoderObject(int name) {
	return decodeMap_[name];
    }
    //@}

    //------------------------------------------------------

    /** @name Funcitons related to exploring subtree. 
     *  
     */
    //@{
    /** Do some initialization for search. */
    virtual void initializeSearch(int argc, 
				  char* argv[], 
				  AlpsModel& model) = 0;

    /** Explore the tree rooted as the given root. */
    virtual void rootSearch(AlpsTreeNode* root) = 0;  

    /** Search best solution for a given model. */
    virtual void search(AlpsModel *model) {   
	AlpsTreeNode* root = model->createRoot();
	rootSearch(root);
    }
    //@}

    //------------------------------------------------------

    /** @name Get/set phase.
     *  
     */
    //@{
    AlpsPhase getPhase() { return phase_; }
    void setPhase(AlpsPhase ph) { phase_ = ph; }
    //@}

    //@{
    AlpsModel *getModel() { return model_; }
    void setModel(AlpsModel *m) { model_ = m; }
    //@}

    /** Get tree depth */
    int getTreeDepth() { return treeDepth_; }

    /** Set peak memory usage. */
    void setPeakMemory(double size) { peakMemory_ = size; }

    /** Get peak memory usage. */
    double getPeakMemory() { return peakMemory_; }
    
    /** @name Interface with the knowledge pools 
     *  
     */
    //@{
    /** Set up knowledge pools for this broker. */
    void setupKnowledgePools();
    
    /** Add a knowledge pool into the Knowledge pools */
    inline void addKnowledgePool(AlpsKnowledgeType kt, AlpsKnowledgePool* kp) {
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    // AlpsKnowledgePool* akp = static_cast<AlpsKnowledgePool*>(kp);
	    pools_->insert
		(std::pair<AlpsKnowledgeType, AlpsKnowledgePool*>(kt, kp));
	}
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "addKnowledgePool()", "AlpsKnowledgeBroker"); 
        }
    }
  
    /** Retrieve a knowledge pool in the Knowledge base */
    inline  AlpsKnowledgePool* getKnowledgePool(AlpsKnowledgeType kt) const { 
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    return (*pools_)[kt];
        }
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "getKnowledgePool()", "AlpsKnowledgeBroker"); 
        }
    }

    /** Query the number of knowledge in the given type of a knowledge pool. */
    virtual int getNumKnowledges(AlpsKnowledgeType kt) const;
    
    /** Query the max number of knowledge can be stored in a given 
	type of knowledge pools. */
    virtual int getMaxNumKnowledges(AlpsKnowledgeType kt) const {
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    return getKnowledgePool(kt)->getMaxNumKnowledges();
        }
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "getMaxNumKnowledges()", "AlpsKnowledgeBroker"); 
        }
    }

    /** Set the max number of knowledge can be stored in a given 
	type o fknowledge pools. */
    virtual void setMaxNumKnowledges(AlpsKnowledgeType kt, int num) {
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    getKnowledgePool(kt)->setMaxNumKnowledges(num);
        }
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "setMaxNumKnowledges()", "AlpsKnowledgeBroker"); 
        }
    }

    /** Query whether there are knowledges in the given type of 
	knowledge pools. */
    virtual bool hasKnowledge(AlpsKnowledgeType kt) const {
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree)
	    return getKnowledgePool(kt)->hasKnowledge();
	else
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "hasKnowledge()", "AlpsKnowledgeBroker"); 
    }

    /** Get a knowledge, but doesn't remove it from the pool*/
    virtual std::pair<AlpsKnowledge*, double> 
	getKnowledge(AlpsKnowledgeType kt) const {
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    return getKnowledgePool(kt)->getKnowledge();
        }
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "getKnowledge()", "AlpsKnowledgeBroker"); 
        }
    }

    /** Remove the a knowledge from the given type of knowledge pools.*/
    virtual void popKnowledge(AlpsKnowledgeType kt) {
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    getKnowledgePool(kt)->popKnowledge();
        }
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "popKnowledge()", "AlpsKnowledgeBroker"); 
        }
    } 

    /** Get the best knowledge in the given type of knowledge pools. */
    virtual std::pair<AlpsKnowledge*, double> 
	getBestKnowledge(AlpsKnowledgeType kt) const;

    /** Get all knowledges in the given type of knowledge pools. */
    virtual void getAllKnowledges (AlpsKnowledgeType kt, 
				   std::vector<std::pair<AlpsKnowledge*, 
				   double> >& kls)  const { 
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    getKnowledgePool(kt)->getAllKnowledges(kls);
        }
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "popKnowledge()", "AlpsKnowledgeBroker"); 
        }
    }

    /** Add a knowledge in the given type of knowledge pools. */
    virtual void addKnowledge(AlpsKnowledgeType kt, 
			      AlpsKnowledge* kl, 
			      double value ) { 
	if(kt == AlpsKnowledgeTypeSolution || kt == AlpsKnowledgeTypeSubTree) {
	    getKnowledgePool(kt)->addKnowledge(kl, value);
        }
	else {
	    throw CoinError("Broker doesn't manage this type of knowledge", 
			    "popKnowledge()", "AlpsKnowledgeBroker"); 
        }
    }
    //@}

    /** @name Querty and set statistics
     *
     */
    /** Query the number of node processed by this process. */
    int getNumNodesProcessed() const {
	return nodeProcessedNum_;
    }

    /** Query the number of node processed by this process. */
    int getNumNodesBranched() const {
	return nodeBranchedNum_;
    }

    /** Query the number of node processed by this process. */
    int getNumNodesDiscarded() const {
	return nodeDiscardedNum_;
    }

    /** Query the number of node in the queue that are pregnant. */
    int getNumNodesPartial() const {
	return nodePartialNum_;
    }

    /** Query the number of node processed by the system. */
    int getNumNodesProcessedSystem() const {
	return systemNodeProcessed_;
    }

    /** Update the number of left nodes on this process. */
    virtual int updateNumNodesLeft();
    
    /** Query the best node in the subtree pool. Return NULL if no node exits. */
    virtual AlpsTreeNode* getBestNode() const;

    /** Query search termination status. */
    AlpsExitStatus getSolStatus() const {
	return exitStatus_;
    }

    /** Set terminate status. */
    void setExitStatus(AlpsExitStatus status) {
	exitStatus_ = status;
    }

    /** Query timer. */
    AlpsTimer & timer() {
	return timer_;
    }

    /** Query subtree timer. */
    AlpsTimer & subTreeTimer() {
	return subTreeTimer_;
    }

    /** Query secondary timer. */
    AlpsTimer & tempTimer() {
	return tempTimer_;
    }

    /** Search statistics log. */
    virtual void searchLog() = 0;
    //@}

    /** @name Query and set the approximate memory size of a tree node
     *
     */
    //@{
    int getNodeMemSize() { return nodeMemSize_; }
    void setNodeMemSize(int ms) { nodeMemSize_ = ms; }
    //@}

    /** @name Query and set the approximate node processing time
     *
     */
    //@{
    double getNodeProcessingTime() { return nodeProcessingTime_; }
    void setNodeProcessingTime(double npTime) { nodeProcessingTime_ = npTime; }
    //@}

    int getLargeSize() const { return largeSize_; }

    /** @name Report the best result
     *  
     */
    //@{
    /** The process queries the objective value of the incumbent that 
	it stores.*/
    virtual double getIncumbentValue() const = 0;

    /** The process (serial) / the master (parallel) queries the quality
        of the best solution that it knows. */
    virtual double getBestQuality() const = 0;

    /** Get best estimalted quality in system. */
    virtual double getBestEstimateQuality() { return ALPS_OBJ_MAX; }

    virtual int getNumNodeLeftSystem(){ return nodeLeftNum_; }

    /** The process (serial) / the master (parallel) outputs the best 
	solution that it knows to a file or std::out. */
    virtual void printBestSolution(char* outputFile = 0) const = 0;
    //@}

    /** Qeury the global rank of process. Note: not useful for serial code.*/
    virtual int getProcRank() const { return 0; }
    
    /** Query the global rank of the Master. */
    virtual int getMasterRank() const { return 0; }

    /** Query the type (master, hub, or worker) of the process */
    virtual AlpsProcessType getProcType() const
    { return AlpsProcessTypeSerial; } /* Default is serial */
    
    /** @name Query and set node index
     *  
     */
    //@{
    /** Query the next index assigned to a newly created node, and then
	increment the nextIndex_ by 1. */
    AlpsNodeIndex_t nextNodeIndex() { return nextIndex_++; }

    /** Query the next index assigned to a newly created node. */
    AlpsNodeIndex_t getNextNodeIndex() const { return nextIndex_; }

    /** Set nextIndex_. */
    void setNextNodeIndex(AlpsNodeIndex_t s) { nextIndex_ = s; } 

    /** Queriy the upper bound of node indices. */
    AlpsNodeIndex_t getMaxNodeIndex() const { return maxIndex_; }
  
    /** Set the upper bound of node indices. */
    void setMaxNodeIndex(AlpsNodeIndex_t s) { maxIndex_ = s; }
    //@}   

    /** @name Query and set comparision
     *  
     */
    //@{
    AlpsSearchStrategy<AlpsSubTree*>* getSubTreeSelection() const { 
	return treeSelection_; 
    }
    void setSubTreeSelection(AlpsSearchStrategy<AlpsSubTree*>* tc) {
        if (treeSelection_) delete treeSelection_;
	treeSelection_ = tc;
	subTreePool_->setComparison(*treeSelection_);
    }
    AlpsSearchStrategy<AlpsTreeNode*>* getNodeSelection() const {
	return nodeSelection_;
    }
    void setNodeSelection(AlpsSearchStrategy<AlpsTreeNode*>* nc) {
        if (nodeSelection_) delete nodeSelection_;
	nodeSelection_ = nc;
    }
    AlpsSearchStrategy<AlpsTreeNode*>* getRampUpNodeSelection() const {
	return rampUpNodeSelection_;
    }
    void setRampUpNodeSelection(AlpsSearchStrategy<AlpsTreeNode*>* nc) {
        if (rampUpNodeSelection_) delete rampUpNodeSelection_;
	rampUpNodeSelection_ = nc;
    }
    //@}
   
    /**@name Message and log file handling */
    //@{
    /** Pass in Message handler (not deleted at end). */
    void passInMessageHandler(CoinMessageHandler * handler);

    /** Set language. */
    void newLanguage(CoinMessages::Language language);
    void setLanguage(CoinMessages::Language language)
    { newLanguage(language); }

    /** Return handler. */
    CoinMessageHandler * messageHandler() const { return handler_; }

    /** Return messages. */
    CoinMessages messages() { return messages_; }

    /** Return pointer to messages. */
    CoinMessages * messagesPointer() { return &messages_; }

    /** Return msg level. */
    int getMsgLevel() { return msgLevel_; }

    /** Return msg level. */
    int getHubMsgLevel() { return hubMsgLevel_; }

    /** Return msg level. */
    int getMasterMsgLevel() { return workerMsgLevel_; }

    /** Return log file level. */
    int getlogFileLevel() { return logFileLevel_; }

    /** Get times that node log has been printed */
    int getNumNodeLog() const { return numNodeLog_; }
    
    /** Get times that node log has been printed */
    void setNumNodeLog(int num) { numNodeLog_ = num; }
    //@}
};
#endif
