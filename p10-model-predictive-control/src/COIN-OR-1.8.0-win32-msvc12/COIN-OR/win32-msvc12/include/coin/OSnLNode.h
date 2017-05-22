/* $Id: OSnLNode.h 4996 2015-04-14 20:38:45Z Gassmann $ */
/** @file OSnLNode.h
 * \brief This file defines the OSnLNode class along with its derived classes.
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2015, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 *
 * In this file we define classes for a subset of the nodes defined in the OSnL schema
 * These nodes fall into two broad classes: 
 * Those that evaluate to scalar values (which inherit from OSnLNode), and
 * those that evaluate to matrices (and inherit from OSnLMNode).
 * OSnLNodes can have OSnLMNode children (e.g., matrixDeterminant) 
 * and vice versa (e.g., matrixScalarTimes). 
 * Both OSnLNode and OSnLMNode inherit from the base class ExprNode. 
 */

#ifndef OSNLNODE_H
#define OSNLNODE_H

#include "OSConfig.h"
#include "OSGeneral.h"
#include "OSErrorClass.h"
#include <iostream>
#include <vector>
#include <map>

#ifdef OS_HAS_CPPAD
# include <cppad/cppad.hpp>
typedef CppAD::AD<double>  ADdouble;
typedef CppAD::vector<ADdouble> ADvector;
#else
typedef double  ADdouble;
typedef std::vector<ADdouble> ADvector;
#endif

/**
 *  Some forward declarations to make sure circular references are handled properly.
 */
class OSnLNode;
class OSnLMNode;
class OSMatrix;

/*! \class ExprNode 
 *  \brief A generic class from which we derive both OSnLNode and OSnLMNode
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 * @version 2.9, 10/Sep/2014
 */
class ExprNode
{
public:

    /**  inodeInt is the unique integer assigned to the OSnLNode or OSnLMNode in OSParameters.h.
     */
    int inodeInt;

    /** inodeType essentially tracks whether the number of children are known or not.
     *  For most nodes this is known, in which case inodeType is set to inumberOfChildren.
     *  For some nodes the number of children is not known a priori, e.g., a sum node,
     *  then inodeType is set to -1.
     */
    int inodeType;

    /**  inumberOfChildren is the number of OSnLNode child elements
     *   If this number is not fixed, e.g., for a sum node, it is temporarily set to 0
     */
    unsigned int inumberOfChildren;

    /**  inumberOfMatrixChildren is the number of OSnLMNode child elements
     *   If this number is not fixed, e.g., for a matrixProduct node, it is temporarily set to 0
     */
    unsigned int inumberOfMatrixChildren;

    /**
     * m_mChildren holds all the operands, that is, nodes that the current node operates on.
     */
    OSnLNode **m_mChildren;

    /**
     * m_mMatrixChildren holds all the matrix-valued operands, if any.
     */
    OSnLMNode **m_mMatrixChildren;


    /**
     * default constructor.
     */
    ExprNode();

    /**
     * default destructor.
     */
    virtual ~ExprNode();


    /**
     * @return the value of inodeInt
     */
    virtual std::string getTokenNumber();

    /**
     * @return the value of the operator name
     */
    virtual std::string getTokenName() = 0;

    /**
     * The following method writes an OSnLNode or OSnLMNode in OSiL format.
     * It is used by OSiLWriter to assist in writing an OSiL file 
     * from a corresponding OSInstance.
     *
     * @return the ExprNode and its children as an OSiL string.
     */
    virtual std::string getNonlinearExpressionInXML();

    /**
     * Get a vector of pointers to OSnLNodes and OSnLMNodes that correspond to
     * the (scalar-valued or matrix-valued) expression tree in prefix format.
     *
     * @return the expression tree as a vector of ExprNodes in prefix.
     */
    virtual std::vector<ExprNode*> getPrefixFromExpressionTree();

