// ******************** FlopCpp **********************************************
// File: MP_expression.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _MP_expression_hpp_
#define _MP_expression_hpp_

#include <vector>
#include <set>

#include "MP_domain.hpp"
#include "MP_constant.hpp"
#include "MP_utilities.hpp"

namespace flopc {

    class Boolean;
    class MP_domain;
    class MP_constraint;
    class Row;
    class MP_variable;
    class VariableRef;
    class CoefLess;

    /** @brief Internal representation of a Coefficient in a matrix.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
    */
    struct Coef {
	Coef(int c, int r, double v, int s = 0) : 
	    col(c), row(r), stage(s), val(v)  {}
	int col, row, stage;
	double val;
    };

    class TerminalExpression;

    /** @brief Functor to facilitate generation of coefficients.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
    */
    class GenerateFunctor : public Functor {
    public:
	GenerateFunctor(std::vector<Coef>& coefs) : Coefs(coefs) {}

	virtual ~GenerateFunctor(){}

	void setConstraint(MP_constraint* r) {
	    R = r;
	}
	void setMultiplicator(std::vector<Constant>& mults, double m) {
	    multiplicators = mults;
	    m_ = m;
	}
	void setTerminalExpression(const TerminalExpression* c) {
	    C = c;
	}
	virtual int row_number() const;

	void operator()() const;

	double m_;
	std::vector<Constant> multiplicators;
	MP_constraint* R;
	const TerminalExpression* C;
	std::vector<Coef>& Coefs;
    };

    /** @brief Functor to facilitate generation of the objective function.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
    */
    class ObjectiveGenerateFunctor : public GenerateFunctor {
    public:
	ObjectiveGenerateFunctor(std::vector<Coef>& coefs) : GenerateFunctor(coefs) {}
	virtual int row_number() const {
	    return -1;
	}
    };

    /** @brief The base class for all expressions.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
    */
    class MP_expression_base {
	friend class MP_expression;
	friend class Handle<MP_expression_base*>;
    private:
	int count;
    public:
	MP_expression_base() : count(0) {}

	virtual double level() const = 0;
	virtual void generate(const MP_domain& domain,
			      std::vector<Constant> multiplicators,
			      GenerateFunctor& f,
			      double m) const = 0;
	virtual void insertVariables(std::set<MP_variable*>& v) const = 0;

	virtual ~MP_expression_base() {}
    };

    /** @brief Symbolic representation of a linear expression.
        @ingroup PublicInterface
        This is one of the main public interface classes.  It is the basis for
        all linear expressions, including constraints, objective function,
        and expressions involving indexes.
        <br> Although these can be created directly and independently, it
        is expected these will be created through the use of the operators
        which are later in this file.  (operator+, operator-, etc.)
        @note There are constructors which are (silently) used to convert \
        other componenets into expressions.
    */
    class MP_expression : public Handle<MP_expression_base*> {
	friend class MP_constraint;
    public:
        /// default constructor
	MP_expression() : Handle<MP_expression_base*>(0) {}
        /** Constructor for internal use
        @todo should this be private?
        */
	MP_expression(MP_expression_base* r) : Handle<MP_expression_base*>(r) {}
    /// Constructor which (silently) converts a Constant to a MP_expression
	MP_expression(const Constant& c);
    /// Constructor which (silently) converts a Variable to a MP_expression
	MP_expression(const VariableRef& v);
	virtual ~MP_expression() {}
    };

    /** @brief The base class for all expressions.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
        @todo can this be moved to the cpp file?
    */
    class TerminalExpression : public MP_expression_base {
    public:
	virtual double getValue() const = 0; 
	virtual int getColumn() const = 0;
	virtual int getStage() const = 0;
    };

    /** @brief The base class for all expressions.
        @ingroup INTERNAL_USE
        @note FOR INTERNAL USE: This is not normally used directly by the
        calling code.
        @todo can this be moved to the cpp file?
    */
    class Expression_operator : public MP_expression_base {
    public:
	Expression_operator(const MP_expression& e1, const MP_expression& e2) : 
	    left(e1),right(e2) {}
	void insertVariables(std::set<MP_variable*>& v) const {
	    left->insertVariables(v);
	    right->insertVariables(v);
	}
    protected:
	MP_expression left,right;
    };

    /// Operator which sums two MP_expressions, forms a new MP_expression
    MP_expression operator+(const MP_expression& e1, const MP_expression& e2);
    /** Operator which sums an MP_expression and a constant, and forms a new
        MP_expression
     */
    MP_expression operator+(const MP_expression& e1, const Constant& e2);
    /** Operator which sums a constant and an MP_expression , and forms a new
        MP_expression
     */
    MP_expression operator+(const Constant& e1, const MP_expression& e2);
    /** Operator which subtracts an MP_expression from an MP_expression, and
        forms a new MP_expression
     */
    MP_expression operator-(const MP_expression& e1, const MP_expression& e2);
    /** Operator which subtracts a Constant from an MP_expression, and
        forms a new MP_expression
     */
    MP_expression operator-(const MP_expression& e1, const Constant& e2);
    /** Operator which subtracts an MP_expression from a Constant, and
        forms a new MP_expression
     */
    MP_expression operator-(const Constant& e1, const MP_expression& e2);
    /** Operator which multiplies a Constant by an MP_expression, and
        forms a new MP_expression
     */
    MP_expression operator*(const Constant& e1, const MP_expression& e2); 
    /** Operator which multiplies an MP_expression by  a Constant, and
        forms a new MP_expression
     */
    MP_expression operator*(const MP_expression& e1, const Constant& e2);
    /** forms an expression by summing an expression over a domain.
        @note it's expected that the expression is defined over that domain.
     */
    MP_expression sum(const MP_domain& d, const MP_expression& e);

} // End of namespace flopc
#endif
