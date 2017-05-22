// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SmiDiscreteRV_H
#define SmiDiscreteRV_H

/** Smi Discrete Distribution

  This class is used for storing and accessing a discrete probability distribution.

  Can't think of a way to do discrete distributions with multiple cores,
  so I don't include a way to set a different core.

*/

#include "CoinPragma.hpp"
#include "SmiScnData.hpp"
#include "SmiLinearData.hpp"

#include <vector>

//forward declarations
class SmiDiscreteRV;

class SmiDiscreteDistribution
{
	friend void SmiDiscreteUnitTest();
public:
	/// add discrete RV
	void addDiscreteRV(SmiDiscreteRV *s)
	{ smiDiscrete_.push_back(s); }

	/// get discrete RV
	SmiDiscreteRV * getDiscreteRV(int i) {return smiDiscrete_[i];}

	/// get number of RV
	int getNumRV() { return (int)smiDiscrete_.size(); }

	/// get core model 
	SmiCoreData *getCore(){ return core_; }

	/// set combine rule
	inline void setCombineWithCoreRule(SmiCoreCombineRule *r){
			combineRule_ = r;
	}

	/// get combine rule
	inline SmiCoreCombineRule *getCombineWithCoreRule() { return combineRule_;}

	
	/// constructor requires core data and combine rule
	SmiDiscreteDistribution(SmiCoreData *c, SmiCoreCombineRule *r=SmiCoreCombineReplace::Instance())
	{
		core_=c;
		this->setCombineWithCoreRule(r);		
	}

	~SmiDiscreteDistribution();

private:
	SmiDiscreteDistribution(){core_=NULL;}
	SmiCoreData *core_;
	std::vector<SmiDiscreteRV *> smiDiscrete_;
	SmiCoreCombineRule *combineRule_;


};


class SmiDiscreteEvent : public SmiLinearData
{
	friend void SmiDiscreteUnitTest();
public:
	inline void setEventProb(double p) {prob_ = p;}
	inline const double getEventProb () { return prob_; }
	SmiDiscreteEvent(SmiLinearData d, double p):SmiLinearData(d),prob_(p){}
	~SmiDiscreteEvent(){}
private:
	double prob_;
};


class SmiDiscreteRV
{
	friend void SmiDiscreteUnitTest();
public:
	void addEvent(CoinPackedMatrix &matrix,
				CoinPackedVector &dclo, CoinPackedVector &dcup,
				CoinPackedVector &dobj,
				CoinPackedVector &drlo, CoinPackedVector &drup, double prob)
	{
		SmiLinearData d(matrix,dclo,dcup,dobj,drlo,drup);
		SmiDiscreteEvent *e = new SmiDiscreteEvent(d,prob);
		events_.push_back(e); 
		prob_+=prob;
	}
	void addEvent(OsiSolverInterface &osi, double prob)
	{
		SmiLinearData d(osi);
		SmiDiscreteEvent *e = new SmiDiscreteEvent(d,prob);
		events_.push_back(e); 
		prob_+=prob;
	}

	inline const CoinPackedMatrix &getEventMatrix(int i)   {return events_[i]->getMatrix(); }
	inline const CoinPackedVector &getEventColLower(int i) {return events_[i]->getColLower();}
	inline const CoinPackedVector &getEventColUpper(int i) {return events_[i]->getColUpper();}
	inline const CoinPackedVector &getEventObjective(int i){return events_[i]->getObjective();}
	inline const CoinPackedVector &getEventRowLower(int i) {return events_[i]->getRowLower();}
	inline const CoinPackedVector &getEventRowUpper(int i) {return events_[i]->getRowUpper();}
	double getEventProb(int i) { 
	//	return events_[i]->getEventProb()/prob_;
			return events_[i]->getEventProb();
	}
	inline size_t getNumEvents() { return events_.size(); }
	inline int getStage() {return stg_;}
	inline void setStage(int p) {stg_=p;}
	SmiDiscreteRV(){prob_=0.0; stg_=-1;}
	SmiDiscreteRV(int p) {prob_=0.0; stg_=p;}
	~SmiDiscreteRV(){
		for (size_t i=0; i<events_.size(); ++i)
			delete events_[i];
	}
private:
	std::vector<SmiDiscreteEvent *> events_;
	SmiStageIndex  stg_;
	double prob_;
};

void SmiDiscreteUnitTest();

#endif //SmiDiscreteRV_H

