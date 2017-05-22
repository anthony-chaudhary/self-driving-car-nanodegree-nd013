/* $Id: OSParameters.h 5018 2015-05-24 22:42:50Z Gassmann $ */
/** @file OSParameters.h
 *
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2015, Horand Gassmann, Jun Ma, Kipp Martin,
 * Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

//kipp fix up the infinity issue
//kipp define OSINFINITY to COIN_DBL_MAX

#ifndef OSPARAMETERS
#define OSPARAMETERS

#include "OSConfig.h"

#include <string>
#include <limits>

#define OS_PLUS                          1001
#define OS_SUM                           1002
#define OS_MINUS                         1003
#define OS_NEGATE                        1004
#define OS_TIMES                         1005
#define OS_DIVIDE                        1006
#define OS_POWER                         1009
#define OS_PRODUCT                       1010
#define OS_ABS                           2001
#define OS_SQUARE                        2005
#define OS_SQRT                          2006
#define OS_LN                            2007
#define OS_EXP                           2010
#define OS_ERF                           2023
#define OS_SIN                           3001
#define OS_COS                           3002
#define OS_MIN                           4010
#define OS_MAX                           4011
#define OS_NUMBER                        5001
#define OS_PI                            5003
#define OS_E                             5004
#define OS_VARIABLE                      6001
#define OS_IF                            7001
#define OS_ALLDIFF                       7016

#define OS_MATRIX_DETERMINANT            8001
#define OS_MATRIX_TRACE                  8002
#define OS_MATRIX_TO_SCALAR              8003


#define OS_MATRIX_PLUS                   8501
#define OS_MATRIX_SUM                    8502
#define OS_MATRIX_MINUS                  8503
#define OS_MATRIX_NEGATE                 8504
#define OS_MATRIX_TIMES                  8505
#define OS_MATRIX_PRODUCT                8506
#define OS_MATRIX_INVERSE                8510
#define OS_MATRIX_TRANSPOSE              8515
#define OS_MATRIX_SCALARTIMES            8518
#define OS_MATRIX_DOTTIMES               8520
#define OS_IDENTITY_MATRIX               8536
#define OS_MATRIX_LOWERTRIANGLE          8537
#define OS_MATRIX_UPPERTRIANGLE          8538
#define OS_MATRIX_DIAGONAL               8539
#define OS_DIAGONAL_MATRIX_FROM_VECTOR   8540
#define OS_MATRIX_REFERENCE              8541
#define OS_MATRIX_SUBMATRIX_AT           8544
#define OS_MATRIX_VAR                    8601
#define OS_MATRIX_OBJ                    8602
#define OS_MATRIX_CON                    8603



#define OS_E_VALUE exp(1.0)
#define OS_PI_VALUE 2*asin(1.0)

#define OS_SCHEMA_VERSION "2.0"

/**
 * we use OS_NEAR_EQUAL in unitTest to see if we
 * are close to the optimal obj value
 */
#define OS_NEAR_EQUAL 1e-2

#define OS_EPS 1e-12

const double OSDBL_MAX = std::numeric_limits<double>::max();
const int    OSINT_MAX = std::numeric_limits<int>::max();

/** checks whether a given double is NaN */
extern "C" bool OSIsnan(double x);
/** returns the value for NaN used in OS */
extern "C" double OSNaN();

std::string OSgetVersionInfo();

/**
 *  Enumeration for the different verbosity levels that can be used in producing output.
 *  The last three levels are used only in debug mode.
 */
enum ENUM_OUTPUT_LEVEL
{
     ENUM_OUTPUT_LEVEL_always = 0,
     ENUM_OUTPUT_LEVEL_error,
     ENUM_OUTPUT_LEVEL_summary,
     ENUM_OUTPUT_LEVEL_warning,
     ENUM_OUTPUT_LEVEL_info,
     ENUM_OUTPUT_LEVEL_debug,
     ENUM_OUTPUT_LEVEL_trace,
     ENUM_OUTPUT_LEVEL_detailed_trace,

     ENUM_OUTPUT_LEVEL_NUMBER_OF_LEVELS    // insert other values above this one...
};

#define DEFAULT_OUTPUT_LEVEL ENUM_OUTPUT_LEVEL_error

/**
 *  Enumeration for the different areas that can produce output.
 *  The last entry ENUM_OUTPUT_AREA_NUMBER_OF_AREAS
 *  gives a convenient way to count them and to allocate space
 */
enum ENUM_OUTPUT_AREA
{
     ENUM_OUTPUT_AREA_main = 1,
     ENUM_OUTPUT_AREA_OSAgent,
     ENUM_OUTPUT_AREA_Command_line_parser,
     ENUM_OUTPUT_AREA_OSiL_parser,
     ENUM_OUTPUT_AREA_OSoL_parser,
     ENUM_OUTPUT_AREA_OSrL_parser,
     ENUM_OUTPUT_AREA_OSGeneral,
     ENUM_OUTPUT_AREA_OSInstance,
     ENUM_OUTPUT_AREA_OSOption,
     ENUM_OUTPUT_AREA_OSResult,
     ENUM_OUTPUT_AREA_OSMatrix,
     ENUM_OUTPUT_AREA_OSiLwriter,
     ENUM_OUTPUT_AREA_OSoLwriter,
     ENUM_OUTPUT_AREA_OSrLwriter,
     ENUM_OUTPUT_AREA_OSModelInterfaces,
     ENUM_OUTPUT_AREA_OSSolverInterfaces,
     ENUM_OUTPUT_AREA_OSUtils,

