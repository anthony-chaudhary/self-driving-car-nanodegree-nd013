/* $Id: OS_init.hpp 2698 2009-06-09 04:14:07Z kmartin $ */
// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef _OS_INIT_H
#define _OS_INIT_H

#include "BCP_USER.hpp"
#include "OS_tm.hpp"

class OS_init : public USER_initialize {

public:



   virtual BCP_tm_user * tm_init(BCP_tm_prob& p,
				 const int argnum,
				 const char * const * arglist);

   virtual BCP_lp_user * lp_init(BCP_lp_prob& p);
   
   virtual BCP_user_pack* packer_init(BCP_user_class* p);
};

#endif
