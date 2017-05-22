/* $Id: OSoLParserData.h 4950 2015-02-06 21:15:56Z tkr $ */
/** @file OSoLParserData.h
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin,
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */
#ifndef OSOLPARSERDATA_H
#define OSOLPARSERDATA_H

#include "OSnLNode.h"
#include <vector>


/*! \class OSoLParserData
 *  \brief The OSoLParserData  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 08/29/2008
 * @since OS 1.1
 *
 * \remarks
 * the OSoLParserData class is used to temporarily
 * hold data found in parsing the OSoL instance
 * we do this so we can have a reentrant parser.
 */
class OSoLParserData
{
public:
    /** track which child elements are present */
    bool osolgeneralPresent;
    bool osolsystemPresent;
    bool osolservicePresent;
    bool osoljobPresent;
    bool osoloptimizationPresent;


    /** children of the <general> element */
    bool serviceURIPresent;
    bool serviceNamePresent;
    bool instanceNamePresent;
    bool instanceLocationPresent;
    bool instanceLocationTypeattON;
    bool jobIDPresent;
    bool solverToInvokePresent;
    bool licensePresent;
    bool usernamePresent;
    bool passwordPresent;
    bool contactPresent;
    bool transportTypeattON;
    bool otherGeneralOptionsPresent;
    int  numberOfOtherGeneralOptions;


    /** children of the <system> element */
    bool minDiskSpacePresent;
    bool minDiskSpaceUnitPresent;

    bool minMemoryPresent;
    bool minMemoryUnitPresent;

    bool minCPUSpeedPresent;
    bool minCPUSpeedUnitPresent;

    bool minCPUNumberPresent;

    bool otherSystemOptionsPresent;
    int  numberOfOtherSystemOptions;


    /** children of the <service> element */
    bool serviceTypePresent;

    bool otherServiceOptionsPresent;
    int  numberOfOtherServiceOptions;


    /** children of the <job> element */
    bool maxTimePresent;
    std::string maxTimeUnit;
    bool maxTimeUnitPresent;
    double maxTimeValue;

    bool requestedStartTimePresent;
    std::string requestedStartTime;

    bool dependenciesPresent;
    int  numberOfDependencies;

    bool requiredDirectoriesPresent;
    int  numberOfRequiredDirectories;

    bool requiredFilesPresent;
    int  numberOfRequiredFiles;

    bool directoriesToMakePresent;
    int  numberOfDirectoriesToMake;

    bool filesToMakePresent;
    int  numberOfFilesToMake;

    std::string pathPairFrom;
    std::string pathPairTo;

    bool pathPairFromPresent;
    bool pathPairToPresent;
    bool pathPairMakeCopyPresent;
    bool pathPairMakeCopy;

    bool inputDirectoriesToMovePresent;
    int  numberOfInputDirectoriesToMove;

    bool inputFilesToMovePresent;
    int  numberOfInputFilesToMove;

    bool outputDirectoriesToMovePresent;
    int  numberOfOutputDirectoriesToMove;

    bool outputFilesToMovePresent;
    int  numberOfOutputFilesToMove;

    bool directoriesToDeletePresent;
    int  numberOfDirectoriesToDelete;

    bool filesToDeletePresent;
    int  numberOfFilesToDelete;

    bool processesToKillPresent;
    int  numberOfProcessesToKill;

    bool otherJobOptionsPresent;
    int  numberOfOtherJobOptions;

    int  numberOfPathPairs;

