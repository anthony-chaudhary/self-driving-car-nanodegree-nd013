/* $Id: OSMathUtil.h 4996 2015-04-14 20:38:45Z Gassmann $ */
/** @file MathUtil.h
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
 * <p>The <code>MathUtil</code> class contains methods for performing
 * mathematics related operations used by many classes in the
 * Optimization Services (OS) framework. </p>
 *
 */


#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "OSParameters.h"
#include "OSdtoa.h"
#include "OSErrorClass.h"
#include "OSGeneral.h"


#include <string>

#ifdef __cplusplus
extern std::string os_dtoa_format(double  x);
#endif

#ifdef __cplusplus
extern "C" {
#endif

    double os_strtod_wrap(const char *str,   char **strEnd);

#ifdef __cplusplus
}
#endif

/*!  \class MathUtil
 *  \brief this class has routines for linear algebra.
 *
 * @author Robert Fourer, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * This class will hold linear algebra routines used by other
 * OS classes. Right now it has a routine to change the column/row
 * storage of a sparse matrix
 */
class MathUtil
{
public:

    /** the class constructor */
    MathUtil();

    /** the class destructor */
    ~MathUtil();

    /**
     * Round a double number to the precision specified.
     *
     * @param X holds the number to be rounded.
     * @param precision holds the number of digit after (or before if negative) the decimal point.
     * @return the rounded number.
     */
    /*public static double round (double x, int precision){
    	double mask = 0.0, y, result;
    	try{
    		mask = Math.pow (10.0, -(double)precision);
    	}
    	catch(ArithmeticException e){
    		return x;
    	}
    	y  = mod(x, mask);
    	result  = x - y;
    	if (y / mask >= 0.5) result += mask;
    	return result;
    }//round
    */
    /**
     * Calculation of x mod y.
     *
     * @param x holds the number before the mod operator.
     * @param x holds the number after the mod operator.
     * @return the result of x mod y.
     */
    /*public static double mod (double x, double y){
    	return  x - Math.floor(x / y) * y;
    }//mod
    */

    /**
     *
     * @param isColumnMajor holds whether the coefMatrix (AMatrix) holding linear program
     * data is stored by column. If false, the matrix is stored by row.
     * @param startSize holds the size of the start array
     * @param valueSize holds the size of the index and value arrays
     * @param start holds an integer array of start elements in coefMatrix (AMatrix),
     * which points to the start of a column (row) of nonzero elements in coefMatrix (AMatrix).
     * @param index holds an integer array of rowIdx (or colIdx) elements in coefMatrix (AMatrix).
     * If the matrix is stored by column (row), rowIdx (colIdx) is the array of row (column) indices.
     * @param value holds a double array of value elements in coefMatrix (AMatrix),
     * which contains nonzero elements.
     * @param dimension holds the column count if the input matrix is row major (row count = start.length-1)
     * or the row number if the input matrix is column major (column count = start.length -1)
     * @return Linear constraint coefficient matrix in the other major of the input matrix. Return null if input matrix not valid.
     */
    static SparseMatrix* convertLinearConstraintCoefficientMatrixToTheOtherMajor(
        bool isColumnMajor, int startSize, int valueSize, int* start, int* index,
        double* value, int dimension);

    /**
     *
     * @param x is the double that gets converted into a string
     * this takes the David Gay dtoa and converts to a formatted string
     */
    std::string format_os_dtoa( double x);


    /**
     *
     * @param str is the char* string that gets converted to double
     * this method actually wraps around os_strtod (which is really the
     * David Gay version of strtod) and will throw an exception
     * if the str contains text or is in anyway not a valid number
     * str should be null terminated
     */
    //double os_strtod_wrap(const char *str) throw(ErrorClass);

    /**
     *
     * @param str is the char* string that gets converted to double
     * @param strEnd should point to the end of str
     * this method actually wraps around os_strtod (which is really the
     * David Gay version of strtod) and will throw an exception
     * if the str contains text or is in anyway not a valid number
     */
    //double os_strtod_wrap(const char *str,  const char *strEnd) throw(ErrorClass);



};//class MathUtil


