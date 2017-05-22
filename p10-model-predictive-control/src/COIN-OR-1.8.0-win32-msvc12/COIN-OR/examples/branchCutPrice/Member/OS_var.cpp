/* $Id: OS_var.cpp 2698 2009-06-09 04:14:07Z kmartin $ */
#include "OS_var.hpp"




/*---------------------------------------------------------------------------*/


OS_var::OS_var(int idx, const CoinPackedVector& f, double w) :
	BCP_var_algo(BCP_IntegerVar, w, 0, 1),
	varIndex( idx), coinPackedVec(false), weight(w)
{
	std::cout << "INSIDE VAR CONSTRUCTOR OS_var(int idx, const CoinPackedVector& f, double w)  " << std::endl;
	new (&coinPackedVec) CoinPackedVector(f.getNumElements(),
			f.getIndices(), f.getElements(), false);

}
/*---------------------------------------------------------------------------*/

OS_var::OS_var( BCP_buffer& buf) : 
    // we don't know the onj coeff (weight) yet, so temporarily set it to 0
    BCP_var_algo( BCP_IntegerVar, 0, 0, 1)
{

	std::cout << "INSIDE VAR CONSTRUCTOR  OS_var(BCP_buffer& buf) " << std::endl;
	
    buf.unpack( varIndex);
    int nonz;
    int* indexes;
    double* values; 
    buf.unpack(indexes, nonz);
    buf.unpack(values,  nonz);
    coinPackedVec.assignVector(nonz, indexes, values, false );  //false -- don't test for duplicates
    buf.unpack( weight);
    set_obj( weight);
}

/*===========================================================================*/



void OS_var::pack(BCP_buffer& buf) const
{
	std::cout << "PACKING  OS_var::pack(BCP_buffer& buf) " << std::endl;
    buf.pack( varIndex);
	int nonz = coinPackedVec.getNumElements();
    buf.pack(coinPackedVec.getIndices(), nonz);
    buf.pack(coinPackedVec.getElements(), nonz);
    buf.pack( weight);
}


