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

#ifndef AlpsKnowledgeBrokerMPI_h_
#define AlpsKnowledgeBrokerMPI_h_

#include <cmath>
#include <iosfwd>

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "mpi.h"

#include "AlpsEnumProcessT.h"
#include "AlpsKnowledge.h"
#include "AlpsKnowledgeBroker.h"
#include "AlpsParams.h"

//#############################################################################

class AlpsKnowledgeBrokerMPI : public AlpsKnowledgeBroker {

 private:
    /** Disable copy construct and assignment operator. */
    AlpsKnowledgeBrokerMPI(const AlpsKnowledgeBrokerMPI&);
    AlpsKnowledgeBrokerMPI& operator=(const AlpsKnowledgeBrokerMPI&);

 protected:

    /** @name Process information
     *
     */
    //@{
    /** The Number of processes launched. */
    int processNum_;

    /** The Number of hubs. */
    int hubNum_;

    /** The rank of the process in MPI_COMM_WORLD. */
    int globalRank_;

    /** Communicator of the cluster to which the process belongs. */
    MPI_Comm clusterComm_;

    /** Communicator consists of all hubs. */
    MPI_Comm hubComm_;

    /** MPI_Group consists of all hubs. */
    MPI_Group hubGroup_;

    /** The actual size of the cluster to which the process belongs. */
    int clusterSize_;

    /** The user reqested size of a cluster. */
    int userClusterSize_;

    /** The local rank of the process in clusterComm_. */
    int clusterRank_;

    /** The global ranks of the hubs. */
    int* hubRanks_;

    /** The global rank of its hub for a worker. */
    int myHubRank_;

    /** The global rank of the master. */
    int masterRank_;

    /** The AlpsProcessType of this process. */
    AlpsProcessType processType_;

    /** The AlpsProcessType of all process. */
    AlpsProcessType* processTypeList_;

    /** Whether hub should also work as a worker. */
    bool hubWork_;

    /** Send subtree request. */
    MPI_Request subTreeRequest_;

    /** Send model knoledge request. */
    MPI_Request solRequestL_;
    MPI_Request solRequestR_;

    /** Send model knoledge request. */
    MPI_Request modelKnowRequestL_;
    MPI_Request modelKnowRequestR_;

    /** Forward model knoledge request. */
    MPI_Request forwardRequestL_;
    MPI_Request forwardRequestR_;
    //@}

    /** @name Incumbent data
     *
     */
    //@{
    /** Incumbent value. */
    double incumbentValue_;

    /** The process id that store the incumbent. */
    int incumbentID_;

    /** Indicate whether the incumbent value is updated between two 
	checking point. */
    bool updateIncumbent_;
    //@}

    /** @name Workload balancing
     *
     */
    //@{
    /** The workload quality of the process. */
    double workQuality_;

    /** The workload quality of the cluster to which the process belong. */
    double clusterWorkQuality_;

    /** The workload quality of the whole system. */
    double systemWorkQuality_;

    /** The workload qualities of hubs. */
    double* hubWorkQualities_;

    /** The workload qualities of workers in the cluster to which this proces
	belongs. Number of nodes is used as the quantities criteria. */
    double* workerWorkQualities_;

    /** The workload quantity of the workload on the process. */
    double workQuantity_;

    /** The workload quantity of the cluster to which the process belongs. */
    double clusterWorkQuantity_;

    /** The workload quantity of the whole system. */
    double systemWorkQuantity_;
    
    /** The workload quantity of the whole system before forcing termination. */
    double systemWorkQuantityForce_;

    /** The workload quantities of all clusters/hubs. */
    double* hubWorkQuantities_;

    /** The workload quantities of workers in the cluster to which this proces
	belongs. */
    double* workerWorkQuantities_;

    /** Indicate which worker has been reported its work. */
    bool* workerReported_;

    /** Indicate which hub has been reported its work. */
    bool* hubReported_;

    /** Indicate whether all hubs have reported status to master at least once.*/
    bool allHubReported_;

