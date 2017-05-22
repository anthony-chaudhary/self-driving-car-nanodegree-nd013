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

#ifndef AlpsModel_h
#define AlpsModel_h

#include <string>

#include "CoinError.hpp"

#include "AlpsKnowledge.h"
#include "AlpsTreeNode.h"
#include "AlpsParams.h"

class AlpsKnowledgeBroker;

class AlpsModel : public AlpsKnowledge {

 private:

    AlpsModel(const AlpsModel&);
    AlpsModel& operator=(const AlpsModel&);

 protected:

    /** Knowledge broker. */
    AlpsKnowledgeBroker *broker_;
    
    /** Data file. */
    std::string dataFile_; 

    /** The parameter set that is used in Alps. */
    AlpsParams * AlpsPar_;

 public:

    /** Default construtor. */
    AlpsModel() :
        broker_(NULL), 
        AlpsPar_(new AlpsParams) { setType(AlpsKnowledgeTypeModel); }

    /** Destructor. */
    virtual ~AlpsModel() { delete AlpsPar_; }

    /** Get knowledge broker. */
    AlpsKnowledgeBroker *getKnowledgeBroker() { return broker_; }

    /** Set knowledge broker. */
    void setKnowledgeBroker(AlpsKnowledgeBroker *b) { broker_ = b; }

    /** Get the input file. */
    inline std::string getDataFile() const { return dataFile_; }

    /** Set the data file. */
    inline void setDataFile(std::string infile) { dataFile_ = infile; }
    
    /** Access Alps Parameters. */
    AlpsParams * AlpsPar() { return AlpsPar_; }

    /** Read in the instance data. At Alps level, nothing to do. */
    virtual void readInstance(const char* dateFile) {
	throw CoinError("readInstance() is not defined.", "readData",
			"AlpsModel");
    } 

    /** Read in Alps parameters. */
    virtual void readParameters(const int argnum, const char * const * arglist);

    /** Write out parameters. */
    void writeParameters(std::ostream& outstream) const;

    /** Do necessary work to make model ready for use, such as classify 
	variable and constraint types.*/
    virtual bool setupSelf() { return true; }

    /** Preprocessing the model. */
    virtual void preprocess() {}

    /** Postprocessing results. */
    virtual void postprocess() {}
    
    /** Create the root node. Default: do nothing */
    virtual AlpsTreeNode * createRoot() {
        throw CoinError("createRoot is not defined.", "createRoot",
			"AlpsModel");
    } 

    /** Problem specific log. */
    virtual void modelLog() {}

    /** Node log. */
    virtual void nodeLog(AlpsTreeNode *node, bool force);

    /** Return true if all nodes on this process can be fathomed.*/
    virtual bool fathomAllNodes() { return false; }
    
    //------------------------------------------------------
    // For parallel.
    //------------------------------------------------------
    
    /** Pack Alps portion of node into an encoded object. */
    AlpsReturnStatus encodeAlps(AlpsEncoded *encoded) const;

    /** Unpack Alps portion of node from an encoded object. */
    AlpsReturnStatus decodeAlps(AlpsEncoded &encoded);
    
    /** Decode model data from the encoded form and fill member data.*/
    virtual void decodeToSelf(AlpsEncoded& encoded) {}
    
    /** Register knowledge class. */
    virtual void registerKnowledge() { /* Default does nothing */ }

    /** Send generated knowledge */
    virtual void sendGeneratedKnowledge() { /* Default does nothing */ }

    /** Receive generated knowledge */
    virtual void receiveGeneratedKnowledge() { /* Default does nothing */ }

    /** Pack knowledge to be shared with others into an encoded object. 
        Return NULL means that no knowledge can be shared. */
    virtual AlpsEncoded* packSharedKnowlege() {
        /* Default does nothing */ 
        AlpsEncoded* encoded = NULL;
        return encoded;
    }
    
    /** Unpack and store shared knowledge from an encoded object. */
    virtual void unpackSharedKnowledge(AlpsEncoded&) 
    { /* Default does nothing */ }
};
#endif