     ENUM_OUTPUT_AREA_NUMBER_OF_AREAS    // insert other values above this one...
};

/****************************************************************************************
 * In the OSxL schemas there is a growing number of string elements that can only take  *
 * a small number of values. In C++ this can be implemented using enumerations, but the *
 * elements appear in the schemas as strings, so they must be stored internally         *
 * as strings also. Below we list the enumerations and provide two utility methods      *
 * that make working with them convenient:                                              *
 * return...() functions that return the integer value associated with that enumeration *
 * verify...() functions that check that a string has a legal value for the enumeration *
 ****************************************************************************************/

enum ENUM_CPUSPEEDUNIT
{
    ENUM_CPUSPEEDUNIT_hertz = 1,
    ENUM_CPUSPEEDUNIT_kilohertz,
    ENUM_CPUSPEEDUNIT_megahertz,
    ENUM_CPUSPEEDUNIT_gigahertz,
    ENUM_CPUSPEEDUNIT_terahertz,
    ENUM_CPUSPEEDUNIT_flops,
    ENUM_CPUSPEEDUNIT_kiloflops,
    ENUM_CPUSPEEDUNIT_megaflops,
    ENUM_CPUSPEEDUNIT_gigaflops,
    ENUM_CPUSPEEDUNIT_teraflops,
    ENUM_CPUSPEEDUNIT_petaflops
};

inline int returnCPUSpeedUnit(std::string unit)
{
    if (unit == "hertz"    ) return ENUM_CPUSPEEDUNIT_hertz;
    if (unit == "kilohertz") return ENUM_CPUSPEEDUNIT_kilohertz;
    if (unit == "megahertz") return ENUM_CPUSPEEDUNIT_megahertz;
    if (unit == "gigahertz") return ENUM_CPUSPEEDUNIT_gigahertz;
    if (unit == "terahertz") return ENUM_CPUSPEEDUNIT_terahertz;
    if (unit == "flops"    ) return ENUM_CPUSPEEDUNIT_flops;
    if (unit == "kiloflops") return ENUM_CPUSPEEDUNIT_kiloflops;
    if (unit == "megaflops") return ENUM_CPUSPEEDUNIT_megaflops;
    if (unit == "gigaflops") return ENUM_CPUSPEEDUNIT_gigaflops;
    if (unit == "teraflops") return ENUM_CPUSPEEDUNIT_teraflops;
    if (unit == "petaflops") return ENUM_CPUSPEEDUNIT_petaflops;
    return 0;
}//returnCPUSpeedUnit

inline bool verifyCPUSpeedUnit(std::string unit)
{
    return (returnCPUSpeedUnit(unit) > 0);
}//verifyCPUSpeedUnit

enum ENUM_STORAGEUNIT
{
    ENUM_STORAGEUNIT_byte = 1,
    ENUM_STORAGEUNIT_kilobyte,
    ENUM_STORAGEUNIT_megabyte,
    ENUM_STORAGEUNIT_gigabyte,
    ENUM_STORAGEUNIT_terabyte,
    ENUM_STORAGEUNIT_petabyte,
    ENUM_STORAGEUNIT_exabyte,
    ENUM_STORAGEUNIT_zettabyte,
    ENUM_STORAGEUNIT_yottabyte
};

inline int returnStorageUnit(std::string unit)
{
    if (unit == "byte"     ) return ENUM_STORAGEUNIT_byte;
    if (unit == "kilobyte" ) return ENUM_STORAGEUNIT_kilobyte;
    if (unit == "megabyte" ) return ENUM_STORAGEUNIT_megabyte;
    if (unit == "gigabyte" ) return ENUM_STORAGEUNIT_gigabyte;
    if (unit == "terabyte" ) return ENUM_STORAGEUNIT_terabyte;
    if (unit == "petabyte" ) return ENUM_STORAGEUNIT_petabyte;
    if (unit == "exabyte"  ) return ENUM_STORAGEUNIT_exabyte;
    if (unit == "zettabyte") return ENUM_STORAGEUNIT_zettabyte;
    if (unit == "yottabyte") return ENUM_STORAGEUNIT_yottabyte;
    return 0;
}//returnStorageUnit

inline bool verifyStorageUnit(std::string unit)
{
    return (returnStorageUnit(unit) > 0);
}//verifyCPUSpeedUnit

enum ENUM_TIMEUNIT
{
    ENUM_TIMEUNIT_tick = 1,
    ENUM_TIMEUNIT_millisecond,
    ENUM_TIMEUNIT_second,
    ENUM_TIMEUNIT_minute,
    ENUM_TIMEUNIT_hour,
    ENUM_TIMEUNIT_day,
    ENUM_TIMEUNIT_week,
    ENUM_TIMEUNIT_month,
    ENUM_TIMEUNIT_year
};

