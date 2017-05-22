/* $Id: OS_packer.cpp 2698 2009-06-09 04:14:07Z kmartin $ */
// Last edit: 5/20/07
//
// Name:     OS_packer.cpp
// Author:   Francois Margot
//           Tepper School of Business
//           Carnegie Mellon University, Pittsburgh, PA 15213
//           email: fmargot@andrew.cmu.edu
// Date:     5/18/07
//-----------------------------------------------------------------------------
// Copyright (C) 2007, Francois Margot, IBM and others.  All Rights Reserved.

#include "BCP_buffer.hpp"
#include "OS_cut.hpp"
#include "OS_var.hpp"
#include "OS_user_data.hpp"
#include "OS_packer.hpp"

using namespace std;

/****************************************************************************/

void OS_packer::pack_cut_algo(const BCP_cut_algo* cut, BCP_buffer& buf) {
	// we don't have indexed cuts so simplify
 // int typ;
  const OS_cut* bb_cut = dynamic_cast<const OS_cut*>(cut);
  //if (bb_cut) {
   // typ = 1;
   // buf.pack(typ);
 // std::cout << "INSIDE pack_cut_algo " << std::endl;
    bb_cut->pack(buf);
    return;
  //}
  throw BCP_fatal_error("OS_pack_cut(): unknown cut type.");
}

/****************************************************************************/

BCP_cut_algo*  OS_packer::unpack_cut_algo(BCP_buffer& buf)
{
	// we don't have indexed cuts so simplify
	//int typ;
	//buf.unpack(typ);
	//switch (typ) {
	//case 0:
	//	throw BCP_fatal_error("OS_unpack_cut(): unknown cut type.");
    //return new OS_indexed_cut(buf);
	//case 1:
	//std::cout << "INSIDE unpack_cut_algo " << std::endl;
	
	return new OS_cut( buf);
	//default:
	//	throw BCP_fatal_error("OS_unpack_cut(): unknown cut type.");
   // break;
 // }
  //return NULL; // fake return
}



void OS_packer::pack_var_algo(const BCP_var_algo* var, BCP_buffer& buf) {

    const OS_var* v = dynamic_cast<const OS_var*>(var);
    // don't worry about type, only one kind of variable
	//int type = 0;
	//buf.pack( type);

	v->pack( buf);
	return;

}

/*---------------------------------------------------------------------------*/

BCP_var_algo* OS_packer::unpack_var_algo(BCP_buffer& buf) {

	
   // int type;
   // buf.unpack( type);
    
    return new OS_var(  buf);

}



/****************************************************************************/

void  OS_packer::pack_user_data(const BCP_user_data* ud, BCP_buffer& buf)
// Normally, no modifications required.
{
  const MY_user_data *mud = dynamic_cast<const MY_user_data*> (ud);
  if(!mud)
    throw BCP_fatal_error("OS_lp::pack_user_data() : unknown data type!\n");
  
  printf("OS_lp::pack_user_data:\n");
  mud->print();
  mud->pack(buf);
}

/****************************************************************************/

BCP_user_data*  OS_packer::unpack_user_data(BCP_buffer& buf)
  // Normally, no modifications required.
{
  MY_user_data *p_ud = new MY_user_data(buf);
  printf("OS_lp::unpack_user_data:\n");
  p_ud->print();

  if (p_ud->is_processed) {
    p_ud->p_rud = NULL;
    delete(p_ud);
    p_ud = NULL;
    printf("user_data deleted\n");
  }

  return(p_ud); 
}
