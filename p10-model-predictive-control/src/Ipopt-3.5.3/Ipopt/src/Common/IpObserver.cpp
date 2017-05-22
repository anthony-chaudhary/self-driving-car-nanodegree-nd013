// Copyright (C) 2004, 2006 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id: IpObserver.cpp 735 2006-06-04 06:10:05Z andreasw $
//
// Authors:  Carl Laird, Andreas Waechter     IBM    2004-08-13

#include "IpObserver.hpp"

namespace Ipopt
{
#ifdef IP_DEBUG_OBSERVER
  const Index Observer::dbg_verbosity = 0;
  const Index Subject::dbg_verbosity = 0;
#endif
} // namespace Ipopt
