/* $Id: OSInstance.h 5014 2015-05-19 20:39:39Z Gassmann $ */
/** @file OSInstance.h
 * \brief This file defines the OSInstance class along with its supporting classes.
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2012, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 *<p> 1. Elements become objects of class type (the ComplexType is the class) </p>
 *
 *<p> 2. The attributes, children of the element, and text correspond to members of the class.  </p>
 *       (Note text does not have a name and becomes .value)
 *
 *<p> 3. Model groups such as choice and sequence and all correspond to arrays </p>
 *
 *<p><b>Exceptions:</b> </p>
 *<ol>
 *<li> anything specific to XML such as base64, multi, incr does not go into classes </li>
 *<li> The root OSnLNode of each <nl> element is called ExpressionTree </li>
 *<li> Root is not called osil; it is called osinstance </li>
 *</ol>
 */

#ifndef OSINSTANCE_H
#define OSINSTANCE_H
#include "OSConfig.h"
#include "OSParameters.h"
#include "OSGeneral.h"
#include "OSMatrix.h"
#include "OSnLNode.h"
#include "OSExpressionTree.h"
#include <string>
#include <map>


/*! \class Variable
 * \brief The in-memory representation of the <b>variable</b> element.
 */
class Variable
{
public:
    /** The Variable class constructor */
    Variable();

    /** The Variable class destructor */
    ~Variable();

    /** lb corresponds to the optional attribute that holds the variable lower bound.
     *  The default value is 0
     */
    double lb;

    /** ub corresponds to the optional attribute that holds the variable upper bound.
     *  The default value is OSINFINITY
     */
    double ub;

    /** type corresponds to the attribute that holds the variable type: C (Continuous),
     *  B (binary), I (general integer), or S (string). The default is C
     */
    char type;

    /** name corresponds to the optional attribute that holds the variable name, the
     *  default value is empty
     */
    std::string name;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Variable *that);
}; // class Variable


/*! \class Variables
 * \brief The in-memory representation of the <b>variables</b> element.
 */
class Variables
{
public:

    /** The Variables class constructor */
    Variables();

    /** The Variables class destructor */
    ~Variables();

    /** numberOfVariables is the number of variables in the instance */
    int numberOfVariables;

    /** Here we define a pointer to an array of var pointers */
    Variable **var;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Variables *that);
}; // class Variables


/*! \class ObjCoef
 * \brief The in-memory representation of the objective
 * function <b><coef></b> element.
 */
class ObjCoef
{
public:

    /** The ObjCoef class constructor */
    ObjCoef();

    /** The ObjCoef class destructor */
    ~ObjCoef();

    /** idx is the index of the variable corresponding
     * to the coefficient
     */
    int idx;

    /** value is the value of the objective function coefficient
     * corresponding to the variable with index idx
     */
    double value;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(ObjCoef *that);
};//class ObjCoef


/*! \class Objective
 * \brief The in-memory representation of the
 * <b><obj></b> element.
 */
class Objective
{
public:

    /** The Objective class constructor */
    Objective();

    /** The Objective class destructor */
    ~Objective();

    /** the name of the objective function */
    std::string name;

    /** declare the objective function to be
     * a max or a min
     */
    std::string maxOrMin;

    /** constant is the constant term added to the
     * objective function, 0 by default
     */
    double constant;

    /** weight is the weight applied to the given
     * objective function, 1.0 by default
     */
    double weight;

    /** numberOfObjCoef is the number of variables
     * with a nonzero objective function coefficient
     */
    int numberOfObjCoef;

    /** coef is pointer to an array of ObjCoef
     * object pointers */
    ObjCoef **coef;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Objective *that);
};//class Objective

/*! \class Objectives
 * \brief The in-memory representation of the
 * <b><objectives></b> element.
 */
class Objectives
{
public:

    /** The Objectives class constructor */
    Objectives();

    /** The Objectives class destructor */
    ~Objectives();

    /** numberOfObjectives is the number of objective
     * functions in the instance
     */
    int numberOfObjectives;

    /** coef is pointer to an array of ObjCoef
     * object pointers */
    Objective **obj;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Objectives *that);
};//class Objectives


/*! \class Constraint
 * \brief The in-memory representation of the
 * <b><con></b> element.
 */
class Constraint
{
public:

    /** The Constraint class constructor */
    Constraint();

    /** The Constraint class destructor */
    ~Constraint();

    /** name is the name of the constraint */
    std::string name;

    /** constant is a value that is added to the constraint */
    double constant;

    /** lb is the lower bound on the constraint */
    double lb;

    /** ub is the upper bound on the constraint */
    double ub;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Constraint *that);
};//class Constraint


/*! \class Constraints
 * \brief The in-memory representation of the
 * <b><constraints></b> element.
 */
class Constraints
{
public:

    /** The Constraints class constructor */
    Constraints();

    /** The Constraints class destructor */
    ~Constraints();

    /** numberOfConstraints is the number of
     * constraints in the instance
     */
    int numberOfConstraints;

    /** con is pointer to an array of Constraint
     * object pointers */
    Constraint **con;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Constraints *that);
};//class Constraints


/*! \class LinearConstraintCoefficients
 * \brief The in-memory representation of the
 * <b><linearConstraintCoefficients></b> element.
 *
 * \remarks
 * if a large part of the problem is linear, then
 * store this is the standard sparse format, either by
 * column or row. There are three arrays, an array of
 * nonzero values, an array of either column or row indices
 * and then a pointer to the start of each column or row.
 */
class LinearConstraintCoefficients
{
public:

    /** The LinearConstraintCoefficients class constructor */
    LinearConstraintCoefficients();

    /** The LinearConstraintCoefficients class destructor */
    ~LinearConstraintCoefficients();

    /** numberOfValues is the number of nonzero elements stored
     * in the <linearConstraintCoefficients> element
     */
    int numberOfValues;

    /** a pointer to the start of each row or column stored in
     * sparse format
     */
    IntVector *start;

    /** a pointer of row indices if the problem is stored by column */
    IntVector *rowIdx;

    /** a pointer of column indices if the problem is stored by row */
    IntVector *colIdx;

    /** a pointer to the array of nonzero values being stored */
    DoubleVector *value;
    /**
     * iNumberOfStartElements counts the number of elements in the <start>
     * section of <linearConstraintCoefficients>. This is useful for the parser in
     * checking consistency of the number of start elements with variables or
     * rows
     */
    int iNumberOfStartElements;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(LinearConstraintCoefficients *that);
};//class LinearConstraintCoefficients


/*! \class QuadraticTerm
 * \brief The in-memory representation of the
 * <b><qTerm></b> element.
 *
 * \remarks
 * quadratic terms can be stored efficiently by storing
 * the index of each variable, the coefficient of the
 * quadratic term, and the row in which it appears
 */
class QuadraticTerm
{
public:

    /** The QuadraticTerm class constructor */
    QuadraticTerm();

    /** The QuadraticTerm class destructor */
    ~QuadraticTerm();

    /** idx is the index of the row in which the
     * quadratic term appears
     */
    int idx;

    /** idxOne is the index of the first variable in
     * the quadratic term
     */
    int idxOne;

    /** idxTwo is the index of the second variable in
     * the quadratic term
     */
    int idxTwo;

    /** coef is the coefficient of the quadratic term */
    double coef;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(QuadraticTerm *that);
}; // QuadraticTerm


/*! \class QuadraticCoefficients
 * \brief The in-memory representation of the
 * <b><quadraticCoefficients></b> element.
 *
 */
class QuadraticCoefficients
{
public:

    /** The QuadraticCoefficients class constructor */
    QuadraticCoefficients();

    /** The QuadraticCoefficients class destructor */
    ~QuadraticCoefficients();

    /** numberOfQuadraticTerms is the number of quadratic
     * terms in the <b><quadraticCoefficients></b> element.
     */
    int numberOfQuadraticTerms;

    /** qTerm is a pointer to an array of QuadraticTerm
     * object pointers */
    QuadraticTerm** qTerm;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(QuadraticCoefficients *that);
}; // QuadraticCoefficients


/*! \class Nl
 * \brief The in-memory representation of the
 * <b><nl></b> element.
 */
class Nl
{
public:
    /** idx holds the row index of the nonlinear expression */
    int idx;

    /** shape holds the shape of the nonlinear expression 
     *  (linear/quadratic/convex/general) (see further up in this file).
     *  this might be useful in guiding solver selection.
     */
    ENUM_NL_EXPR_SHAPE shape;

    /** m_bDeleteExpressionTree is true, if in garbage collection, we
     * should delete the osExpression tree object, if the OSInstance class
     * created a map of the expression trees this should be false since the
     * osExpressionTree is deleted by the OSInstance object
     */
    bool m_bDeleteExpressionTree;

    /** osExpressionTree contains the root of the ScalarExpressionTree */
    ScalarExpressionTree *osExpressionTree;

    /**
     * default constructor.
     */
    Nl();

    /**
     * default destructor.
     */
    ~Nl();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Nl *that);
};//end Nl

/*! \class NonlinearExpressions
 * \brief The in-memory representation of the
 * <b><nonlinearExpressions></b> element.
 */
class NonlinearExpressions
{
public:

    /** The NonlinearExpressions class constructor */
    NonlinearExpressions();

    /** The NonlinearExpressions class destructor */
    ~NonlinearExpressions();

    /** numberOfNonlinearExpressions is the number of
     * <nl> elements in the
     * <b><nonlinearExpressions></b> element.
     */
    int numberOfNonlinearExpressions;

    /** nl is pointer to an array of Nl object pointers */
    Nl **nl;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(NonlinearExpressions *that);
}; // NonlinearExpressions


/*! \class Matrices
 * \brief The in-memory representation of the
 * <b><matrices></b> element.
 */
class Matrices
{
public:

    /** The Matrices class constructor */
    Matrices();

    /** The Matrices class destructor */
    ~Matrices();

    /** numberOfMatrices is the number of
     * <nl> elements in the
     * <b><matrices></b> element.
     */
    int numberOfMatrices;

    /** matrix is a pointer to an array of OSMatrix object pointers */
    OSMatrix **matrix;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Matrices *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(Matrices *that);
}; // Matrices


/*! \class Cone
 * \brief The in-memory representation of a generic cone 
 * Specific cone types are derived from this generic class
 */
class Cone
{
public:

    /** The Cone class constructor */
    Cone();

    /** The Cone class destructor */
    virtual ~Cone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Cones can also be formed by Multidimensional tensors.
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions,
     *  although they have not yet been implemented.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone */
    ENUM_CONE_TYPE coneType;

    /** The cone can have a name for easier identification */
    std::string name;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a Cone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML() = 0;


    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Cone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(Cone *that);
}; // Cone

/*! \class NonnegativeCone
 *  \brief The NonnegativeCone Class.
 *
 * \remarks
 * The in-memory representation of the OSiL element <nonnegativeCone>
 *
 */
class NonnegativeCone : public Cone
{
public:
    /**
     * default constructor.
     */
    NonnegativeCone();

    /**
     * default destructor.
     */
    ~NonnegativeCone();

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a NonnegativeCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(NonnegativeCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(NonnegativeCone *that);

};//end NonnegativeCone

/*! \class NonpositiveCone
 *  \brief The NonpositiveCone Class.
 *
 * \remarks
 * The in-memory representation of the OSiL element <nonpositiveCone>
 *
 */
class NonpositiveCone : public Cone
{
public:
    /**
     * default constructor.
     */
    NonpositiveCone();

    /**
     * default destructor.
     */
    ~NonpositiveCone();

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a NonpositiveCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(NonpositiveCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(NonpositiveCone *that);

};//end NonpositiveCone


/*! \class OrthantCone
 *  \brief The OrthantCone Class.
 *
 * \remarks
 * The in-memory representation of the OSiL element <orthantCone>
 *
 */
class OrthantCone : public Cone
{
public:
    /** For each dimension of the cone, give the upper and lower bounds
     *  The upper bound can be only zero or +infty,
     *  the lower bound can be only zero or -infty,
     */
    double* ub;
    double* lb;

    /**
     * default constructor.
     */
    OrthantCone();

    /**
     * default destructor.
     */
    ~OrthantCone();

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write an OrthantCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(OrthantCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(OrthantCone *that);
};//end OrthantCone

/*! \class PolyhedralCone
 * \brief The in-memory representation of a polyhedral cone 
 */
class PolyhedralCone : public Cone
{
public:

    /** The PolyhedralCone class constructor */
    PolyhedralCone();

    /** The PolyhedralCone class destructor */
    ~PolyhedralCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** Polyhedral cones use a reference to a previously defined matrix for the extreme rays */
    int referenceMatrixIdx;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a PolyhedralCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(PolyhedralCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(PolyhedralCone *that);
}; // PolyhedralCone


/*! \class QuadraticCone
 * \brief The in-memory representation of a quadratic cone 
 */
class QuadraticCone : public Cone
{
public:

    /** The QuadraticCone class constructor */
    QuadraticCone();

    /** The QuadraticCone class destructor */
    ~QuadraticCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** quadratic cones normally are of the form x0 >= x1^2 + x2^2 + ...
     *  However, the appearance can be modified using a norm factor k 
     *  and a distortion matrix M to the form
     *  x0 >= p (x1, x2, ...) M (x1, x2, ...)'
     *  @default: k= 1, M = -1.
     */
    double normScaleFactor;
    int distortionMatrixIdx;

    /** The index of the first component can be changed
     *  Since there are possibly many dimensions, the index is coded 
     *  as i0*n1*n2*... + i1*n2*n3... + ... + i_r, 
     *  where i0, i1, etc are zero-based indexes for the different dimensions:
     *  i0 = 0, 1, ..., n0 -1, where n0 is the number of rows,
     *  i1 = 0, 1, ..., n1 -1, where n1 is the number of columns,
     *  and so on for higher dimensions (if any)
     *  (@default: 0)
     */
    int axisDirection;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a QuadraticCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(QuadraticCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(QuadraticCone *that);
}; // QuadraticCone


/*! \class RotatedQuadraticCone
 * \brief The in-memory representation of a rotated quadratic cone 
 */
class RotatedQuadraticCone : public Cone
{
public:

    /** The RotatedQuadraticCone class constructor */
    RotatedQuadraticCone();

