// ******************** FlopCpp **********************************************
// File: MP_boolean.hpp
// $Id$
// Author: Tim Helge Hultberg (thh@mat.ua.pt)
// Copyright (C) 2003 Tim Helge Hultberg
// All Rights Reserved.
// ****************************************************************************

#ifndef _MP_boolean_hpp_
#define _MP_boolean_hpp_

#include "MP_index.hpp"
#include "MP_constant.hpp"
#include "MP_utilities.hpp"

namespace flopc {

    /** @brief Base class for all "boolean" types of data.
    @ingroup INTERNAL_USE
    @note FOR INTERNAL USE: This is not normally used directly by the
    calling code.
    */
    class Boolean_base {
	friend class Handle<Boolean_base*>;
	friend class MP_boolean;
    public:
	virtual bool evaluate() const = 0;
    protected:
	Boolean_base() : count(0) {}
	virtual ~Boolean_base() {}

	int count;
    };

    class SUBSETREF;

    /** @brief Reference counted class for all "boolean" types of data.
    @ingroup PublicInterface
    This contains counters to ConstantBase pointers.
    These pointers may be of any of the Boolean_base * type.  
    This can be a constant valued boolean as well.  
    @TODO explain SUBSETREF
    @TODO explain using pointer in -- should be private?
    */
    class MP_boolean : public Handle<Boolean_base*> {
	friend class MP_domain_base_;
    public:
	MP_boolean() : Handle<Boolean_base*>(0) {}
	MP_boolean(bool b);
	MP_boolean(const Constant& c); 
	MP_boolean(SUBSETREF& c); 
	MP_boolean(Boolean_base* r) : Handle<Boolean_base*>(r) {}

    };

    /** @brief For computing the logical negation of a boolean
    @ingroup PublicInterface
    This is used in the normal formation of an expression.
    @param b boolean 
    @returns A boolean which evaluates to the negation of the input expression.
    @todo true? haven't used it.
    */
    MP_boolean operator!(const MP_boolean& b);

    /** @brief For computing the logical AND of two booleans
    @ingroup PublicInterface
    This is used in the normal formation of an expression.
    @param e1 first boolean 
    @param e2 second boolean 
    @returns A boolean which evaluates to true of both booleans are true.
    @todo true? haven't used it.
    */
    MP_boolean operator&&(const MP_boolean& e1, const MP_boolean& e2);
    /** @brief For computing the logical OR of two booleans
    @ingroup PublicInterface
    This is used in the normal formation of an expression.
    @param e1 first boolean 
    @param e2 second boolean 
    @returns A boolean which evaluates to true if either booleans are true.
    @todo true? haven't used it.
    */
    MP_boolean operator||(const MP_boolean& e1, const MP_boolean& e2);

    /** @brief boolean which returns true if all in domain evaluate to true.
    @ingroup PublicInterface
    This is used in the normal formation of an expression.
    @param d MP_domain to evaluate with
    @param b boolean expression to evaluate.
    @returns A boolean which evaluates to true all domain evaluations of the boolean evaluate to true.
    @todo true? haven't used it.
    */
    MP_boolean alltrue(const MP_domain& d, const MP_boolean& b);

    /** @brief constructs a boolean evaluator using operator overloading
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This is useful when combining index expressions.
        @param e1 is an index expression involving an MP_index
        @param e2 second index expression
        @li used in forming sets of tuples of index values, or subsets.
        @todo improve this a bit?
        
        The brief code below is a bit contrived, but the right hand side
        illustrate the utility of combining an index expression.  
        <code> 
        MP_index i; 
        <br>MP_index j;
        <br>MP_boolean &b = (i+5)<=(j);
        </code> 
     */
    MP_boolean operator<=(const MP_index_exp& e1, const MP_index_exp& e2);
    /** @brief constructs a boolean evaluator by comparing two constants.
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This utility of this is when comparing constants 
        @param e1 first constant expression
        @param e2 second constant expression
    */
    MP_boolean operator<=(const Constant& e1, const Constant& e2);
    /** @brief constructs a boolean evaluator using operator overloading
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This is useful when combining index expressions.
        @param e1 is an index expression involving an MP_index
        @param e2 second index expression
        @li used in forming sets of tuples of index values, or subsets.
        @todo improve this a bit?
    */
    MP_boolean operator<(const MP_index_exp& e1, const MP_index_exp& e2);
    /** @brief constructs a boolean evaluator by comparing two constants.
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This utility of this is when comparing constants 
        @param e1 first constant expression
        @param e2 second constant expression
    */
    MP_boolean operator<(const Constant& e1, const Constant& e2); 
    /** @brief constructs a boolean evaluator using operator overloading
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This is useful when combining index expressions.
        @param e1 is an index expression involving an MP_index
        @param e2 second index expression
        @li used in forming sets of tuples of index values, or subsets.
        @todo improve this a bit?
    */
    MP_boolean operator>=(const MP_index_exp& e1, const MP_index_exp& e2);
    /** @brief constructs a boolean evaluator by comparing two constants.
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This utility of this is when comparing constants 
        @param e1 first constant expression
        @param e2 second constant expression
    */
    MP_boolean operator>=(const Constant& e1, const Constant& e2);
    /** @brief constructs a boolean evaluator using operator overloading
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This is useful when combining index expressions.
        @param e1 is an index expression involving an MP_index
        @param e2 second index expression
        @li used in forming sets of tuples of index values, or subsets.
        @todo improve this a bit?
    */
    MP_boolean operator>(const MP_index_exp& e1, const MP_index_exp& e2);
    /** @brief constructs a boolean evaluator by comparing two constants.
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This utility of this is when comparing constants 
        @param e1 first constant expression
        @param e2 second constant expression
    */
    MP_boolean operator>(const Constant& e1, const Constant& e2);
    /** @brief constructs a boolean evaluator using operator overloading
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This is useful when combining index expressions.
        @param e1 is an index expression involving an MP_index
        @param e2 second index expression
        @li used in forming sets of tuples of index values, or subsets.
        @todo improve this a bit?
    */
    MP_boolean operator==(const MP_index_exp& e1, const MP_index_exp& e2);
    /** @brief constructs a boolean evaluator by comparing two constants.
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This utility of this is when comparing constants 
        @param e1 first constant expression
        @param e2 second constant expression
    */
    MP_boolean operator==(const Constant& e1, const Constant& e2);
    /** @brief constructs a boolean evaluator using operator overloading
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This is useful when combining index expressions.
        @param e1 is an index expression involving an MP_index
        @param e2 second index expression
        @li used in forming sets of tuples of index values, or subsets.
        @todo improve this a bit?
    */
    MP_boolean operator!=(const MP_index_exp& e1, const MP_index_exp& e2);
    /** @brief constructs a boolean evaluator by comparing two constants.
        @ingroup PublicInterface
        This is used in the normal formation of an expression.
        This utility of this is when comparing constants 
        @param e1 first constant expression
        @param e2 second constant expression
    */
    MP_boolean operator!=(const Constant& e1, const Constant& e2);

} // End of namespace flopc
#endif
