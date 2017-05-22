/* $Id: OSiLParserData.h 4891 2014-12-03 03:05:50Z Gassmann $ */
/** @file OSiLParserData.h
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin,
 *
 * \remarks
 * Copyright (C) 2005-2014, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */
#ifndef OSILPARSERDATA_H
#define OSILPARSERDATA_H

#include "OSnLNode.h"
#include <vector>

/*! \class OSiLParserData
 *  \brief The OSiLParserData Class, used to store parser data.
 *
 * @author Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * The OSiLParserData class is used to hold the nonlinear
 * part of the problem when an OSiL instance is parsed.
 * We do this so we can have a reentrant parser. We do not
 * have to store the linear part because we do not use flex/bison
 * to parse the linear part of the problem.
 */
class OSiLParserData
{
public:
    /** the OSiLParserData class constructor */
    OSiLParserData( );

    /** the OSiLParserData class destructor */
    ~OSiLParserData() ;

    /** scanner is used to store data in a reentrant lexer
     * we use this to pass an OSiLParserData object to the parser*/
    void* scanner;

    /** if there is a parser error, osillineno holds the line number in the
     * OSiL file where the error occured.
     */
    int osillineno;

    /** These variables are used for processing the <quadraticCoefficients> element */

    /** store the number of quadratic terms */
    int qtermcount;

    /** qtermidxOneattON is true if we have found the first index
     * of the quadratic term */
    bool qtermidxOneattON;

    /** qtermidxTwoattON is true if we have found the second index
     * of the quadratic term */
    bool qtermidxTwoattON;

    /** qtermidxattON is true if we have found the row index of the
     * of a quadratic term */
    bool qtermidxattON;

    /** qtermidattON is true if we have found the id of the quadratic term */
    bool qtermidattON ;

    /** qtermcoefattON is true if we have found the coefficient of the
     * quadratic term
     */
    bool qtermcoefattON;



    /** These variables are used to parse the <timeDomain> element */

    /** store the type of <timeDomain> (extend as needed) */
    bool timeDomainStages;
    bool timeDomainInterval;

    /** store the number of stages */
    int stagecount;

    /** stagenameON is true if the current stage was given a name */
    bool stagenameON;

    /** store the name of the current stage */
    std::string stagename;

    /** for each stage we need to track whether the <variables>, <constraints>,
     *  <objectives> elements are present... */
    bool stageVariablesON;
    bool stageConstraintsON;
    bool stageObjectivesON;

    /**  ...we need to track whether the variables, constraints, objectives
     *   are given in temporal order... */
    bool stageVariablesOrdered;
    bool stageConstraintsOrdered;
    bool stageObjectivesOrdered;

    /**  ...we need to track the first variable, constraint, objective... */
    int stageVariableStartIdx;
    int stageConstraintStartIdx;
    int stageObjectiveStartIdx;

    /** ...and we need to track the number of variables we have seen */
    int stagevarcount;
    int stageconcount;
    int stageobjcount;

    /** these two integers track how many variables and constraints have been assigned to a stage;
     *  this is used for consistency checks. */
    int nvarcovered;
    int nconcovered;

    /**
     * m_miVarStageInfo is an array that for each variable gives the stage to which it belongs.
     */
    int *m_miVarStageInfo;

    /**
     * m_miConStageInfo is an array that for each constraint gives the stage to which it belongs.
     */
    int *m_miConStageInfo;

    /**
     * m_miObjStageInfo is an array that for each objective gives the stage to which it belongs.
     */
    int *m_miObjStageInfo;

    /** intervalhorizonON is true if we have found a horizon
     *  for the time interval */
    bool intervalhorizonON;

    /** intervalhorizon holds the value of the end of the planning horizon */
    double intervalhorizon;

    /** intervalstartON is true if we have found a start time
     *  for the time interval */
    bool intervalstartON;

    /** intervalstart holds the value for the start of the planning horizon */
    double intervalstart;

    /** some elements to hold matrices and cones */
    bool numberOfMatricesPresent; 
    int  numberOfMatrices;
    bool numberOfConesPresent; 
    int  numberOfCones;
    int  coneCounter;
    bool numberOfRowsPresent; 
    int  numberOfRows;
    bool numberOfColumnsPresent; 
    int  numberOfColumns;

    int  numberOfEl;
    int  numberOf;
    int  elCounter;

    bool namePresent;
    std::string name;

    bool referenceMatrixIdxPresent;
    int  referenceMatrixIdx;

    bool normScaleFactorPresent;
    double normScaleFactor;
    bool distortionMatrixPresent;
    int distortionMatrix;
    bool axisDirectionPresent;
    int axisDirection;
    bool firstAxisDirectionPresent;
    int firstAxisDirection;
    bool secondAxisDirectionPresent;
    int secondAxisDirection;
    bool semidefinitenessPresent;
    std::string semidefiniteness;

    /* some elements to hold <matrixProgramming> constructs */
    int numberOfMatrixVar;
    int numberOfMatrixObj;
    int numberOfMatrixCon;
    int numberOfMatrixTerms;
    int numberOfMatrixExpr;

    bool numberOfMatrixTermsPresent;

    bool matrixIdxPresent;
    bool lbMatrixIdxPresent;
    bool lbConeIdxPresent;
    bool ubMatrixIdxPresent;
    bool ubConeIdxPresent;
    bool orderConeIdxPresent;
    bool templateMatrixIdxPresent;
    bool constantMatrixIdxPresent;
    bool varReferenceMatrixIdxPresent;
    bool objReferenceMatrixIdxPresent;
    bool conReferenceMatrixIdxPresent;
    bool varTypePresent;

    int  matrixIdx;
    int  lbMatrixIdx;   
    int  lbConeIdx;
    int  ubMatrixIdx;
    int  ubConeIdx;
    int  orderConeIdx;
    int  templateMatrixIdx;
    int  constantMatrixIdx;
    int  varReferenceMatrixIdx;
    int  objReferenceMatrixIdx;
    int  conReferenceMatrixIdx;
    char varType;

    bool shapePresent;
    std::string shape;

    bool matrixTermInObj;

    int kounter;
    int kount2;
    double tempVal;

    /** if the parser finds invalid text it is held here and we delete
     *  if the file was not valid
     */
    bool ignoreDataAfterErrors;
    bool suppressFurtherErrorMessages;
    char *errorText;

    /** used to accumulate error message so the parser does not die
     *  on the first error encountered
     */
    std::string parser_errors;
};

#endif /*OSILPARSERDATA_H_*/