    /** The RotatedQuadraticCone class destructor */
    ~RotatedQuadraticCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** rotated quadratic cones normally are of the form x0x1 >= x2^2 + x3^2 + ...
     *  However, the appearance can be modified using a norm factor k 
     *  and a distortion matrix M to the form
     *  x0x1 >= p (x2, x3, ...) M (x2, x3, ...)'
     *  @default: k= 1, M = -1.
     */
    double normScaleFactor;
    int distortionMatrixIdx;

    /** The indices of the first two component can be changed
     *  Since there are possibly many dimensions, each index is coded 
     *  as i0*n1*n2*... + i1*n2*n3... + ... + i_r, 
     *  where i0, i1, etc are zero-based indexes for the different dimensions:
     *  i0 = 0, 1, ..., n0 -1, where n0 is the number of rows,
     *  i1 = 0, 1, ..., n1 -1, where n1 is the number of columns,
     *  and so on for higher dimensions (if any)
     *  @default: i0 = 0, i1 = 1.
     */
    int firstAxisDirection;
    int secondAxisDirection;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a RotatedQuadraticCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(RotatedQuadraticCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(RotatedQuadraticCone *that);
}; // RotatedQuadraticCone

/*  Not yet implemented:
    ENUM_CONE_TYPE_normed,
*/

/*! \class SemidefiniteCone
 * \brief The in-memory representation of a cone of semidefinite matrices 
 */
class SemidefiniteCone : public Cone
{
public:

    /** The SemidefiniteCone class constructor */
    SemidefiniteCone();

    /** The SemidefiniteCone class destructor */
    ~SemidefiniteCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** we need to distinguish positive and negative semidefiniteness */
    std::string semidefiniteness;

    /** information about semidefiniteness is also tracked in a boolean variable */
    bool isPositiveSemiDefinite;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a SemidefiniteCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(SemidefiniteCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(SemidefiniteCone *that);
}; // SemidefiniteCone


/*! \class CopositiveMatricesCone
 *  \brief The CopositiveMatricesCone Class.
 *
 * \remarks
 * The in-memory representation of the OSiL element <copositiveMatricesCone>
 *
 */
class CopositiveMatricesCone : public Cone
{
public:
    /**
     * default constructor.
     */
    CopositiveMatricesCone();

    /**
     * default destructor.
     */
    ~CopositiveMatricesCone();

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a CopositiveMatricesCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(CopositiveMatricesCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(CopositiveMatricesCone *that);

};//end CopositiveMatricesCone


/*! \class CompletelyPositiveMatricesCone
 *  \brief The CompletelyPositiveMatricesCone Class.
 *
 * \remarks
 * The in-memory representation of the OSiL element <completelyPositiveMatricesCone>
 *
 */
class CompletelyPositiveMatricesCone : public Cone
{
public:
    /**

     * default constructor.
     */
    CompletelyPositiveMatricesCone();

    /**
     * default destructor.
     */
    ~CompletelyPositiveMatricesCone();

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**

     * Write a CompletelyPositiveMatricesCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(CompletelyPositiveMatricesCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(CompletelyPositiveMatricesCone *that);

};//end CompletelyPositiveMatricesCone

/*  Not yet implemented: 
    ENUM_CONE_TYPE_hyperbolicity,
    ENUM_CONE_TYPE_nonnegativePolynomials,
    ENUM_CONE_TYPE_moments,
*/

/*! \class ProductCone
 * \brief The in-memory representation of a product cone 
 */
class ProductCone : public Cone
{
public:

    /** The ProductCone class constructor */
    ProductCone();

    /** The ProductCone class destructor */
    ~ProductCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** the list of "factors" contributing to the product 
     *  each factor contains a reference to a previously defined cone
     */
    IntVector* factors;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write a ProductCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(ProductCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);


    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(ProductCone *that);
}; // ProductCone

/*! \class IntersectionCone
 * \brief The in-memory representation of an intersection cone 
 */
class IntersectionCone : public Cone
{
public:

    /** The IntersectionCone class constructor */
    IntersectionCone();

    /** The IntersectionCone class destructor */
    ~IntersectionCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** the list of components contributing to the intersection 
     *  each component contains a reference to a previously defined cone
     */
    IntVector* components;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * Write an IntersectionCone object in XML format. 
     * This is used by OSiLWriter to write a <cone> element.
     *
     * @return the cone and its children as an XML string.
     */
    virtual std::string getConeInXML();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(IntersectionCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(IntersectionCone *that);
}; // IntersectionCone


/*! \class DualCone
 * \brief The in-memory representation of a dual cone 
 */
class DualCone : public Cone
{
public:

    /** The DualCone class constructor */
    DualCone();

    /** The DualCone class destructor */
    ~DualCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** Dual cones use a reference to another, previously defined cone */
    int referenceConeIdx;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(DualCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(DualCone *that);
}; // DualCone

/*! \class PolarCone
 * \brief The in-memory representation of a polar cone 
 */
class PolarCone : public Cone
{
public:

    /** The PolarCone class constructor */
    PolarCone();

    /** The PolarCone class destructor */
    ~PolarCone();

    /** Every cone has (at least) two dimensions; no distinction
     *  is made between vector cones and matrix cones
     */
    int numberOfRows;
    int numberOfColumns;

    /** Multidimensional tensors can also form cones
     *  (the Kronecker product, for instance, can be
     *   thought of as a four-dimensional tensor).
     *  We therefore allow additional dimensions.
     */
    int numberOfOtherIndexes;
    int* otherIndexes;

    /** The type of the cone (one of the values in ENUM_CONE_TYPE) */
    int coneType;

    /** cones are referenced by an (automatically created) index */
    int idx;

    /** Polar cones use a reference to another, previously defined cone */
    int referenceConeIdx;

    /**
     * @return the type of cone as a string
     */
    virtual std::string getConeName();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(PolarCone *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(PolarCone *that);
}; // PolarCone


/*! \class Cones
 * \brief The in-memory representation of the
 * <b><cones></b> element.
 */
class Cones
{
public:

    /** The Cones class constructor */
    Cones();

    /** The Cones class destructor */
    ~Cones();

    /** numberOfCones is the number of
     * <nl> elements in the
     * <b><cones></b> element.
     */
    int numberOfCones;

    /** cone is pointer to an array of Cone object pointers */
    Cone **cone;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(Cones *that);

    /**
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(Cones *that);
}; // Cones


/*! \class MatrixVar
 * \brief The in-memory representation of the
 * <b><matrixVar></b> element.
 */
class MatrixVar
{
public:
    /** numberOfRows gives the number of rows of this matrix */
    int numberOfRows;

    /** numberOfColumns gives the number of columns of this matrix */
    int numberOfColumns;

    /** templateMatrixIdx refers to a matrix that describes the
     *  locations in this matrixVar that are allowed to be nonzero
     */
    int templateMatrixIdx;

    /** varReferenceMatrixIdx allows some or all of the components of this matrix variable to be
     *  copied from variables defined in the core
     */
    int varReferenceMatrixIdx;

    /** lbMatrixIdx gives a lower bound for this matrixVar */
    int lbMatrixIdx;

    /** lbConeIdx gives a cone that must contain matrixVar - lbMatrix */
    int lbConeIdx;

    /** ubMatrixIdx gives an upper bound for this matrixVar */
    int ubMatrixIdx;

    /** ubConeIdx gives a cone that must contain ubMatrix - matrixVar */
    int ubConeIdx;

    /** an optional name to this matrixVar */
    std::string name;

    /** an optional variable type (C, B, I, D, J, S).
     *  @remark must be the same for each component of this matrixVar
     */ 
    char varType;

    /** The MatrixVar class constructor */
    MatrixVar();

    /** The MatrixVar class destructor */
    ~MatrixVar();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixVar *that);
}; // MatrixVar


/*! \class MatrixVariables
 * \brief The in-memory representation of the
 * <b><matrixVariables></b> element.
 */
class MatrixVariables
{
public:
    /** numberOfMatrixVar gives the number of <matrixVar> children */
    int numberOfMatrixVar;

    /** matrixVar is an array of pointers to the <matrixVar> children */
    MatrixVar** matrixVar;

    /** The MatrixVariables class constructor */
    MatrixVariables();

    /** The MatrixVariables class destructor */
    ~MatrixVariables();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixVariables *that);
}; // MatrixVariables


/*! \class MatrixObj
 * \brief The in-memory representation of the
 * <b><matrixObj></b> element.
 */
class MatrixObj
{
public:
    /** numberOfRows gives the number of rows of this matrix */
    int numberOfRows;

    /** numberOfColumns gives the number of columns of this matrix */
    int numberOfColumns;

    /** templateMatrixIdx refers to a matrix that describes the
     *  locations in this matrixObj that are allowed to be nonzero
     */
    int templateMatrixIdx;

    /** objReferenceMatrixIdx allows some or all of the components 
     *  of this matrixObj to be copied from objectives defined in the core
     */
    int objReferenceMatrixIdx;

    /** orderConeIdx gives a cone that expresses preferences during the optimization 
     *  x is (weakly) preferred to y if obj(x) - obj(y) lies in the cone. 
     */
    int orderConeIdx;

    /** constantMatrixIdx gives a constant added to the matrixObj */
    int constantMatrixIdx;

    /** an optional name to this matrixObj */
    std::string name;

    /** The MatrixVar class constructor */
    MatrixObj();

    /** The MatrixVar class destructor */
    ~MatrixObj();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixObj *that);
}; // MatrixObj


/*! \class MatrixObjectives
 * \brief The in-memory representation of the
 * <b><matrixObjectives></b> element.
 */
class MatrixObjectives
{
public:

    /** The MatrixObjectives class constructor */
    MatrixObjectives();

    /** The MatrixObjectives class destructor */
    ~MatrixObjectives();

    /** numberOfMatrixObj gives the number of <matrixObj> children */
    int numberOfMatrixObj;

    /** matrixObj is an array of pointers to the <matrixObj> children */
    MatrixObj** matrixObj;


    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixObjectives *that);
}; // MatrixObjectives


/*! \class MatrixCon
 * \brief The in-memory representation of the
 * <b><matrixCon></b> element.
 */
class MatrixCon
{
public:
    /** numberOfRows gives the number of rows of this matrix */
    int numberOfRows;

    /** numberOfColumns gives the number of columns of this matrix */
    int numberOfColumns;

    /** templateMatrixIdx refers to a matrix that describes the
     *  locations in this matrixVar that are allowed to be nonzero
     */
    int templateMatrixIdx;

    /** conReferenceMatrixIdx allows some or all of the components of this matrixCon to be
     *  copied from constraints defined in the core
     */
    int conReferenceMatrixIdx;

    /** lbMatrixIdx gives a lower bound for this matrixCon */
    int lbMatrixIdx;

    /** lbConeIdx gives a cone that must contain matrixCon - lbMatrix */
    int lbConeIdx;

    /** ubMatrixIdx gives an upper bound for this matrixCon */
    int ubMatrixIdx;

    /** ubConeIdx gives a cone that must contain ubMatrix - matrixCon */
    int ubConeIdx;

    /** an optional name to this MatrixCon */
    std::string name;

    /** The MatrixCon class constructor */
    MatrixCon();

    /** The MatrixCon class destructor */
    ~MatrixCon();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixCon *that);
}; // MatrixCon


/*! \class MatrixConstraints
 * \brief The in-memory representation of the
 * <b><matrixConstraints></b> element.
 */
class MatrixConstraints
{
public:

    /** The MatrixConstraints class constructor */
    MatrixConstraints();

    /** The MatrixConstraints class destructor */
    ~MatrixConstraints();

    /** numberOfMatrixCon gives the number of <matrixCon> children */
    int numberOfMatrixCon;

    /** matrixCon is an array of pointers to the <matrixCon> children */
    MatrixCon** matrixCon;


    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixConstraints *that);
}; // MatrixConstraints

/*! \class MatrixExpression
 * \brief The in-memory representation of the <b><expr></b> element,
 * which is like a nonlinear expression, but since it involves matrices,
 * the expression could be linear, so a "shape" attribute is added
 * to distinguish linear and nonlinear expressions.
 */
class MatrixExpression
{
public:
    /** idx holds the row index of the nonlinear expression */
    int idx;

    /** shape holds the shape of the nonlinear expression 
     *  (linear/quadratic/convex/general) (see further up in this file).
     *  this might be useful in guiding solver selection.
     */
    ENUM_NL_EXPR_SHAPE shape;

    /** matrixExpressionTree contains the root of the MatrixExpressionTree */
    MatrixExpressionTree *matrixExpressionTree;

    /** if m_bDeleteExpressionTree is true during garbage collection,
     *  we should delete the osExpression tree object, 
     *  if the OSInstance class created a map of the expression trees,
     *  this should be false since the osExpressionTree is deleted by the OSInstance object
     */
    bool m_bDeleteExpressionTree;

    /** The MatrixExpression class constructor */
    MatrixExpression();

    /** The MatrixExpression class destructor */
    ~MatrixExpression();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixExpression *that);
}; // MatrixExpression


/*! \class MatrixExpressions
 * \brief The in-memory representation of the 
   <b><matrixExpressions></b> element.
 */
class MatrixExpressions
{
public:
    /** numberOfExpr gives the number of expressions */
    int numberOfExpr;

    /** a pointer to an array of linear and nonlinear
     *  expressions that evaluate to matrices
     */
    MatrixExpression **expr;

    /** The MatrixExpressions class constructor */
    MatrixExpressions();

    /** The MatrixExpressions class destructor */
    ~MatrixExpressions();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixExpressions *that);
}; // MatrixExpressions

/*! \class MatrixProgramming
 * \brief The in-memory representation of the 
   <b><matrixProgramming></b> element.
 */
class MatrixProgramming
{
public:
    /** The MatrixProgramming class constructor */
    MatrixProgramming();

    /** The MatrixProgramming class destructor */
    ~MatrixProgramming();

    /** a pointer to the matrixVariables object */
    MatrixVariables* matrixVariables;

    /** a pointer to the matrixObjectives object */
    MatrixObjectives* matrixObjectives;

    /** a pointer to the matrixConstraints object */
    MatrixConstraints* matrixConstraints;

