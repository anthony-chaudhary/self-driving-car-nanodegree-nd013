// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef SmiScnData_HPP
#define SmiScnData_HPP

#include "CoinPragma.hpp"
#include "OsiSolverInterface.hpp"
#include "CoinPackedVector.hpp"
#include "CoinMpsIO.hpp"
#include "SmiCoreCombineRule.hpp"
#include "SmiQuadratic.hpp"

//#include "SmiLinearData.hpp"

#include <map>
#include <vector>
#include <exception>


typedef int SmiCoreIndex;
typedef int SmiScenarioIndex;
typedef int SmiStageIndex;

class SmiCoreData;

class SmiNodeData
	//: public SmiLinearData
{
public:
	typedef std::map<int,CoinPackedVector *> SmiRowMap;
	typedef std::map<int,double *> SmiDenseRowMap;
	void setCoreNode();
	/*
	CoinPackedVector * getRow(int i) {
		SmiRowMap::iterator r=rowMap.find(i);
		if (r!=rowMap.end()) return r->second;
		else return NULL;}
	*/
	double * getDenseRow(int i);

	inline SmiCoreData * getCore() { return core_;}
	inline int getStage() { return stg_;}

	inline void setCoreCombineRule(SmiCoreCombineRule *r){combineRule_=r;}
	inline SmiCoreCombineRule * getCoreCombineRule() { return combineRule_;}

	void copyRowLower(double * drlo);
	void copyRowUpper(double * drup);
	void copyColLower(double * dclo);
	void copyColUpper(double * dcup);
	void copyObjective(double * dobj);

	const int getNumMatrixElements(){
		if (this->has_matrix_)
			return this->strt_[getMatEnd()]-this->strt_[getMatStart()];
		else
			return 0;
	}
	 const int getRowLength(int irow){
		 if (this->has_matrix_)
			 return getLength(this->getMatStart()+irow-this->rowbeg_);
		 else
			return 0;
	 }
	 const int getRowLowerLength()   {return getLength(this->getRloStart());}
	 const int getRowUpperLength()   {return getLength(this->getRupStart());}
	 const int getColLowerLength()   {return getLength(this->getCloStart());}
	 const int getColUpperLength()   {return getLength(this->getCupStart());}
	 const int getObjectiveLength()  {return getLength(this->getObjStart());}
     const bool isCoreNode() { return isCoreNode_; }

	 const int *getRowIndices(int irow){
		 if (this->has_matrix_)
			 return getIndices(this->getMatStart()+irow-this->rowbeg_);
		 else
			 return NULL;
	 }
	 int *getMutableRowIndices(int irow){
		 // using const_cast to stop warnings about cast-away constness
		 return const_cast<int *>(getRowIndices(irow));
	 }
	 const int *getRowLowerIndices()   {return getIndices(this->getRloStart());}
	 const int *getRowUpperIndices()   {return getIndices(this->getRupStart());}
	 const int *getColLowerIndices()   {return getIndices(this->getCloStart());}
	 const int *getColUpperIndices()   {return getIndices(this->getCupStart());}
	 const int *getObjectiveIndices()  {return getIndices(this->getObjStart());}

	 const double *getRowElements(int irow){
		 if (this->has_matrix_)
			 return getElements(this->getMatStart()+irow-this->rowbeg_);
		 else
			 return NULL;
	 }

	 const int *getRowStarts(int irow){
		if (this->has_matrix_)
			return &strt_[this->getMatStart()+irow-this->rowbeg_];
		else
			return NULL;
		 }
	 double *getMutableRowElements(int irow){
		 // using const_cast to stop warnings about cast-away constness
		 return const_cast<double *>(getRowElements(irow));
	 }
	 const double *getRowLowerElements()   {return getElements(this->getRloStart());}
	 const double *getRowUpperElements()   {return getElements(this->getRupStart());}
	 const double *getColLowerElements()   {return getElements(this->getCloStart());}
	 const double *getColUpperElements()   {return getElements(this->getCupStart());}
	 const double *getObjectiveElements()  {return getElements(this->getObjStart());}


	CoinPackedVector * combineWithCoreRow(CoinPackedVector *cr, CoinPackedVector *nr);
	int combineWithDenseCoreRow(double *dr,CoinPackedVector *cpv,double *dels,int *indx);
	int combineWithDenseCoreRow(double *dr,const int nels,const int *inds, const double *dels, double *dest_dels,int *dest_indx);

	SmiNodeData(SmiStageIndex stg, SmiCoreData *core,
				 const CoinPackedMatrix *const matrix,
				 CoinPackedVector *dclo,
				 CoinPackedVector *dcup,
				 CoinPackedVector *dobj,
				 CoinPackedVector *drlo,
				 CoinPackedVector *drup);

	void addQuadraticObjective(int stage, SmiCoreData *smicore, SmiQuadraticData *sqdata);

	bool hasQdata() {return hasQdata_;}
	void setHasQdata(bool b) {hasQdata_=b;}
		
	SmiQuadraticData *getQdata(){ return nqdata_;}

	int addPtr() { return ++ptr_count; }

	~SmiNodeData();

protected:
	void combineWithCoreDoubleArray(double *d_out, const CoinPackedVector &cpv, int o);
	void combineWithCoreDoubleArray(double *d_out, const int len, const int * inds, const double *dels, int o);
	//Memory assignment/deletion
	void assignMemory();
	void deleteMemory();

	//Covers for starts of arrays
	 const int getMatStart() { return mat_strt_;}
	 const int getMatEnd()   { return mat_strt_+nrow_;}
	 const int getRloStart() { return rlo_strt_;}
	 const int getRupStart() { return rup_strt_;}
	 const int getCloStart() { return clo_strt_;}
	 const int getCupStart() { return cup_strt_;}
	 const int getObjStart() { return obj_strt_;}

	//Access to stored memory
	 //Return number of Elements in row/bounds/ranges/objective istart
	 const int getLength  (int istart){
		return this->strt_[istart+1] - this->strt_[istart];
	}
	 //Return index array that starts at row/bounds/ranges/objective istart
	 const int * getIndices (int istart){
		return this->inds_+this->strt_[istart];
	}
	 //Return double array that starts at row/bounds/ranges/objective istart
	 const double *getElements(int istart){
		return this->dels_+this->strt_[istart];
	}


private:

	SmiStageIndex stg_; //Stage to which the node belongs
	SmiRowMap rowMap; //not used
	SmiDenseRowMap dRowMap; //Map of <int,double*>, stores row entries in dense format

	SmiCoreData *core_; //Link to core data
	bool isCoreNode_; //Is this node a core node? If so, it should, at least in my opinion, not turn up in the Tree
	SmiCoreCombineRule *combineRule_;

	int numarrays_; //= number of arrays used = 5 (clo,cup,rlo,rup,obj)
	int nels_; //number of elements in the matrix and the other arrays
	int nrow_; //number of rows (of corresponding stage in core model)
	int ncol_; //number of columns (of corresponding stage in core model)
	int rowbeg_; //index, where rows of corresponding stage begins in the core model
	int colbeg_; //index, where columns begin wrt to the columns of the corresponding stage in the core model
	int nstrt_; //number of strt indices = numarray + nrow+1 (see assignMemory())
	bool has_matrix_; //If this node has a matrix entry
	int mat_strt_; //start arrays for corresponding entries
	int clo_strt_;
	int cup_strt_;
	int obj_strt_;
	int rlo_strt_;
	int rup_strt_;
	double * dels_; //concrete elements
	int    * inds_; //indices for the elements
	int    * strt_; //start array, where new row begins

	int ptr_count; //Propably for memory management?!

	bool hasQdata_;
	SmiQuadraticDataDC *nqdata_;
};