    /**
     * Called by getPrefixFromExpressionTree().  
     * This method calls itself recursively and
     * generates a vector of pointers to ExprNode in prefix
     * 
     * @param a pointer prefixVector to a vector of pointers of ExprNodes
     * @return a vector of pointers to ExprNode in prefix.
     */
    virtual std::vector<ExprNode*> preOrderOSnLNodeTraversal( std::vector<ExprNode*> *prefixVector);

    /**
     * Get a vector of pointers to ExprNodes that correspond to
     * the expression tree in postfix format
     *
     * @return the expression tree as a vector of ExprNodes in postfix.
     */
    virtual std::vector<ExprNode*> getPostfixFromExpressionTree();

    /**
     * Called by getPostfixFromExpressionTree(). 
     * This method calls itself recursively and
     * generates a vector of pointers to ExprNodes in postfix.
     * 
     * @param a pointer postfixVector to a vector of pointers of ExprNodes
     * @return a vector of pointers to ExprNodes in postfix.
     */
    virtual std::vector<ExprNode*> postOrderOSnLNodeTraversal( std::vector<ExprNode*> *postfixVector);

    /**
     * Create or clone a node of this type.
     * This is an abstract method which is required to be implemented by the concrete
     * operator nodes that derive or extend from this class.
     */
    virtual ExprNode *cloneExprNode() = 0;

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(ExprNode *that);
};//end ExprNode


/*! \class OSnLNode 
 *  \brief The OSnLNode Class for nonlinear expressions.
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 */
class OSnLNode: public ExprNode
{
public:
    /**
     * m_dFunctionValue holds the function value given the current variable values.
     */
    double m_dFunctionValue;

    /**
     * m_ADTape stores the expression tree for the this OSnLNode as an ADdouble.
     */
    ADdouble m_ADTape;


    /**
     * default constructor.
     */
    OSnLNode();

    /**
     * default destructor.
     */
    virtual ~OSnLNode();


    /**
     * varIdx is a map where the key is the index of an OSnLNodeVariable and
     * (*varIdx)[ idx] is the kth variable in the map, e.g.
     * (*varIdx)[ 5] = 2 means that variable indexed by 5 is the second variable
     * in the OSnLNode and all of its children
     * 
     * @param a pointer to a map of the variables in the OSnLNode and its children
     */
    virtual void getVariableIndexMap(std::map<int, int> *varIdx);

    /**
     * Calculate the function value given the current variable values.
     * This is an abstract method which is required to be implemented by the concrete
     * operator nodes that derive or extend from this OSnLNode class.
     *
     * @param x holds the values of the variables in a double array.
     * @return the function value given the current variable values.
     */
    virtual double calculateFunction(double *x) = 0;

    /**
     * Create the AD tape to be evaluated by AD.
     * This is an abstract method which is required to be implemented by the concrete
     * operator nodes that derive or extend from this OSnLNode class.
     *
     * @return the expression tree.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD) = 0;

    /**
     * Take a vector of ExprNodes (OSnLNodes and OSnLMNodes) in prefix format
     * and create a scalar-valued OSExpressionTree root node
     * 
     * @param nlNodeVec holds a vector of pointers to OSnLNodes and OSnLMNodes
     * in prefix format
     * @return a pointer to an OSnLNode which is the root of
     * an OSExpressionTree.
     */
    OSnLNode* createExpressionTreeFromPrefix(std::vector<ExprNode*> nlNodeVec);

    /**
     * Get a vector of pointers to OSnLNodes and OSnLMNodes that correspond to
     * the (scalar-valued or matrix-valued) expression tree in prefix format.
     *
     * @return the expression tree as a vector of ExprNodes in prefix.
     */
    virtual std::vector<ExprNode*> getPrefixFromExpressionTree();

