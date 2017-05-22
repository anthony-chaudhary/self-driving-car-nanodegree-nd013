// ******************** FlopCpp **********************************************
// File: MP_set.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _MP_set_hpp_
#define _MP_set_hpp_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "MP_domain.hpp"
#include "MP_index.hpp"
#include "MP_utilities.hpp"

namespace flopc {

/** @brief Internal representation of a "set" 
    @ingroup INTERNAL_USE
    @note FOR INTERNAL USE: This is not normally used directly by the calling code.
*/
class MP_set_base : public MP_index , public Named {
public:
    MP_set_base() : Cyclic(false) {}

    virtual int size() const = 0;
    virtual operator MP_domain() const = 0;
    virtual MP_domain operator()(const MP_index_exp& i) const = 0;
    void display()const;

    int check(int i) const {
	if ((i>=0) && (i<size())) {
	    return i;
	} else {
	    if (Cyclic == true) {
		return mod(i,size());
	    } else {
		return outOfBound;
	    }
	}
    }
    int checkStage(int i) const {
	if ((i>=0) && (i<size())) {
	    return i*isStage();
	} else {
	    if (Cyclic == true) {
		return mod(i,size())*isStage();
	    } else {
		return outOfBound;
	    }
	}
    }
    ///
    virtual int isStage() const {
	return 0;
    }

    bool Cyclic;
};

    
/** @brief Representation of a set for indexing into some other construct.
    @ingroup PublicInterface
    This is one of the main public interface classes.  One uses this when
    constructing MP_domains, and subsets.
    It is frequent that one would directly construct sets of indices, then
    use expressions to subset or slice the data.
    @note term: cardinality is the number of elements in the set.
    @note term: dimension is the number of indices used to reference into it.
    @note there is a distince 'empty' MP_set
*/
class MP_set : public MP_set_base {
public:
    /// constructs a set with specific cardinality.
    MP_set(int i = 0): cardinality(i) {}
    /** @brief Constructs an MP_domain on the stack given an index expression
        into the set.
        @todo is the internal use?
     */
    MP_domain operator()(const MP_index_exp& i) const {
	return i->getDomain(const_cast<MP_set*>(this));
    }
    /// @brief constructs an MP_domain from the MP_set.
    operator MP_domain() const {
	return new MP_domain_set(this,const_cast<MP_set*>(this));
    }
    /** @brief constructs  a domain by subsetting this MP_set where
        the MP_boolean evaluates to 'true'
     */
    MP_domain such_that(const MP_boolean& b) {
	return (MP_domain(new MP_domain_set(this,this))).such_that(b);
    }
    /** setter for 'cyclic' property
        @todo better explain the use of cyclic.
     */
    void cyclic() {
	Cyclic = true;
    }
    /// getter for the cardinality of this MP_set.
    virtual int size() const {
	return cardinality;
    }
    int last() {
	return cardinality-1;
    }
    /// gets the distinct 'empty' MP_set.
    static MP_set &getEmpty();
private:
    static MP_set Empty;
    int cardinality;
};
    
class MP_stage : public MP_set {
public:
    MP_stage(int i = 0): MP_set(i) {}
    virtual int isStage() const {
	return 1;
    }
};

template <int nbr> class MP_subset;

/** @brief Internal representation of a "set" 
    @ingroup INTERNAL_USE
    @note FOR INTERNAL USE: This is not normally used directly by the calling code.
*/

template<int nbr> class InsertFunctor : public Functor {
public:
    InsertFunctor( MP_subset<nbr>* s, std::vector<MP_index_exp> i) 
	: S(s), I(i) {}
    void operator()() const { 
	std::vector<int> elm(nbr);
	for (int i=0; i<nbr; i++) {
	    elm[i] = I[i]->evaluate();
	}
	S->insert(elm);
    }
private:
    MP_subset<nbr>* S;
    std::vector<MP_index_exp> I;
};

template <int nbr> class SubsetRef;

/** @brief Internal representation of a "set" 
    @ingroup INTERNAL_USE
    @note FOR INTERNAL USE: This is not normally used directly by the
    calling code.
    @note this is often implicitly created with many expressions which may
    subset a set.
*/

template <int nbr>
class MP_subset : public MP_set {
    friend class MP_domain_subset<nbr>;
    friend class SubsetRef<nbr>;
public:
    MP_subset(const MP_set& s1, 
	      const MP_set& s2=MP_set::getEmpty(), 
	      const MP_set& s3=MP_set::getEmpty(), 
	      const MP_set& s4=MP_set::getEmpty(), 
	      const MP_set& s5=MP_set::getEmpty()) {
	S = makeVector<nbr,const MP_set*>(&s1,&s2,&s3,&s4,&s5);
    }
	void display(const std::string& s = "") const 
	{
// 		Messenger &msgr = *MP_model::getCurrentModel()->getMessenger();
// 		msgr.logMessage(5,s.c_str());
// 		std::map<std::vector<int>, int>::const_iterator i;
// 		for (i = elements.begin(); i != elements.end(); i++) 
// 		{
// 			std::stringstream ss;
// 			for (int j=0; j<nbr; j++) 
// 			{
// 				ss<<(*i).first[j]<<"  ";
// 			}
// 			ss<<(*i).second<<std::ends;
// 			msgr.logMessage(5,ss.str().c_str());
// 		}
	}

    MP_subset(std::vector<const MP_set*> s) : S(s) {}

    ~MP_subset() {}