class SmiCoreData
{
public:
	inline int getNumCols(){ return ncol_;} //Gesamtzahl Variablen f�r das CoreModell
	inline int getNumRows(){ return nrow_;} //Gesamtzahl Restriktionen f�r das CoreModell
	inline int getNumElements(){ return nz_;} // Gesamtzahl Elemente f�r das CoreModell
	inline int getNumStages(){ return nstag_;} //Anzahl an Stufen
	inline int getNumCols(SmiStageIndex t){ return nColInStage_[t];} //Anzahl Variablen in Stufe t 
	inline int getNumRows(SmiStageIndex t){ return nRowInStage_[t];} //Number of Restrictions in stage t
	inline int getColStart(SmiStageIndex t){ return stageColPtr_[t];} //Index where Variables of stage t starts
	inline int getRowStart(SmiStageIndex t){ return stageRowPtr_[t];} //Index where Rows of stage t starts
	inline int getColStage(int i){ return colStage_[i];} //Stage to Variable i
	inline int getRowStage(int i){ return rowStage_[i];} //Stage to Row i
	//Christian: TODO: In my Opinion these methods do not belong here, as it takes care of converting indices from/to det. eq. to core model formulation
	inline int getRowInternalIndex(int i){ return rowEx2In_[i];} //Get Internal Index (in case of det. eq.) from external index (core model)
	inline int getColInternalIndex(int i){ return colEx2In_[i];}
	inline int getRowExternalIndex(int i){ return rowIn2Ex_[i];} //Get external index (core model) from internal index (det. eq. model)
	inline int getColExternalIndex(int i){ return colIn2Ex_[i];}
	/*
	inline CoinPackedVector * getMatrixRow(SmiStageIndex t, int i){ return nodes_[t]->getRow(i);}
	*/
	inline const double * getDenseRowLower(SmiStageIndex t){return cdrlo_[t];}
	inline const double * getDenseRowUpper(SmiStageIndex t){return cdrup_[t];}
	inline const double * getDenseColLower(SmiStageIndex t){return cdclo_[t];}
	inline const double * getDenseColUpper(SmiStageIndex t){return cdcup_[t];}
	inline const double * getDenseObjCoefficients(SmiStageIndex t){return cdobj_[t];}