    /** a pointer to the matrixExpressions object */
    MatrixExpressions* matrixExpressions;


    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixProgramming *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     * @param iMin: lowest index value (inclusive) that a variable reference in this matrix can take
     * @param iMax: greatest index value (inclusive) that a variable reference in this matrix can take
     */
    bool setRandom(double density, bool conformant, int iMin, int iMax);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    bool deepCopyFrom(MatrixProgramming *that);
}; // MatrixProgramming


/*! \class TimeDomainStageVar
 * \brief The in-memory representation of the
 * <b><var></b> element.
 */
class TimeDomainStageVar
{
public:

    /** The TimeDomainStageVar class constructor */
    TimeDomainStageVar();

    /** The TimeDomainStageVar class destructor */
    ~TimeDomainStageVar();

    /** idx gives the index of this variable */
    int idx;
}; // TimeDomainStageVar

/*! \class TimeDomainStageVariables
 * \brief The in-memory representation of the
 * <b><variables></b> child of the <stage> element.
 */
class TimeDomainStageVariables
{
public:

    /** The TimeDomainStageVariables class constructor */
    TimeDomainStageVariables();

    /** The TimeDomainStageVariables class destructor */
    ~TimeDomainStageVariables();

    /** numberOfVariables gives the number of variables contained in this stage */
    int numberOfVariables;

    /** startdIdx gives the number of the first variable contained in this stage */
    int startIdx;

    /** var is a pointer to an array of TimeDomainStageVar object pointers */
    TimeDomainStageVar** var;
}; // TimeDomainStageVariables


/*! \class TimeDomainStageCon
 * \brief The in-memory representation of the
 * <b><con></b> element.
 */
class TimeDomainStageCon
{
public:

    /** The TimeDomainStageCon class constructor */
    TimeDomainStageCon();

    /** The TimeDomainStageCon class destructor */
    ~TimeDomainStageCon();

    /** idx gives the index of this constraint */
    int idx;
}; // TimeDomainStageCon

/*! \class TimeDomainStageConstraints
 * \brief The in-memory representation of the
 * <b><constraints></b> child of the <stage> element.
 */
class TimeDomainStageConstraints
{
public:

    /** The TimeDomainStageConstraints class constructor */
    TimeDomainStageConstraints();

    /** The TimeDomainStageConstraints class destructor */
    ~TimeDomainStageConstraints();

    /** numberOfConstraints gives the number of constraints contained in this stage */
    int numberOfConstraints;

    /** startdIdx gives the number of the first constraint contained in this stage */
    int startIdx;

    /** con is a pointer to an array of TimeDomainStageCon object pointers */
    TimeDomainStageCon** con;
}; // TimeDomainStageConstraints


/*! \class TimeDomainStageObj
 * \brief The in-memory representation of the
 * <b><obj></b> element.
 */
class TimeDomainStageObj
{
public:

    /** The TimeDomainStageObj class constructor */
    TimeDomainStageObj();

    /** The TimeDomainStageObj class destructor */
    ~TimeDomainStageObj();

    /** idx gives the index of this variable */
    int idx;
}; // TimeDomainStageObj

/*! \class TimeDomainStageObjectives
 * \brief The in-memory representation of the
 * <b><objectives></b> child of the <stage> element.
 */
class TimeDomainStageObjectives
{
public:

    /** The TimeDomainStageObjectives class constructor */
    TimeDomainStageObjectives();

    /** The TimeDomainStageObjectives class destructor */
    ~TimeDomainStageObjectives();

    /** numberOfObjectives gives the number of objectives contained in this stage */
    int numberOfObjectives;

    /** startdIdx gives the number of the first objective contained in this stage */
    int startIdx;

    /** obj is a pointer to an array of TimeDomainStageObj object pointers */
    TimeDomainStageObj** obj;
}; // TimeDomainStageObjectives


/*! \class TimeDomainStage
 * \brief The in-memory representation of the
 * <b><stage></b> element.
 */
class TimeDomainStage
{
public:

    /** The TimeDomainStage class constructor */
    TimeDomainStage();

    /** The TimeDomainStage class destructor */
    ~TimeDomainStage();

    /** name corresponds to the optional attribute that holds
     * the name of the stage; the default value is empty
     */
    std::string name;

    /** variables is a pointer to a TimeDomainVariables object */
    TimeDomainStageVariables *variables;

    /** constraints is a pointer to a TimeDomainConstraints object */
    TimeDomainStageConstraints *constraints;

    /** objectives is a pointer to a TimeDomainObjectives object */
    TimeDomainStageObjectives *objectives;
}; // TimeDomainStage

/*! \class TimeDomainStages
 * \brief The in-memory representation of the
 * <b><stages></b> element.
 */
class TimeDomainStages
{
public:

    /** The Stages class constructor */
    TimeDomainStages();

    /** The Stages class destructor */
    ~TimeDomainStages();

    /** numberOfStages is the number of stages
     * in the <b><stages></b> element.
     */
    int numberOfStages;

    /** stage is pointer to an array of stage object pointers */
    TimeDomainStage **stage;
}; // Stages

/*! \class Interval
 * \brief The in-memory representation of the
 * <b><interval></b> element.
 */
class TimeDomainInterval
{
public:

    /** The Interval class constructor */
    TimeDomainInterval();

    /** The Interval class destructor */
    ~TimeDomainInterval();

    /** start is the start of the planning period
     * in the <b><interval></b> element.
     */
    double start;

    /** horizon is the end of the planning period
     * in the <b><interval></b> element.
     */
    double horizon;
}; // Interval

/*! \class TimeDomain
 * \brief The in-memory representation of the
 * <b><timeDomain></b> element.
 */
class TimeDomain
{
public:

    /** The TimeDomain class constructor */
    TimeDomain();

    /** The TimeDomain class destructor */
    ~TimeDomain();

    /** stages is a pointer to a Stages object
     */
    TimeDomainStages *stages;

    /** interval is a pointer to an Interval object
     */
    TimeDomainInterval *interval;
}; // TimeDomain

/*! \class InstanceData
 * \brief The in-memory representation of the
 * <b><instanceData></b> element.
 * \remarks
 * The InstanceData object contains the objects
 * that define the instance --
 * <ul>
 * <li><b>Variables</b> object</li>
 * <li><b>Objectives</b> object</li>
 * <li><b>Constraints</b> object</li>
 * <li><b>LinearConstraintCoefficients</b> object</li>
 * <li><b>QuadraticCoefficients</b> object</li>
 * <li><b>NonlinearExpressions/b> object</li>
 * <li><b>TimeDomain/b> object</li>
 * </ul>
 */
class InstanceData
{
public:

    /** The InstanceData class constructor */
    InstanceData();

    /** The InstanceData class destructor */
    ~InstanceData();

    /** variables is a pointer to a Variables object */
    Variables *variables;

    /** objectives is a pointer to a Objectives object */
    Objectives *objectives;

    /** constraints is a pointer to a Constraints object */
    Constraints *constraints;

    /** linearConstraintCoefficients is a pointer to a
     * LinearConstraintCoefficients object
     */
    LinearConstraintCoefficients *linearConstraintCoefficients;

    /** quadraticCoefficients is a pointer to a
     *  QuadraticCoefficients object
     */
    QuadraticCoefficients* quadraticCoefficients;

    /** nonlinearExpressions is a pointer to a
     *  NonlinearExpressions object
     */
    NonlinearExpressions* nonlinearExpressions;

    /** matrices is a pointer to a
     *  Matrices object
     */
    Matrices* matrices;

    /** cones is a pointer to a
     *  Cones object
     */
    Cones* cones;

    /** matrixProgramming is a pointer to a
     *  MatrixProgramming object
     */
    MatrixProgramming* matrixProgramming;

    /** timeDomain is a pointer to a
     *  TimeDomain object
     */
    TimeDomain* timeDomain;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(InstanceData *that);
}; // class InstanceData


/*! \class OSInstance OSInstance.h "OSInstance.h"
 *  \brief The in-memory representation of an OSiL instance..
 *
 * \remarks
 * <p> 1. Elements become objects of class type
 *  (the ComplexType is the class) </p>
 *
 * <p> 2. The attributes, children of the element, and text
 * correspond to members of the class.
 * (Note text does not have a name and becomes .value) </p>
 *
 * <p> 3. Model groups such as choice and sequence
 *  and all correspond to arrays </p>
 *
 * <ol>
 * <li> anything specific to XML such as base64, multi,
 * incr do not go into classes </li>
 * <li> The root OSnLNode of each <nl> element is called ExpressionTree </li>
 * <li> Root is not called osil it is called osinstance </li>
 * </ol>
 *
 * The OSInstance class is composed of two objects: 
 * the header object instanceHeader 
 * and the data object instanceData
 *
 *
 */
class OSInstance
{
public:

    /** The OSInstance class constructor */
    OSInstance();

    /** The OSInstance class destructor */
    ~OSInstance();

    /** the instanceHeader is implemented as a general file header object 
     *  to allow sharing of classes between schemas 
     */
    GeneralFileHeader *instanceHeader;

    /** A pointer to an InstanceData object */
    InstanceData *instanceData;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(OSInstance *that);

    /**
     * bVariablesModified is true if the variables data has been modified.
     */
    bool bVariablesModified;

    /**
     * bObjectivesModified is true if the objective function data has been modified.
     */
    bool bObjectivesModified;

    /**
     * bConstraintsModified is true if the constraints data has been modified.
     */
    bool bConstraintsModified;

    /**
     * bAMatrixModified is true if the A matrix data has been modified.
     */
    bool bAMatrixModified;

private:
    /** ------- data items for InstanceHeader ------- **/
    /**
     * m_sInstanceName holds the instance name.
     */
    std::string m_sInstanceName;
    /**
     * m_sInstanceSource holds the instance source.
     */
    std::string m_sInstanceSource;
    /**
     * m_sInstanceDescription holds the instance description.
     */
    std::string m_sInstanceDescription;
    /**
     * m_sInstanceSource holds the instance source.
     */
    std::string m_sInstanceCreator;
    /**
     * m_sInstanceDescription holds the instance fileCreator info.
     */
    std::string m_sInstanceLicence;


    /** ------- data items for Variables ------- **/
    /**
     * m_bProcessVariables holds whether the variables are processed.
     */
    bool m_bProcessVariables;

    /**
     * m_iVariableNumber holds the variable number.
     */
    int m_iVariableNumber;

    /**
     * m_iNumberOfIntegerVariables holds the number of integer variables.
     */
    int m_iNumberOfIntegerVariables;

    /**
     * m_iNumberOfBinaryVariables holds the number of binary variables.
     */
    int m_iNumberOfBinaryVariables;

    /**
     * m_iNumberOfSemiContinuousVariables holds the number of semi-continuous variables.
     */
    int m_iNumberOfSemiContinuousVariables;

    /**
     * m_iNumberOfSemiIntegerVariables holds the number of semi-integer variables.
     */
    int m_iNumberOfSemiIntegerVariables;

    /**
     * m_iNumberOfStringVariables holds the number of string-valued variables.
     */
    int m_iNumberOfStringVariables;

    /**
     * m_msVariableNames holds an array of variable names.
     */
    std::string* m_msVariableNames;

    /**
     * m_mcVariableTypes holds a char array of variable types (default = 'C').
     * (C for Continuous; B for Binary; I for Integer; S for String)
     */
    char* m_mcVariableTypes;

    /**
     * m_mdVariableLowerBounds holds a double array of variable lower bounds (default = 0.0).
     */
    double* m_mdVariableLowerBounds;

    /**
     * m_mdVariableUpperBounds holds a double array of variable upper bounds (default = INF).
     */
    double* m_mdVariableUpperBounds;


    /** ------- data items for Objectives ------- **/
    /**
     * m_bProcessObjectives holds whether the objectives are processed.
     */
    bool m_bProcessObjectives;

    /**
     * m_iObjectiveNumber is the number of objective functions.
     */
    int m_iObjectiveNumber;

    /**
     * m_iObjectiveNumber is the number of objective functions with a nonlinear term.
     */
    int m_iObjectiveNumberNonlinear;

    /**
     * m_msObjectiveNames holds an array of objective names.
     */
    std::string* m_msObjectiveNames;

    /**
     * m_msMaxOrMins holds a std::string array of objective maxOrMins ("max" or "min").
     */
    std::string* m_msMaxOrMins;

    /**
     * m_miNumberOfObjCoef holds an integer array of number of objective coefficients (default = 0).
     */
    int* m_miNumberOfObjCoef;

    /**
     * m_mdObjectiveConstants holds an array of objective constants (default = 0.0).
     */
    double* m_mdObjectiveConstants;

    /**
     * m_mdObjectiveWeights holds an array of objective weights (default = 1.0).
     */
    double* m_mdObjectiveWeights;

    /**
     * m_mObjectiveCoefficients holds an array of objective coefficients, one set of objective
     * coefficients for each objective.
     */
    SparseVector** m_mObjectiveCoefficients;

    /**
     * m_bGetDenseObjectives holds whether the dense objective functions are processed.
     */
    bool m_bGetDenseObjectives;

    /**
     * m_mmdDenseObjectiveCoefficients holds an array of pointers, each pointer points
     * to a vector of dense objective function coefficients
     */
    double** m_mmdDenseObjectiveCoefficients;


    /** ------- data items for Constraints ------- **/
    /**
     * m_bProcessConstraints holds whether the constraints are processed.
     */
    bool m_bProcessConstraints;

    /**
     * m_iConstraintNumber is the number of constraints.
     */
    int m_iConstraintNumber;

    /**
     * m_iConstraintNumberNonlinear is the number of constraints that have a nonlinear term.
     */
    int m_iConstraintNumberNonlinear;

    /**
     * m_msConstraintNames holds an array of constraint names.
     */
    std::string* m_msConstraintNames;

    /**
     * m_mdConstraintLowerBounds holds an array of constraint lower bounds (default = -INF).
     */
    double* m_mdConstraintLowerBounds;

    /**
     * m_mdConstraintUpperBounds holds an array of constraint upper bounds (default = INF).
     */
    double* m_mdConstraintUpperBounds;

    /**
     * m_mdConstraintConstants holds an array of constraint constants (default = 0.0).
     */

    double* m_mdConstraintConstants;

    /**
     * m_mcConstraintTypes holds a char array of constraint types
     * (R for range; L for <=; G for >=; E for =; U for unconstrained)
     */
    char* m_mcConstraintTypes;


    /** ------- data items for linear constraint coefficients ------- **/
    /**
     * m_bProcessLinearConstraintCoefficients holds whether 
     * the linear constraint coefficients have been processed.
     */
    bool m_bProcessLinearConstraintCoefficients;