    /** Whether master do load balance. 0: do; >0: blocked. */
    int masterDoBalance_;

    /** Whether a hub do load balance. 0: do; >0: blocked. */
    int hubDoBalance_;

    /** To record how many nodes processed for each worker in a cluster. */
    int* workerNodeProcesseds_;

    /** To record how many nodes by a cluster. */
    int clusterNodeProcessed_;

    /** To record how many nodes processed for each hub */
    int* hubNodeProcesseds_;
    //@}

    /** @name Message counts
     *
     */
    //@{
    /** The number of new messages sent by the process after last survey. */
    int sendCount_;

    /** The number of new messages received by the process after last survey.*/
    int recvCount_;

    /** The number of new messages sent by the processes in clusterComm_ 
	after last survey.*/
    int clusterSendCount_;

    /** The number of new messages received by the processes in clusterComm_ 
	after last survey.*/
    int clusterRecvCount_;

    /** The total number of messages sent by the all processes. */
    int systemSendCount_;

    /** The total number of messages sent by the all processes. */ 
    int systemRecvCount_;
    //@}

    /** @name Node index
     *
     */
    //@{
    int masterIndexBatch_;
    //@}

    /** @name Parallel statistics
     *
     */
    //@{
    /** Master timer */
    AlpsTimer masterTimer_;

    /** Hub timer */
    AlpsTimer hubTimer_;

    /** Worker timer */
    AlpsTimer workerTimer_;
    
    /** The time spent in ramp up. */
    double rampUpTime_;

    /** The time spent in ramp down. */
    double rampDownTime_;

    /** The time spent waiting for work. */
    double idleTime_;

    /** The time spent processing messages (include idle). */
    double msgTime_;

    /** More statistics */
    AlpsPsStats psStats_;
    //@}

    /** Terminate due to reaching limits (time and node) or other reason. */
    bool forceTerminate_;

    /** Indicate whether do termination check */
    bool blockTermCheck_;

    /** Indicate whether a hub need to report state to master */
    bool blockHubReport_;

    /** Indicate whether a worker need to report state to its hub */
    bool blockWorkerReport_;

    /** Indicate whether a worker need to as for work from its hub */
    bool blockAskForWork_;

    /** Buffer attached to MPI when sharing generated knowledge. */
    char *attachBuffer_;
    
    /** Large message buffer. */
    char *largeBuffer_;

    /** Large message buffer. Used for sharing model knowledge */
    char *largeBuffer2_;

    /** Small message buffer. */
    char *smallBuffer_;
    
    /** The period that master do load balancing. It changes 
        as search progresses. */
    double masterBalancePeriod_;
    
    /** The period that a hub load balancing and report cluster status. 
        It changes as search progresses. */
    double hubReportPeriod_;

    /** The global rank of the process that share generated model knowledge. */
    int modelGenID_;

    /** Size of the shared knowledge. */
    int modelGenPos_;
    
    /** A subtree used in during up. */
    AlpsSubTree* rampUpSubTree_;

    /** Number of nodes in one unit of work */
    int unitWorkNodes_;

    /** Temporily halt search */
    int haltSearch_;

 protected:

    /** Initialize member data. */
    void init();
    
    /** @name Core member functions for master, hubs and workers.
     */
    //@{
    /** Master generates subtrees and sends them to hubs in Round-Robin way.
	Master periodically do inter-cluster load balancing, 
	and termination check. */
    void masterMain(AlpsTreeNode* root);

    /** Hub generates subtrees and sends them to workers in Round-Robin way.
	Hub do intra-cluster load balancing. */
    void hubMain();

    /** Worker first receive subtrees, then start to explore them. Worker also
	peroidically check message and process message. */
    void workerMain();
    //@}

    /** Explore a subtree from subtree pool for certain units of work/time.*/
    // The same subtree will be explored next time if it still have 
    // unexplored nodes. 
    AlpsReturnStatus doOneUnitWork(int unitWork,
				   double unitTime, 
				   AlpsExitStatus & exitStatus,
				   int & numNodesProcessed,
				   int & numNodesBranched,
				   int & numNodesDiscarded,
				   int & numNodesPartial,
				   int & depth,
				   bool & betterSolution);
    