    /**
     * Called by getPrefixFromExpressionTree().  
     * This method calls itself recursively and
     * generates a vector of pointers to ExprNode in prefix
     * 
     * @param a pointer prefixVector to a vector of pointers of ExprNodes
     * @return a vector of pointers to ExprNode in prefix.
     */
    virtual std::vector<ExprNode*> preOrderOSnLNodeTraversal( std::vector<ExprNode*> *prefixVector);

    /**
     * Take a vector of ExprNodes (OSnLNodes and OSnLMNodes) in postfix format 
     * and create a scalar-valued OSExpressionTree root node
     * 
     * @param nlNodeVec holds a vector of pointers to OSnLNodes
     * in postfix format
     * @return a pointer to an OSnLNode which is the root of
     * an OSExpressionTree.
     */
    OSnLNode* createExpressionTreeFromPostfix(std::vector<ExprNode*> nlNodeVec);

    /**
     * Get a vector of pointers to ExprNodes that correspond to
     * the expression tree in postfix format
     *
     * @return the expression tree as a vector of ExprNodes in postfix.
     */
    virtual std::vector<ExprNode*> getPostfixFromExpressionTree();

    /**
     * Called by getPostfixFromExpressionTree(). 
     * This method calls itself recursively and
     * generates a vector of pointers to ExprNodes in postfix.
     * 
     * @param a pointer postfixVector to a vector of pointers of ExprNodes
     * @return a vector of pointers to ExprNodes in postfix.
     */
    virtual std::vector<ExprNode*> postOrderOSnLNodeTraversal( std::vector<ExprNode*> *postfixVector);

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLNode* copyNodeAndDescendants();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(OSnLNode *that);
};//end OSnLNode


/*! \class OSnLNodePlus
 *  \brief The OSnLNodePlus Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <plus>
 *
 */
class OSnLNodePlus : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodePlus();

    /**
     * default destructor.
     */
    ~OSnLNodePlus();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodePlus::calculateFunction(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn double OSnLNodePlus::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

};//end OSnLNodePlus

/*! \class OSnLNodeSum
 *  \brief The OSnLNodeSum Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <sum>
 *
 */
class OSnLNodeSum : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeSum();

    /**
     * default destructor.
     */
    ~OSnLNodeSum();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeSum::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeSum::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeSum

/*! \class OSnLNodeMax
 *  \brief The OSnLNodeMax Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <max>
 *
 */
class OSnLNodeMax : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeMax();

    /**
     * default destructor.
     */
    ~OSnLNodeMax();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeMax::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeMax::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeMax

/*! \class OSnLNodeMin
 *  \brief The OSnLNodeMin Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <min>
 *
 */
class OSnLNodeMin : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeMin();

    /**
     * default destructor.
     */
    ~OSnLNodeMin();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeMin::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */

    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeMin::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

};//end OSnLNodeMin


/*! \class OSnLNodeMinus
 *  \brief The OSnLNodeMinus Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <minus>
 *
 */
class OSnLNodeMinus : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeMinus();

    /**
     * default destructor.
     */
    ~OSnLNodeMinus();


    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeMinus::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeMinus::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeMinus


/*! \class OSnLNodeNegate
 *  \brief The OSnLNodeNegate Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <negate>
 *
 */
class OSnLNodeNegate : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeNegate();

    /**
     * default destructor.
     */
    ~OSnLNodeNegate();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeNegate::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();


    /*! \fn double OSnLNodeNegate::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeNegate


/*! \class OSnLNodeTimes
 *  \brief The OSnLNodeTimes Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <times>
 *
 */
class OSnLNodeTimes : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeTimes();

    /**
     * default destructor.
     */
    ~OSnLNodeTimes();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeTimes::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeNegate::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

};//end OSnLNodeTimes


/*! \class OSnLNodeDivide
 *  \brief The OSnLNodeDivide Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <divide>
 *
 */
class OSnLNodeDivide : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeDivide();

    /**
     * default destructor.
     */
    ~OSnLNodeDivide();

    /**
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeDivide::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeDivide::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeDivide


/*! \class OSnLNodePower
 *  \brief The OSnLNodePower Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <power>
 *
 */