    /**
     * m_iLinearConstraintCoefficientNumber holds the number of specified
     * (usually nonzero) linear constraint coefficient values.
     */
    int m_iLinearConstraintCoefficientNumber;

    /**
     * m_bColumnMajor holds whether the linear constraint coefficients
     * are stored in column major (if m_bColumnMajor = true) or row major.
     */
    bool m_bColumnMajor;

    /**
     * m_linearConstraintCoefficientsInColumnMajor holds the standard 
     * three-array data structure for linear constraint coefficients
     * (starts, indexes and values) in column major.
     */
    SparseMatrix* m_linearConstraintCoefficientsInColumnMajor;

    /**
     * m_linearConstraintCoefficientsInRowMajor holds the standard 
     * three-array data structure for linear constraint coefficients
     * (starts, indexes and values) in row major.
     */
    SparseMatrix* m_linearConstraintCoefficientsInRowMajor;


    /** ------- data items for quadratic coefficients ------- **/
    /**
     * m_iNumberOfQuadraticRowIndexes holds the number of distinct rows and objectives with quadratic terms.
     */
    int m_iNumberOfQuadraticRowIndexes;

    /**
     * m_bQuadraticRowIndexesProcessed is true if getQuadraticRowIndexes() has been called.
     */
    bool m_bQuadraticRowIndexesProcessed;

    /**
     * m_miQuadRowIndexes is an integer pointer to the distinct row indexes with a quadratic term.
     */
    int *m_miQuadRowIndexes;

    /**
     * m_bProcessQuadraticTerms holds whether the quadratic terms are processed.
     */
    bool m_bProcessQuadraticTerms;

    /**
     * m_iQuadraticTermNumber holds the number of specified
     * (usually nonzero) qTerms in the quadratic coefficients.
     */
    int m_iQuadraticTermNumber;

    /**
     * m_quadraticTerms holds the data structure for all the quadratic terms in the instance. `
     * (rowIdx, varOneIdx, varTwoIdx, coef)
     */
    QuadraticTerms* m_quadraticTerms;

    /** m_bQTermsAdded is true if we added the quadratic terms to the expression tree
     */
    bool m_bQTermsAdded;


    /** ------- data items for nonlinear expressions ------- **/
    /**
     * m_iNumberOfNonlinearExpressionTreeIndexes holds the number of 
     * distinct rows and objectives with nonlinear terms.
     */
    int m_iNumberOfNonlinearExpressionTreeIndexes;
  
    /**
     * m_bNonlinearExpressionTreeIndexesProcessed is true 
     * if getNonlinearExpressionTreeIndexes() has been called.
     */
    bool m_bNonlinearExpressionTreeIndexesProcessed;

    /**
     * m_miNonlinearExpressionTreeIndexes is an integer pointer 
     * to the distinct rows indexes in the nonlinear expression
     * tree map.
     */
    int *m_miNonlinearExpressionTreeIndexes;

    /**
     * m_iNumberOfNonlinearExpressionTreeModIndexes holds the number of distinct
     * rows and objectives with nonlinear terms including quadratic terms 
     * added to the nonlinear expression trees.
     */
    int m_iNumberOfNonlinearExpressionTreeModIndexes;

    /**
     * m_bNonlinearExpressionTreeModIndexesProcessed is true 
     *  if getNonlinearExpressionTreeModIndexes has been called.
     */
    bool m_bNonlinearExpressionTreeModIndexesProcessed;

    /**
     * m_miNonlinearExpressionTreeModIndexes is an integer pointer to
     * the distinct rows indexes in the modified  expression tree map.
     */
    int *m_miNonlinearExpressionTreeModIndexes;


    /** ------- data items for automatic differentiation ------- **/
    /**
     * m_binitForAlgDiff is true if initForAlgDiff() has been called.
     */
    bool m_binitForAlgDiff;

    /**
     * m_iNumberOfNonlinearVariables is the number of variables that appear
     * in a nonlinear expression.
     */
    unsigned int m_iNumberOfNonlinearVariables;

    /**
     * m_bProcessNonlinearExpressions holds whether the nonlinear expressions are processed.
     */
    bool m_bProcessNonlinearExpressions;

    /**
     * m_iNonlinearExpressionNumber holds the number of nonlinear expressions.
     */
    int m_iNonlinearExpressionNumber;

    /**
     * m_miNonlinearExpressionIndexes holds an integer array of nonlinear expression indexes,
     * negative indexes correspond to objectives.
     */
    int* m_miNonlinearExpressionIndexes;

    /**
     * m_bProcessExpressionTrees is true if the expression trees have been processed.
     */
    bool m_bProcessExpressionTrees;

    /**
     * m_bProcessExpressionTreesMod is true if the modified expression trees have been processed.
     */
    bool m_bProcessExpressionTreesMod;

    /**
     * m_mdConstraintFunctionValues holds a double array of constraint function values
     * -- the size of the array is equal to getConstraintNumber().
     */
    double *m_mdConstraintFunctionValues;

    /**
     * m_mdObjectiveFunctionValues holds a double array of objective function values
     * -- the size of the array is equal to getObjectiveNumber().
     */
    double *m_mdObjectiveFunctionValues;

    /**
     * m_iJacValueSize is the number of nonzero partial derivates in the Jacobian.
     */
    int m_iJacValueSize;

    /**
     * m_miJacStart holds a int array of starts for the Jacobian matrix in sparse form (row major).
     */
    int *m_miJacStart;

    /**
    * m_miJacIndex holds a int array of variable indices for the Jacobian matrix in sparse form (row major).
    */
    int *m_miJacIndex;

    /**
    * m_mdJacValue holds a double array of partial derivatives for the Jacobian matrix in sparse form (row major).
    */
    double *m_mdJacValue;

    /**
     * m_miJacNumConTerms holds a int array of the number of constant
     * terms (gradient does not change) for the Jacobian matrix in sparse form (row major).
     */
    int *m_miJacNumConTerms;

    /**
     * m_sparseJacMatrix is the Jacobian matrix stored in sparse matrix format
     */
    SparseJacobianMatrix *m_sparseJacMatrix;

    /**
     * m_iHighestTaylorCoeffOrder is the order of highest calculated
     * Taylor coefficient
     */
    int m_iHighestTaylorCoeffOrder;

    /**
     * m_mapExpressionTrees holds a hash map of scalar-valued expression tree pointers. 
     * The key is the row index and the value is the (single) expression tree 
     * representing the nonlinear expression of that row.
     *
     * @remark For this to work all nonlinear expressions (including quadratic ones)
     *         must be combined into a single expression.
     */
    std::map<int, ScalarExpressionTree*> m_mapExpressionTrees;

    /**
     * m_mapOSADFunRangeIndex is an inverse of the previous map. The key is the number of the
     * scalar-valued expression tree, and the value is the row to which this tree belongs. 
     */
    std::map<int, int> m_mapOSADFunRangeIndex;

    /**
     * m_mapMatrixExpressionTrees holds a hash map of matrix-valued expression tree pointers. 
     * The key is the "row" index (multivariate cone objective or cone constraint) 
     * and the value is the (single) expression tree representing the expression of that row.
     *
     * @remark For this to work all matrix expressions (including linear and quadratic ones)
     *         must be combined into a single expression.
     */
    std::map<int, MatrixExpressionTree*> m_mapMatrixExpressionTrees;

    /**
     * m_LagrangianExpTree is an ScalarExpressionTree object that is the expression tree
     * for the Lagrangian function.
     */
    ScalarExpressionTree *m_LagrangianExpTree;

    /**
     * m_bLagrangianHessionCreated is true if a Lagrangian function for the Hessian has been created
     */
    bool m_bLagrangianExpTreeCreated;

    /**
     *m_LagrangianSparseHessian is the Hessian Matrix of the Lagrangian function in sparse format
     */
    SparseHessianMatrix* m_LagrangianSparseHessian;

    /**
     * m_bLagrangianSparseHessianCreated is true if the sparse Hessian Matrix for the
     * Lagrangian was created
     */
    bool m_bLagrangianSparseHessianCreated;

    /**
     * m_mapAllNonlinearVariablesIndexMap is a map of the variables in the Lagrangian function
     */
    std::map<int, int> m_mapAllNonlinearVariablesIndex;

    /**
     * m_miNonLinearVarsReverseMap maps the nonlinear variable number
     * back into the original variable space
     */
    int *m_miNonLinearVarsReverseMap;

    /**
     * m_bAllNonlinearVariablesIndexMap is true if the map of the variables in the
     * Lagrangian function has been constructed
     */
    bool m_bAllNonlinearVariablesIndex;

    /**
     * m_mapExpressionTreesMod holds a map of expression trees, 
     * with the key being the row index and value being the expression tree 
     * representing a modification of the nonlinear expression of that row.
     * We incorporate the linear and quadratic term for a variable 
     * into the corresponding expression tree before gradient and Hessian calculations
     */
    std::map<int, ScalarExpressionTree*> m_mapExpressionTreesMod ;

    /**
     * m_bOSADFunIsCreated is true if we have created the OSInstanc
     * OSAD Function

     */
    bool m_bOSADFunIsCreated;

    /**
     * is true if a CppAD Expresion Tree has been built for each row and objective
     * with a nonlinear expression.
     */
    bool m_bCppADTapesBuilt;

    /**
     * is true if a CppAD Expresion Tree has an expression that can change depending on
     * the value of the input, e.g. an if statement -- false by default
     */
    bool m_bCppADMustReTape;

    /**
     * m_bDuplicateExpressionTreeMap is true if m_mapExpressionTrees was duplicated.
     */
    bool m_bDuplicateExpressionTreesMap;

    /**
     * m_bNonLinearStructuresInitialized is true if initializeNonLinearStructures() has been called.
     */
    bool m_bNonLinearStructuresInitialized;

    /**
     * m_bSparseJacobianCalculated is true if getJacobianSparsityPattern() has been called.
     */
    bool m_bSparseJacobianCalculated;

    /**
     * m_mapExpressionTreesInPostfix holds a hash map of expression trees
     * in postfix format, with the key being the row index and value being  
     * the expression tree representing the nonlinear expression of that row.
     */
    std::map<int, std::vector<OSnLNode*> > m_mapExpressionTreesInPostfix ;

    /**
     * m_iHighestOrderEvaluated is the highest order derivative
     * of the current iterate
     */
    int m_iHighestOrderEvaluated;

    /**
     * m_mdObjGradient holds an array of pointers, each pointer points
     * to gradient of one objective function (as a dense array of double)
     */
    double **m_mmdObjGradient;


    /** ------- data vectors for nonlinear optimization ------- **/
    /**
     * m_vdX is a vector of primal variables at each iteration
     */
    std::vector<double> m_vdX;

    /**
     * m_vdYval is a vector of function values
     */
    std::vector<double> m_vdYval;

    /**
     * m_vbLagHessNonz is a boolean vector holding the nonzero pattern
     * of the Lagrangian of the Hessian
     */
    std::vector<bool> m_vbLagHessNonz;

    /**
     * m_vdYval is a vector equal to a column or row of the Jacobian
     */
    std::vector<double> m_vdYjacval;

    /**
     * m_vdYval is a vector of derivatives -- output  from a reverse sweep
     */
    std::vector<double> m_vdw;

    /**
     * m_vdYval is a vector of Lagrange multipliers
     */
    std::vector<double> m_vdLambda;

    /**
     * m_vdDomainUnitVec is a unit vector in the domain space
     */
    std::vector<double> m_vdDomainUnitVec;

    /**
     * m_vdRangeUnitVec is a unit vector in the range space
     */
    std::vector<double> m_vdRangeUnitVec;


    /** ------- data items for matrices ------- **/
    /**
     * m_bProcessMatrices holds whether the matrices have been processed.
     */
    bool m_bProcessMatrices;

    /**
     * m_iMatrixNumber holds the number of matrices
     */
    int m_iMatrixNumber;

    /**
     * m_miMatrixSymmetry holds the symmetry property of each matrix.
     * @remark for more information see the enumeration ENUM_MATRIX_SYMMETRY in OSParameters.h
     */
    ENUM_MATRIX_SYMMETRY* m_miMatrixSymmetry;

    /**
     * m_miMatrixType holds the type of each matrix.
     * @remark for more information see the enumeration ENUM_MATRIX_TYPE in OSParameters.h
     */
    ENUM_MATRIX_TYPE* m_miMatrixType;

    /**
     * m_miMatrixNumberOfColumns holds the number of columns for each matrix.
     */
    int* m_miMatrixNumberOfColumns;

    /**
     * m_miMatrixNumberOfRows holds the number of rows for each matrix.
     */
    int* m_miMatrixNumberOfRows;

    /**
     * m_msMatrixNames holds the names of the matrices
     */
    std::string* m_msMatrixNames;

    /**
     * m_mMatrix holds the list of constructors for each matrix.
     * Each solver interface must access the list of matrices,
     * check that the data as given can be handled by the solver
     * and perform whatever transformations are necessary to 
     * send the data to the solver.
     */
    OSMatrix** m_mMatrix;

#if 0
    /**
     * This and the following definitions are placeholders for the representation
     * of the matrices in different formats. For now three formats are provided:
     * column major format, row major format, and a symmetric block representation 
     * needed for the CSDP interface.
     *
     * Additional placeholders may be defined (and implemented!) as the need arises.
     *
     * In each case the values are stored in sparse matrix format (start, index, value)
     * and all values are converted to the most general form encountered. 
     * (The form is also available in m_miMatrixType.)
     *
     * For instance, in the matrix
     *
     *     |  5      x1   |
     *     | x2^2  ln(x3) |
     * 
     * every (nonzero) entry would be converted to a nonlinear expression, and 
     * m_miMatrixType would be set to ENUM_MATRIX_TYPE_general.
     * 
     * m_mExpandedMatricesInColumnMajor holds each matrix in column major format
     */
    GeneralSparseMatrix** m_mExpandedMatricesInColumnMajor;

    /**
     * m_mExpandedMatricesInRowMajor holds each matrix in row major format
     */
    GeneralSparseMatrix** m_mExpandedMatricesInRowMajor;

    /**
     * m_mMatrixBlocksInColumnMajor holds each matrix in a block partition form.
     * In addition, this format assumes that the matrix is stored in.
     * column major form and that all nesting has been resolved.
     */
    ExpandedMatrixBlocks** m_mMatrixBlocksInColumnMajor;

