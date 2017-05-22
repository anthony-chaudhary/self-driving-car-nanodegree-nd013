// Copyright (C) 2004, International Business Machines
// Corporation and others.  All Rights Reserved.
//
// SmiLinear.hpp: interface for the SmiLinearData classes.
//
//
// Alan King 
// 9 March 2004
//////////////////////////////////////////////////////////////////////

#ifndef SmiLinearData_HPP
#define SmiLinearData_HPP

#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#include "CoinPackedVector.hpp"
#include "CoinPackedMatrix.hpp"
#include "OsiSolverInterface.hpp"

class SmiLinearData
{
public:
	inline const CoinPackedMatrix &getMatrix()   {return matrix_; }
	inline const CoinPackedVector &getColLower() {return dclo_;}
	inline const CoinPackedVector &getColUpper() {return dcup_;}
	inline const CoinPackedVector &getObjective(){return dobj_;}
	inline const CoinPackedVector &getRowLower() {return drlo_;}
	inline const CoinPackedVector &getRowUpper() {return drup_;}

	inline   CoinPackedMatrix &getMutableMatrix()   {return matrix_; }
	inline   CoinPackedVector &getMutableColLower() {return dclo_;}
	inline   CoinPackedVector &getMutableColUpper() {return dcup_;}
	inline   CoinPackedVector &getMutableObjective(){return dobj_;}
	inline   CoinPackedVector &getMutableRowLower() {return drlo_;}
	inline   CoinPackedVector &getMutableRowUpper() {return drup_;}

	
	SmiLinearData():matrix_(),
						   dclo_(),
						   dcup_(),
						   dobj_(),
						   drlo_(),
						   drup_() 
	{}

	SmiLinearData(SmiLinearData &d):
								matrix_(d.getMatrix()),
								dclo_(d.getColLower()),
								dcup_(d.getColUpper()),
								dobj_(d.getObjective()),
								drlo_(d.getRowLower()),
								drup_(d.getRowUpper()) 
	{}

	SmiLinearData(CoinPackedMatrix &matrix,
				CoinPackedVector &dclo, CoinPackedVector &dcup,
				CoinPackedVector &dobj,
				CoinPackedVector &drlo, CoinPackedVector &drup): matrix_(matrix),
						   dclo_(dclo),
						   dcup_(dcup),
						   dobj_(dobj),
						   drlo_(drlo),
						   drup_(drup)
	{}
	SmiLinearData(OsiSolverInterface &osi):matrix_((*osi.getMatrixByCol()))
	{
		dclo_=CoinPackedVector(matrix_.getNumCols(),osi.getColLower());
		dcup_=CoinPackedVector(matrix_.getNumCols(),osi.getColUpper());
		dobj_=CoinPackedVector(matrix_.getNumCols(),osi.getObjCoefficients());
		drlo_=CoinPackedVector(matrix_.getNumRows(),osi.getRowLower());
		drup_=CoinPackedVector(matrix_.getNumRows(),osi.getRowUpper());
	}
				
	~SmiLinearData(){}
private:
	CoinPackedMatrix matrix_;
	CoinPackedVector dclo_;
	CoinPackedVector dcup_;
	CoinPackedVector dobj_;
	CoinPackedVector drlo_;
	CoinPackedVector drup_;
};

#endif //SmiLinearData_HPP