inline int returnTimeUnit(std::string unit)
{
    if (unit == "tick"       ) return ENUM_TIMEUNIT_tick;
    if (unit == "millisecond") return ENUM_TIMEUNIT_millisecond;
    if (unit == "second"     ) return ENUM_TIMEUNIT_second;
    if (unit == "minute"     ) return ENUM_TIMEUNIT_minute;
    if (unit == "hour"       ) return ENUM_TIMEUNIT_hour;
    if (unit == "day"        ) return ENUM_TIMEUNIT_day;
    if (unit == "week"       ) return ENUM_TIMEUNIT_week;
    if (unit == "month"      ) return ENUM_TIMEUNIT_month;
    if (unit == "year"       ) return ENUM_TIMEUNIT_year;
    return 0;
}//returnTimeUnit

inline bool verifyTimeUnit(std::string unit)
{
    return (returnTimeUnit(unit) > 0);
}//verifyTimeUnit

enum ENUM_TIMETYPE
{
    ENUM_TIMETYPE_cpuTime = 1,
    ENUM_TIMETYPE_elapsedTime,
    ENUM_TIMETYPE_other
};

inline int returnTimeType(std::string type)
{
    if (type == "cpuTime"    ) return ENUM_TIMETYPE_cpuTime;
    if (type == "elapsedTime") return ENUM_TIMETYPE_elapsedTime;
    if (type == "other"      ) return ENUM_TIMETYPE_other;
    return 0;
}//returnTimeType

inline bool verifyTimeType(std::string type)
{
    return (returnTimeType(type) > 0);
}//verifyTimeType

enum ENUM_TIMECATEGORY
{
    ENUM_TIMECATEGORY_total = 1,
    ENUM_TIMECATEGORY_input,
    ENUM_TIMECATEGORY_preprocessing,
    ENUM_TIMECATEGORY_optimization,
    ENUM_TIMECATEGORY_postprocessing,
    ENUM_TIMECATEGORY_output,
    ENUM_TIMECATEGORY_other
};

inline int returnTimeCategory(std::string category)
{
    if (category == "total"         ) return ENUM_TIMECATEGORY_total;
    if (category == "input"         ) return ENUM_TIMECATEGORY_input;
    if (category == "preprocessing" ) return ENUM_TIMECATEGORY_preprocessing;
    if (category == "optimization"  ) return ENUM_TIMECATEGORY_optimization;
    if (category == "postprocessing") return ENUM_TIMECATEGORY_postprocessing;
    if (category == "output"        ) return ENUM_TIMECATEGORY_output;
    if (category == "other"         ) return ENUM_TIMECATEGORY_other;
    return 0;
}//returnTimeCategory

inline bool verifyTimeCategory(std::string category)
{
    return (returnTimeCategory(category) > 0);
}//verifyTimeCategory

enum ENUM_LOCATIONTYPE
{
    ENUM_LOCATIONTYPE_local = 1,
    ENUM_LOCATIONTYPE_http,
    ENUM_LOCATIONTYPE_ftp
};

inline int returnLocationType(std::string type)
{
    if (type == "local") return ENUM_LOCATIONTYPE_local;
    if (type == "http" ) return ENUM_LOCATIONTYPE_http;
    if (type == "ftp"  ) return ENUM_LOCATIONTYPE_ftp;
    return 0;
}//returnLocationType

inline bool verifyLocationType(std::string type)
{
    return (returnLocationType(type) > 0);
}//verifyLocationType

enum ENUM_TRANSPORT_TYPE
{
    ENUM_TRANSPORT_TYPE_osp = 1,
    ENUM_TRANSPORT_TYPE_http,
    ENUM_TRANSPORT_TYPE_smtp,
    ENUM_TRANSPORT_TYPE_ftp,
    ENUM_TRANSPORT_TYPE_other
};

inline int returnTransportType(std::string type)
{
    if (type == "osp"  ) return ENUM_TRANSPORT_TYPE_osp;
    if (type == "http" ) return ENUM_TRANSPORT_TYPE_http;
    if (type == "smtp" ) return ENUM_TRANSPORT_TYPE_smtp;
    if (type == "ftp"  ) return ENUM_TRANSPORT_TYPE_ftp;
    if (type == "other") return ENUM_TRANSPORT_TYPE_other;
    return 0;
}//returnTransportType

inline bool verifyTransportType(std::string type)
{
    return (returnTransportType(type) > 0);
}//verifyTransportType

enum ENUM_SERVICE_TYPE
{
    ENUM_SERVICE_TYPE_analyzer = 1,
    ENUM_SERVICE_TYPE_solver,
    ENUM_SERVICE_TYPE_scheduler,
    ENUM_SERVICE_TYPE_modeler,
    ENUM_SERVICE_TYPE_registry,
    ENUM_SERVICE_TYPE_agent,
    ENUM_SERVICE_TYPE_simulations
};

inline int returnServiceType(std::string type)
{
    if (type == "analyzer"   ) return ENUM_SERVICE_TYPE_analyzer;
    if (type == "solver"     ) return ENUM_SERVICE_TYPE_solver;
    if (type == "scheduler"  ) return ENUM_SERVICE_TYPE_scheduler;
    if (type == "modeler"    ) return ENUM_SERVICE_TYPE_modeler;
    if (type == "registry"   ) return ENUM_SERVICE_TYPE_registry;
    if (type == "agent"      ) return ENUM_SERVICE_TYPE_agent;
    if (type == "simulations") return ENUM_SERVICE_TYPE_simulations;
    return 0;
}//returnServiceType

