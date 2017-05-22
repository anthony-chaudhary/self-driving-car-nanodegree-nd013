// ******************** FlopCpp **********************************************
// File: MP_domain.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************
#ifndef _MP_domain_hpp_
#define _MP_domain_hpp_

#include <vector>
#include <map>
#include "MP_utilities.hpp"
#include "MP_boolean.hpp" 
#include "MP_index.hpp" 

namespace flopc {

class MP_set_base;
class MP_set;
class MP_index;

template<int nbr> class MP_subset;

    /** @brief Reference to a set of index values.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
     */
class MP_domain_base : public Functor, public MP_index_base {
    friend class MP_domain;
    friend class Handle<MP_domain_base*>;
    friend MP_domain operator*(const MP_domain& a, const MP_domain& b);
private:
    int count;
public:
    MP_domain_base();
	virtual ~MP_domain_base();

    virtual Functor* makeInsertFunctor() const;
    virtual MP_index* getIndex() const = 0;
    virtual const MP_set_base* getSet() const = 0;
    void display()const;
    virtual size_t size() const ;
    const Functor* donext;
};

    /** @brief Range over which some other constuct is defined.
        @ingroup PublicInterface
        This is one of the main public interface classes.  One uses this in
        the context of a constraint, objective, variable, or data.  It is
        usually used in conjunction with an MP_set, or a subset, but can
        be used without one. It is the range over which the other construct
        is defined.
        @li there is a special domain known as "empty".  It is static and a
        reference can be obtained using MP_domain::getEmpty();
        @li The empty set is used when defaulting in parameters
        for dimensions which are not used.
        
    */
class MP_domain : public Handle<MP_domain_base*> {
    friend MP_domain operator*(const MP_domain& a, const MP_domain& b);
    
    friend class MP_constraint;
    friend class MP_index_exp;
public:
    /** @briefConstructs a set  which points to nothing.
        @note This is not the same as the "empty" set.
     */
    MP_domain();
	~MP_domain();
    /** For internal use.
        @todo make this constructor private?
    */
    MP_domain(MP_domain_base* r);

    /** @brief Special conditional creation of a subset.

        This method allows for a test for inclusion of a condition
        during construction of a subset.  The output MP_domain will include
        references where the condition is satisfied.  
     */
    MP_domain such_that(const MP_boolean& b);

    /** @brief Special conditional operation on the domain
        
    This method will call the functor for each member of the MP_domain.
     */
    void Forall(const Functor* op) const;
    /// returns number of elements in the domain.
    size_t size() const;
    /// returns a reference to the "empty" set.
    static const MP_domain& getEmpty();
private:
	std::vector<MP_boolean> condition;
    Handle<MP_domain_base*> last;
    static const MP_domain* Empty;
};

    /** @brief Range over which some other constuct is defined.
        @ingroup PublicInterface
        This is one of the main public interface classes.  One uses this in
        the context of a constraint, objective, variable, or data.
        This class in the MP_domain family uses an MP_set and and index
        for defining the Range over which the construct is defined.
        @li This defines the domain as the contents of the set when referred
        into by the index.
        
    */
class MP_domain_set : public MP_domain_base {
public:
    /// Constructor taking a set pointer and an index pointer.
    MP_domain_set(const MP_set* s, MP_index* i);
    /// @todo should this be public?  Should it be called?  If so, needs doc.
    void operator()() const;
    /** Evaluates the index within the domain.  
        @note For internal use
     */
    int evaluate() const;
    /// Getter for the set used in construction.
    const MP_set_base* getSet() const;
    /// Getter for the index used in construction.
    MP_index* getIndex() const;
    /** @note Internal use. 
        @todo Is internal? can be private? or needs more doc.
    */
    MP_domain getDomain(MP_set* s) const ;
    ~MP_domain_set();
private:
    const MP_set* S;
     MP_index* I;
};

template<int nbr> class MP_domain_subset;

    /** @brief Inserter for construction of a subset.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
    */
    template<int nbr> class insertFunctor : public Functor {
public:
    insertFunctor(MP_domain_subset<nbr>* d) : D(d) {}
    /// @todo should this be public?  Should it be called?  If so, needs doc.
    void operator()() const { 
	std::vector<int> elm(nbr);
	for (int i=0; i<nbr; i++) {
	    elm[i] = D->I[i]->evaluate();
	}
	D->S->insert(elm);
    }
private:
    MP_domain_subset<nbr>* D;
};

    /** @brief Range over which some other constuct is defined. Uses subsetting.
        @ingroup PublicInterface
        This is one of the main public interface classes.  One uses this in
        the context of a constraint, objective, variable, or data.
        This class in the MP_domain family uses an MP_subset and a vector of
        indexes for defining the Range over which the construct is defined.
        @li This defines the domain as the contents of the subset when referred
        into by the indexes.
        
    */
template<int nbr> class MP_domain_subset : public MP_domain_base {
    friend class insertFunctor<nbr>;
public:
    MP_domain_subset<nbr> (MP_subset<nbr>* s, 
			   const std::vector<MP_index*> &i) : S(s), I(i){}

    /** Evaluates the index within the domain.  
        @note For internal use
     */
    int evaluate() const {
	return S->evaluate(I);
    }
    /// getter for obtaining the set used in construction
    MP_set_base* getSet() const {
	return S;
    }
    /// getter for obtaining the index used in construction
    MP_index* getIndex() const {
	return S;
    }
    /** @note Internal use. 
        @todo Is internal? can be private? or needs more doc.
    */
    MP_domain getDomain(MP_set* s) const {
	return MP_domain(const_cast<MP_domain_subset<nbr>*>(this));
    }
    /// @todo should this be public?  Should it be called?  If so, needs doc.
    void operator()() const {
	bool isBound[nbr];
	bool allBound = true;
	for (int j=0; j<nbr; j++) {
	    if (I[j]->isInstantiated() == true) {
		isBound[j] = true;
	    } else {
		isBound[j] = false;
		allBound = false;
		if (I[j]!=&MP_index::getEmpty()) { 
		    I[j]->instantiate();
		}
	    }
	}
	if (allBound == true) {
	    (*donext)(); 
	} else {
	    std::map<std::vector<int>, int>::const_iterator i;
	    int counter = 0;
	    for (i = S->elements.begin(); i != S->elements.end(); i++) {
		S->assign(counter);
		counter++;
		bool goOn = true;
		for (int j=0; j<nbr; j++) {
		    if (isBound[j] == true) {
			if (I[j]->evaluate() != i->first[j]) {
			    goOn = false;
			    break;
			}
		    } else {
			I[j]->assign(i->first[j]);
		    }
		}
		if (goOn == true) {
		    (*donext)();
		}
	    }
	}
	for (int j=0; j<nbr; j++) {
	    if (isBound[j] == false) {
		I[j]->assign(0);
		I[j]->unInstantiate();
	    }
	}
    }    

    /// @todo should this be public?  Should it be called?  If so, needs doc.
    Functor* makeInsertFunctor() const {
	return new insertFunctor<nbr>(
	    const_cast<MP_domain_subset<nbr>*>(this));
    }
private:
    MP_subset<nbr>* S;
    std::vector<MP_index*> I;
};

    /** operator which creates a new domain as the pairwise combinations
        of two input domains.
     */
MP_domain operator*(const MP_domain& a, const MP_domain& b);
  
}  // End of namespace flopc
#endif