class OSnLNodePower : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodePower();

    /**
     * default destructor.
     */
    ~OSnLNodePower();

    /**
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodePower::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodePower::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodePower


/*! \class OSnLNodeProduct
 *  \brief The OSnLNodeProduct Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <product>
 *
 */
class OSnLNodeProduct : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeProduct();

    /**
     * default destructor.
     */
    ~OSnLNodeProduct();

    /**
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeProduct::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeProduct::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeProduct


/*! \class OSnLNodeLn
 *  \brief The OSnLNodeLn Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <ln>
 *
 */
class OSnLNodeLn : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeLn();

    /**
     * default destructor.
     */
    ~OSnLNodeLn();

    /**
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeLn::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeLn::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeLn


/*! \class OSnLNodeSqrt
 *  \brief The OSnLNodeSqrt Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <sqrt>
 *
 */
class OSnLNodeSqrt : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeSqrt();

    /**
     * default destructor.
     */
    ~OSnLNodeSqrt();

    /**
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeSqrt::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();
    /*! \fn double OSnLNodeSqrt::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeSqrt


/*! \class OSnLNodeSquare
 *  \brief The OSnLNodeSquare Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <square>
 *
 */
class OSnLNodeSquare : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeSquare();

    /**
     * default destructor.
     */
    ~OSnLNodeSquare();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeSquare::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeSquare::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeSquare


/*! \class OSnLNodeCos
 *  \brief The OSnLNodeCos Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <cos>
 *
 */
class OSnLNodeCos : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeCos();

    /**
     * default destructor.
     */
    ~OSnLNodeCos();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeCos::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeCos::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeCos


/*! \class OSnLNodeSin
 *  \brief The OSnLNodeSin Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <sin>
 *
 */
class OSnLNodeSin : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeSin();

    /**
     * default destructor.
     */
    ~OSnLNodeSin();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeSin::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeSin::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeSin


/*! \class OSnLNodeExp
 *  \brief The OSnLNodeExp Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <exp>
 *
 */
class OSnLNodeExp : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeExp();

    /**
     * default destructor.
     */
    ~OSnLNodeExp();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeExp::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn double OSnLNodeExp::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();
};//end OSnLNodeExp


/*! \class OSnLNodeAbs
 *  \brief The OSnLNodeAbs Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <abs>
 *
 */
class OSnLNodeAbs : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeAbs();

    /**
     * default destructor.
     */
    ~OSnLNodeAbs();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeAbs::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeAbs::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeAbs


/*! \class OSnLNodeErf
 *  \brief The OSnLNodeErf Class.
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <ln>
 *
 */
class OSnLNodeErf : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeErf();

    /**
     * default destructor.
     */
    ~OSnLNodeErf();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeErf::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeLn::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeErf


/*! \class OSnLNodeIf
 *  \brief The OSnLNodeIf Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <if>
 *
 */
class OSnLNodeIf : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeIf();

    /**
     * default destructor.
     */
    ~OSnLNodeIf();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeIf::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeIf::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeIf


/*! \class OSnLNodeNumber
 *  \brief The OSnLNodeNumber Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <number>
 *
 */
class OSnLNodeNumber : public OSnLNode
{
public:
    /** value is the value of the number */
    double value;

    /** in the C++ type is real */
    std::string type;

    /** later, e.g. stochastic programming, we may wish
     * to give an id to a number
     */
    std::string id;

    /**
     * default constructor.
     */
    OSnLNodeNumber();

    /**
     * default destructor.
     */
    ~OSnLNodeNumber();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the number node.
     */
    virtual std::string getNonlinearExpressionInXML();

