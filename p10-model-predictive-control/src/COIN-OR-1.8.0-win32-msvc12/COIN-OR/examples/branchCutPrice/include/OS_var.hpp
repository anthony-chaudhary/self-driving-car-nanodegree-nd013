/* $Id: OS_var.hpp 2698 2009-06-09 04:14:07Z kmartin $ */
#ifndef OS_var_hpp
#define OS_var_hpp

#include "CoinPackedVector.hpp"
#include "BCP_buffer.hpp"
#include "BCP_var.hpp"


class OS_var : public BCP_var_algo {
public:
	
    int varIndex;
    
    CoinPackedVector coinPackedVec;
    
    double weight;
	
	//overload OS_var
	
    OS_var(int idx, const CoinPackedVector& f, double w);

    OS_var(BCP_buffer& buf);
    
    ~OS_var() {}

    void pack(BCP_buffer& buf) const;
};


#endif
