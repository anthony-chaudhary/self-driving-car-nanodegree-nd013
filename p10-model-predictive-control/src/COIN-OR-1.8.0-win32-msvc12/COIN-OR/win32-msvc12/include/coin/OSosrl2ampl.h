/* $Id: OSosrl2ampl.h 4249 2011-08-11 01:08:14Z Gassmann $ */
/** @file OSosrl2ampl.h
 * 
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2012, Horand Gassmann, Jun Ma, Kipp Martin,
 * and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License. 
 * Please see the accompanying LICENSE file in root directory for terms.
 * 
 */
 

#ifndef OSRL2AMPL_H
#define OSRL2AMPL_H



#include "OSResult.h"
#include "OSMathUtil.h"
#include <string>
#include <vector>


/*! \class OSosrl2ampl
 *  \brief The OSosrl2ampl Class.
 * 
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 * 
 * \remarks
 * The OSosrl2ampl class is used for writing an AMPL sol file, 
 * including any results indexed over variables, constraints, objectives. 
 * 
 */ 

struct ograd;
struct cgrad;
struct ASL;
struct expr;


class OSosrl2ampl
{
public:
    /** the OSosrl2ampl class constructor */
    OSosrl2ampl();
 
    /** the OSosrl2ampl class destructor */
    ~OSosrl2ampl();
    
    /**
     * Convert the solution to AMPL .sol format 
     *
     * @param osrl is a string containing the result information
     * @param  asl is a pointer to an ASL data structure
     * @param filename is the name of the output file
     * (e.g., as returned from the solver). 
     * @return whether the .sol file was created successfully. 
     */
    bool writeSolFile(std::string osrl, ASL *asl, std::string filename); 

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
    ASL *asl, *cw, *rw;

    /** stub is the name of the file with the nl instance
     */
    std::string stub;

};  //end of OSosrl2ampl

#endif
