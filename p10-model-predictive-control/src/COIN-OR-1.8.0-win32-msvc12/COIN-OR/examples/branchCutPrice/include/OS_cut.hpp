/* $Id: OS_cut.hpp 2710 2009-06-10 21:13:43Z kmartin $ */
// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef _OS_CUT_H
#define _OS_CUT_H

#include "BCP_cut.hpp"
#include "BCP_mempool.hpp"
#include "OsiRowCut.hpp"

//class BCP_buffer;

/** Simple representation of a cut by storing non zero coefficients only */ 

/****************************************************************************/
class OS_cut : public BCP_cut_algo, public OsiRowCut {

private:

   static BCP_MemPool memPool;

public:

   static inline void * operator new(size_t size) {
      return memPool.alloc( size);
   }

   static inline void operator delete(void *p, size_t size) {
      memPool.free(p, size);
   }

  /// Packing cut to a buffer
   void pack(BCP_buffer& buf) const;

  /**@name Constructors and destructors */
  //@{
  /// Constructor from content of buffer 
   OS_cut(BCP_buffer& buf);

  /// Constructor from an OsiRowCut 
   OS_cut(const OsiRowCut& cut);

  /// Destructor
   ~OS_cut() {}
};

#endif


