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

#ifndef AlpsKnowledge_h
#define AlpsKnowledge_h

#include <map>
#include <memory>
#include <typeinfo>

#include "Alps.h"
#include "AlpsEncoded.h"

//#############################################################################
/** A function object to perform lexicographic lexicographic comparison
    between two C style strings. */
//#############################################################################

struct AlpsStrLess 
{
    inline bool operator()(const char* s1, const char* s2) const {
        return strcmp(s1, s2) < 0;
    }
};

//#############################################################################
/** The abstract base class of any user-defined class that Alps has to know
    about in order to encode/decode. These classes must all be registered so
    that the proper decode method can be called. */
//#############################################################################

class AlpsKnowledge {

private:
    AlpsKnowledge(const AlpsKnowledge&);
    AlpsKnowledge& operator=(const AlpsKnowledge&);
    
    /** The encoded object in an encoded form (could be compressed!) */
    //FIXME: For now, we just use a regular pointer here to get it to compile.
    //CoinPtr<AlpsEncoded> encoded_;
    AlpsEncoded* encoded_;

protected:
    
    KnowledgeType type_;

public:
    
    AlpsKnowledge()	: encoded_(0), type_(AlpsKnowledgeTypeUndefined) {}
    virtual ~AlpsKnowledge() {}

    KnowledgeType getType() { return type_; }
    void setType(KnowledgeType t) { type_ = t; }

    /** This method should encode the content of the object and return a
        pointer to the encoded form. 
        
        NOTE: This default implementation can not be
        used when the memory of data members is not continously allocated,
        for example, some data members are pointers, STL set, map, etc. */
    virtual AlpsEncoded* encode() const;

    /** Pack into a encode object. */
    virtual AlpsReturnStatus encode(AlpsEncoded *encoded)
	{ return AlpsReturnStatusOk; }
    
    /** This method should decode and return a pointer to a \em brand \em new
        \em object, i.e., the method must create a new object on the heap from
        the decoded data instead of filling up the object for which the method
        was invoked. 
            
        NOTE: This default implementation can not be
        used when the memory of data members is not continously allocated,
        for example, some data members are pointers, STL set, map, etc.
    */
    virtual AlpsKnowledge* decode(AlpsEncoded& encoded) const;
    
    /** Get/set encoded. */
    inline AlpsEncoded* getEncoded() const { return encoded_; }
    inline void setEncoded(AlpsEncoded* e) { encoded_ = e; }
};

//#############################################################################

#endif