/**
 * getMultIncr
 *
 * Identify the next run in an integer array
 *
 * @param i holds a pointer to the array to be processed.
 * @param mult holds the length of the run. This parameter is passed by reference
 * @param incr holds the increment. This parameter is also passed by reference
 * @param size holds the number of elements in the array. This parameter is passed by value
 * @param defaultIncr holds the default value for incr from the schema file. Using just <el mult="..."
 * saves space whenever a run of two or more elements has been encountered, whereas <el mult="..." incr="..."
 * saves space only for runs of three or more elements. Thus the defaultIncr must be treated specially
 * (and it might change from one schema element to the next).
 *
 */
inline void getMultIncr(int* i, int *mult, int *incr, int size, int defaultIncr)
{
    int k;

    *mult = 1;
    *incr = defaultIncr;

    if (size == 1) return;

    for (k=1; (k < size) && (i[k] - i[k-1] == defaultIncr); k++)
    {
        (*mult)++;
    }
    if (*mult > 1 || size == 2) return;

    *incr = i[1] - i[0];
    if (i[2] - i[1] != *incr) return;

    *mult = 3;
    for (k=3; (k < size) && (i[k] - i[k-1] == *incr); k++)
    {
        (*mult)++;
    }
    return;
}


/**
 * getMultIncr
 *
 * Identify the next run in an array of type double.
 *
 * @param i holds a pointer to the array to be processed.
 * @param mult holds the length of the run. This parameter is passed by reference
 * @param incr holds the increment. This parameter is also passed by reference
 * @param size holds the number of elements in the array. This parameter is passed by value
 *
 */
inline void getMultIncr(double* a, int *mult, double *incr, int size)
{
    double mark;
    int k;

    *mult = 1;
    *incr = 0;

    if (size == 1) return;

    mark = a[0];
    for (k=1; (k < size) && (a[k] == mark); k++)
    {
        (*mult)++;
    }
    if (*mult > 1 || size == 2) return;

    *incr = a[1] - a[0];
    if (a[2] - a[1] != *incr) return;

    *mult = 3;
    for (k=3; (k < size) && (a[k] - a[k-1] == *incr); k++)
    {
        (*mult)++;
    }
    return;
}



/**
 * getMult
 *
 * Identify the number of duplicates at the start of an integer array
 *
 * @param i holds a pointer to the array to be processed.
 * @param size holds the number of elements in the array.
 *
 * @return the length of the run.
 */
inline int getMult(int* i, int size)
{
    int mark;

    int mult = 1;

    if (size == 1) return mult;

    mark = i[0];
    for (int k=1; (k < size) && (i[k] == mark); k++)
    {
        mult++;
    }
    return mult;
}



/**
 * getMult
 *
 * Identify the number of duplicates at the start of an array of type double
 *
 * @param i holds a pointer to the array to be processed.
 * @param size holds the number of elements in the array.
 *
 * @return the length of the run.
 */
inline int getMult(double* a, int size)
{
    double mark;

    int mult = 1;

    if (size == 1) return mult;

    mark = a[0];
    for (int k=1; (k < size) && (a[k] == mark); k++)
    {
        mult++;
    }
    return mult;
}


/**
 * OSRand()
 *
 * @return a uniformly distributed random number between 0 and 1 (inclusive)
 * @notes The random number generator used, rand(), is not very good
 * and should be replaced by a serious random number generator for serious work.
 */
double OSRand();


/**
 * OSiRand(int iMin, int iMax)
 *
 * @return a uniformly distributed random integer between iMin and iMax (inclusive)
 * @notes The random number generator used, rand(), is not very good
 * and should be replaced by a serious random number generator for serious work.
 */
double OSiRand(int iMin, int iMax);

#endif
