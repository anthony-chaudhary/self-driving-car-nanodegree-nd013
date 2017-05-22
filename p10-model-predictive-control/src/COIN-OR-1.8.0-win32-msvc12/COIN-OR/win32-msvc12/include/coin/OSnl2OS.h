/* $Id: OSnl2OS.h 4249 2011-08-11 01:08:14Z Gassmann $ */
/** @file OSnl2osol.h
 * 
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 */
 

#ifndef OSNL2OS_H
#define OSNL2OS_H


#include "OSoLReader.h"
#include "OSOption.h"
#include "OSInstance.h"
#include "OSnLNode.h"
#include "OSMathUtil.h"
#include <string>
#include <vector>


/*! \class OSnl2OS
 *  \brief The OSnl2OS  Class.
 * 
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 * 
 * \remarks
 * The OSnl2OS class is used for reading an AMPL nl file, 
 * extracting from it the instance along with any options indexed over variables, 
 * constraints, objectives. The latter may, if needed, be merged with the contents
 * of an OSoL file. The processed information is stored into an OSInstance object 
 * and either an OSOption object (if not NULL) or an OSoL string.
 * 
 */ 

struct ograd;
struct cgrad;
struct ASL;
struct expr;

enum OS_AMPL_SUFFIX_TYPE
{
    OS_AMPL_SUFFIX_TYPE_integer,
    OS_AMPL_SUFFIX_TYPE_double
};

enum OS_AMPL_SUFFIX_SCOPE
{
    OS_AMPL_SUFFIX_SCOPE_variables,
    OS_AMPL_SUFFIX_SCOPE_constraints,
    OS_AMPL_SUFFIX_SCOPE_objectives,
    OS_AMPL_SUFFIX_SCOPE_problems
};

enum OS_AMPL_SUFFIX_DIRECTION
{
    OS_AMPL_SUFFIX_DIRECTION_toSolver,
    OS_AMPL_SUFFIX_DIRECTION_toAMPL,
    OS_AMPL_SUFFIX_DIRECTION_both,
    OS_AMPL_SUFFIX_DIRECTION_local
};

struct OS_AMPL_SUFFIX
{
    std::string name;
    OS_AMPL_SUFFIX_TYPE type;
    OS_AMPL_SUFFIX_SCOPE scope;
    OS_AMPL_SUFFIX_DIRECTION direction;
};

class OSnl2OS
{
public:
    /** the OSnl2OS class constructor */
    //OSnl2OS( std::string nlfilename, std::string osol);
    OSnl2OS();

    /** alternate constructor which does not allocate the ASL structs 
     *
     *  &param cw  a pointer to a (previously allocated) struct used for column-wise representation 
     *  &param rw  a pointer to a (previously allocated) struct used for row-wise representation
     *  &param asl an extra pointer that can be used to switch between rw and cw
     */
    OSnl2OS(ASL *cw, ASL *rw, ASL *asl);
 
    /** the OSnl2OS class destructor */
    ~OSnl2OS();
    
    /**
     * return a pointer to an ASL object
     * @param name carries the name of the ASL object
     * (there are three of them: asl, rw, cw)
     * @return the pointer to the object named
     */
    ASL* getASL(std::string name);

    /**
     * read the nl file
     * @param stub is the (relevant part of the) file name
     * @return whether the read was successful
     */
    bool readNl(std::string stub);

    /** set the osol string */
    void setOsol(std::string osol);

    /** set the job ID */
    void setJobID(std::string jobID);

    /**
     * set the pointers to the three ASL objects
     * @param asl carries a pointer to the object named "asl"
     * @param rw  carries a pointer to the object named "rw"
     * @param cw  carries a pointer to the object named "cw"
     * (asl should point to the same location as either rw or cw)
     * @return whether the operation was successful
     */
    bool setASL(ASL *asl, ASL *rw, ASL *cw);

    /**
     * create an OSInstance and OSOption representation from the AMPL nl content 
     * (Some of the information in the nl file --- such as initial values, 
     * basis information, branching priorities, etc. --- cannot be stored 
     * into an OSInstance and must be stored in an OSOption object instead.)
     *
     * @return whether the objects were created successfully.
     */
    bool createOSObjects();

    /**
     * store a number of variables into an OSInstance object 
     * @param osinstance: a pointer to the OSInstance object
     * @param lower: index of the first variable to be set in this call
     * @param upper: set all variables from lower...upper-1
     * @param vartype: the type of the variable (in AMPL this is 'C', 'B' or 'I')
     */
    void setVar(OSInstance *osinstance, int lower, int upper, char vartype);

    /**
     * special version of the previous method because AMPL makes no distinction
     * between integer and binary variables that occur in nonlinear expressions.
     * The actual type ('B' or 'I') must be inferred from the variable bounds. 
     * @param osinstance: a pointer to the OSInstance object
     * @param lower: index of the first variable to be set in this call
     * @param upper: set all variables from lower...upper-1
     */
    void setIBVar(OSInstance *osinstance, int lower, int upper);

    /**
     * we may need to parse an OSoL file if there is suffix information 
     * indicated in the AMPL nl content
     */
    OSoLReader *osolreader;

    /**
     * parse an nl tree structure holding a nonlinear expression
     *
     * @return the AMPL nonlinear structure as an OSnLNode.
     */
    OSnLNode* walkTree(expr *e);

    /** osinstance is a pointer to the OSInstance object that gets created
     *  from the information in the nl file
     */
    OSInstance *osinstance;
    
    /** osoption is a pointer to the OSOption object that gets created
     *  from the information in the nl file (and the OSoL string if present)
     */
    OSOption *osoption;

    /** osol is a string containing the content of the OS option file
     *  (it may be empty if no option file was provided). 
     *  If osoption is NULL, the option information is found in osol.
     */
    std::string osol;

    /** jobID is a string containing a jobID that may have been supplied
     *  on the command line (it may be empty). 
     *  If osoption is not NULL, the jobID has been duplicated to osoption.
     */
    std::string jobID;
    
    std::vector<std::string> op_type;
    std::vector<double> operand;
    int numkount;

private:
    
    /** og is a pointer to the AMPL data structure holding the
     * objective function coefficients
     */
    ograd *og;

    /** Pointers to AMPL data structures.
     * cw is loaded in column-wise format.
     * rw is loaded in row-wise format.
     * asl is for conveniently switching.
     */
    ASL *cw, *rw, *asl;

    /** stub is the name of the file with the nl instance
     */
    std::string stub;

};  //end of  OSnl2OS

#endif
