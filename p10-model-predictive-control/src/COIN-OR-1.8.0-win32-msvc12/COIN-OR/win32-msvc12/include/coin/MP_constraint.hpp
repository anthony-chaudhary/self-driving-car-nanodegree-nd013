// ******************** FlopCpp **********************************************
// File: MP_constraint.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _MP_constraint_hpp_
#define _MP_constraint_hpp_

#include <set>
#include <string>

#include "MP_set.hpp"
#include "MP_domain.hpp"
#include "MP_utilities.hpp"
#include "MP_expression.hpp"
#include "MP_boolean.hpp"
#include "MP_data.hpp"

namespace flopc {

    class MP_constraint;
    class Constant;
    class MP_model;
    class MP_variable;

    /** @brief Enumeration for indicating direction of a constraint.
        @ingroup INTERNAL_USE
     */
    enum  Sense_enum {LE,GE,EQ};

    /** Semantic representation of a constraint in a Math Program
        @ingroup INTERNAL_USE
        @see MP_constraint for a public interface.
        @note of interest is the operator overloads which are 'friends'
     */
    class Constraint {
	friend class MP_constraint;
	friend class MP_model;
	friend Constraint operator<=(const MP_expression& l, const MP_expression& r);
	friend Constraint operator<=(const Constant& l, const MP_expression& r); 
	friend Constraint operator<=(const MP_expression& l, const Constant& r); 
	friend Constraint operator<=(const VariableRef& l, const VariableRef& r); 

	friend Constraint operator>=(const MP_expression& l, const MP_expression& r);
	friend Constraint operator>=(const Constant& l, const MP_expression& r); 
	friend Constraint operator>=(const MP_expression& l, const Constant& r); 
	friend Constraint operator>=(const VariableRef& l, const VariableRef& r);

	friend Constraint operator==(const MP_expression& l, const MP_expression& r);
	friend Constraint operator==(const Constant& l, const MP_expression& r); 
	friend Constraint operator==(const MP_expression& l, const Constant& r); 
	friend Constraint operator==(const VariableRef& l, const VariableRef& r); 
    private:
	Constraint(const MP_expression& l, const MP_expression& r, Sense_enum s) : 
	    left(l), right(r), sense(s) {}

	MP_expression left,right;
	Sense_enum sense;
    };

    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator<=(const MP_expression& l, const MP_expression& r) {
	return Constraint(l, r, LE);
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator<=(const Constant& l, const MP_expression& r) {
	return operator<=(MP_expression(l), r);
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator<=(const MP_expression& l, const Constant& r){
	return operator<=(l, MP_expression(r));
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator<=(const VariableRef& l, const VariableRef& r) {
	return *new Constraint(l, r, LE);
    }
    
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator>=(const MP_expression& l, const MP_expression& r) {
	return *new Constraint(l, r, GE);
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator>=(const Constant& l, const MP_expression& r){
	return operator>=(MP_expression(l), r);
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator>=(const MP_expression& l, const Constant& r){
	return operator>=(l, MP_expression(r));
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator>=(const VariableRef& l, const VariableRef& r) {
	return *new Constraint(l, r, GE);
    }
    
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator==(const MP_expression& l, const MP_expression& r) {
	return *new Constraint(l, r, EQ);
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator==(const Constant& l, const MP_expression& r){
	return operator==(MP_expression(l), r);
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator==(const MP_expression& l, const Constant& r) {
	return operator==(l, MP_expression(r));
    }
    /** @brief Uses operator overloading to construct an Constraint 
        @ingroup PublicInterface
        Constucts a Constraint using operator overloading.
        @see MP_constraint
     */
    inline Constraint operator==(const VariableRef& l, const VariableRef& r) {
	return *new Constraint(l, r, EQ);
    }


    class GenerateFunctor;

    /** @brief Semantic representation of a linear constraint.
    @ingroup PublicInterface
    This is one of the main public interface classes.  It is always constructed
    through operator overloading between expressions, constants, and
    variables.  
    There are many 'friend' overloaded operators to do the constuction.
    The basic idea is to make the constraint look like a paper-model
    constraint in C++ code.  Once constructed, it should be added to the model.

    The snippet below is an overly simplistic example, but is ok for
    illustration.
    <code> <br>
    MP_model aModel; // your model<br>
    MP_set I; // the set the constraint is defined over. <br>
    MP_variable x(I); // your variable<br>
    ...<br>
    MP_constraint cons(I); // construct the right number of constraints.<br>
    cons = x <= 3;<br> // Assign in the semantic rep to it.
    aModel.add(cons); // add it to the model <br>
    </code>
    <br>
    There is quite a bit of C++ machinery going on there.
	@li MP_expression(const VariableRef& v); converts the VariableRef x into an MP_expression.
	@li MP_constraint cons(I); construct the right dimensioned sized bundle of constraints.<br>
	@li friend Constraint operator<=(const MP_expression& l, const Constant& r);  converts the x <= 3 into an Constraint.
    @todo more work on MP_constraint.
    
    */
    class MP_constraint : public RowMajor, public Named {
    public: 
        /// construct the MP_constraint with appropriate sets for indexing.
	MP_constraint(
	    const MP_set_base &s1 = MP_set::getEmpty(), 
	    const MP_set_base &s2 = MP_set::getEmpty(), 
	    const MP_set_base &s3 = MP_set::getEmpty(),
	    const MP_set_base &s4 = MP_set::getEmpty(), 
	    const MP_set_base &s5 = MP_set::getEmpty()
	    );

	MP_constraint& operator()(
	    const MP_index_exp& i1 = MP_index_exp::getEmpty(), 
	    const MP_index_exp& i2 = MP_index_exp::getEmpty(), 
	    const MP_index_exp& i3 = MP_index_exp::getEmpty(), 
	    const MP_index_exp& i4 = MP_index_exp::getEmpty(), 
	    const MP_index_exp& i5 = MP_index_exp::getEmpty()
	    )  {
	    I1 = i1; I2 = i2; I3 = i3; I4 = i4; I5 = i5;
	    return *this;
	}

	operator int() {
	    return offset + f(I1->evaluate(),I2->evaluate(),I3->evaluate(),
			      I4->evaluate(),I5->evaluate());
	}

	virtual ~MP_constraint() {}

	double price(int i1=0, int i2=0, int i3=0, int i4=0, int i5=0) const;

	void coefficients(GenerateFunctor& f);

	int row_number() const;

	MP_constraint& such_that(const MP_boolean& b) {
	    B = b; 
	    return *this; 
	}

	void insertVariables(std::set<MP_variable*>& v);

	void operator=(const Constraint& v); 
    
	void display(std::string s="") const;

	MP_model* M;
	int offset;
	MP_expression left,right;
	Sense_enum sense;
    private:
	MP_boolean B;
	const MP_set_base &S1, &S2, &S3, &S4, &S5; 
	MP_index_exp I1, I2, I3, I4, I5;
    };

}  // End of namespace flopc
#endif