    /*! \fn double OSnLNodeNumber::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLNode* copyNodeAndDescendants();

    /*! \fn double OSnLNodeNumber::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLNodeNumber *that);
};//end OSnLNodeNumber


/*! \class OSnLNodeE
 *  \brief The OSnLNodeE Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <E>
 *
 */
class OSnLNodeE : public OSnLNode
{
public:

    /**
     * default constructor.
     */
    OSnLNodeE();

    /**
     * default destructor.
     */
    ~OSnLNodeE();


    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();


    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenName();

    /**
     *
     * @return the OSiL XML for the number node.
     */
    virtual std::string getNonlinearExpressionInXML();

    /*! \fn double OSnLNodeE::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();


    /*! \fn double OSnLNodeE::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeE


/*! \class OSnLNodePI
 *  \brief The OSnLNodePI Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <pi>
 *
 */
class OSnLNodePI : public OSnLNode
{
public:

    /**
     * default constructor.
     */
    OSnLNodePI();

    /**
     * default destructor.
     */
    ~OSnLNodePI();


    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();


    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenName();

    /**
     *
     * @return the OSiL XML for the number node.
     */
    virtual std::string getNonlinearExpressionInXML();

    /*! \fn double OSnLNodePI::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();


    /*! \fn double OSnLNodePI::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodePI


/*! \class OSnLNodeVariable
 *  \brief The OSnLNodeVariable Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <variable>
 *
 */
class OSnLNodeVariable : public OSnLNode
{
public:

    /** coef is an option coefficient on the variable, the
     * default value is 1.0
     */
    double coef;

    /** idx is the index of the variable */
    int idx;

    /**
     * default constructor.
     */
    OSnLNodeVariable();

    /**
     * default destructor.
     */
    ~OSnLNodeVariable();

    virtual void getVariableIndexMap(std::map<int, int> *varIdx);

    /**
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();

    /**
     * @return a std::string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenName();

    /**
     * @return the OSiL XML for the variable node.
     */
    virtual std::string getNonlinearExpressionInXML();

    /*! \fn double OSnLNodeVariable::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLNode* copyNodeAndDescendants();

    /*! \fn double OSnLNodeVariable::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLNodeVariable *that);
};//end OSnLNodeVariable


/*! \class OSnLNodeAllDiff
 *  \brief The OSnLNodeAllDiff Class.
 *
 * @author  Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 10/05/2005
 * @since   OS1.0
 *
 * \remarks
 * The in-memory representation of the OSnL element <alldiff>
 *
 */

class OSnLNodeAllDiff : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeAllDiff();

    /**
     * default destructor.
     */
    ~OSnLNodeAllDiff();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeAllDiff::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNode of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

    /*! \fn double OSnLNodeAllDiff::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
};//end OSnLNodeAllDiff


/** The next few nodes evaluate to a scalar even though one or more of its arguments are matrices **/

/*! \class OSnLNodeMatrixDeterminant
 *  \brief The OSnLNodeMatrixDeterminant Class.
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 * @date    11/06/2014
 * @since   OS2.8
 *
 * \remarks
 * The in-memory representation of the OSnL element <matrixDeterminant>
 *
 */
class OSnLNodeMatrixDeterminant : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeMatrixDeterminant();

    /**
     * default destructor.
     */
    ~OSnLNodeMatrixDeterminant();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeMatrixDeterminant::calculateFunction(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn double OSnLNodeMatrixDeterminant::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

    /*! \fn OSnLNodeMatrixDeterminant *cloneExprNodeMatrixDeterminant(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNodeMatrixDeterminant of the proper type.
     */
    virtual OSnLNode *cloneExprNode();

};//end OSnLNodeMatrixDeterminant

/*! \class OSnLNodeMatrixTrace
 *  \brief The OSnLNodeMatrixTrace Class.
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 * @date    11/06/2014
 * @since   OS2.8
 *
 * \remarks
 * The in-memory representation of the OSnL element <matrixTrace>
 *
 */
class OSnLNodeMatrixTrace : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeMatrixTrace();