    /**
     * m_mMatrixTransformation holds each matrix as a single transformation.
     * Any other matrix constructors are concatenated by superposition.
     * HIG: More work needed here!!! For now only allow transformation as single constructor
     */
    OSnLMNode *m_mMatrixTransformation;
#endif

    /** ------- data items for matrix programming ------- **/
    /**
     * m_iMatrixVarNumber holds the number of matrix variables
     */
    int m_iMatrixVarNumber;

    /**
     * m_iMatrixObjNumber holds the number of matrix objectives
     */
    int m_iMatrixObjNumber;

    /**
     * m_iMatrixConNumber holds the number of matrix constraints
     */
    int m_iMatrixConNumber;

    /**
     * m_iMatrixExpressionNumber holds the number of matrix expressions
     */
    int m_iMatrixExpressionNumber;


    /** ------- data items for time domain ------- **/
    /**
     * m_bProcessTimeDomain holds whether the time domain has been processed.
     */
    bool m_bProcessTimeDomain;

    /**
     * m_bProcessTimeStages holds whether the time stages have been processed.
     */
    bool m_bProcessTimeStages;

    /**
     * m_bProcessTimeInterval holds whether a time interval has been processed.
     */
    bool m_bProcessTimeInterval;

    /**
     * m_bFiniteTimeStages holds whether the time domain has the form of finite (discrete) stages.
     */
    bool m_bFiniteTimeStages;

    /**
     * m_iNumberOfTimeStages holds the number of discrete stages
     */
    int m_iNumberOfTimeStages;

    /**
     * m_sTimeDomainFormat holds the format ("stages"/"interval") of the time domain.
     */
    std::string m_sTimeDomainFormat;

    /**
     * m_msTimeDomainStageNames holds the names of the time stages.
     */
    std::string* m_msTimeDomainStageNames;




    /**
     * m_miTimeDomainStageVariableNumber holds the number of variables in each stage.
     */
    int* m_miTimeDomainStageVariableNumber;

    /**
     * m_mmiTimeDomainStageVarList holds the list of variables in each stage.
     */
    int** m_mmiTimeDomainStageVarList;

    /**
     * m_miTimeDomainStageConstraintNumber holds the number of constraints in each stage.
     */
    int* m_miTimeDomainStageConstraintNumber;

    /**
     * m_mmiTimeDomainStageConList holds the list of constraints in each stage.
     */
    int** m_mmiTimeDomainStageConList;

    /**
     * m_miTimeDomainStageObjectiveNumber holds the number of objectives in each stage.
     */
    int* m_miTimeDomainStageObjectiveNumber;

    /**
     * m_mmiTimeDomainStageObjList holds the list of objectives in each stage.
     */
    int** m_mmiTimeDomainStageObjList;


    /**
     * process variables.
     *
     * @return true if the variables are processed.
     * @throws Exception if the elements in variables are logically inconsistent.
     */
    bool processVariables();

    /**
     * process objectives.
     *
     * @return true if the objectives are processed.
     * @throws Exception if the elements in objectives are logically inconsistent.
     */
    bool processObjectives();

    /**
     * process constraints.
     *
     * @return true if the constraints are processed.
     * @throws Exception if the elements in constraints are logically inconsistent.
     */
    bool processConstraints();

    /**
     * process linear constraint coefficients.
     *
     * @return true if the linear constraint coefficients are processed.
     * @throws Exception if the elements in linear constraint coefficients are logically inconsistent.
     */
    bool processLinearConstraintCoefficients();

    /**
     * process matrices.
     *
     * @return true if the matrices have been processed.
     * @throws Exception if the declaration of any matrix is logically inconsistent.
     */
    bool processMatrices();

public:

    /**
     * Get instance name.
     * @return instance name. Null or empty std::string ("") if there is no instance name.
     */
    std::string getInstanceName();

    /**
     * Get instance source.
     * @return instance source. Null or empty std::string ("") if there is no instance source.
     */
    std::string getInstanceSource();

    /**
     * Get instance description.
     *
     * @return instance description. Null or empty std::string ("") if there is no instance description.
     */
    std::string getInstanceDescription();

    /**
     * Get instance fileCreator.
     * @return instance fileCreator. Null or empty std::string ("") if there is no instance file creator.
     */
    std::string getInstanceCreator();

    /**
     * Get instance licence.
     *
     * @return instance licence. Null or empty std::string ("") if there is no instance licence.
     */
    std::string getInstanceLicence();


    /**
     * Get number of variables.
     *
     * @return  number of variables.
     */
    int getVariableNumber();

    /**
     * Get variable names.
     *
     * @return a std::string array of variable names, null if no variable names.
     * @throws Exception if the elements in variables are logically inconsistent.
     */
    std::string* getVariableNames();

    /**
     * Get variable initial values.
     *
     * @return a double array of variable initial values, null if no initial variable values.
     * @throws Exception if the elements in variables are logically inconsistent.
     * -- now deprecated
     */
    //double* getVariableInitialValues();

    /**
     * Get variable initial std::string values.
     *
     * @return a std::string array of variable initial values, null if no initial variable std::string values.
     * @throws Exception if the elements in variables are logically inconsistent.
     * -- now deprecated
     */
    //std::string* getVariableInitialStringValues();

    /**
     * Get variable types.
     * <ul>
     * <li>C for Continuous </li>
     * <li>B for Binary </li>
     * <li>I for Integer</li>
     * <li>S for String</li>
     * </ul>
     * @return a char array of variable types.
     * @throws Exception if the elements in variables are logically inconsistent.
     */
    char* getVariableTypes();

    /**
     * getNumberOfIntegerVariables
     * @return an integer which is the number of I variables.
     */
    int getNumberOfIntegerVariables();

    /**
     * getNumberOfBinaryVariables
     * @return an integer which is the number of B variables.
     */
    int getNumberOfBinaryVariables();

    /**
     * getNumberOfSemiContinuousVariables
     * @return an integer which is the number of D variables.
     */
    int getNumberOfSemiContinuousVariables();

    /**
     * getNumberOfSemiIntegerVariables
     * @return an integer which is the number of J variables.
     */
    int getNumberOfSemiIntegerVariables();

    /**
     * getNumberOfStringVariables
     * @return an integer which is the number of S variables.
     */
    int getNumberOfStringVariables();

    /**
     * Get variable lower bounds.
     *
     * @return a double array of variable lower bounds.
     * @throws Exception if the elements in variables are logically inconsistent.
     */
    double* getVariableLowerBounds();

    /**
     * Get variable upper bounds.
     *
     * @return a double array of variable upper bounds.
     * @throws Exception if the elements in variables are logically inconsistent.
     */
    double* getVariableUpperBounds();

    /**
     * Get number of objectives.
     *
     * @return number of objectives.
     */
    int getObjectiveNumber();


    /**
     * Get objective names.
     *
     * @return a std::string array of objective names. Null if no objective names.
     * @throws Exception if the elements in objectives are logically inconsistent.
     */
    std::string* getObjectiveNames();

    /**
     * Get objective maxOrMins. One maxOrMin for each objective.
     *
     * @return a std::string array of objective maxOrMins ("max" or "min"), null if no objectives.
     * @throws Exception if the elements in objectives are logically inconsistent.
     */
    std::string* getObjectiveMaxOrMins();


    /**
     * Get objective coefficient number. One number for each objective.
     *
     * @return an integer array of size of which is equal to number of objectives,
     * each element of the array is the number of nonzero coefficients in that
     * objective function, null if no objectives.
     * @throws Exception if the elements in objectives are logically inconsistent.
     */
    int* getObjectiveCoefficientNumbers();

    /**
     * Get objective constants. One constant for each objective.
     *
     * @return a double array of objective constants, null if no objectives.
     * @throws Exception if the elements in objectives are logically inconsistent.
     */
    double* getObjectiveConstants();

    /**
     * Get objective weights. One weight for each objective.
     *
     * @return a double array of objective weights, null if no objectives.
     * @throws Exception if the elements in objectives are logically inconsistent.
     */
    double* getObjectiveWeights();

    /**
     * Get objective coefficients. One set of objective coefficients for each objective.
     *
     * @see org.optimizationservices.oscommon.datastructure.SparseVector
     * @return an array of objective coefficients, null if no objectives.
     * Each member of the array is of type ObjectiveCoefficients.
     * The ObjectiveCoefficients class contains two arrays:
     * variableIndexes is an integer array and values is a double array of coefficient values.
     * @throws Exception if the elements in objectives are logically inconsistent.
     */
    SparseVector** getObjectiveCoefficients();

    /**
     * getDenseObjectiveCoefficients.
     *
     * @return an vector of pointers, each pointer points to a dense vector of
     * ObjectiveCoefficients.
     */
    double** getDenseObjectiveCoefficients();

    /**
     * Get number of constraints.
     *
     * @return number of constraints.
     */
    int getConstraintNumber();

    /**
     * Get constraint names.
     *
     * @return a std::string array of constraint names, null if no constraint names.
     * @throws Exception if the elements in constraints are logically inconsistent.
     */
    std::string* getConstraintNames();

    /**
     * Get constraint lower bounds.
     *
     * @return a double array of constraint lower bounds, null if no constraints.
     * @throws Exception if the elements in constraints are logically inconsistent.
     */
    double* getConstraintLowerBounds();

    /**
     * Get constraint upper bounds.
     *
     * @return a double array of constraint upper bounds, null if no constraints.
     * @throws Exception if the elements in constraints are logically inconsistent.
     */
    double *getConstraintUpperBounds();

    /**
     * Get constraint constants.
     *
     * @return a double array of constraint constants, null if no constraints.
     * @throws Exception if the elements in constraints are logically inconsistent.
     */
    double *getConstraintConstants();

    /**
     * Get constraint types. The constraint types are not part of the OSiL schema,
     * but they are used in solver interfaces such as OSLindoSolver.cpp.
     * <ul>
     * <li>R for range constraint lb <= constraint <= ub </li>
     * <li>L for less than constraint  -INF <= con <= ub or con <= ub</li>
     * <li>G for greater than constraint lb <= con <= INF or con >= lb </li>
     * <li>E for equal to constraint  lb <= con <= ub where lb = ub  or con = lb (or con = ub) </li>
     * <li>U for unconstrained constraint -INF <= con <= INF</li>
     * </ul>
     * @return a char array of constraint types, null if no constraints.
     * @throws Exception if the elements in constraints are logically inconsistent.
     */
    char* getConstraintTypes();

    /**
     * Get number of specified (usually nonzero) linear constraint coefficient values.
     *
     * @return number of specified (usually nonzero) linear constraint coefficient values.
     */
    int getLinearConstraintCoefficientNumber();


    /**
     * Get whether the constraint coefficients is in column major (true) or row major (false).
     *
     * @return whether the constraint coefficients is in column major (true) or row major (false).
     * @throws Exception if the elements in linear constraint coefficients are logically inconsistent.
     */
    bool getLinearConstraintCoefficientMajor();


    /**
     * Get linear constraint coefficients in column major.
     * @return a sparse matrix representation of linear constraint coefficients in column major, null if no linear constraint coefficients.
     * @throws Exception if the elements in linear constraint coefficients are logically inconsistent.
     * @see org.optimizationservices.oscommon.datastructure.SparseMatrix
     */
    SparseMatrix* getLinearConstraintCoefficientsInColumnMajor();

    /**
     * Get linear constraint coefficients in row major.
     * @return a sparse matrix representation of linear constraint coefficients in row major, null if no linear constraint coefficients.
     * @throws Exception if the elements in linear constraint coefficients are logically inconsistent.
     * @see org.optimizationservices.oscommon.datastructure.SparseMatrix
     */
    SparseMatrix* getLinearConstraintCoefficientsInRowMajor();

    /**
     * Get the number of specified (usually nonzero) qTerms in the quadratic coefficients.
     *
     * @return qTerm number.
     */
    int getNumberOfQuadraticTerms();

    /**
     * Get all the quadratic terms in the instance.
     *
     * @return the QuadraticTerms data structure for all quadratic terms in the instance, null if no quadratic terms.
     * The QuadraticTerms contains four arrays: rowIndexes, varOneIndexes, varTwoIndexes, coefficients.
     * @throws Exception if the elements in quadratic coefficients are logically inconsistent.
     * @see org.optimizationservices.oscommon.datastructure.QuadraticTerms
     */
    QuadraticTerms* getQuadraticTerms();

    /**
     * Get the indexes of rows which have a quadratic term.
     *
     * @return an integer pointer to the row indexes of rows with quadratic terms,
     * objectives functions have index < 0
     * NULL if there are no quadratic terms.
     */
    int* getQuadraticRowIndexes();

    /**
     * Get the number of rows which have a quadratic term.
     *
     * @return an integer which is the number of distinct rows (including obj) with quadratic terms,
     */
    int getNumberOfQuadraticRowIndexes();


/*********************************************************************
 *                                                                   *
 * Here we have a number of methods for dealing with                 *
 * scalar-valued expression trees.                                   *
 * Even though the tree can contain OSnLMNodes (e.g., to compute     *
 * the trace of a matrix), the root of the tree is of type OSnLNode. *
 *                                                                   *
 *********************************************************************/

    /**
     * Get number of nonlinear expressions.
     *
     * @return the number of nonlinear expressions.
     */
    int getNumberOfNonlinearExpressions();

    /**
     * Get the pointers to the roots of all expression trees
     *
     * @return an array of pointers to Nl objects
     */
    Nl** getNonlinearExpressions();

    /**
     * Get the expression tree for a given row index
     *
     * @return an expression tree
     */
    ScalarExpressionTree* getNonlinearExpressionTree(int rowIdx);

    /**
     * Get the expression tree for a given row index for
     * the modified expression trees (quadratic terms added)
     *
     * @return an expression tree
     */
    ScalarExpressionTree* getNonlinearExpressionTreeMod(int rowIdx);

    /**
     * Get the postfix tokens for a given row index.
     *
     * @return a vector of pointers to ExprNodes in postfix, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     *
     * @remark The root node of the expression tree is of type OSnLNode
     */
    std::vector<ExprNode*> getNonlinearExpressionTreeInPostfix( int rowIdx);