inline bool verifyServiceType(std::string type)
{
    return (returnServiceType(type) > 0);
}//verifyServiceType

enum ENUM_GENERAL_RESULT_STATUS
{
    ENUM_GENERAL_RESULT_STATUS_error = 1,
    ENUM_GENERAL_RESULT_STATUS_warning,
    ENUM_GENERAL_RESULT_STATUS_normal
};

inline int returnGeneralResultStatus(std::string status)
{
    if (status == "error"  ) return ENUM_GENERAL_RESULT_STATUS_error;
    if (status == "warning") return ENUM_GENERAL_RESULT_STATUS_warning;
    if (status == "normal" ) return ENUM_GENERAL_RESULT_STATUS_normal;
    return 0;
}//returnGeneralResultStatus

inline bool verifyGeneralResultStatus(std::string status)
{
    return (returnGeneralResultStatus(status) > 0);
}//verifyGeneralResultStatus

enum ENUM_SYSTEM_CURRENT_STATE
{
    ENUM_SYSTEM_CURRENT_STATE_busy = 1,
    ENUM_SYSTEM_CURRENT_STATE_busyButAccepting,
    ENUM_SYSTEM_CURRENT_STATE_idle,
    ENUM_SYSTEM_CURRENT_STATE_idleButNotAccepting,
    ENUM_SYSTEM_CURRENT_STATE_noResponse
};

inline int returnSystemCurrentState(std::string status)
{
    if (status == "busy"               ) return ENUM_SYSTEM_CURRENT_STATE_busy;
    if (status == "busyButAccepting"   ) return ENUM_SYSTEM_CURRENT_STATE_busyButAccepting;
    if (status == "idle"               ) return ENUM_SYSTEM_CURRENT_STATE_idle;
    if (status == "idleButNotAccepting") return ENUM_SYSTEM_CURRENT_STATE_idleButNotAccepting;
    if (status == "noResponse"         ) return ENUM_SYSTEM_CURRENT_STATE_noResponse;
    return 0;
}//returnSystemCurrentState

inline bool verifySystemCurrentState(std::string status)
{
    return (returnSystemCurrentState(status) > 0);
}//verifySystemCurrentState

enum ENUM_JOB_STATUS
{
    ENUM_JOB_STATUS_waiting = 1,
    ENUM_JOB_STATUS_running,
    ENUM_JOB_STATUS_killed,
    ENUM_JOB_STATUS_finished,
    ENUM_JOB_STATUS_unknown
};

inline int returnJobStatus(std::string status)
{
    if (status == "waiting" ) return ENUM_JOB_STATUS_waiting;
    if (status == "running" ) return ENUM_JOB_STATUS_running;
    if (status == "killed"  ) return ENUM_JOB_STATUS_killed;
    if (status == "finished") return ENUM_JOB_STATUS_finished;
    if (status == "unknown" ) return ENUM_JOB_STATUS_unknown;
    return 0;
}//returnJobStatus

inline bool verifyJobStatus(std::string status)
{
    return (returnJobStatus(status) > 0);
}//verifyJobStatus

/**
 *  Enumeration for the different states that can be used in representating a basis
 *  The last state, ENUM_BASIS_STATUS_NUMBER_OF_STATES, is used *only* to record the
 *  number of states, which makes it easier to convert between different representations.
 *  (For instance, AMPL uses a different order, so there may be a need to recode values.
 *   See OSosrl2ampl.cpp for an application.) 
 */
enum ENUM_BASIS_STATUS
{
    ENUM_BASIS_STATUS_basic = 0,
    ENUM_BASIS_STATUS_atLower,
    ENUM_BASIS_STATUS_atUpper,
    ENUM_BASIS_STATUS_atEquality,
    ENUM_BASIS_STATUS_isFree,
    ENUM_BASIS_STATUS_superbasic,
    ENUM_BASIS_STATUS_unknown,
    ENUM_BASIS_STATUS_NUMBER_OF_STATES
};

inline int returnBasisStatus(std::string status)
{
    if (status == "basic"     ) return ENUM_BASIS_STATUS_basic;
    if (status == "atLower"   ) return ENUM_BASIS_STATUS_atLower;
    if (status == "atUpper"   ) return ENUM_BASIS_STATUS_atUpper;
    if (status == "atEquality") return ENUM_BASIS_STATUS_atEquality;
    if (status == "isFree"    ) return ENUM_BASIS_STATUS_isFree;
    if (status == "superBasic") return ENUM_BASIS_STATUS_superbasic;
    if (status == "unknown"   ) return ENUM_BASIS_STATUS_unknown;
    return 0;
}//returnBasisStatus

inline bool verifyBasisStatus(std::string status)
{
    return (returnBasisStatus(status) > 0);
}//verifyBasisStatus

