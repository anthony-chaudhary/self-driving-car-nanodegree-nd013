/* $Id: OS_init.cpp 2698 2009-06-09 04:14:07Z kmartin $ */
// Last edit: 12/26/05
//
// Name:     OS_tm.cpp
// Author:   Francois Margot
//           Tepper School of Business
//           Carnegie Mellon University, Pittsburgh, PA 15213
//           email: fmargot@andrew.cmu.edu
// Date:     12/28/03
//-----------------------------------------------------------------------------
// Copyright (C) 2003, Francois Margot, International Business Machines
// Corporation and others.  All Rights Reserved.

#include "OS_init.hpp"
#include "OS_lp.hpp"
#include "OS_tm.hpp"
#include "OS_packer.hpp"

using namespace std;

/****************************************************************************/
BCP_lp_user *OS_init::lp_init(BCP_lp_prob& p) {
  return new OS_lp;
}

/****************************************************************************/
BCP_tm_user*  OS_init::tm_init(BCP_tm_prob& p,
		 const int argnum, const char * const * arglist)
{
	

	
  cout << "Compilation flags: ";
  
#ifdef HEUR_SOL
  cout << "HEUR_SOL ";
#endif
  
#ifdef CUSTOM_BRANCH
  cout << "CUSTOM_BRANCH ";
#endif
  
#ifdef USER_DATA
  cout << "USER_DATA ";
#endif

  cout << endl << endl;
  

  OS_tm* tm = new OS_tm;

  if(argnum > 1) {
	  //std::cout << "ARGNUM = " << argnum << std::endl;
    tm->readInput( arglist[1]);
  }
  else {
    tm->readInput(NULL);
  }
  return tm;
}

BCP_user_pack * OS_init::packer_init(BCP_user_class* p) {
    return new OS_packer;
}