    /** Processing messages. */
    void processMessages(char *&buffer,
                         MPI_Status &status, 
                         MPI_Request &request);

    /** Static load balancing: Root Initialization */
    void rootInitMaster(AlpsTreeNode* root);
    void rootInitHub();
    void rootInitWorker();

    /** Static load balancing: spiral */
    void spiralMaster(AlpsTreeNode* root);
    void spiralHub();
    void spiralWorker();
    
    //------------------------------------------------------

    /** @name Load balancing member functions
     */
    //@{
    /** Master asks a hub to donate its workload to another hub. */
    void masterAskHubDonate(int donorID, 
                            int receiverID, 
                            double receiverWorkload);
    
    /** Hub asks a worker to donate its workload to another worker. */
    void hubAskWorkerDonate(int donorID, 
                            int receiverID, 
                            double receiverWorkload);
    
    /** Calculate the work quality and quantity on this process. */
    void updateWorkloadInfo();

    virtual int getNumNodeLeftSystem()
    { return static_cast<int>(systemWorkQuantity_); }
    
    /** A worker donate its workload to the specified worker. */
    void donateWork(char*& buf, 
		    int tag,
		    MPI_Status* status,  
		    int recvID = -1,
		    double recvWL = 0.0);
    
    /** Hub allocates the donated workload to its workers. */
    void hubAllocateDonation(char*& buf, MPI_Status* status);
    
    /** Hub balances the workloads of its workers. */
    void hubBalanceWorkers();
    
    /** Hub satisfies the workload rquest from a worker. */
    void hubSatisfyWorkerRequest(char*& buf, MPI_Status* status);
    
    /** A hub reports its status (workload and msg counts) to the master. */
    // NOTE: comm is hubComm_ or MPI_COMM_WORLD.
    void hubReportStatus(int tag, MPI_Comm comm);
    
    /** A hub unpacks the status of a worker from buffer. */
    // NOTE: comm is clusterComm_ or MPI_COMM_WORLD. 
    void hubUpdateCluStatus(char*& buf, MPI_Status* status, MPI_Comm comm);
    
    /** Two hubs share their workload. */
    void hubsShareWork(char*& buf, MPI_Status* status);

    /** Master balance the workload of hubs. */
    void masterBalanceHubs();

    /** Master unpack the status of a hub from buf and update system status.*/
    // NOTE: comm is hubComm or MPI_COMM_WORLD. 
    void masterUpdateSysStatus(char*& buf, MPI_Status* status, MPI_Comm comm);

    /** The master re-calculate the system status. */
    void refreshSysStatus();

    /** A hub adds its status to the cluster's status. */
    void refreshClusterStatus();
    
    /** A worker report its status (workload and msg counts) to its hub. */
    // NOTE: comm is clusterComm_ or MPI_COMM_WORLD. 
    void workerReportStatus(int tag, MPI_Comm comm);
    //@}

    //------------------------------------------------------

    /** @name Node index functions  // msg counts is modified inside
     *
     */
    //@{
    /** A worker ask for node index from master. */
    void workerAskIndices();

    /** A worker receive node index from master. */
    void workerRecvIndices(char *&bufLarge);

    /** Master send a batch of node indices to the receiving worker. */
    void masterSendIndices(char *&bufLarge);
    //@}

    //------------------------------------------------------

    /** @name Other message passing member functions
     *
     */
    //@{
    /** Broadcast the model from source to other processes. */
    void broadcastModel(const int id, const int source);

    /** Sent the incumbent value and rank to its two child if eixt */
    void sendIncumbent();

    /** unpack the incumbent value, then store it and the id of the process 
	having the incumbent in AlpsDataPool. */
    bool unpackSetIncumbent(char*& buf, MPI_Status* status);

    /** Send the best solution from the process having it to destination. */
    void collectBestSolution(int destination);

