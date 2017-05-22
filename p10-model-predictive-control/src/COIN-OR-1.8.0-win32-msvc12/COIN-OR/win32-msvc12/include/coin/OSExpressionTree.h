/* $Id: OSExpressionTree.h 4996 2015-04-14 20:38:45Z Gassmann $ */
/** @file OSExpressionTree.h
 *
 * @author  Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2014, Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include "OSnLNode.h"
#include <vector>
#include <map>

class ExprNode; 
class OSnLNode; 
class OSnLMNode; 

/*! \class OSExpressionTree
 *  \brief Used to hold the instance in memory.
 *
 * \remarks
 * This is a generic class.
 * Specific classes ScalarExpressionTree (for expressions that evaluate to scalar values)
 * and MatrixExpressionTrees (for expressions that evaluate to matrices) are derived
 * from this class. 
 *
 */

class OSExpressionTree
{
public:
    /**
     * default constructor.
     */
    OSExpressionTree();

    /**
     * default destructor.
     */
    ~OSExpressionTree();

    /**
     * m_mapVarIdx is a map used to generate the infix expression for AD   
     * the key is idx, a variable number;
     * the value of the map is the location of the corresponding entry in the sparse Jacobian
     */
    std::map<int, int> *mapVarIdx;

    /**
     * Retrieve a map of the indices of the variables
     * that are in the expression tree
     *
     * </p>
     *
     * @return a map of the variables in the current expression tree.
     */
    //virtual std::map<int, int> *getVariableIndicesMap();

    /**
     * m_bIndexMapGenerated is set to true if getVariableIndicesMap() has been called
     */
    bool m_bIndexMapGenerated;

    /**
     * is true if an AD Expression Tree has an expression that can change depending on
     * the value of the input, e.g. an if statement -- false by default
     */
    bool bADMustReTape;

    /**
     * m_bDestroyNlNodes is true if the destructor deletes the nodes in the Expression tree
     */
    bool bDestroyNlNodes;

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(OSExpressionTree *that);
};//end OSExpressionTree


/*! \class ScalarExpressionTree
 *  \brief Used to hold part of the instance in memory.
 *
 * \remarks
 * This class stores the OSiL instance in memory as
 * an expression tree.
 *
 */

class ScalarExpressionTree : public OSExpressionTree
{
public:

    /**
     * m_treeRoot holds the root node (of OSnLNode type) of the expression tree.
     */
    OSnLNode *m_treeRoot;

    /**
     * default constructor.
     */
    ScalarExpressionTree();

    /**
     * default destructor.
     */
    ~ScalarExpressionTree();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(ScalarExpressionTree *that);

    /**
     * Get a vector of pointers to ExprNodes that correspond to
     * a scalar-valued OSExpressionTree in prefix format
     *
     * </p>
     *
     * @return the expression tree as a vector of ExprNodes in prefix.
     */
    std::vector<ExprNode*> getPrefixFromExpressionTree();

    /**
     * Get a vector of pointers to ExprNodes that correspond to
     * a scalar-valued OSExpressionTree in postfix format
     *
     * </p>
     *
     * @return the expression tree as a vector of ExprNodes in postfix.
     */
    std::vector<ExprNode*> getPostfixFromExpressionTree();

    /**
     * Retrieve a map of the indices of the variables
     * that are in the expression tree
     *
     * </p>
     *
     * @return a map of the variables in the current expression tree.
     */
    std::map<int, int> *getVariableIndicesMap();

#if 0
    /**
     * m_bIndexMapGenerated is set to true if getVariableIndicesMap() has been called
     */
    bool m_bIndexMapGenerated;

    /**
     * is true if an AD Expression Tree has an expression that can change depending on
     * the value of the input, e.g. an if statement -- false by default
     */
    bool bADMustReTape;

    /**
     * m_bDestroyNlNodes is true if the destructor deletes the OSnLNodes in the Expression tree
     */
    bool bDestroyNlNodes;
#endif

    /**
     * Calculate the expression tree function value given the current variable
     * values using the calculateFunction method of OSnLNode.
     * If the function has been calculated, the method will retrieve it.
     *
     * </p>
     *
     * @param x holds the values of the variables in a double array.
     * @param new_x is false if any evaluation method was previously called for the current x
     * @return the expression tree function value given the current variable values.
     */
    double calculateFunction( double *x, bool new_x);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */    
    //bool deepCopyFrom(ScalarExpressionTree *that);

private:
    /**
     * m_dTreeRootValue is the function value of the root node
     */
    double m_dTreeRootValue;
};//end ScalarExpressionTree


/*! \class MatrixExpressionTree
 *  \brief Used to hold the instance in memory.
 *
 * \remarks
 * This class stores a matrix-valued linear or nonlinear expression
 * in memory as an expression tree.
 *
 */

class MatrixExpressionTree : public OSExpressionTree
{
public:
    /**
     * m_treeRoot holds the root node (of OSnLMNode type) of the expression tree.
     */
    OSnLMNode *m_treeRoot;

    /**
     * default constructor.
     */
    MatrixExpressionTree();

    /**
     * default destructor.
     */
    ~MatrixExpressionTree();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixExpressionTree *that);

    /**
     * Get a vector of pointers to ExprNodes that correspond to
     * a scalar-valued OSExpressionTree in prefix format
     *
     * </p>
     *
     * @return the expression tree as a vector of ExprNodes in prefix.
     */
    std::vector<ExprNode*> getPrefixFromExpressionTree();

    /**
     * Get a vector of pointers to ExprNodes that correspond to
     * a scalar-valued OSExpressionTree in postfix format
     *
     * </p>
     *
     * @return the expression tree as a vector of ExprNodes in postfix.
     */
    std::vector<ExprNode*> getPostfixFromExpressionTree();

    /**
     * Retrieve a map of the indices of the variables
     * that are in the expression tree
     *
     * </p>
     *
     * @return a map of the variables in the current expression tree.
     */
    //std::map<int, int> *getVariableIndicesMap();

#if 0
    /**
     * m_bIndexMapGenerated is set to true if getVariableIndicesMap() has been called
     */
    bool m_bIndexMapGenerated;

    /**
     * is true if an AD Expression Tree has an expression that can change depending on
     * the value of the input, e.g. an if statement -- false by default
     */
    bool bADMustReTape;

    /**
     * m_bDestroyNlNodes is true if the destructor deletes the OSnLNodes in the Expression tree
     */
    bool bDestroyNlNodes;

    /**
     * Calculate the expression tree function value given the current variable
     * values using the calculateFunction method of OSnLNode.
     * If the function has been calculated, the method will retrieve it.
     *
     * </p>
     *
     * @param x holds the values of the variables in a double array.
     * @param new_x is false if any evaluation method was previously called for the current x
     * @return the expression tree function value given the current variable values.
     */
    double calculateFunction( double *x, bool new_x);
#endif

private:

    /**
     * m_dTreeRootValue is the function value of the root node
     */
    //double m_dTreeRootValue;
};//end MatrixExpressionTree

#endif