inline std::string returnBasisStatusString(ENUM_BASIS_STATUS status)
{
    if (status == ENUM_BASIS_STATUS_basic     ) return "basic";
    if (status == ENUM_BASIS_STATUS_atLower   ) return "atLower";
    if (status == ENUM_BASIS_STATUS_atUpper   ) return "atUpper";
    if (status == ENUM_BASIS_STATUS_atEquality) return "atEquality";
    if (status == ENUM_BASIS_STATUS_isFree    ) return "isFree";
    if (status == ENUM_BASIS_STATUS_superbasic) return "superBasic";
    if (status == ENUM_BASIS_STATUS_unknown   ) return "unknown";
    return "status???";
}//returnBasisStatus


enum ENUM_SOLUTION_STATUS
{
    ENUM_SOLUTION_STATUS_unbounded = 1,
    ENUM_SOLUTION_STATUS_globallyOptimal,
    ENUM_SOLUTION_STATUS_locallyOptimal,
    ENUM_SOLUTION_STATUS_optimal,
    ENUM_SOLUTION_STATUS_bestSoFar,
    ENUM_SOLUTION_STATUS_feasible,
    ENUM_SOLUTION_STATUS_infeasible,
    ENUM_SOLUTION_STATUS_unsure,
    ENUM_SOLUTION_STATUS_error,
    ENUM_SOLUTION_STATUS_other
};

inline int returnSolutionStatus(std::string status)
{
    if (status == "unbounded"      ) return ENUM_SOLUTION_STATUS_unbounded;
    if (status == "globallyOptimal") return ENUM_SOLUTION_STATUS_globallyOptimal;
    if (status == "locallyOptimal" ) return ENUM_SOLUTION_STATUS_locallyOptimal;
    if (status == "optimal"        ) return ENUM_SOLUTION_STATUS_optimal;
    if (status == "bestSoFar"      ) return ENUM_SOLUTION_STATUS_bestSoFar;
    if (status == "feasible"       ) return ENUM_SOLUTION_STATUS_feasible;
    if (status == "infeasible"     ) return ENUM_SOLUTION_STATUS_infeasible;
    if (status == "unsure"         ) return ENUM_SOLUTION_STATUS_unsure;
    if (status == "error"          ) return ENUM_SOLUTION_STATUS_error;
    if (status == "other"          ) return ENUM_SOLUTION_STATUS_other;
    return 0;
}//returnSolutionStatus

inline bool verifySolutionStatus(std::string status)
{
    return (returnSolutionStatus(status) > 0);
}//verifySolutionStatus

enum ENUM_SOLUTION_SUBSTATUSTYPE
{
    ENUM_SOLUTION_SUBSTATUSTYPE_stoppedByLimit = 1,
    ENUM_SOLUTION_SUBSTATUSTYPE_stoppedByBounds,
    ENUM_SOLUTION_SUBSTATUSTYPE_other
};

inline int returnSolutionSubstatusType(std::string type)
{
    if (type == "stoppedByLimit" ) return ENUM_SOLUTION_SUBSTATUSTYPE_stoppedByLimit;
    if (type == "stoppedByBounds") return ENUM_SOLUTION_SUBSTATUSTYPE_stoppedByBounds;
    if (type == "other"          ) return ENUM_SOLUTION_SUBSTATUSTYPE_other;
    return 0;
}//returnSolutionSubstatusType

inline bool verifySolutionSubstatusType(std::string type)
{
    return (returnSolutionSubstatusType(type) > 0);
}//verifySolutionSubstatusType

enum ENUM_PROBLEM_COMPONENT
{
    ENUM_PROBLEM_COMPONENT_variables = 1,
    ENUM_PROBLEM_COMPONENT_objectives,
    ENUM_PROBLEM_COMPONENT_constraints
};

enum ENUM_VARTYPE
{
    ENUM_VARTYPE_continuous = 1,
    ENUM_VARTYPE_binary,
    ENUM_VARTYPE_integer,
    ENUM_VARTYPE_string,
    ENUM_VARTYPE_semicontinuous,
    ENUM_VARTYPE_semiinteger
};

inline int returnVarType(char vt)
{
    if (vt == 'C') return ENUM_VARTYPE_continuous;
    if (vt == 'B') return ENUM_VARTYPE_binary;
    if (vt == 'I') return ENUM_VARTYPE_integer;
    if (vt == 'S') return ENUM_VARTYPE_string;
    if (vt == 'D') return ENUM_VARTYPE_semicontinuous;
    if (vt == 'J') return ENUM_VARTYPE_semiinteger;
    return 0;
}//returnVarType

inline bool verifyVarType(char vt)
{
    return (returnVarType(vt) > 0);
}//verifyVarType


enum ENUM_PATHPAIR
{
    ENUM_PATHPAIR_input_dir = 1,
    ENUM_PATHPAIR_input_file,
    ENUM_PATHPAIR_output_file,
    ENUM_PATHPAIR_output_dir
};

/**
 * An enum to track the many different types of values that a matrix can contain
 * Note that these types are partially ordered, which makes it easier to infer a matrix's type
 * from the types of its constructors
 */
enum ENUM_MATRIX_TYPE
{
    ENUM_MATRIX_TYPE_empty = 1,         // matrix has no elements (i.e., zero matrix)

    ENUM_MATRIX_TYPE_constant = 10,     // matrix elements contain constant values (i.e., real numbers)
    ENUM_MATRIX_TYPE_varReference,      // matrix elements contain indexes of variables in the core
    ENUM_MATRIX_TYPE_linear,            // matrix contains linear expressions
    ENUM_MATRIX_TYPE_quadratic,         // matrix contains quadratic expressions
    ENUM_MATRIX_TYPE_general,           // matrix contains general nonlinear expressions