    /** children of the <optimization> element */
    int  numberOfVariables;
    bool numberOfVariablesPresent;
    int  numberOfObjectives;
    bool numberOfObjectivesPresent;
    int  numberOfConstraints;
    bool numberOfConstraintsPresent;
    bool variablesPresent;
    bool objectivesPresent;
    bool constraintsPresent;
    bool solverOptionsPresent;
    bool idxAttributePresent;
    bool valAttributePresent;
    bool lbValAttributePresent;
    bool ubValAttributePresent;
    int  numberOfOtherVariableOptions;
    bool initialVariableValuesPresent;
    int  numberOfVar;
    bool initialVariableValuesStringPresent;
    int  numberOfVarStr;
    bool initialBasisStatusPresent;
    int  numberOfBasVar;
    bool sosIdxAttributePresent;
    bool groupWeightAttributePresent;
    bool numberOfVarAttributePresent;
    bool numberOfObjAttributePresent;
    bool numberOfConAttributePresent;
    bool numberOfEnumerationsAttributePresent;
    int  numberOfIntWt;
    int  numberOfSOS;
    int  currentSOS;
    int  sosIdx;
    int  numberOfSOSVar;
    int  numberOfObj;
    int  numberOfOtherObjectiveOptions;
    bool initialObjectiveValuesPresent;
    int  numberOfObjValues;
    bool initialObjectiveBoundsPresent;
    int  numberOfObjBounds;
    int  numberOfCon;
    int  numberOfOtherConstraintOptions;
    bool initialConstraintValuesPresent;
    bool initialDualVariableValuesPresent;
    int  numberOfDuals;
    int  numberOfSolverOptions;
    double groupWeight;
    double lbDualValue;
    double ubDualValue;
    int numberOfEnumerations;
    int otherOptionType;

    /** attributes of <other> options */
    bool otherOptionNumberPresent;
    bool otherOptionNamePresent;
    bool otherOptionValuePresent;
    bool otherOptionSolverPresent;
    bool otherOptionCategoryPresent;
    bool otherOptionTypePresent;
    bool otherOptionDescriptionPresent;
    bool numberOfItemsPresent;
    int  numberOfItems;

    /** attributes of <solverOptions> element */
    bool solverOptionNamePresent;
    bool solverOptionValuePresent;
    bool solverOptionSolverPresent;
    bool solverOptionCategoryPresent;
    bool solverOptionTypePresent;
    bool solverOptionDescriptionPresent;
    std::string itemContent;


    /** generic attributes */
    bool categoryAttributePresent;
    std::string categoryAttribute;
    bool typeAttributePresent;
    std::string typeAttribute;
    bool varTypeAttributePresent;
    std::string varTypeAttribute;
    bool objTypeAttributePresent;
    std::string objTypeAttribute;
    bool conTypeAttributePresent;
    std::string conTypeAttribute;
    bool enumTypeAttributePresent;
    std::string enumTypeAttribute;
    bool nameAttributePresent;
    std::string nameAttribute;
    bool valueAttributePresent;
    std::string valueAttribute;
    bool lbValueAttributePresent;
    std::string lbValueAttribute;
    bool ubValueAttributePresent;
    std::string ubValueAttribute;
    bool descriptionAttributePresent;
    std::string descriptionAttribute;
    bool solverAttributePresent;
    std::string solverAttribute;
    bool unitAttributePresent;
    std::string unitAttribute;
    int  idxAttribute;


    /** all arrays are collected here */
    std::string* jobDependencies;
    std::string* paths;
    std::string* fromPaths;
    std::string* toPaths;
    std::string* processesToKill;

    std::string* valueString;
    std::string* lbValueString;
    std::string* ubValueString;
    std::string* itemList;

    bool* makeCopy;
    int*  idxArray;

    double* valArray;
    double* lbValArray;
    double* ubValArray;

    std::string* namArray;

    /** some temporary items to facilitate code sharing */
    int tempInt;
    int numberOf;
    int kounter;
    int iOther;
    int iOption;
    double tempVal;
    std::string tempStr;

    /** the OSoLParserData class constructor */
    OSoLParserData( );

    /** the OSoLParserData class destructor */
    ~OSoLParserData() ;

    /**  the status type of the result */
    std::string statusType;

    /** the status Description of the solution */
    std::string statusDescription;

    /** scanner is used to store data in a reentrant lexer
     *  we use this to pass an OSoLParserData object to the parser
     */
    void* scanner;

    /** if the parser finds invalid text it is held here and we delete
     *  if the file was not valid
     */
    char *errorText;

    /** used to accumulate error message so the parser does not die
     *  on the first error encountered
     */
    std::string parser_errors;

    /** two booleans to govern the behavior after an error has been encountered */
    bool ignoreDataAfterErrors;
    bool suppressFurtherErrorMessages; 
};//OSoLParserData

#endif /*OSOLPARSERDATA_H_*/