    /** Inform master that a proc has received workload during a load 
	balance initialized by master. */
    void tellMasterRecv();
 
    /** Inform hub that a proc has received workload during a load 
	balance initialized by a hub. */
    // Not used
    void tellHubRecv();

    /** Pack an AlpsEncoded instance into buf. Return filled buf and size of 
	packed message.
        position: where to start if buf is allocated.
    */
    void packEncoded(AlpsEncoded* enc, 
                     char*& buf, 
                     int& size,
                     int& position,
                     MPI_Comm comm);
 
    /** Unpack the given buffer into an AlpsEncoded instance. */
    AlpsEncoded* unpackEncoded(char*& buf, 
                               int& position, 
                               MPI_Comm comm,
                               int size = -1);

    /** Receive the size of buffer, allocate memory for buffer, then 
	receive the message and put it in buffer. */
    // NOTE: comm is hubComm_ or clusterComm_
    void receiveSizeBuf(char*& buf, 
			int sender, 
			int tag, 
			MPI_Comm comm, 
			MPI_Status* status);

    /** First receive the size and the contend of a node, then construct 
	a subtree with this received node. */
    // NOTE: comm is hubComm_ or clusterComm_
    void receiveRampUpNode(int sender,
			   MPI_Comm comm, 
			   MPI_Status* status);

    /** Receive a subtree from the sender process and add it into
	the subtree pool.*/
    void receiveSubTree(char*& buf, int sender, MPI_Status* status);

    /** Send the size and content of a buffer to the target process. */
    // NOTE: comm is hubComm_ or clusterComm_.
    void sendSizeBuf(char*& buf, 
		     int size, 
		     int position,
		     const int target, 
		     const int tag, 
		     MPI_Comm comm);

    /** Send the size and the content of the best node of a given subtree 
	to the target process. */
    // NOTE: comm is hubComm_ or clusterComm_.
    void sendRampUpNode(const int target, MPI_Comm comm);

    /** Send a node from rampUpSubTree's node pool and generated model 
        knowledge */
    void sendNodeModelGen(int receiver, int doUnitWork);
    
    /** Send a given subtree to the target process. */
    bool sendSubTree(const int target, AlpsSubTree*& st, int tag);

    /** Send finish initialization signal to the target process. */
    // NOTE: comm is hubComm_ or clusterComm_.
    void sendFinishInit(const int target, MPI_Comm comm);
    //@}

    /** Delete subTrees in pools and the active subtree. */
    void deleteSubTrees();

    
    void forwardModelKnowledge();
    
    /** Set generated knowlege (related to model) to receiver. */
    // NOTE: comm is hubComm_ or MPI_COMM_WORLD.
    void sendModelKnowledge(MPI_Comm comm, int receiver=-1);

    /** Receive generated knowlege (related to model) from sender. */
    // NOTE: comm is hubComm_ or MPI_COMM_WORLD.
    void receiveModelKnowledge(MPI_Comm comm);

    /** @name Change message counts functions
     */
    //@{
    /** Increment the number of sent message. */
    void incSendCount(const char* how, int s = 1);
    /** Decrement the number of sent message. */
    void decSendCount(const char* how, int s = 1);
    /** Increment the number of received message. */
    void incRecvCount(const char* how, int s = 1);
    /** Decrement the number of sent message. */
    void decRecvCount(const char* how, int s = 1);
    //@}

    /** Master tell hubs to terminate due to reaching limits or other reason.*/
    void masterForceHubTerm();

    /** Hub tell workers to terminate due to reaching limits or other reason.*/
    void hubForceWorkerTerm();

    /** Change subtree to be explored if it is too worse. */
    void changeWorkingSubTree(double & changeWorkThreshold);

    /** Send error code to master. */
    void sendErrorCodeToMaster(int errorCode);
    
    /** Receive error code and set solution status. */
    void recvErrorCode(char *& bufLarge);
    
    /** Unpack the node, explore it and send load info to master. */
    void spiralRecvProcessNode();
    
    /** Unpack msg and donate a node. */
    void spiralDonateNode();
    
 public:
    