    ENUM_MATRIX_TYPE_objReference = 20, // matrix elements contain indexes of constraints in the core
    ENUM_MATRIX_TYPE_conReference,      // matrix elements contain indexes of objectives in the core
    ENUM_MATRIX_TYPE_mixedRowReference, // mixed reference to objectives and constraints

    ENUM_MATRIX_TYPE_string,            // matrix elements contain string values

    ENUM_MATRIX_TYPE_unknown = 99
};

inline int returnMatrixType(std::string type)
{
    if (type == "empty"            ) return ENUM_MATRIX_TYPE_empty;
    if (type == "constant"         ) return ENUM_MATRIX_TYPE_constant;
    if (type == "varReference"     ) return ENUM_MATRIX_TYPE_varReference;
    if (type == "linear"           ) return ENUM_MATRIX_TYPE_linear;
    if (type == "quadratic"        ) return ENUM_MATRIX_TYPE_quadratic;
    if (type == "general"          ) return ENUM_MATRIX_TYPE_general;

    if (type == "objReference"     ) return ENUM_MATRIX_TYPE_objReference;
    if (type == "conReference"     ) return ENUM_MATRIX_TYPE_conReference;
    if (type == "mixedRowReference") return ENUM_MATRIX_TYPE_mixedRowReference;

    if (type == "string"           ) return ENUM_MATRIX_TYPE_string;

    if (type == "unknown"          ) return ENUM_MATRIX_TYPE_unknown;
    return 0;
}//returnMatrixType

inline std::string returnMatrixTypeString(ENUM_MATRIX_TYPE type)
{
    if (type == ENUM_MATRIX_TYPE_empty)             return "empty";
    if (type == ENUM_MATRIX_TYPE_constant)          return "constant";
    if (type == ENUM_MATRIX_TYPE_varReference)      return "varReference";
    if (type == ENUM_MATRIX_TYPE_linear)            return "linear";
    if (type == ENUM_MATRIX_TYPE_quadratic)         return "quadratic";
    if (type == ENUM_MATRIX_TYPE_general)           return "general";
    if (type == ENUM_MATRIX_TYPE_objReference)      return "objReference";
    if (type == ENUM_MATRIX_TYPE_conReference)      return "conReference";
    if (type == ENUM_MATRIX_TYPE_mixedRowReference) return "mixedRowReference";
    if (type == ENUM_MATRIX_TYPE_unknown)           return "unknown";
    if (type == ENUM_MATRIX_TYPE_string)            return "string";
    return "unknown";
}//returnMatrixTypeString

inline bool verifyMatrixType(std::string type)
{
    return (returnMatrixType(type) > 0);
}//verifyMatrixType

/**
 *  A function to merge two matrix types so we can infer the type of a matrix recursively
 */
inline ENUM_MATRIX_TYPE mergeMatrixType(ENUM_MATRIX_TYPE type1, ENUM_MATRIX_TYPE type2)
{
    // two matrices of same type 
    if (type1 == type2) return type1;

    if (type1 == ENUM_MATRIX_TYPE_string || type2 == ENUM_MATRIX_TYPE_string) 
        return ENUM_MATRIX_TYPE_string;

    if (type1 == ENUM_MATRIX_TYPE_unknown) return type2;
    if (type2 == ENUM_MATRIX_TYPE_unknown) return type1;
    if (type1 == ENUM_MATRIX_TYPE_empty) return type2;
    if (type2 == ENUM_MATRIX_TYPE_empty) return type1;

    // column and objective references can be mixed  
    if (type1 >= ENUM_MATRIX_TYPE_conReference) // row reference (objective or constraint)
    {
        if (type2 >= ENUM_MATRIX_TYPE_conReference)
            return ENUM_MATRIX_TYPE_mixedRowReference;
        else
            return ENUM_MATRIX_TYPE_general;
    }                                  
    else // type1 is a linear or nonlinear expression   
    {
        if (type2 >= ENUM_MATRIX_TYPE_conReference) return ENUM_MATRIX_TYPE_general;  
        else // varReference must be treated like linear if it is mixed with any other remaining type
            if (type1 < type2)
                if (type2 == ENUM_MATRIX_TYPE_varReference)
                    return ENUM_MATRIX_TYPE_linear;
                 else
                    return type2;
            else 
                if (type1 == ENUM_MATRIX_TYPE_varReference)
                    return ENUM_MATRIX_TYPE_linear;
                 else
                    return type1;
    }
    return ENUM_MATRIX_TYPE_unknown;
}//end of mergeMatrixType

/**
 *  An enum to track the type of value contained in a reference to a constraint 
 */
enum ENUM_CONREFERENCE_VALUETYPE
{
    ENUM_CONREFERENCE_VALUETYPE_value = 1,
    ENUM_CONREFERENCE_VALUETYPE_status,
    ENUM_CONREFERENCE_VALUETYPE_surplus,
    ENUM_CONREFERENCE_VALUETYPE_shortage
};

