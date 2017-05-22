/* $Id: OSgLParserData.h 2698 2009-06-09 04:14:07Z kmartin $ */
/** @file OSgLParserData.h
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */
#ifndef OSGLPARSERDATA_H
#define OSGLPARSERDATA_H

#include "OSGeneral.h"
#include "OSMatrix.h"

#include <stdio.h>
#include <string>



/*! \class OSgLParserData
 *  \brief The OSgLParserData Class.
 *
 * \remarks
 * the OSgLParserData class is used to temporarily
 * hold data found in parsing the OSgL data structures.
 * we do this so we can write reusable code.
 */
class OSgLParserData
{
public:

    /** data structure to process an IntVector and hold the data temporarily */
    int* osglIntArray;
    int  osglCounter;
    int  osglTempint;

    double* osglDblArray;
    int*    osglValArray;

    /** data structure to process a GeneralFileHeader and hold the data temporarily */
    std::string fileName;
    std::string source;
    std::string fileCreator;
    std::string licence;
    bool fileNamePresent;
    bool sourcePresent;
    bool fileCreatorPresent;
    bool licencePresent;

    /** the OSgLParserData class constructor */
    OSgLParserData( );

    //** the OSgLParserData class destructor */
    ~OSgLParserData() ;


    /** scanner is used to store data in a reentrant lexer
     * we use this to pass an OSgLParserData object to the parser
     */
    void* scanner;

    /**  if the parser finds invalid text it is held here and we delete
     * if the file was not valid
     */
    char *errorText;

    /** used to accumulate error message so the parser does not die
     *  on the first error encountered
     */
    std::string parser_errors;

    /** two booleans to govern the behavior after an error has been encountered */
    bool ignoreDataAfterErrors;
    bool suppressFurtherErrorMessages;

    /** We need to hold an array of <matrix> elements temporarily */
    OSMatrix** matrix;

    /** There are also other variants of these ... */
    OSMatrixWithMatrixVarIdx** matrixWithMatrixVarIdx;
    OSMatrixWithMatrixObjIdx** matrixWithMatrixObjIdx;
    OSMatrixWithMatrixConIdx** matrixWithMatrixConIdx;

    /** In order to use synergies in the parser, we store matrixXXXIdx in a separate vector */
    int* matrixVarIndexes;
    int* matrixObjIndexes;
    int* matrixConIndexes;

    /** We also need to keep track locally of the number of matrices */
    int  matrixCounter;

    /** Linear matrices need a counter to count the number of terms within each element */
    int nonzeroCounter;

    /** This matrix constructor is needed in order to properly push the constructor vector */
    MatrixNode* tempC;

    /** Several vectors to process the matrix nodes into the right order */
    std::vector<MatrixNode*> mtxConstructorVec;
    std::vector<MatrixNode*> mtxBlocksVec;
    std::vector<MatrixNode*> mtxBlkVec;
    std::vector<int> nBlocksVec;

    /** 
     *  Vectors to hold rowOffset and colOffset arrays in a place where they
     *  are easily accessible while the <block> children are processed 
     */
    std::vector<int*> rowOffsets;
    std::vector<int*> colOffsets;

    /** Data elements for parsing number-valued attributes and elements */

    bool numberOfBlocksPresent;
    bool numberOfColumnsPresent;
    bool numberOfConPresent;
    bool numberOfConIdxPresent;
    bool numberOfConstraintsPresent;
    bool numberOfElPresent;
    bool numberOfEnumerationsPresent;
    bool numberOfItemsPresent;
    bool numberOfMatricesPresent;
    bool numberOfMatrixConPresent;
    bool numberOfMatrixObjPresent;
    bool numberOfMatrixVarPresent;
    bool numberOfObjPresent;
    bool numberOfObjIdxPresent;
    bool numberOfObjectivesPresent;
    bool numberOfRowsPresent;
    bool numberOfValuesPresent;
    bool numberOfVarPresent;
    bool numberOfVarIdxPresent;
    bool numberOfVariablesPresent;

    bool base64SizePresent;
    bool baseMatrixIdxPresent;
    bool baseMatrixStartRowPresent;
    bool baseMatrixStartColPresent;
    bool baseMatrixEndRowPresent;
    bool baseMatrixEndColPresent;
    bool blockRowIdxPresent;
    bool blockColIdxPresent;
    bool coefPresent;
    bool constantPresent;
    bool idxPresent;
    bool incrPresent;
    bool matrixConIdxPresent;
    bool matrixObjIdxPresent;
    bool matrixVarIdxPresent;
    bool multPresent;
    bool scalarMultiplierPresent;
    bool targetMatrixFirstRowPresent;
    bool targetMatrixFirstColPresent;

    int numberOfBlocks;
    int numberOfColumns;
    int numberOfCon;
    int numberOfConIdx;
    int numberOfConstraints;
    int numberOfEl;
    int numberOfEnumerations;
    int numberOfItems;
    int numberOfMatrices;
    int numberOfMatrixCon;
    int numberOfMatrixObj;
    int numberOfMatrixVar;
    int numberOfObj;
    int numberOfObjIdx;
    int numberOfObjectives;
    int numberOfRows;
    int numberOfValues;
    int numberOfVar;
    int numberOfVarIdx;
    int numberOfVariables;

    int base64Size;
    int baseMatrixIdx;
    int baseMatrixStartRow;
    int baseMatrixStartCol;
    int baseMatrixEndRow;
    int baseMatrixEndCol;
    int blockRowIdx;
    int blockColIdx;
    double coef;
    double constant;
    int idx;
    int incr;
    int matrixConIdx;
    int matrixObjIdx;
    int matrixVarIdx;
    int mult;
    double scalarMultiplier;
    int targetMatrixFirstRow;
    int targetMatrixFirstCol;

    /** Data elements for parsing string-valued attributes and text elements */

    bool baseTransposePresent;
    bool categoryPresent;
    bool conTypePresent;
    bool descriptionPresent;
    bool enumTypePresent;
    bool matrixConTypePresent;
    bool matrixNamePresent;
    bool matrixObjTypePresent;
    bool matrixTypePresent;
    bool matrixVarTypePresent;
    bool namePresent;
    bool objTypePresent;
    bool rowMajorPresent;
    bool shapePresent;
    bool solverPresent;
    bool symmetryPresent;
    bool typePresent;
    bool unitPresent;
    bool valuePresent;
    bool valueTypePresent;
    bool varTypePresent;

    bool baseTranspose;
    std::string category;
    std::string conType;
    std::string description;
    std::string enumType;
    std::string matrixConType; 
    std::string matrixName;
    std::string matrixObjType;
    std::string matrixType;
    std::string matrixVarType;
    std::string name;
    std::string objType;
    bool rowMajor;
    std::string shape;
    std::string solver;
    std::string symmetry;
    std::string type;
    std::string unit;
    std::string value;
    std::string valueType;
    std::string varType; 
};//OSgLParserData

inline void osgl_empty_vectors( OSgLParserData* osglData)
{
    if (osglData->osglIntArray != NULL)
        delete[] osglData->osglIntArray;
    osglData->osglIntArray = NULL;
}//end osgl_empty_vectors


#endif /*OSGLPARSERDATA_H_*/