    /**

     * default destructor.
     */
    ~OSnLNodeMatrixTrace();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeMatrixTrace::calculateFunction(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn double OSnLNodeMatrixTrace::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

    /*! \fn OSnLNodeMatrixTrace *cloneExprNodeMatrixTrace(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNodeMatrixTrace of the proper type.
     */
    virtual OSnLNode *cloneExprNode();
};//end OSnLNodeMatrixToScalar

/*! \class OSnLNodeMatrixToScalar
 *  \brief The OSnLNodeMatrixTrace Class.
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 * @date    11/06/2014
 * @since   OS2.8
 *
 * \remarks
 * The in-memory representation of the OSnL element <matrixToScalar>
 *
 */
class OSnLNodeMatrixToScalar : public OSnLNode
{
public:
    /**
     * default constructor.
     */
    OSnLNodeMatrixToScalar();

    /**

     * default destructor.
     */
    ~OSnLNodeMatrixToScalar();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeMatrixTrace::calculateFunction(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);

    /*! \fn double OSnLNodeMatrixTrace::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);

    /*! \fn OSnLNodeMatrixTrace *cloneExprNodeMatrixTrace(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLNodeMatrixTrace of the proper type.
     */
    virtual OSnLNode *cloneExprNode();
};//end OSnLNodeMatrixToScalar


/*! \class OSnLMNode 
 *  \brief The OSnLMNode Class for nonlinear expressions involving matrices
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 * @date    11/06/2014
 * @since   OS2.8
 */

class OSnLMNode: public ExprNode
{
public:

    /**
     * default constructor.
     */
    OSnLMNode();

    /**
     * default destructor.
     */
    virtual ~OSnLMNode();


    /**
     * Take a vector of ExprNodes (OSnLNodes and OSnLMNodes) in prefix format
     * and create a matrix-valued OSExpressionTree root node
     * 
     * @param nlNodeVec holds a vector of pointers to OSnLNodes and OSnLMNodes
     * in prefix format
     * @return a pointer to an OSnLMNode which is the root of
     * an OSExpressionTree.
     */
    OSnLMNode* createExpressionTreeFromPrefix(std::vector<ExprNode*> nlNodeVec);

    /**
     * Get a vector of pointers to OSnLNodes and OSnLMNodes that correspond to
     * the (matrix-valued) expression tree in prefix format.
     *
     * @return the expression tree as a vector of ExprNodes in prefix.
     */
    std::vector<ExprNode*> getPrefixFromExpressionTree();

    /**
     * Called by getPrefixFromExpressionTree().  
     * This method calls itself recursively and
     * generates a vector of pointers to ExprNode in prefix
     * 
     * @param a pointer prefixVector to a vector of pointers of ExprNodes
     * @return a vector of pointers to ExprNode in prefix.
     */
    std::vector<ExprNode*> preOrderOSnLNodeTraversal( std::vector<ExprNode*> *prefixVector);

    /**
     * Take a vector of ExprNodes (OSnLNodes and OSnLMNodes) in postfix format
     * and create a matrix-valued OSExpressionTree root node
     * 
     * @param nlNodeVec holds a vector of pointers to OSnLNodes and OSnLMNodes
     * in postfix format
     * @return a pointer to an OSnLMNode which is the root of
     * an OSExpressionTree.
     */
    OSnLMNode* createExpressionTreeFromPostfix(std::vector<ExprNode*> nlNodeVec);

    /**
     * Get a vector of pointers to ExprNodes that correspond to
     * the expression tree in postfix format
     *
     * @return the expression tree as a vector of ExprNodes in postfix.
     */
    std::vector<ExprNode*> getPostfixFromExpressionTree();