inline std::string returnConReferenceValueTypeString(ENUM_CONREFERENCE_VALUETYPE valueType)
{
    if (valueType == ENUM_CONREFERENCE_VALUETYPE_value   ) return "none";
    if (valueType == ENUM_CONREFERENCE_VALUETYPE_status  ) return "status";
    if (valueType == ENUM_CONREFERENCE_VALUETYPE_surplus ) return "surplus";
    if (valueType == ENUM_CONREFERENCE_VALUETYPE_shortage) return "shortage";
    return "none";
}//returnConReferenceValueTypeString

inline int returnConReferenceValueType(std::string valueType)
{
    if (valueType == "value"    ) return ENUM_CONREFERENCE_VALUETYPE_value;
    if (valueType == "status"   ) return ENUM_CONREFERENCE_VALUETYPE_status;
    if (valueType == "surplus"  ) return ENUM_CONREFERENCE_VALUETYPE_surplus;
    if (valueType == "shortage" ) return ENUM_CONREFERENCE_VALUETYPE_shortage;
    return 0;
}//returnConReferenceValueType

inline bool verifyConReferenceValueType(std::string valueType)
{
    return (returnConReferenceValueType(valueType) > 0);
}//verifyConReferenceValueType


enum ENUM_MATRIX_SYMMETRY
{
    ENUM_MATRIX_SYMMETRY_none = 1,
    ENUM_MATRIX_SYMMETRY_upper,
    ENUM_MATRIX_SYMMETRY_lower,
    ENUM_MATRIX_SYMMETRY_skewUpper,
    ENUM_MATRIX_SYMMETRY_skewLower,
    ENUM_MATRIX_SYMMETRY_HermitianLower,
    ENUM_MATRIX_SYMMETRY_HermitianUpper
};

inline std::string returnMatrixSymmetryString(ENUM_MATRIX_SYMMETRY symmetry)
{
    if (symmetry == ENUM_MATRIX_SYMMETRY_none          ) return "none";
    if (symmetry == ENUM_MATRIX_SYMMETRY_upper         ) return "upper";
    if (symmetry == ENUM_MATRIX_SYMMETRY_lower         ) return "lower";
    if (symmetry == ENUM_MATRIX_SYMMETRY_skewUpper     ) return "skewUpper";
    if (symmetry == ENUM_MATRIX_SYMMETRY_skewLower     ) return "skewLower";
    if (symmetry == ENUM_MATRIX_SYMMETRY_HermitianLower) return "HermitianLower";
    if (symmetry == ENUM_MATRIX_SYMMETRY_HermitianLower) return "HermitianLower";
    return "none";
}//returnMatrixSymmetryString

inline int returnMatrixSymmetry(std::string symmetry)
{
    if (symmetry == "none"          ) return ENUM_MATRIX_SYMMETRY_none;
    if (symmetry == "upper"         ) return ENUM_MATRIX_SYMMETRY_upper;
    if (symmetry == "lower"         ) return ENUM_MATRIX_SYMMETRY_lower;
    if (symmetry == "skewUpper"     ) return ENUM_MATRIX_SYMMETRY_skewUpper;
    if (symmetry == "skewLower"     ) return ENUM_MATRIX_SYMMETRY_skewLower;
    if (symmetry == "HermitianLower") return ENUM_MATRIX_SYMMETRY_HermitianLower;
    if (symmetry == "HermitianLower") return ENUM_MATRIX_SYMMETRY_HermitianLower;
    return 0;
}//returnMatrixSymmetry

inline bool verifyMatrixSymmetry(std::string symmetry)
{
    return (returnMatrixSymmetry(symmetry) > 0);
}//verifyMatrixSymmetry


enum ENUM_MATRIX_CONSTRUCTOR_TYPE
{
    ENUM_MATRIX_CONSTRUCTOR_TYPE_unknown = 0,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_baseMatrix,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_constantElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_varRefElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_linearElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_generalElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_objRefElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_conRefElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_mixedRowRefElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_stringValuedElements,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_transformation,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_blocks,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_block,
    ENUM_MATRIX_CONSTRUCTOR_TYPE_matrix
};