    /**
     * Get the postfix tokens for a given row index for the modified
     * Expression Tree (quadratic terms added).
     *
     * @return a vector of pointers to ExprNodes in postfix, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::vector<ExprNode*> getNonlinearExpressionTreeModInPostfix( int rowIdx);

    /**
     * Get the prefix tokens for a given row index.
     *
     * @return a vector of pointers to ExprNodes in prefix, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::vector<ExprNode*> getNonlinearExpressionTreeInPrefix( int rowIdx);

    /**
     * Get the infix representation for a given row (or objective function) index.

     * @param rowIdx is the index of the row we want to express in infix.
     * @return a string representation of the tree, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::string getNonlinearExpressionTreeInInfix( int rowIdx);


    /**
     * Get the prefix tokens for a given row index for the modified
     * Expression Tree (quadratic terms added).
     *
     * @return a vector of pointers to ExprNodes in prefix, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::vector<ExprNode*> getNonlinearExpressionTreeModInPrefix( int rowIdx);


    /**
     * @return the number of Objectives with a nonlinear term
     */
    int getNumberOfNonlinearObjectives();

    /**
     * @return the number of Constraints with a nonlinear term
     */
    int getNumberOfNonlinearConstraints();

    /**
     * @return a map: the key is the row index and the value is the corresponding expression tree
     * \remark If there are several expressions in a single row, 
     * this method combines them by adding OSnLPlus nodes 
     */
    std::map<int, ScalarExpressionTree* > getAllNonlinearExpressionTrees();

    /**
     * @return a map: the key is the row index and the value is the corresponding expression tree
     */
    std::map<int, ScalarExpressionTree* > getAllNonlinearExpressionTreesMod();

    /**
    * Get all the nonlinear expression tree indexes, i.e., 
    * indexes of rows (objectives or constraints) that contain nonlinear expressions.
    *
    * @return a pointer to an integer array of nonlinear expression tree indexes.
    */
    int* getNonlinearExpressionTreeIndexes();


    /**
    * Get the number of unique nonlinear expression tree indexes.
    *
    * @return the number of unique nonlinear expression tree indexes.
    */
    int getNumberOfNonlinearExpressionTreeIndexes();


    /**
    * Get all the nonlinear expression tree indexes, i.e., indexes of rows 
    * (objectives or constraints) that contain nonlinear expressions
    * after modifying the expression tree to contain quadratic terms.
    *
    * @return a pointer to an integer array of nonlinear expression tree indexes (including quadratic terms).
    */
    int* getNonlinearExpressionTreeModIndexes();

    /**
     * Get the number of unique nonlinear expression tree indexes after
     * modifying the expression tree to contain quadratic terms.
     *
     * @return the number of unique nonlinear expression tree indexes (including quadratic terms).
     */
    int getNumberOfNonlinearExpressionTreeModIndexes();

    /**
     * Get the number of matrices.
     *
     * @return the number of matrices.
     */
    int getMatrixNumber();

    /**
     * Get the matrix type.
     *
     * @return the type of elements contained in the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @remark only the most general element type is returned.
     * (e.g., if matrix contains both constants and general expressions, 
     * matrix type is ENUM_MATRIX_TYPE_general
     * @remark for possible types see OSParameters.h
     */
    ENUM_MATRIX_TYPE getMatrixType(int n);

    /**
     * Get the matrix symmetry.
     *
     * @return the type of symmetry found in the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @remark for possible symmetry types see OSParameters.h
     */
    ENUM_MATRIX_SYMMETRY getMatrixSymmetry(int n);

    /**
     * Get the number of blocks in the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the number of blocks.
     */
//    int getNumberOfBlocksForMatrix(int n);

    /**
     * Get the number of columns in the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the number of columns.
     */
    int getNumberOfColumnsForMatrix(int n);

    /**
     * Get the number of rows in the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the number of rows.
     */
    int getNumberOfRowsForMatrix(int n);

    /**
     * Get the number of (nonzero) values in the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the number of values.
     */
    int getNumberOfValuesForMatrix(int n);

    /**
     * Get the name of the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the matrix name.
     */
    std::string getMatrixName(int n);

    /**
     *  Several tools to parse the constructor list of a matrix
     *
     * @param n is the index number associated with the matrix.
     *
     */
    bool matrixHasBase(int n);
    bool matrixHasElements(int n);
    bool matrixHasTransformations(int n);
    bool matrixHasBlocks(int n);
    int  getNumberOfElementConstructors(int n);
    int  getNumberOfTransformationConstructors(int n);
    int  getNumberOfBlocksConstructors(int n);

    /**
     * Get the list of constructors of the matrix.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the matrix constructors.
     */
    OSMatrix* getMatrix(int n);

    /**
     * Get the (nonzero) elements of the matrix in column major form.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the (nonzero) matrix elements.
     */
    GeneralSparseMatrix* getMatrixCoefficientsInColumnMajor(int n);

    /**
     * Get the (nonzero) elements of the matrix in row major form.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the (nonzero) matrix elements.
     */
    GeneralSparseMatrix* getMatrixCoefficientsInRowMajor(int n);

    /**
     * Get the (nonzero) elements of the matrix in symmetric block form.
     *
     * @param n is the index number associated with the matrix.
     *
     * @return the (nonzero) matrix elements.
     */
//    SymmetricMatrixBlocks* getSymmetricMatrixBlocks(int n);

    /**
     * Get a block of the matrix (in symmetric column major form).
     *
     * @param n is the index number associated with the matrix.
     * @param columnIdx is the column index of the block's location
     * @param rowIdx is the row index of the block's location
     *
     * @return the (nonzero) matrix elements on column major form.
     *
     * @remark if the block in this location is empty, return NULL.
     */
    GeneralSparseMatrix* getMatrixBlockInColumnMajorForm(int n, int columnIdx, int rowIdx);


/***********************************************************************
 *                                                                     *
 * Here we have a number of methods for dealing with                   *
 * matrix programming and matrix-valued expression trees.              *
 * Even though the tree can contain OSnLNodes (e.g., to compute the    *
 * scalar multiple of a matrix), the root of the tree is an OSnLMNode. *
 *                                                                     *
 ***********************************************************************/

    /**
     * Get the number of matrix variables
     *
     * @return the number of matrix variables
     */
    int getNumberOfMatrixVariables();

    /**
     * Get the number of matrix objectives
     *
     * @return the number of matrix objectives
     */
    int getNumberOfMatrixObjectives();

    /**
     * Get the number of matrix constraints
     *
     * @return the number of matrix constraints
     */
    int getNumberOfMatrixConstraints();

    /**
     * Get the number of matrix-valued expressions
     *
     * @return the number of matrix-valued variables
     */
    int getNumberOfMatrixExpressions();

    /**
     * Get the pointers to the roots of all matrix expression trees
     *
     * @return an array of pointers to MatrixExpression objects
     */
    MatrixExpression** getMatrixExpressions();

    /**
     * Get the matrix expression tree for a given row index
     *
     * @return a matrix expression tree
     */
    MatrixExpressionTree* getMatrixExpressionTree(int rowIdx);

    /**
     * Get the postfix tokens for a given row index.
     *
     * @return a vector of pointers to OSnLNodes in postfix, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::vector<ExprNode*> getMatrixExpressionTreeInPostfix( int rowIdx);

    /**
     * Get the postfix tokens for a given row index for the modified
     * Expression Tree (quadratic terms added).
     *
     * @return a vector of pointers to OSnLNodes in postfix, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::vector<ExprNode*> getMatrixExpressionTreeModInPostfix( int rowIdx);

    /**
     * Get the prefix tokens for a given row index.
     *
     * @return a vector of pointers to OSnLNodes in prefix, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::vector<ExprNode*> getMatrixExpressionTreeInPrefix( int rowIdx);

    /**
     * Get the infix representation for a given row (or objective function) index.
     *
     * @param rowIdx is the index of the row we want to express in infix.
     * @return a string representation of the tree, if rowIdx
     * does not index a row with a nonlinear term throw an exception
     */
    std::string getMatrixExpressionTreeInInfix( int rowIdx);


    /**
     * @return a map: the key is the row index and the value is the corresponding expression tree
     */
    std::map<int, MatrixExpressionTree* > getAllMatrixExpressionTrees();


    /**
     * @return a map: the key is the row index and the value is the corresponding expression tree
     */
    std::map<int, MatrixExpressionTree* > getAllMatrixExpressionTreesMod();

    /**
    * Get all the matrix expression tree indexes, i.e. indexes of matrix objectives 
    * or matrix constraints that contain matrix expressions.
    *
    * @return a pointer to an integer array of matrix expression tree indexes.
    */
    int* getMatrixExpressionTreeIndexes();


    /**
    * Get the number of unique matrix expression tree indexes.
    *
    * @return the number of unique matrix expression tree indexes.
    */
    int getNumberOfMatrixExpressionTreeIndexes();

//===============================================

    /**
    * Get the format of the time domain ("stages"/"interval")
    *
    * @return the format of the time domain.
    */
    std::string getTimeDomainFormat();

    /**
    * Get the number of stages that make up the time domain
    *
    * @return the number of time stages.
    */
    int getTimeDomainStageNumber();

    /**
    * Get the names of the stages (NULL or empty string ("") if a stage has not been given a name
    *
    * @return the names of time stages.
    */
    std::string* getTimeDomainStageNames();

    /**
    * Get the number of variables contained in each time stage
    *
    * @return a vector of size numberOfStages.
    */
    int* getTimeDomainStageNumberOfVariables();


    /**
    * Get the number of constraints contained in each time stage
    *
    * @return a vector of size numberOfStages.
    */
    int* getTimeDomainStageNumberOfConstraints();

    /**
    * Get the number of objectives contained in each time stage
    *
    * @return a vector of size numberOfStages.
    */
    int* getTimeDomainStageNumberOfObjectives();

    /**
    * Get the list of variables in each stage
    *
    * @return one array of integers for each stage.
    */
    int** getTimeDomainStageVarList();

    /**
    * Get the list of constraints in each stage
    *
    * @return one array of integers for each stage.
    */
    int** getTimeDomainStageConList();

    /**
    * Get the list of objectives in each stage
    *
    * @return one array of integers for each stage.
    */
    int** getTimeDomainStageObjList();

    /**
    * Get the start for the time domain interval
    *
    * @return start end of the time interval.
    */
    double getTimeDomainIntervalStart();

    /**
    * Get the horizon for the time domain interval
    *
    * @return the end of the time interval.
    */
    double getTimeDomainIntervalHorizon();



    // the set() methods


    /**
     * set the instance name.
     *
     * @param name holds the instance name.
     * @return whether the instance name was set successfully.
     */
    bool setInstanceName(std::string name);

    /**
     * set the instance source.
     *
     * @param source holds the instance source.
     * @return whether the instance source was set successfully.
     */
    bool setInstanceSource(std::string source);

    /**
     * set the instance description.
     *
     * @param description holds the instance description.
     * @return whether the instance description was set successfully.
     */
    bool setInstanceDescription(std::string description);

    /**
     * set the instance creator.
     *
     * @param fileCreator holds the instance creator.
     * @return whether the instance creator was set successfully.
     */
    bool setInstanceCreator(std::string fileCreator);

    /**
     * set the instance licence.
     *
     * @param licence holds the instance licence.
     * @return whether the instance licence was set successfully.
     */
    bool setInstanceLicence(std::string licence);


    /**
     * set the number of variables.
     *
     * @param number holds the number of variables.
     * @return whether the number was set successfully.
     */
    bool setVariableNumber(int number);

    /**
     * add a variable. In order to use the add method, the setVariableNumber must first be called
     * so that the number of variables is known ahead of time to allocate appropriate memory.
     * If a variable with the given variable index already exists, the old variable will be replaced.
     *
     * <p>
     *
     * @param index holds the variable index. It is required.
     * @param name holds the variable name; use null or empty std::string ("") if no variable name.
     * @param lowerBound holds the variable lower bound; use -OSDBL_MAX if no lower bound.
     * @param upperBound holds the variable upper bound; use  OSDBL_MAX if no upper bound.
     * @param type holds the variable type character: C for Continuous, B for Binary, I for Integer, 
     *        S for String, D for semi-continuous, J for semi-integer (i.e., either 0 or integer >=n). 
     * @return whether the variable was added successfully.
     */
    bool addVariable(int index, std::string name, double lowerBound, double upperBound, char type);

    /**
     * set all the variable related elements. All the previous variable-related elements will be deleted.
     *
     * <p>
     *
     * @param number holds the number of variables. It is required.
     * @param names holds a std::string array of variable names; use null if no variable names.
     * @param lowerBounds holds a double array of variable lower bounds; use null if all lower bounds are 0;
     *  use -OSDBL_MAX if no lower bound for a specific variable in the array.
     * @param upperBounds holds a double array of variable upper bounds; use null if no upper bounds;
     *  use OSDBL_MAX if no upper bound for a specific variable in the array.
     * @param types holds a char array of variable types; use null if all variables are continuous;
     *        for a specfic variable in the array use C for Continuous, B for Binary, I for Integer, 
     *        S for String, D for semi-continuous, J for semi-integer (i.e., either 0 or integer >=n). 
     * @param inits holds a double array of varible initial values; use null if no initial values. -- deprecated
     * @param initsString holds a std::string array of varible initial values; use null
     *        if no initial std::string values.  -- deprecated
     * @return whether the variables were set successfully.
     */
    bool setVariables(int number, std::string* names, double* lowerBounds,
                      double* upperBounds, char* types);


    /**
     * set the number of objectives.
     *
     * @param number holds the number of objectives.
     * @return whether the number of objectives was set successfully.
     */
    bool setObjectiveNumber(int number);

    /**
     * add an objective. In order to use the add method, the setObjectiveNumber must first be called
     * so that the objective number is known ahead of time to allocate appropriate memory.
     * If a objective with the given objective index already exists, the old objective will be replaced.
     * Objective index will start from -1, -2, -3, ... down, with -1 corresponding to the first objective.
     *
     * <p>
     *
     * @param index holds the objective index. Remember the first objective index is -1, second -2, ...
     * @param name holds the objective name; use null or empty std::string ("") if no objective name.
     * @param maxOrMin holds the objective sense or direction; it can only take two values: "max" or "min".
     * @param constant holds the objective constant; use 0.0 if no objective constant.
     * @param weight holds the objective weight; use 1.0 if no objective weight.
     * @param objectiveCoefficients holds the objective coefficients (null if no objective coefficients)
     *        in a sparse representation that holds two arrays: index array and a value array.
     * @return whether the objective was added successfully.
     */
    bool addObjective(int index, std::string name, std::string maxOrMin, double constant, double weight, SparseVector* objectiveCoefficients);

