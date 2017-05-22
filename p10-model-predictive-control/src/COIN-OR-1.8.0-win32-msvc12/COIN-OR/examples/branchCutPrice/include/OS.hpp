/* $Id: OS.hpp 2698 2009-06-09 04:14:07Z kmartin $ */
// Last edit: 2/10/05
//
// Name:     BB.hpp
// Author:   Francois Margot
//           Tepper School of Business
//           Carnegie Mellon University, Pittsburgh, PA 15213
//           email: fmargot@andrew.cmu.edu
// Date:     12/28/03
//-----------------------------------------------------------------------------
// Copyright (C) 2003, Francois Margot, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef _OS_H
#define _OS_H

#include "OSInstance.h"
#include "OSiLReader.h"

class CoinPackedMatrix;
class BCP_buffer;

/** Class holding data for describing the problem */

class OS_prob {
private:
   OS_prob(const OS_prob&);
   OS_prob& operator=(const OS_prob&);

public:
	/// Numerical precision for double arithmetic
	double EPSILON; 
	OSInstance *osinstance;
	OSiLReader *osilreader;

public:

  /**@name Constructors and destructors */
  //@{
  /// Default constructor 
  OS_prob();

  /// Default destructor 
  ~OS_prob();
  //@}
};

#endif