inline int returnMatrixConstructorType(std::string cType)
{
    if (cType == "baseMatrix"       ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_baseMatrix;
    if (cType == "constantElements" ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_constantElements;
    if (cType == "varRefElements"   ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_varRefElements;
    if (cType == "linearElements"   ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_linearElements;
    if (cType == "generalElements"  ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_generalElements;
    if (cType == "objRefElements"   ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_objRefElements;
    if (cType == "conRefElements"   ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_conRefElements;
    if (cType == "stringElements"   ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_stringValuedElements;
    if (cType == "transformation"   ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_transformation;
    if (cType == "blocks"           ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_blocks;
    if (cType == "block"            ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_block;
    if (cType == "matrix"           ) return ENUM_MATRIX_CONSTRUCTOR_TYPE_matrix;
    return 0;
}//returnMatrixConstructorType

inline bool verifyMatrixConstructorType(std::string type)
{
    return (returnMatrixConstructorType(type) > 0);
}//verifyMatrixConstructorType


/**
 * An enum to streamline set() methods of vectors
 */
enum ENUM_COMBINE_ARRAYS
{
    ENUM_COMBINE_ARRAYS_replace, // silently replace previous data (if any)
    ENUM_COMBINE_ARRAYS_merge,   // merge two vectors into one
    ENUM_COMBINE_ARRAYS_ignore,  // silently ignore current vector if previous data exist
    ENUM_COMBINE_ARRAYS_throw    // throw an error if previous data detected
};


/* An enumeration to track the shape of a nonlinear expression */
enum ENUM_NL_EXPR_SHAPE
{
    ENUM_NL_EXPR_SHAPE_general = 1,
    ENUM_NL_EXPR_SHAPE_convex,
    ENUM_NL_EXPR_SHAPE_quadratic,
    ENUM_NL_EXPR_SHAPE_linear,
    ENUM_NL_EXPR_SHAPE_constant
};

inline int returnNlExprShape(std::string shape)
{
    if (shape == "general"  ) return ENUM_NL_EXPR_SHAPE_general;
    if (shape == "convex"   ) return ENUM_NL_EXPR_SHAPE_convex;
    if (shape == "quadratic") return ENUM_NL_EXPR_SHAPE_quadratic;
    if (shape == "linear"   ) return ENUM_NL_EXPR_SHAPE_linear;
    if (shape == "constant" ) return ENUM_NL_EXPR_SHAPE_constant;
    return 1;
}//returnNlExprShape 

inline std::string returnExprShapeString(ENUM_NL_EXPR_SHAPE shape)
{
    if (shape == ENUM_NL_EXPR_SHAPE_general  ) return "general";
    if (shape == ENUM_NL_EXPR_SHAPE_convex   ) return "convex";
    if (shape == ENUM_NL_EXPR_SHAPE_quadratic) return "quadratic";
    if (shape == ENUM_NL_EXPR_SHAPE_linear   ) return "linear";
    if (shape == ENUM_NL_EXPR_SHAPE_constant ) return "constant";
    return "";
}//returnExprShapeString 
 
inline bool verifyNlExprShape(std::string shape)
{
    return (returnNlExprShape(shape) > 0);
}//verifyNlExprShape


enum ENUM_CONE_TYPE
{
    ENUM_CONE_TYPE_nonnegative = 1,
    ENUM_CONE_TYPE_nonpositive,
    ENUM_CONE_TYPE_orthant,
    ENUM_CONE_TYPE_polyhedral,
    ENUM_CONE_TYPE_quadratic,
    ENUM_CONE_TYPE_rotatedQuadratic,
    ENUM_CONE_TYPE_normed,
    ENUM_CONE_TYPE_rotatedNormed,
    ENUM_CONE_TYPE_semidefinite,
    ENUM_CONE_TYPE_copositiveMatrices,
    ENUM_CONE_TYPE_completelyPositiveMatrices,
    ENUM_CONE_TYPE_hyperbolicity,
    ENUM_CONE_TYPE_sumOfSquaresPolynomials,
    ENUM_CONE_TYPE_nonnegativePolynomials,
    ENUM_CONE_TYPE_moments,
    ENUM_CONE_TYPE_product,
    ENUM_CONE_TYPE_intersection,
    ENUM_CONE_TYPE_dual,
    ENUM_CONE_TYPE_polar,
    ENUM_CONE_TYPE_unknown
};

inline int returnConeType(std::string type)
{
    if (type == "nonnegative"               ) return ENUM_CONE_TYPE_nonnegative;
    if (type == "nonpositive"               ) return ENUM_CONE_TYPE_nonpositive;
    if (type == "orthant"                   ) return ENUM_CONE_TYPE_orthant;
    if (type == "polyhedral"                ) return ENUM_CONE_TYPE_polyhedral;
    if (type == "quadratic"                 ) return ENUM_CONE_TYPE_quadratic;

    if (type == "rotatedQuadratic"          ) return ENUM_CONE_TYPE_rotatedQuadratic;
    if (type == "normed"                    ) return ENUM_CONE_TYPE_normed;
    if (type == "rotatedNormed"             ) return ENUM_CONE_TYPE_rotatedNormed;
    if (type == "semidefinite"              ) return ENUM_CONE_TYPE_semidefinite;
    if (type == "copositiveMatrices"        ) return ENUM_CONE_TYPE_copositiveMatrices;
    if (type == "completelyPositiveMatrices") return ENUM_CONE_TYPE_completelyPositiveMatrices;
    if (type == "hyperbolicity"             ) return ENUM_CONE_TYPE_hyperbolicity;
    if (type == "sumOfSquaresPolynomials"   ) return ENUM_CONE_TYPE_sumOfSquaresPolynomials;
    if (type == "nonnegativePolynomials"    ) return ENUM_CONE_TYPE_nonnegativePolynomials;
    if (type == "moments"                   ) return ENUM_CONE_TYPE_moments;
    if (type == "product"                   ) return ENUM_CONE_TYPE_product;
    if (type == "intersection"              ) return ENUM_CONE_TYPE_intersection;
    if (type == "dual"                      ) return ENUM_CONE_TYPE_dual;
    if (type == "polar"                     ) return ENUM_CONE_TYPE_polar;
    if (type == "unknown"                   ) return ENUM_CONE_TYPE_unknown;
    return 0;
}//returnConeType

inline bool verifyConeType(std::string type)
{
    return (returnConeType(type) > 0);
}//verifyConeType


#endif