    int operator()(int i1, int i2=0, int i3=0, int i4=0, int i5=0) {
	std::map<std::vector<int>, int>::const_iterator pos;
	pos = elements.find(makeVector<nbr>(i1, i2, i3, i4, i5));
	if (pos==elements.end()) {
	    return outOfBound;
	} else {
	    return pos->second;
	}
    }

    SubsetRef<nbr>& operator()(const MP_index_exp& i1, 
			       const MP_index_exp& i2=MP_index::getEmpty(),  
			       const MP_index_exp& i3=MP_index::getEmpty(),
			       const MP_index_exp& i4=MP_index::getEmpty(),
			       const MP_index_exp& i5=MP_index::getEmpty()) {
	return *new SubsetRef<nbr>(this,i1,i2,i3,i4,i5);
    }

    MP_domain& operator()(const SUBSETREF& s) {
	return MP_domain(s);
    }

    int evaluate(const std::vector<MP_index*>& I) const {
	std::vector<int> vi;
	for (int k=0; k<nbr; k++) {
	    int temp = I[k]->evaluate();
	    vi.push_back(temp);
	}
	std::map<std::vector<int>, int>::const_iterator pos;
	pos = elements.find(vi);
	if (pos==elements.end()) {
	    return outOfBound;
	} else {
	    return pos->second;
	}
    }
    
    void insert(const std::vector<int> &args) {
	bool isOk = true;
	for (int i=0; i<nbr; i++) {
	    if ( S[i]->check(args[i]) == outOfBound ) {
		isOk = false;
	    }
	}
	if (isOk == true) {
	    std::map<std::vector<int>, int>::const_iterator pos;
	    pos = elements.find(args);
	    if (pos==elements.end()) {  // insert if not existent
		const int v = static_cast<int>(elements.size());
		elements[args] = v;
	    }
	}
    }
    void insert(int i1, int i2=0, int i3=0, int i4=0, int i5=0) {
	insert(makeVector<nbr>(i1, i2, i3, i4, i5));
    }
    const InsertFunctor<nbr>& insert(MP_index_exp i1, 
				     MP_index_exp i2=MP_index_exp::getEmpty(), 
				     MP_index_exp i3=MP_index_exp::getEmpty(), 
				     MP_index_exp i4=MP_index_exp::getEmpty(), 
				     MP_index_exp i5=MP_index_exp::getEmpty()) {
	return *new InsertFunctor<nbr>(this,makeVector<nbr>(i1, i2, i3, i4, i5));
    }
    virtual int size() const {
	return static_cast<int>(elements.size());
    }

private:
    std::vector<const MP_set*> S; 
	std::map<std::vector<int>, int> elements;
};

/** @brief Internal representation of a "set" 
    @ingroup INTERNAL_USE
    @note FOR INTERNAL USE: This is not normally used directly by the
    calling code.
    @note this is often implicitly created with many expressions which may
    subset a set.
*/

    class SUBSETREF : public MP_index_base {
    public:
	virtual MP_index* getIndex() const {
	    return 0;
	}
	virtual MP_domain getDomain(MP_set* s) const {
	    return MP_domain::getEmpty();
	}
	int evaluate() const {
	    return 0;
	}
    };

/** @brief Internal representation of a "set" 
    @ingroup INTERNAL_USE
    @note FOR INTERNAL USE: This is not normally used directly by the
    calling code.
    @note this is often implicitly created with many expressions which may
    subset a set.
*/
    template <int nbr>
    class SubsetRef : public SUBSETREF {
    public:
	SubsetRef(MP_subset<nbr>* s, 
		  const MP_index_exp& i1,
		  const MP_index_exp& i2,
		  const MP_index_exp& i3,
		  const MP_index_exp& i4,
		  const MP_index_exp& i5) : 
	    S(s),I1(i1),I2(i2),I3(i3),I4(i4),I5(i5) {} 
	void display()const
	{
// 		Messenger &msgr=*MP_model::getCurrentModel()->getMessenger();
// 		msgr.logMessage(5,toString().c_str());
	}
    
	operator MP_domain() const {
// 	    MP_domain_base* base;
// 	    base = new MP_domain_subset<nbr>(S,
// 		makeVector<nbr>(I1->getIndex(), I2->getIndex(), 
// 				I3->getIndex(), I4->getIndex(), 
// 				I5->getIndex()) );
// 	    base->such_that(B);
// 	    return base; 
	    return new MP_domain_subset<nbr>(S,
 		          makeVector<nbr>(I1->getIndex(), I2->getIndex(), 
					  I3->getIndex(), I4->getIndex(), 
					  I5->getIndex()) );
	}
    
	virtual MP_domain getDomain(MP_set* s) const {
	    return new MP_domain_subset<nbr>(S,
	        makeVector<nbr>(I1->getIndex(), I2->getIndex(), 
				I3->getIndex(), I4->getIndex(), 
				I5->getIndex()) );
	}

	SubsetRef& such_that(const MP_boolean& b) {
	    B = b;
	    return *this;
	}

 	int evaluate() const {
	    std::vector<MP_index_exp> I = makeVector<nbr>(I1,I2,I3,I4,I5);
	    std::vector<int> vi;
	    for (int k=0; k<nbr; k++) {
		int temp = I[k]->evaluate();
		vi.push_back(temp);
	    }
	    std::map<std::vector<int>, int>::const_iterator pos;
	    pos = S->elements.find(vi);
	    if (pos==S->elements.end()) {
		return outOfBound;
	    } else {
		return pos->second;
	    }
	}
	MP_index* getIndex() const {
	    return S;
	}
	MP_boolean B;
	MP_subset<nbr>* S;
	MP_index_exp I1,I2,I3,I4,I5;
    };

} // End of namespace flopc
#endif