    /** Default construtor. 
        NOTE: must call initializeSearch() later. */
    AlpsKnowledgeBrokerMPI()
	:
	AlpsKnowledgeBroker()
	{ 
	    init(); 
	}
 
    /** Useful construtor. */
    AlpsKnowledgeBrokerMPI(int argc, 
			   char* argv[], 
			   AlpsModel& model)
	:
	AlpsKnowledgeBroker() 
	{    
	    init();
	    initializeSearch(argc, argv, model);
	}
    
    /** Destructor. */
    ~AlpsKnowledgeBrokerMPI();

    /** Query the global rank of the process. */
    virtual int getProcRank() const { return globalRank_; }

    /** Query the global rank of the Master. */
    virtual int getMasterRank() const { return masterRank_; }
    
    /** Query the type (master, hub, or worker) of the process. */
    virtual AlpsProcessType getProcType() const { return processType_; }
    
    /** This function
     * <ul>
     *  <li> initializes the message environment;
     *  <li> the master reads in ALPS and user's parameter sets. If the model 
     *  data is input from file, then it reads in the model data.
     *  <li> sets up user params and model;
     *  <li> broadcast parameters from the master to all other processes;
     *  <li> creates MPI communicators and groups;
     *  <li> classifies process types, sets up subtree and pools
     *  <li> determines their hub's global rank for workers
     * </ul>
     */
    void initializeSearch(int argc, char* argv[], AlpsModel& model);

    /** Search best solution for a given model. */
    void search(AlpsModel *model);
    
    /** This function
     * <ul>
     * <li> broadcasts model data from the master to all other processes;
     * <li> calls its associated main function to explore the sub tree;
     * <li> collects the best solution found.
     * </ul>
     */
    void rootSearch(AlpsTreeNode* root);

    /** @name Report search results. */
    //@{
    /** The process queries the quality of the incumbent this process stores. */
    virtual double getIncumbentValue() const {
        double bestObj = ALPS_OBJ_MAX;
        if (AlpsKnowledgeBroker::hasKnowledge(AlpsKnowledgeTypeSolution)) {
	    bestObj = getBestKnowledge(AlpsKnowledgeTypeSolution).second;
            if (incumbentValue_ > bestObj) {
                return bestObj;
            }
        }
	return incumbentValue_;
    }
 
    /** The master queries the quality of the best solution it knowns. */
    virtual double getBestQuality() const {
	if (globalRank_ == masterRank_) {
            if (getNumKnowledges(AlpsKnowledgeTypeSolution) > 0) {
                return getBestKnowledge(AlpsKnowledgeTypeSolution).second;   
            }
            else {
                return ALPS_OBJ_MAX;
            }
	}
        else {
            return ALPS_OBJ_MAX;
        }
    }

    /** Get best estimalted quality in system. */
    virtual double getBestEstimateQuality() { return systemWorkQuality_; }

    /** Master prints out the best solution that it knows. */
    virtual void printBestSolution(char* outputFile = 0) const;

    /** Log search statistics. */
    virtual void searchLog();
    //@}

    //------------------------------------------------------

    /** @name Knowledge sharing functions
     */
    //@{
    /** Set knowlege. */
    void sendKnowledge(AlpsKnowledgeType type, 
                       int sender,
                       int receiver,
                       char *& msgBuffer,
                       int msgSize,
                       int msgTag,
                       MPI_Comm comm,
                       bool blocking);

    /** Receive knowlege. */
    void receiveKnowledge(AlpsKnowledgeType type, 
                          int sender,
                          int receiver,
                          char *& msgBuffer,
                          int msgSize,
                          int msgTag,
                          MPI_Comm comm,
                          MPI_Status* status,
                          bool blocking);
    
    /** Request knowlege. */
    void requestKnowledge(AlpsKnowledgeType type, 
                          int sender,
                          int receiver,
                          char *& msgBuffer,
                          int msgSize,
                          int msgTag,
                          MPI_Comm comm,
                          bool blocking);
    //@}

};
#endif

//#############################################################################