    /**
     * set all the objectives related elements. All the previous objective-related elements will be deleted.
     *
     * <p>
     *
     * @param number holds the number of objectives. It is required.
     * @param names holds a std::string array of objective names; use null if no objective names.
     * @param maxOrMins holds a std::string array of objective objective senses or directions: "max" or "min"; use null if all objectives are "min".
     * @param constants holds a double array of objective constants; use null if all objective constants are 0.0.
     * @param weights holds a double array of objective weights; use null if all objective weights are 1.0.
     * @param objectiveCoefficients holds an array of objective coefficients, (null if no objective has any coefficients)
     * For each objective, the coefficients are stored in a sparse representation that holds two arrays: index array and a value array.
     * If for a specific objective, there are no objective coefficients, use null for the corresponding array member.
     * @return whether the objectives were set successfully.
     */
    bool setObjectives(int number, std::string *names, std::string *maxOrMins, double *constants, double *weights, SparseVector **objectitiveCoefficients);

    /**
     * set the number of constraints.
     *
     * @param number holds the number of constraints.
     * @return whether the number of constraints was set successfully.
     */
    bool setConstraintNumber(int number);

    /**
     * add a constraint. In order to use the add method, the setConstraintNumber must first be called
     * so that the constraint number is known ahead of time to allocate appropriate memory.
     * If a constraint with the given constraint index already exists, the old constraint will be replaced.
     *
     * <p>
     *
     * @param index holds the constraint index. It is required.
     * @param name holds the constraint name; use null or empty std::string ("") if no constraint name.
     * @param lowerBound holds the constraint lower bound; use -OSDBL_MAX if no lower bound.
     * @param upperBound holds the constraint upper bound; use  OSDBL_MAX if no upper bound.
     * @return whether the constraint was added successfully.
     */
    bool addConstraint(int index, std::string name, double lowerBound, double upperBound, double constant);

    /**
     * set all the constraint related elements. All the previous constraint-related elements will be deleted.
     *
     * <p>
     *
     * @param number holds the number of constraints. It is required.
     * @param names holds a std::string array of constraint names; use null if no constraint names.
     * @param lowerBounds holds a double array of constraint lower bounds; use null if no lower bounds;
     *        use -OSDBL_MAX if no lower bound for a specific constraint in the array.
     * @param upperBounds holds a double array of constraint upper bounds; use null if no upper bounds;
     *        use  OSDBL_MAX if no upper bound for a specific constraint in the array.
     * @return whether the constraints were set successfully.
     */
    bool setConstraints(int number, std::string* names, double* lowerBounds, double* upperBounds, double* constants);

    /**
     * set linear constraint coefficients
     *
     * <p>
     *
     * @param numberOfValues holds the number of specified coefficient values (usually nonzero) in the coefficient matrix.
     * @param isColumnMajor holds whether the coefficient matrix is stored in column major (true) or row major (false).
     * @param values holds a double array coefficient values in the matrix.
     * @param valuesBegin holds the begin index of the values array to copy from (usually 0).
     * @param valuesEnd holds the end index of the values array to copy till (usually values.length - 1).
     * @param indexes holds an integer array column/row indexes for each value in the values array.
     * @param indexesBegin holds the begin index of the indexes array to copy from (usually 0).
     * @param indexesEnd holds the end index of the indexes array to copy till (usually indexes.length - 1).
     * @param starts holds an integer array start indexes in the matrix; the first value of starts should always be 0.
     * @param startsBegin holds the begin index of the starts array to copy from (usually 0).
     * @param startsEnd holds the end index of the starts array to copy till (usually starts.length - 1).
     * @return whether the linear constraint coefficients were set successfully.
     */
    bool setLinearConstraintCoefficients(int numberOfValues, bool isColumnMajor,
                                         double* values, int valuesBegin, int valuesEnd,
                                         int* indexes, int indexesBegin, int indexesEnd,
                                         int* starts, int startsBegin, int startsEnd);

    /**
     * copy linear constraint coefficients: perform a deep copy of the sparse matrix
     *
     * <p>
     *
     * @param numberOfValues holds the number of specified coefficient values (usually nonzero) in the coefficient matrix.
     * @param isColumnMajor holds whether the coefficient matrix is stored in column major (true) or row major (false).
     * @param values holds a double array coefficient values in the matrix.
     * @param valuesBegin holds the begin index of the values array to copy from (usually 0).
     * @param valuesEnd holds the end index of the values array to copy till (usually values.length - 1).
     * @param indexes holds an integer array column/row indexes for each value in the values array.
     * @param indexesBegin holds the begin index of the indexes array to copy from (usually 0).
     * @param indexesEnd holds the end index of the indexes array to copy till (usually indexes.length - 1).
     * @param starts holds an integer array start indexes in the matrix; the first value of starts should always be 0.
     * @param startsBegin holds the begin index of the starts array to copy from (usually 0).
     * @param startsEnd holds the end index of the starts array to copy till (usually starts.length - 1).
     * @return whether the linear constraint coefficients were copied successfully.
     */
    bool copyLinearConstraintCoefficients(int numberOfValues, bool isColumnMajor,
                                         double* values, int valuesBegin, int valuesEnd,
                                         int* indexes, int indexesBegin, int indexesEnd,
                                         int* starts, int startsBegin, int startsEnd);

    /**
     * set the number of quadratic terms
     *
     * <p>
     *
     * @param nq holds the number of quadratic terms.
     * @return whether the number of quadratic terms was set successfully.
     */
    bool setNumberOfQuadraticTerms(int nq);

    /**
     * set quadratic coefficients into the QuadraticCoefficients->qTerm data structure
     *
     * <p>
     *
     * @param number holds the number of quadratic terms.
     * @param rowIndexes holds an integer array of row indexes of all the quadratic terms.
     *        A negative integer corresponds to an objective row, e.g. -1 for 1st objective and -2 for 2nd.
     * @param varOneIndexes holds an integer array of the first  variable indexes of all the quadratic terms.
     * @param varTwoIndexes holds an integer array of the second variable indexes of all the quadratic terms.
     * @param coefficients holds an array of double containing all the quadratic term coefficients.
     * @param begin holds the begin index of all the arrays to copy from (usually = 0).
     * @param end holds the end index of all the arrays to copy till (usually = array length -1).
     * @return whether the quadratic terms were set successfully.
     *
     */
    bool setQuadraticCoefficients(int number,
                                  int* rowIndexes, int* varOneIndexes, int* varTwoIndexes,
                                  double* coefficients, int begin, int end);


    /**
     * set quadratic terms in nonlinearExpressions
     *
     * <p>
     *
     * @param number holds the number of quadratic terms.
     * @param rowIndexes holds an integer array of row indexes of all the quadratic terms.
     *        A negative integer corresponds to an objective row, e.g. -1 for 1st objective and -2 for 2nd.
     * @param varOneIndexes holds an integer array of the first  variable indexes of all the quadratic terms.
     * @param varTwoIndexes holds an integer array of the second variable indexes of all the quadratic terms.
     * @param coefficients holds a double array all the quadratic term coefficients.
     * @return whether the quadratic terms were set successfully.
     */
    bool setQuadraticTermsInNonlinearExpressions(int number,
            int* rowIndexes, int* varOneIndexes, int* varTwoIndexes, double* coefficients);

    /**
     * set nonlinear expressions
     *
     * <p>
     *
     * @param nexpr holds the number of nonlinear expressions.
     * @param root holds a pointer array to the root nodes of all the nonlinear expressions.
     * @return whether the nonlinear expressions were set successfully.
     */
    bool setNonlinearExpressions(int nexpr, Nl** root);

    /**
     * set the number of matrices
     *
     * @param number holds the number of matrices
     * @return whether the number of matrices was set successfully.
     */
    bool setMatrixNumber(int number);

    /**
     * add a matrix. In order to use the add method, the setMatrixNumber must first be called
     * so that the number of matrices is known ahead of time to allocate appropriate memory.
     * If a matrix with the given matrix index already exists, the old matrix will be replaced.
     *
     * <p>
     *
     * @param index holds the matrix index. It is required.
     * @param name holds the matrix name; use null or empty std::string ("") if no matrix name.
     * @param numberOfRows holds the number of rows. It is required. Use 1 for column vectors.
     * @param numberOfColumns holds the number of columns. It is required. Use 1 for row vectors.
     * @param symmetry holds the type of symmetry used in the definition of the matrix. 
     *        For more information  see the enumeration ENUM_MATRIX_SYMMETRY in OSGeneral.h.
     *        If no symmetry, use ENUM_MATRIX_SYMMETRY_none.
     * @param matrixType tracks the type of elements contained in this matrix.
     *        For more information  see the enumeration ENUM_MATRIX_TYPE in OSGeneral.h.
     *        If unsure, use ENUM_MATRIX_TYPE_unknown.
     * @param inumberOfChildren is the number of MatrixNode child elements,
     *        i.e., the number of matrix constructors in the m_mChildren array. 
     * @param m_mChildren is the array of matrix constructors used in the definition of this matrix.
     * @return whether the matrix was added successfully.
     */
    bool addMatrix(int index, std::string name, int numberOfRows, int numberOfColumns, 
                   ENUM_MATRIX_SYMMETRY symmetry, ENUM_MATRIX_TYPE matrixType, 
                   unsigned int inumberOfChildren, MatrixNode **m_mChildren);