    /**
     * Called by getPostfixFromExpressionTree(). 
     * This method calls itself recursively and
     * generates a vector of pointers to ExprNodes in postfix.
     * 
     * @param a pointer postfixVector to a vector of pointers of ExprNodes
     * @return a vector of pointers to ExprNodes in postfix.
     */
    std::vector<ExprNode*> postOrderOSnLNodeTraversal( std::vector<ExprNode*> *postfixVector);


    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLMNode* copyNodeAndDescendants();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(OSnLMNode *that);
};//end OSnLMNode

class OSnLMNodeMatrixPlus : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixPlus();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixPlus();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixPlus

class OSnLMNodeMatrixSum : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixSum();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixSum();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();
    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

#if 0
    /*! \fn double OSnLMNodeMatrixPlus::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
#endif

};//end OSnLMNodeMatrixPlus

class OSnLMNodeMatrixMinus : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixMinus();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixMinus();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixMinus

class OSnLMNodeMatrixNegate : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixNegate();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixNegate();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();


    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixNegate

class OSnLMNodeMatrixTimes : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixTimes();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixTimes();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixTimes

class OSnLMNodeMatrixInverse : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixInverse();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixInverse();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixInverse

class OSnLMNodeMatrixTranspose : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixTranspose();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixTranspose();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixTranspose

class OSnLMNodeMatrixScalarTimes : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixScalarTimes();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixScalarTimes();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixScalarTimes

class OSnLMNodeMatrixDotTimes : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixDotTimes();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixDotTimes();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixDotTimes

class OSnLMNodeIdentityMatrix : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeIdentityMatrix();

    /**
     * default destructor.
     */
    ~OSnLMNodeIdentityMatrix();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeIdentityMatrix

class OSnLMNodeMatrixLowerTriangle : public OSnLMNode
{
public:
    /**
     *  A boolean to express whether the diagonal is to be part of the upper triangle or not
     */
    bool includeDiagonal;

    /**
     * default constructor.
     */
    OSnLMNodeMatrixLowerTriangle();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixLowerTriangle();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLMNode* copyNodeAndDescendants();

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLMNodeMatrixLowerTriangle *that);
};//end OSnLMNodeMatrixLowerTriangle

class OSnLMNodeMatrixUpperTriangle : public OSnLMNode
{
public:
    /**
     *  A boolean to express whether the diagonal is to be part of the upper triangle or not
     */
    bool includeDiagonal;

    /**
     * default constructor.
     */
    OSnLMNodeMatrixUpperTriangle();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixUpperTriangle();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLMNode* copyNodeAndDescendants();

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLMNodeMatrixUpperTriangle *that);
};//end OSnLMNodeMatrixUpperTriangle


class OSnLMNodeMatrixDiagonal : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixDiagonal();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixDiagonal();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixDiagonal

class OSnLMNodeDiagonalMatrixFromVector : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeDiagonalMatrixFromVector();

    /**
     * default destructor.
     */
    ~OSnLMNodeDiagonalMatrixFromVector();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeDiagonalMatrixFromVector


class OSnLMNodeMatrixSubmatrixAt : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixSubmatrixAt();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixSubmatrixAt();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
//    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrix>.
     */
//    virtual std::string getNonlinearExpressionInXML();

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();
};//end OSnLMNodeMatrixSubmatrixAt


class OSnLMNodeMatrixReference : public OSnLMNode
{
public:
    /**
     *  The index of the matrix
     */
     int idx;

    /**
     * default constructor.
     */
    OSnLMNodeMatrixReference();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixReference();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrixReference>.
     */
    virtual std::string getNonlinearExpressionInXML();

#if 0
    /*! \fn double OSnLNodeNumber::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);
#endif

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLMNode* copyNodeAndDescendants();

#if 0
    /*! \fn double OSnLMNodeMatrixReference::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
#endif

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLMNodeMatrixReference *that);
};//end OSnLMNodeMatrixReference


class OSnLMNodeMatrixVar : public OSnLMNode
{
public:
    /**
     *  The index of the matrixVar
     */
     int idx;