    inline std::vector<int> getIntCols(int stage) { return intColsStagewise[stage]; };
	inline int* getIntegerIndices() { return integerIndices_; } //indices of integer variables
	inline int getIntegerLength() { return integerLength_; }
    inline int* getBinaryIndices() { return binaryIndices_; } //indices of binary variables
    inline int getBinaryLength() { return binaryLength_; }
    inline double getInfinity() { return infinity_; }

	void copyRowLower(double * drlo,SmiStageIndex t );
	void copyRowUpper(double * drup,SmiStageIndex t);
	void copyColLower(double * dclo,SmiStageIndex t);
	void copyColUpper(double * dcup,SmiStageIndex t);
	void copyObjective(double * dobj,SmiStageIndex t);

	inline SmiNodeData * getNode(SmiStageIndex t){return nodes_[t];}

	inline void setColumnNames(char** namesStrict, char** namesFree) { this->colNamesStrict = namesStrict; this->colNamesFree = namesFree; }
	inline char** getColumnNames(bool strictFormat = true) { if (strictFormat) return this->colNamesStrict; else return this->colNamesFree; }

    SmiCoreData(CoinMpsIO *cMps, int nstag, int *cstag, int *rstag,int *integerIndices = 0, int integerLength = 0, int *binaryIndices = 0, int binaryLength = 0);
    SmiCoreData(OsiSolverInterface *osi, int nstag, int *cstag, int *rstag,int *integerIndices = 0, int integerLength = 0, int *binaryIndices = 0, int binaryLength = 0);

	///Adds QP data after the constructor has been called
	void addQuadraticObjectiveToCore(int *starts,int *indx,double *dels);

	void setHasQdata (bool b) {hasQdata_ = b;}
	bool hasQdata() {return hasQdata_;}


	
	~SmiCoreData();

private:

	//Christian: Creates SmiCoreData for whole tree from deterministic core model and information about stages. Splits
	// rows and columns into nstag stages, given by cstag and rstag. Creates a core node for every stage.
    void gutsOfConstructor(int nrow,int ncol,int nstag, int *cstag,int *rstag, CoinPackedMatrix *matrix, CoinPackedVector *dclo, CoinPackedVector *dcup, CoinPackedVector *dobj, CoinPackedVector *drlo, CoinPackedVector *drup, int* integerIndices = 0,int integerLength = 0,int* binaryIndices = 0,int binaryLength = 0);

    OsiSolverInterface* generateCoreProblem(OsiSolverInterface* osi);


private:
	int nrow_;
	int ncol_;
    int nz_; // We can count the total number of elements, but I do not if we need this at one point
	SmiStageIndex nstag_; //Total number of stages in the problem, apart from the first stage. A two-stage problem has nstag_ = 1 and so on. If the problem is deterministic, nstag_ = 0.
	int *nColInStage_; //Number of Columns in Stage
	int *nRowInStage_; //Number of Rows in Stage
	int *stageColPtr_; //Start Index of Columns in Stage, with respect to ncol_
	int *stageRowPtr_; //Start Index of Rows in Stage, with respect to nrow_
	int *colStage_; //To which stage belongs the column?
	int *rowStage_; //To which stage belongs the row?
	int *colEx2In_; //Not clear yet
	int *rowEx2In_; //Not clear yet
	int *colIn2Ex_; // Not clear yet
	int *rowIn2Ex_; // Not clear yet
    int *integerIndices_; //indices of integer variables
    int integerLength_; //number of integer variables
    int *binaryIndices_; //indices of binary variables
    int binaryLength_; //number of binary variables
    double infinity_; //Infinity of solver
	double **cdrlo_;
	double **cdrup_;
	double **cdobj_;
	double **cdclo_;
	double **cdcup_;
	SmiQuadraticData *sqp_;
	bool hasQdata_;
	std::vector<SmiNodeData*> nodes_; //Nodes, that contain stage dependent constraints (with Bounds,Ranges,Objective,Matrix), so called CoreNodes
	std::vector<double *> pDenseRow_; //dense probability vector
	std::vector< std::vector<int> > intColsStagewise; // For each stage separately, it contains the position of every integer column
	char **colNamesStrict;
	char **colNamesFree;
};

#endif //#define SmiScnData_HPP