   /**
     * set the number of cones
     *
     * @param number holds the number of cones
     * @return whether the number of cones was set successfully.
     */
    bool setConeNumber(int number);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for cones that require basic information only.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be one of
     *            ENUM_CONE_TYPE_nonnegative, 
     *            ENUM_CONE_TYPE_nonpositive, 
     *            ENUM_CONE_TYPE_copositiveMatrices,
     *            ENUM_CONE_TYPE_completelyPositiveMatrices.
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, int numberOfOtherIndexes = 0, int* otherIndexes = NULL);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for product and intersection cones.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be one of
     *            ENUM_CONE_TYPE_product, 
     *            ENUM_CONE_TYPE_intersection. 
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     * @param numberOfComponents holds the number of components of this cone.
     * @param components holds the indexes of the components of this cone.
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, int numberOfComponents, int* components, 
                 int numberOfOtherIndexes = 0, int* otherIndexes = NULL);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for positive or negative cones that reference another cone or matrix.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be one of
     *            ENUM_CONE_TYPE_dual, 
     *            ENUM_CONE_TYPE_polar, 
     *            ENUM_CONE_TYPE_polyhedral. 
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     * @param referenceIdx holds the index of a cone or matrix used in the definition of this cone.
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, int referenceIdx, int numberOfOtherIndexes = 0, int* otherIndexes = NULL);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for positive or negative semidefinite cones.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be
     *            ENUM_CONE_TYPE_semidefinite. 
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     * @param semidefiniteness distinguishes positive and negative semidefinite cones.
     *        It must be either "positive" or "negative".
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, std::string semidefiniteness, int numberOfOtherIndexes = 0, int* otherIndexes = NULL);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for quadratic cones.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be
     *            ENUM_CONE_TYPE_quadratic. 
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     * @param distortionMatrixIdx holds the index of a distortion matrix. Use -1 if there is none.
     * @param normFactor holds a scale factor for the norm. Use 1 if there is none.
     * @param axisDirection holds the index of the axis direction. The most usual value is 0.
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, int distortionMatrixIdx, double normFactor, int axisDirection, 
                 int numberOfOtherIndexes = 0, int* otherIndexes = NULL);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for rotated quadratic cones.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be
     *            ENUM_CONE_TYPE_rotatedQuadratic. 
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     * @param distortionMatrixIdx holds the index of a distortion matrix. Use -1 if there is none.
     * @param normFactor holds a scale factor for the norm. Use 1 if there is none.
     * @param  firstAxisDirection holds the index of the  first axis direction. The most usual value is 0.
     * @param secondAxisDirection holds the index of the second axis direction. The most usual value is 1.
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, int distortionMatrixIdx, double normFactor, int firstAxisDirection, 
                 int secondAxisDirection, int numberOfOtherIndexes = 0, int* otherIndexes = NULL);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for normed cones.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be
     *            ENUM_CONE_TYPE_normed. 
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     * @param distortionMatrixIdx holds the index of a distortion matrix. Use -1 if there is none.
     * @param normFactor holds a scale factor for the norm. Use 1 if there is none.
     * @param pNorm holds the norm descriptor. It must be greater than or equal to 1.
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, int distortionMatrixIdx, double normFactor, int axisDirection, double pNorm, 
                 int numberOfOtherIndexes = 0, int* otherIndexes = NULL);

    /**
     * add a cone. In order to use the add method, the setConeNumber must first be called
     * so that the number of cones is known ahead of time to allocate appropriate memory.
     * If a cone with the given cone index already exists, the old cone will be replaced.
     *
     * @remark This method has different signatures to cater for different types of cones.
     *         This signature is used for cones of nonnegative polynomials and similar cones.
     *
     * @param index holds the cone index. It is required.
     * @param numberOfRows holds the number of rows. It is required.
     * @param numberOfColumns holds the number of columns. It is required.
     * @param coneType holds the cone type. For more information consult the enumeration ENUM_CONE_TYPE
     *        further up in this file. This argument is required and must be
     *            ENUM_CONE_TYPE_nonnegativePolynomials. 
     *            ENUM_CONE_TYPE_sumOfSquaresPolynomials. 
     *            ENUM_CONE_TYPE_moment. 
     *
     * @param name holds the cone name; use null or empty std::string ("") if no cone name.
     * @param maxDegree holds the maximum degree of the polynomials. Use 1, 2, 3, ..., INF.
     * @param numberOfUB holds the number of (box-type) upper bound constraints. Use 0 if there are none.
     * @param ub holds the upper bound values. Use null if there are no upper bounds.
     * @param numberOfLB holds the number of (box-type) lower bound constraints. Use 0 if there are none.
     * @param lb holds the lower bound values. Use null if there are no lower bounds.
     *
     * @param numberOfOtherIndexes holds the number of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to 0.
     * @param otherIndexes holds the array of other indexes if the cone contains higher-dimensional tensors.
     *        This argument is optional and can be omitted. It defaults to null.
     * @return whether the cone was added successfully.
     */
    bool addCone(int index, int numberOfRows, int numberOfColumns, ENUM_CONE_TYPE coneType,
                 std::string name, int maxDegree, int numberOfUB, double* ub, int numberOfLB, double* lb,
                 int numberOfOtherIndexes = 0, int* otherIndexes = NULL);


    // methods to print the current model or parts of it

    /**
     * Print the infix representation of the problem.
     * @return a string with the infix representation
     */
    std::string printModel( );

    /**
     * Print the infix representation of the row (which could be an
     * an objective function row) indexed by rowIdx.
     *
     * @param rowIdx is the index of the row we want to express in infix.
     * @return a string with the infix representation
     */
    std::string printModel( int rowIdx);



    // nonlinear API methods


    /**
     * Initialize the data structures for the nonlinear API
     *
     * @return true if we have initialized the nonlinear data strucutres.
     */
    bool initializeNonLinearStructures( );

    /**
     * Calculate the function value for function (constraint or objective)
     * indexed by idx
     *
     * <p>
     *
     * @param idx is the index on the constraint (0, 1, 2, 3, ...) or objective function (-1, -2, -3, ...).
     * @param x is a pointer (double array) to the current variable values
     * @param new_x is false if any evaluation method was previously called for the current x
     * has been evaluated for the current iterate x
     * use a value of false if not sure
     * @return the function value as a double.
     */
    double calculateFunctionValue(int idx, double* x, bool new_x);

    /**
     * Calculate all of the constraint function values
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objLambda is the Lagrange multiplier on the objective function
     * @param conLambda is pointer (double array) of Lagrange multipliers on
     * the constratins
     * @param new_x is false if any evaluation method was previously called for the current x
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return a double array of constraint function values -- the size of the array is equal to getConstraintNumber().
     */
    double *calculateAllConstraintFunctionValues(double* x, double *objLambda, double *conLambda,
            bool new_x, int highestOrder);

    /**
     * Calculate all of the constraint function values, we are overloading this function
     * and this version of the method will not use any AD and will evaluate function
     * values from the OS Expression Tree
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @return a double array of constraint function values --
     * the size of the array is equal to getConstraintNumber().
     */
    double *calculateAllConstraintFunctionValues(double* x, bool new_x);

    /**
     * Calculate all of the objective function values
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objLambda is the Lagrange multiplier on the objective function
     * @param conLambda is pointer (double array) of Lagrange multipliers on
     * the constratins
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return a double array of objective function values -- the size of the array
     *  is equal to getObjectiveNumber().
     */
    double *calculateAllObjectiveFunctionValues(double* x, double *objLambda, double *conLambda,
            bool new_x, int highestOrder);

    /**
     * Calculate all of the objective function values, we are overloading this function
     * and this version of the method will not use any AD and will evaluate function
     * values from the OS Expression Tree
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @return a double array of objective function values --
     * the size of the array is equal to getObjectiveNumber().
     */
    double *calculateAllObjectiveFunctionValues(double* x, bool new_x);


    /**
     * Calculate the gradient of all constraint functions
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objLambda is the Lagrange multiplier on the objective function
     * @param conLambda is pointer (double array) of Lagrange multipliers on
     * the constratins
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return a pointer a SparseJacobianMatrix.
     */
    SparseJacobianMatrix *calculateAllConstraintFunctionGradients(double* x, double *objLambda,
            double *conLambda, bool new_x, int highestOrder);


    /**
     * Calculate the gradient of the constraint function indexed by idx
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objLambda is the Lagrange multiplier on the objective function
     * @param conLambda is pointer (double array) of Lagrange multipliers on
     * the constratins
     * @parma idx is the index of the constraint function gradient
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return a pointer to a sparse vector of doubles.
     */
    SparseVector *calculateConstraintFunctionGradient(double* x, double *objLambda, double *conLambda,
            int idx, bool new_x, int highestOrder);

    /**
     * Calculate the gradient of the constraint function indexed by idx
     * this function is overloaded
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @parma idx is the index of the constraint function gradient
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return a pointer to a sparse vector of doubles.
     */
    SparseVector *calculateConstraintFunctionGradient(double* x, int idx, bool new_x );

    /**
     * Calculate the gradient of all objective functions
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objLambda is the Lagrange multiplier on the objective function
     * @param conLambda is pointer (double array) of Lagrange multipliers on
     * the constratins
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return an array of pointer to dense objective function gradients.
     */
    double **calculateAllObjectiveFunctionGradients(double* x, double *objLambda, double *conLambda,
            bool new_x, int highestOrder);

    /**
     * Calculate the gradient of the objective function indexed by objIdx
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objLambda is the Lagrange multiplier on the objective function
     * @param conLambda is pointer (double array) of Lagrange multipliers on
     * the constratins
     * @parma objIdx is the index of the objective function being optimized
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return a pointer to a dense vector of doubles.
     */
    double *calculateObjectiveFunctionGradient(double* x, double *objLambda, double *conLambda,
            int objIdx, bool new_x, int highestOrder);

    /**

     * Calculate the gradient of the objective function indexed by objIdx
     * this function is overloaded
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objIdx is the index of the objective function being optimized
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @return a pointer to a dense vector of doubles.
     */
    double *calculateObjectiveFunctionGradient(double* x, int objIdx, bool new_x );

    /**
     * Calculate the Hessian of the Lagrangian Expression Tree
     * This method will build the CppAD expression tree for only the first iteration
     * Use this method on if the value of x does not affect the operations sequence.
     *
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param objLambda is the Lagrange multiplier on the objective function
     * @param conLambda is pointer (double array) of Lagrange multipliers on
     * the constratins
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @param highestOrder is the highest order of the derivative being calculated
     * @return a pointer a SparseHessianMatrix.
     * Each array member corresponds to one constraint gradient.
     */
    SparseHessianMatrix *calculateLagrangianHessian( double* x, double *objLambda, double *conLambda,
            bool new_x, int highestOrder);

    /**
     * Calculate the Hessian of a constraint or objective function
     * <p>
     *
     * @param x is a pointer (double array) to the current variable values
     * @param new_x is false if any evaluation method was previously called
     * for the current iterate
     * @parma idx is the index of the either a constraint or objective
     * function Hessian
     * @return a pointer a SparseVector.
     * Each array member corresponds to one constraint gradient.
     */
    SparseHessianMatrix *calculateHessian( double* x, int idx, bool new_x);


    /**
     *
     * @return true if successful in generating the constraints gradient.
     */
    bool getSparseJacobianFromColumnMajor();

    /**
     *
     * @return true if successful in generating the constraints gradient.
     */
    bool getSparseJacobianFromRowMajor();

    /**
     * @return a pointer to the ExpressionTree for the Lagrangian function of current instance
     * we only take the Lagrangian of the rows with nonlinear terms
     */
    ScalarExpressionTree* getLagrangianExpTree( );

    /**
     * @return a pointer to a map of the indices of all of the variables
     * that appear in the Lagrangian function
     */
    std::map<int, int> getAllNonlinearVariablesIndexMap( );

    /**
     * @return a pointer to a SparseHessianMatrix with the nonzero structure
     * of the Lagrangian Expression Tree
     */
    SparseHessianMatrix* getLagrangianHessianSparsityPattern();

    /**
     *
     * @return true if successful in adding the qTerms to the ExpressionTree.
     * \remark due to the typo in the name of the method, this has been flagged as obsolescent
     * and is being replaced by addQTermsToExpressionTree() -- see below
     */
    bool addQTermsToExressionTree();

    /**
     * This method adds quadratic terms into the array of expression trees.
     * There is at most one expression tree per row (see getAllNonlinearExpressionTrees)
     *
     * @return true if successful in adding the qTerms to the ExpressionTree.
     */
    bool addQTermsToExpressionTree();

    /**
     *
     * @return pointer to a SparseJacobianMatrix.
     */
    SparseJacobianMatrix *getJacobianSparsityPattern();

    /**
     *
     *  duplicate the map of expression trees.
     */
    void duplicateExpressionTreesMap();

#ifdef OS_HAS_CPPAD
    /**
     * F is a CppAD function the range space is the objective +
     * constraints functions, x is the domeain space
     */
    CppAD::ADFun<double> *Fad;
#endif
    /**
     * Create the a CppAD Function object: this is a function where the domain is
     * the set of variables for the problem and the range is the objective function
     * plus constraints
     *
     * <p>
     *
     * @param vdX is a vector of doubles holding the current primal variable values
     * the size of x should equal instanceData->variables->numberOfVariables
     * @return if successfully created
     */
    bool createOSADFun(std::vector<double> vdX );

    /**
     * Perform an AD forward sweep
     *
     * <p>
     * @param p is the highest order Taylor coefficient
     * @param vdX is a vector of doubles of the current primal variable values
     * the size of vdX m_iNumberOfNonlinearVariables
     * @return a double vector equal to the dimension of the range space
     * the result of the forward p sweep
     */
    std::vector<double> forwardAD(int p, std::vector<double> vdX);

    /**
     * Perform an AD reverse sweep
     *
     * <p>
     *
     * @param p is the order of the sweep
     * @param vdlambda is a vector of doubles of the current dual (lagrange) variable values
     * the size of lambda should equal number of objective functions plus number of constraints
     * @return a double vector equal to the n*p
     */
    std::vector<double> reverseAD(int p, std::vector<double> vdlambda);

    /**
     * end revised AD code
     */


    /**
     * Call the AD routine to fill in m_vbLagHessNonz and determine the nonzeros.
     *
     * <p>
     *
     * @return the number of nonzeros in the Hessian
     */
    int  getADSparsityHessian();

    /**
     * end revised AD code
     */

    /**
     * Get the information for each iteration. Get the functions values, Jacobian
     * and Hessian of the Lagrangian
     *
     * <p>
     *
     * @param x is a pointer of doubles of primal values  for the current iteration
     * @param objLambda is is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the objective functions
     * @param conLambda is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the constraints
     * @param new_x is false if any evaluation method was previously called
     * @param highestOrder is the highest order derivative to be calculated
     * @return true if successful
     */
    bool getIterateResults(double *x, double *objLambda, double *conLambda,
                           bool new_x, int highestOrder);



    /**
     * Calculate function values
     *
     * <p>
     *
     * @param x is a pointer of doubles of primal values  for the current iteration
     * @param objLambda is is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the objective functions
     * @param conLambda is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the constraints
     * @return true if successful
     */
    bool getZeroOrderResults(double *x, double *objLambda, double *conLambda);

    /**
     * Calculate first derivatives
     *
     * <p>
     *
     * @param x is a pointer of doubles of primal values  for the current iteration
     * @param objLambda is is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the objective functions
     * @param conLambda is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the constraints
     * @return true if successful
     */
    bool getFirstOrderResults(double *x, double *objLambda, double *conLambda );

    /**
     * Calculate second derivatives
     *
     * <p>
     *
     * @param x is a pointer of doubles of primal values  for the current iteration
     * @param objLambda is is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the objective functions
     * @param conLambda is a pointer of doubles of the current dual (Lagrange) multipliers
     *  on the constraints
     * @return true if successful
     */
    bool getSecondOrderResults(double *x, double *objLambda, double *conLambda );


    /**
     * This should be called by nonlinear solvers using callback functions
     *
     * <p>
     *
     * initForAlgDiff will initialize the correct nonlinear structures in preparation
     * for using the algorithmic differentiation routines.
     * @return true if successful
     */
    bool initForAlgDiff();

    /**
     * This should be called by initForAlgDiff()
     *
     * <p>
     *
     * initObjGradients will initialize the objective function gradients to be equal to the
     * coefficients given in the <coef> section of the OSiL instance
     * @return true if successful
     */
    bool initObjGradients();


    /**
     * bUseExpTreeForFunEval is set to true if you wish to use the OS Expression Tree for
     * function evaluations instead of AD -- false by default.
     */
    bool bUseExpTreeForFunEval;


    /**
     * This sets the format of the time domain ("stages"/"interval"/"none")
     */
    bool setTimeDomain(std::string format);

    /**
     * This sets the number (and optionally names) of the time stages
     */
    bool setTimeDomainStages(int number, std::string *names);

    /**

    * This sets the variables associated with each time domain stage in temporal order.
    * (I.e., for each stage numberOfVariables gives the number of variables accociated
    * with this stage and startIdx gives the first variable in this stage.)
    */
    bool setTimeDomainStageVariablesOrdered(int numberOfStages, int *numberOfVariables, int *startIdx);

    /**

    * This sets the variables associated with each time domain stage in srbitrary order.
    * (I.e., for each stage numberOfVariables gives the number of variables accociated
    * with this stage and varIndex[i] gives the index of each variable in stage[i].)
    */
    bool setTimeDomainStageVariablesUnordered(int numberOfStages, int *numberOfVariables, int **varIndex);

    /**
    * This sets the constraints associated with each time domain stage in temporal order.
    * (I.e., for each stage numberOfConstraints gives the number of constraints accociated
    * with this stage and startIdx gives the first constraint in this stage.)
    */
    bool setTimeDomainStageConstraintsOrdered(int numberOfStages, int *numberOfConstraints, int *startIdx);

    /**
    * This sets the constraints associated with each time domain stage in srbitrary order.
    * (I.e., for each stage numberOfConstraints gives the number of constraints accociated
    * with this stage and conIndex[i] gives the index of each constraint in stage[i].)
    */
    bool setTimeDomainStageConstraintsUnordered(int numberOfStages, int *numberOfConstraints, int **conIndex);

    /**
    * This sets the objectives associated with each time domain stage in temporal order.
    * (I.e., for each stage numberOfObjectives gives the number of objectives accociated
    * with this stage and startIdx gives the first objective in this stage.)
    */
    bool setTimeDomainStageObjectivesOrdered(int numberOfStages, int *numberOfObjectives, int *startIdx);

    /**
    * This sets the objectives associated with each time domain stage in arbitrary order.
    * (I.e., for each stage numberOfObjectives gives the number of objectives accociated
    * with this stage and objIndex[i] gives the index of each objective in stage[i].)
    */
    bool setTimeDomainStageObjectivesUnordered(int numberOfStages, int *numberOfObjectives, int **varIndex);

    /**
     * This sets the start and end of the time interval
     */
    bool setTimeDomainInterval(double start, double horizon);


}; //class OSInstance

#endif

