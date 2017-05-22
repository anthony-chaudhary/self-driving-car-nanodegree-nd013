/* $Id: OS_cut.cpp 2698 2009-06-09 04:14:07Z kmartin $ */
// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.

#include "BCP_math.hpp"
#include "BCP_buffer.hpp"
#include "OS_cut.hpp"
   
/****************************************************************************/
void
OS_cut::pack(BCP_buffer& buf) const
{
  buf.pack(OsiRowCut::lb());
  buf.pack(OsiRowCut::ub());
  const CoinPackedVector& v = OsiRowCut::row();
  const int numElem = v.getNumElements();
  buf.pack(v.getIndices(), numElem);
  buf.pack(v.getElements(), numElem);
}

/****************************************************************************/
OS_cut::OS_cut(BCP_buffer& buf) :
   BCP_cut_algo(-BCP_DBL_MAX, BCP_DBL_MAX), OsiRowCut()
{
	double lb, ub;
	buf.unpack( lb);
	buf.unpack( ub);
	OsiRowCut::setLb( lb);
	OsiRowCut::setUb( ub);

	int numElem;
	int* indices;
	double* elements;
	buf.unpack(indices, numElem, true);
	buf.unpack(elements, numElem, true);
	OsiRowCut::setRow(numElem, indices, elements);

	if(numElem > 0) {
		delete[] indices;
		delete[] elements;
	}
}

/****************************************************************************/
OS_cut::OS_cut(const OsiRowCut& cut) :
   BCP_cut_algo(cut.lb(), cut.ub()), OsiRowCut(cut)
{}

/****************************************************************************/
BCP_MemPool OS_cut::memPool(sizeof(OS_cut));