    /**
     * default constructor.
     */
    OSnLMNodeMatrixVar();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixVar();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrixReference>.
     */
    virtual std::string getNonlinearExpressionInXML();

#if 0
    /*! \fn double OSnLNodeNumber::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);
#endif

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLMNode* copyNodeAndDescendants();

#if 0
    /*! \fn double OSnLMNodeMatrixReference::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
#endif

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLMNodeMatrixVar *that);
};//end OSnLMNodeMatrixVar


class OSnLMNodeMatrixObj : public OSnLMNode
{
public:
    /**
     *  The index of the matrixObj
     */
     int idx;

    /**
     * default constructor.
     */
    OSnLMNodeMatrixObj();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixObj();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrixObj>.
     */
    virtual std::string getNonlinearExpressionInXML();

#if 0
    /*! \fn double OSnLNodeNumber::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);
#endif

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLMNode* copyNodeAndDescendants();

#if 0
    /*! \fn double OSnLMNodeMatrixObj::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
#endif

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLMNodeMatrixObj *that);
};//end OSnLMNodeMatrixObj


class OSnLMNodeMatrixCon : public OSnLMNode
{
public:
    /**
     *  The index of the matrixCon
     */
     int idx;

    /**
     * default constructor.
     */
    OSnLMNodeMatrixCon();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixCon();

    /**
     *
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /**
     *
     * @return a string token that corresponds to the OSnLNode.
     */
    virtual std::string getTokenNumber();

    /**
     *
     * @return the OSiL XML for the OSnLMNode <matrixCon>.
     */
    virtual std::string getNonlinearExpressionInXML();

#if 0
    /*! \fn double OSnLNodeNumber::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */
    virtual double calculateFunction( double *x);
#endif

    /*! \fn OSnLMNode *cloneOSnLMNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

    /**
     * make a copy of this node and all its descendants
     * @return a pointer to the duplicate node
     */
    virtual OSnLMNode* copyNodeAndDescendants();

#if 0
    /*! \fn double OSnLMNodeMatrixCon::constructADTape(std::map<int, int> *ADIdx, vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD);
#endif

    /**
     * A function to check for the equality of two objects
     */
    virtual bool IsEqual(OSnLMNodeMatrixCon *that);
};//end OSnLMNodeMatrixCon


/*! \class OSnLMNodeMatrixProduct
 *  \brief The OSnLMNodeMatrixProduct Class.
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 08/Dec/2014
 * @since   OS2.9
 *
 * \remarks
 * The in-memory representation of the OSnL element <matrixProduct>
 *
 */
class OSnLMNodeMatrixProduct : public OSnLMNode
{
public:
    /**
     * default constructor.
     */
    OSnLMNodeMatrixProduct();

    /**
     * default destructor.
     */
    ~OSnLMNodeMatrixProduct();

    /**
     * @return the value of operator name
     */
    virtual std::string getTokenName();

    /*! \fn double OSnLNodeProduct::double(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a double.
     */    
    //virtual double calculateFunction( double *x);

    /*! \fn OSnLNode *cloneExprNode(double *x)
     *  \brief The implementation of the virtual functions.
     *  \return a pointer to a new OSnLMNode of the proper type.
     */
    virtual OSnLMNode *cloneExprNode();

    /*! \fn double OSnLMNodeMatrixProduct::constructADTape(std::map<int, int> *ADIdx, 
     *                                                     vector< ADdouble > *XAD)
     *  \brief The implementation of the virtual functions.
     *  \return a ADdouble.
     */
    //virtual ADdouble constructADTape(std::map<int, int> *ADIdx, ADvector *XAD); 
};//end OSnLNodeProduct



/*
TO DO:

1.  Allow for a variable node with a child

2.  When we create the Expression Tree from postfix, do we check if the coefficient of a variable is 1?
*/
#endif


