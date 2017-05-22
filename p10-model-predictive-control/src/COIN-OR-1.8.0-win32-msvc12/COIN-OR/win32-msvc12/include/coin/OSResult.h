/* $Id: OSResult.h 5035 2015-07-01 16:52:07Z Gassmann $ */
/** @file OSResult.h
 *
 * @author  Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * Copyright (C) 2005-2011, Horand Gassmann, Jun Ma, Kipp Martin,
 * Dalhousie University, Northwestern University, and the University of Chicago.
 * All Rights Reserved.
 * This software is licensed under the Eclipse Public License.
 * Please see the accompanying LICENSE file in root directory for terms.
 *
 */

#ifndef OSRESULT_H
#define OSRESULT_H
#include <string>
#include <vector>
#include "OSGeneral.h"
#include "OSMatrix.h"

//#define DEBUG


/*! \struct IndexStringPair
 *  \brief A commonly used structure holding an index-string pair
 *  This definition is based on the definition of IndexValuePair in OSGeneral.h
 */
struct IndexStringPair
{
    /** idx holds the index of a string-valued entity (such as a variable, constraint, objective)
     *  that is part of a sparse vector
     */
    int idx;

    /** value is a string that holds the value of the entity
     */
    std::string value;

};


/*! \class GeneralSubstatus
 *  \brief The GeneralSubstatus  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 11/03/2008
 * @since OS 2.0
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSrL schema.
 */
class GeneralSubstatus
{

public:

    /** the name of the substatus */
    std::string name;

    /** the description of the substatus */
    std::string description;

    /**
     *
     * Default constructor.
     */
    GeneralSubstatus();
    /**
     *
     * Class destructor.
     */
    ~GeneralSubstatus();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(GeneralSubstatus *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//GeneralSubstatus


/*! \class GeneralStatus
 *  \brief The GeneralStatus  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSrL schema.
 */
class GeneralStatus
{

public:

    /** the number of substatuses */
    int numberOfSubstatuses;

    /** the type of status */
    std::string type;

    /** the description of the status */
    std::string description;

    /** the array of substatuses */
    GeneralSubstatus **substatus;

    /**
     *
     * Default constructor.
     */
    GeneralStatus();
    /**
     *
     * Class destructor.
     */
    ~GeneralStatus();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(GeneralStatus *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//GeneralStatus


/*! \class OtherResult
 *  \brief The OtherResult Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 10/09/2009
 * @since OS 2.0
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSrL schema.
 */
class OtherResult
{

public:

    /** the name of the other result */
    std::string name;

    /** the value of the other result */
    std::string value;

    /** the description of the other result */
    std::string description;

    /**
     *
     * Default constructor.
     */
    OtherResult();
    /**
     *
     * Class destructor.
     */
    ~OtherResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherResult


/*! \class OtherResults
 *  \brief The OtherResults  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 10/09/2009
 * @since OS 1.0
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSrL schema.
 */
class OtherResults
{

public:

    /** the number of other results */
    int numberOfOtherResults;

    /** the array of other results */
    OtherResult** other;

    /**
     *
     * Default constructor.
     */
    OtherResults();
    /**
     *
     * Class destructor.
     */
    ~OtherResults();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherResults *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherResults

/*! \class GeneralResult
 *  \brief The GeneralResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides the general information
 * that is defined in the OSrL schema.
 */
class GeneralResult
{
public:

    /** a pointer to the GeneralStatus class
     */
    GeneralStatus *generalStatus;

    /** any general message associated with the optimization
     */
    std::string message;

    /** the serviceURI is the URI of the solver service
     *  that did the optimization
     */
    std::string serviceURI;

    /** the serviceName is the name of the solver service
     *  that did the optimization
     */
    std::string serviceName;

    /** the name of the instance that was solved
     */
    std::string instanceName;

    /** the jobID is the ID associated with the solution
     *  of this instance
     */
    std::string jobID;

    /** the name of the solver used
     */
    std::string solverInvoked;

    /** a time stamp associated with the process
     */
    std::string timeStamp;

    /** a pointer to the OtherResults class
     */
    OtherResults *otherResults;


    /**
     *
     * Default constructor.
     */
    GeneralResult();
    /**
     *
     * Class destructor.
     */
    ~GeneralResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(GeneralResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//class GeneralResult


/*! \class SystemResult
 *  \brief The SystemResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides the system information
 * that is defined in the OSrL schema.
 */
class SystemResult
{
public:

    /** a string containing some basic system information */
    std::string systemInformation;

    /** a pointer to the DiskSpace class
     */
    StorageCapacity *availableDiskSpace;

    /** a pointer to the MemorySize class
     */
    StorageCapacity *availableMemory;

    /** a pointer to the CPUSpeed class
     */
    CPUSpeed *availableCPUSpeed;

    /** a pointer to the CPUNumber class
     */
    CPUNumber *availableCPUNumber;

    /** a pointer to the OtherResults class
     */
    OtherResults *otherResults;

    /**
     *
     * Default constructor.
     */
    SystemResult();
    /**
     *
     * Class destructor.
     */
    ~SystemResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(SystemResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//class SystemResult


/*! \class ServiceResult
 *  \brief The ServiceResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides the system information
 * that is defined in the OSrL schema.
 */
class ServiceResult
{
public:

    /** a string describing the current state of the service */
    std::string currentState;

    /** the number of jobs currently running */
    int currentJobCount;

    /** total jobs processed so far */
    int totalJobsSoFar;

    /** the time when the service was started */
    std::string timeServiceStarted;

    /** service utilization */
    double serviceUtilization;

    /** a pointer to the OtherResults class
     */
    OtherResults *otherResults;

    /**
     *
     * Default constructor.
     */
    ServiceResult();
    /**
     *
     * Class destructor.
     */
    ~ServiceResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ServiceResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//class ServiceResult

#if 0
/*! \class Time
 *  \brief The Time Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides an individual time measurement
 * as defined in the OSrL schema.
 * This class is superseded by class TimeMeasurement since version 2.3
 */
class Time
{
public:

    /**
     * The type of timer used (cpuTime/elapsedTime/other)
     */
    std::string type;

    /**
     * The category of time (total/input/preprocessing/optimization/postprocessing/output/other)
     */
    std::string category;

    /**
     * The unit of time (tick/millisecond/second/minute/hour/day/week/month/year)
     */
    std::string unit;

    /**
     * Further description on the timer used
     */
    std::string description;

    /**
     * The time measurement
     */
    double value;

    /**
     *
     * Default constructor.
     */
    Time();
    /**
     *
     * Class destructor.
     */
    ~Time();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(Time *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//class Time
#endif

/*! \class TimeMeasurement
 *  \brief The TimeMeasurement Class.
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 *
 * \remarks
 * A class that provides an individual time measurement as defined in the OSrL schema.
 * Extends the class TimeSpan defined in OSGeneral.h by adding three elements
 * type, category and description.
 * This class supersedes the old class Time since version 2.3.
 */
class TimeMeasurement : public TimeSpan
{
public:

    /**
     * The type of timer used (cpuTime/elapsedTime/other)
     */
    std::string type;

    /**
     * The category of time (total/input/preprocessing/optimization/postprocessing/output/other)
     */
    std::string category;

    /**
     * Further description on the timer used
     */
    std::string description;

    /**
     *
     * Default constructor.
     */
    TimeMeasurement();
    /**
     *
     * Class destructor.
     */
    ~TimeMeasurement();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(TimeMeasurement *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//class TimeMeasurement


/*! \class TimingInformation
 *  \brief The TimingInformation  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides the timer information
 * that is defined in the OSrL schema.
 */
class TimingInformation
{
public:

    /**
     *
     * The number of elements in the time array
     */
    int numberOfTimes;

    /**
     *
     * An array of time measurements
     */
    TimeMeasurement** time;

    /**
     *
     * Default constructor.
     */
    TimingInformation();
    /**
     *
     * Class destructor.
     */
    ~TimingInformation();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(TimingInformation *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//class TimingInformation


/*! \class JobResult
 *  \brief The JobResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides the system information
 * that is defined in the OSrL schema.
 */
class JobResult
{
public:

    /** job status */
    std::string status;

    /** time the job was submitted */
    std::string submitTime;

    /** the time when the job was supposed to start */
    std::string scheduledStartTime;

    /** the time when the job actually started */
    std::string actualStartTime;

    /** the time when the job finished */
    std::string endTime;

    /** a pointer to the TimingInformation class */
    TimingInformation* timingInformation;

    /** a pointer to the DiskSpace class
     */
    StorageCapacity *usedDiskSpace;

    /** a pointer to the MemorySize class

     */
    StorageCapacity *usedMemory;

    /** a pointer to the CPUSpeed class
     */
    CPUSpeed *usedCPUSpeed;

    /** a pointer to the CPUNumber class
     */
    CPUNumber *usedCPUNumber;

    /** a pointer to the OtherResults class
     */
    OtherResults *otherResults;

    /**
     *
     * Default constructor.
     */
    JobResult();
    /**
     *
     * Class destructor.
     */
    ~JobResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(JobResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//class JobResult




/*! \class OptimizationSolutionSubstatus
 *  \brief The OptimizationSolutionSubstatus Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for holding various attributes of a solution
 * status
 */
class OptimizationSolutionSubstatus
{
public:

    /** the type of the solution substatus */
    std::string type;

    /** a description of the solution substatus */
    std::string description;

    /**
     * Default constructor.
     */
    OptimizationSolutionSubstatus();
    /**
     *
     * Class destructor.
     */
    ~OptimizationSolutionSubstatus();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OptimizationSolutionSubstatus *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OptimizationSolutionSubstatus


/*! \class OptimizationSolutionStatus
 *  \brief The OptimizationSolutionStatus Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for holding various attributes of a solution
 * status
 */
class OptimizationSolutionStatus
{
public:

    /** the number of substatus objects */
    int numberOfSubstatuses;

    /** the type of solution status */
    std::string type;

    /** a description of the solution status type */
    std::string description;

    /** a pointer to an array of substatus objects */
    OptimizationSolutionSubstatus** substatus;

    /**
     * Default constructor.
     */
    OptimizationSolutionStatus();
    /**
     *
     * Class destructor.
     */
    ~OptimizationSolutionStatus();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OptimizationSolutionStatus *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OptimizationSolutionStatus


/*! \class VarValue
 *  \brief VarValue  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that is used to provide the value and index
 * associated with the variables in the solution.
 */
class VarValue
{
public:

    /** idx is the index on variable in the solution */
    int idx;

    /** optional name */
    std::string name;

    /* value is the value of the variable indexed by idx
     * in the solution
     */
    double value;

    /**
     *
     * Default constructor.
     */
    VarValue();
    /**
     *
     * Class destructor.
     */
    ~VarValue();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(VarValue *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};// class VarValue


/*! \class VariableValues
 *  \brief The VariableValues Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that contains values for all the variables
 */
class VariableValues
{
public:

    /** the number of variable values that are in the solution
     */
    int numberOfVar;

    /** a vector of VarValue objects, there will be one
     * for each variable in the solution
     */
    VarValue** var;


    /**
     *
     * Default constructor.
     */
    VariableValues();

    /**
     *
     * Class destructor.
     */
    ~VariableValues();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(VariableValues *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};// class VariableValues


/*! \class VarValueString
 *  \brief VarValueString  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that is used to provide the value and index
 * associated with the string-valued variables in the solution.
 */
class VarValueString
{
public:

    /** idx is the index on variable in the solution */
    int idx;

    /** optional name */
    std::string name;

    /* value is the value of the variable indexed by idx
     * in the solution
     */
    std::string value;

    /**
     *
     * Default constructor.
     */
    VarValueString();

    /**
     *
     * Class destructor.
     */
    ~VarValueString();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(VarValueString *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};// class VarValueString


/*! \class VariableValuesString
 *  \brief The VariableValuesString Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that contains values for all the string-valued variables
 */
class VariableValuesString
{
public:

    /** the number of string-valued variable values that are in the solution
     */
    int numberOfVar;

    /** a vector of VarValueString objects, there will be one
     * for each variable in the solution
     */
    VarValueString** var;


    /**
     *
     * Default constructor.
     */
    VariableValuesString();

    /**
     *
     * Class destructor.
     */
    ~VariableValuesString();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(VariableValuesString *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};// class VariableValuesString


/*! \class OtherVarResult
 *  \brief OtherVarResult Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks A class used to provide solution information
 * associated with the variables.
 */
class OtherVarResult
{
public:

    /** the index of a variable in the solution */
    int idx;

    /** optional name */
    std::string name;

    /** value holds a general value associated with a variable,
     *  for example, rather than the value of a variable
     *  we may wish to store the variable name associated
     *  with the variable with index idx, hence we want
     *  a string here and not a double
     */
    std::string value;

    /**
     *
     * Default constructor.
     */
    OtherVarResult();
    /**
     *
     * Class destructor.
     */
    ~OtherVarResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherVarResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherVarResult



/*! \class OtherVariableResult
 *  \brief The OtherVariableResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that allows the solver to report an
 * arbitrary result associated with variables
 */
class OtherVariableResult
{
public:

    /** the number of variables which have values for this
        particular type of result
     */
    int numberOfVar;

    /** the number of distinct values for this
        particular type of result
     */
    int numberOfEnumerations;

    /** the name of the result the user is defining */
    std::string name;

    /** this element allows a specific value associated with this
        particular type of result
     */
    std::string value;

    /** type of the result value (integer, double, boolean, string) */
    std::string type;

    /** a brief description of the type of result */
    std::string description;

    /** the solver of the result value */
    std::string solver;

    /** the category of the result value */
    std::string category;

    /* a pointer to OtherVarResult objects that will
     * give for each variable the index and value for
     * this user defined variable result
     */
    OtherVarResult** var;

    /** type of the values in the var array */
    std::string varType;

    /* a pointer to OtherOptionOrResultEnumeration objects that will
     * give for each distinct value the set of indices for
     * this user defined variable result
     */
    OtherOptionOrResultEnumeration** enumeration;

    /** type of the values in the enumeration array */
    std::string enumType;

    /**
     *
     * Default constructor.
     */
    OtherVariableResult();

    /**
     *
     * Class destructor.
     */
    ~OtherVariableResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherVariableResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherVariableResult


/*! \class VariableSolution
 *  \brief The VariableSolution Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for reporting all of the types of solution values
 * associated with variables.
 */
class VariableSolution
{
public:

    /** the number of types of variable results other
     *  than the value of the variable
     */
    int numberOfOtherVariableResults;

    /** a pointer to a VariableValues object */
    VariableValues *values;

    /** a pointer to a VariableValuesString object */
    VariableValuesString *valuesString;

    /** a pointer to a BasisStatus object */
    BasisStatus *basisStatus;

    /** a pointer to an array of other pointer objects for
     * variables
     */
    OtherVariableResult** other;

    /**
     *
     * Default constructor.
     */
    VariableSolution();

    /**
     *
     * Class destructor.
     */
    ~VariableSolution();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(VariableSolution *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};// class VariableSolution


/*! \class ObjValue
 *  \brief The ObjValue  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides the value of an objective
 * function
 */
class ObjValue
{
public:

    /** idx is the index on an objective function */
    int idx;

    /** optional name */
    std::string name;

    /** the value of the objective indexed by idx */
    double value;

    /**
     *
     * Default constructor.
     */
    ObjValue();

    /**
     *
     * Class destructor.
     */
    ~ObjValue();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ObjValue *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);
};//ObjValue

/*! \class ObjectiveValues
 *  \brief The ObjectiveValues  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for reporting objective function values
 */
class ObjectiveValues
{
public:

    /** record the number of objective rows for which values are given
     */
    int numberOfObj;

    /** obj is a pointer to an array of ObjValue objects that
     * give an index and objective function value for
     * each objective function.
     */
    ObjValue** obj;

    /**
     *
     * Default constructor.
     */
    ObjectiveValues();

    /**
     *
     * Class destructor.
     */
    ~ObjectiveValues();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ObjectiveValues *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//ObjectiveValues


/*! \class OtherObjResult
 *  \brief The OtherObjResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides general result information
 * for objective functions.
 */
class OtherObjResult
{
public:

    /** idx is the index on a objective function */
    int idx;

    /** optional name */
    std::string name;

    /** value is a value associated with an objective function
     *  indexed by idx
     */
    std::string value;

    /**
     *
     * Default constructor.
     */
    OtherObjResult();

    /**
     *
     * Class destructor.
     */
    ~OtherObjResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherObjResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherObjResult


/*! \class OtherObjectiveResult
 *  \brief The OtherObjectiveResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that allows the solver to report an
 * arbitrary result associated with objective functions
 */
class OtherObjectiveResult
{
public:

    /** the number of objectives which have values for this
        particular type of result
     */
    int numberOfObj;

    /** the number of distinct values for this
        particular type of result
     */
    int numberOfEnumerations;

    /** the name of the result the user is defining */
    std::string name;

    /** this element allows a specific value associated with this
        particular type of result
     */
    std::string value;

    /** type of the result value (integer, double, boolean, string) */
    std::string type;

    /** a brief description of the type of result */
    std::string description;

    /** the solver of the result value */
    std::string solver;

    /** the category of the result value */
    std::string category;

    /* a pointer to OtherObjResult objects that will
     * give for each objective function the index and
     * value for this user defined objective function result
     */
    OtherObjResult** obj;

    /** type of the values in the obj array */
    std::string objType;

    /* a pointer to OtherOptionOrResultEnumeration objects that will
     * give for each distinct value the set of indices for
     * this user defined variable result
     */
    OtherOptionOrResultEnumeration** enumeration;

    /** type of the values in the enumeration array */
    std::string enumType;

    /**
     *
     * Default constructor.
     */
    OtherObjectiveResult();

    /**
     *
     * Class destructor.
     */
    ~OtherObjectiveResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherObjectiveResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherObjectiveResult



/*! \class ObjectiveSolution
 *  \brief The ObjectiveSolution Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for reporting all of the types of solution values
 * associated with objective functions.
 */
class ObjectiveSolution
{
public:

    /** the number of types of objective function results other
     * than the basic objective function values
     */
    int numberOfOtherObjectiveResults;

    /** a pointer to an array of ObjectiveValues objects */
    ObjectiveValues *values;

    /** a pointer to a BasisStatus object */
    BasisStatus *basisStatus;

    /** a pointer to an array of other pointer objects for
     * objective functions
     */
    OtherObjectiveResult** other;

    /**
     *
     * Default constructor.
     */
    ObjectiveSolution();

    /**
     *
     * Class destructor.
     */
    ~ObjectiveSolution();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ObjectiveSolution *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//ObjectiveSolution



/*! \class DualVarValue
 *  \brief The DualVarValue  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides the dual value of a constraint
 */
class DualVarValue
{
public:

    /** idx is the index on a constraint */
    int idx;

    /** optional name */
    std::string name;

    /** value of dual variable on
     * the constraint indexed by idx
     */
    double value;

    /**
     *
     * Default constructor.
     */
    DualVarValue();

    /**
     *
     * Class destructor.
     */
    ~DualVarValue();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(DualVarValue *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//DualVarValue



/*! \class DualVariableValues
 *  \brief The DualVariableValues Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for reporting dual variable values
 */
class DualVariableValues
{
public:

    /** record the number of constraints for which values are given
     */
    int numberOfCon;

    /** con is a vector of DualVarValue objects that
     * give an index and dual variable value for
     * each constraint function.
     */
    DualVarValue** con;

    /**
     *
     * Default constructor.
     */
    DualVariableValues();

    /**
     *
     * Class destructor.
     */
    ~DualVariableValues();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(DualVariableValues *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//DualVariableValues


/*! \class OtherConResult
 *  \brief The OtherConResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that provides general result information
 * for constraints.
 */
class OtherConResult
{
public:

    /** idx is the index on the constraint */
    int idx;

    /** optional name */
    std::string name;

    /** value is a value associated with the constraint
     * indexed by idx, for example value might be the
     * value of a dual variable or it might be the name
     * of the constraint.
     */
    std::string value;

    /**
     *
     * Default constructor.
     */
    OtherConResult();

    /**
     *
     * Class destructor.
     */
    ~OtherConResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherConResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherConResult


/*! \class OtherConstraintResult
 *  \brief The OtherConstraintResult  Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that allows the solver to report an
 * arbitrary result associated with constraints.
 */
class OtherConstraintResult
{
public:

    /** the number of constraints which have values for this
        particular type of result
     */
    int numberOfCon;

    /** the number of distinct values for this
        particular type of result
     */
    int numberOfEnumerations;

    /** the name of the result the user is defining */
    std::string name;

    /** this element allows a specific value associated with this
        particular type of result
     */
    std::string value;

    /** type of the result value (integer, double, boolean, string) */
    std::string type;

    /** a brief description of the type of result */
    std::string description;

    /** the solver of the result value */
    std::string solver;

    /** the category of the result value */
    std::string category;

    /* a vector of OtherConResult objects that will
     * give for each constraint the index and
     * value for this user defined objective function result
     */
    OtherConResult** con;

    /** type of the values in the con array */
    std::string conType;

    /* a pointer to OtherOptionOrResultEnumeration objects that will
     * give for each distinct value the set of indices for
     * this user defined variable result
     */
    OtherOptionOrResultEnumeration** enumeration;

    /** type of the values in the enumeration array */
    std::string enumType;

    /**
     *
     * Default constructor.
     */
    OtherConstraintResult();

    /**
     *
     * Class destructor.
     */
    ~OtherConstraintResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherConstraintResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherConstraintResult


/*! \class ConstraintSolution
 *  \brief The ConstraintSolution Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for reporting all of the types of solution values
 * associated with objective functions.
 */
class ConstraintSolution
{

public:

    /** the number of types of constraint function results other
     * than the basic constraint function values
     */
    int numberOfOtherConstraintResults;

    /** a pointer to an array of DualVariableValues objects */
    DualVariableValues *dualValues;

    /** a pointer to a BasisStatus object */
    BasisStatus *basisStatus;

    /** a pointer to an array of other pointer objects for
     * constraint functions
     */
    OtherConstraintResult** other;

    /**
     *
     * Default constructor.
     */
    ConstraintSolution();

    /**
     *
     * Class destructor.
     */
    ~ConstraintSolution();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ConstraintSolution *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//ConstraintSolution

/*! \class MatrixVariableValues
 * \brief The in-memory representation of the
 * <b><matrixVariables></b> element.
 */
class MatrixVariableValues
{
public:
    /** numberOfMatrixVar gives the number of <matrixVar> children */
    int numberOfMatrixVar;

    /** matrixVar is an array of pointers to the <matrixVar> children */
    OSMatrixWithMatrixVarIdx** matrixVar;

    /** The MatrixVariableValues class constructor */
    MatrixVariableValues();

    /** The MatrixVariableValues class destructor */
    ~MatrixVariableValues();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixVariableValues *that);
}; // MatrixVariableValues

/*! \class OtherMatrixVariableResult
 * \brief The in-memory representation of the
 * <b><matrixVariables> <other></b> element.
 */
class OtherMatrixVariableResult
{
public:
    /** Gives a name to this result. This is s mandatory data element */
    std::string name;

    /** other data elements are optional */
    std::string description;
    std::string value;
    std::string type;
    std::string solver;
    std::string category;

    /** number of matrix variables affected by or associated with this result */
    int numberOfMatrixVar;

    /** the type of matrixVar */
    std::string matrixType;
 
    /** the list of matrices and their values */
    OSMatrixWithMatrixVarIdx** matrixVar;
    
    /** number of levels in an enumeration associated with this result */
    int numberOfEnumerations;

    /** the type of the enumeration */
    std::string enumType;

    /** the enumeration. 
     *  Each enumeration level has a list of matrixVar indices associated with that level 
     */
    OtherOptionOrResultEnumeration** enumeration;

    /** The standard MatrixVariableValues class constructor */
    OtherMatrixVariableResult();

    /** Altername MatrixVariableValues class constructor */
    OtherMatrixVariableResult(std::string name_);

    /** The MatrixVariableValues class destructor */
    ~OtherMatrixVariableResult();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherMatrixVariableResult *that);
}; // OtherMatrixVariableResult


/*! \class MatrixVariableSolution
 * \brief The in-memory representation of the
 * <b><MatrixVariableSolution></b> element.
 */
class MatrixVariableSolution
{
public:
    /** numberOfOtherMatrixVariableResults gives the number of <other> children */
    int numberOfOtherMatrixVariableResults;

    /** values is pointer to the <values> child */
    MatrixVariableValues* values;

    /** other is a pointer to an array of <other> children */
    OtherMatrixVariableResult** other;

    /** The MatrixVariableSolution class constructor */
    MatrixVariableSolution();

    /** The MatrixVariableSolution class destructor */
    ~MatrixVariableSolution();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixVariableSolution *that);
}; // MatrixVariableSolution


/*! \class MatrixObjectiveSolution
 * \brief The in-memory representation of the
 * <b><MatrixVariableSolution></b> element.
 */
class MatrixObjectiveSolution
{
public:
    /** numberOfOtherMatrixObjectiveResults gives the number of <other> children */
    int numberOfOtherMatrixObjectiveResults;

    /** matrixObj is an array of pointers to the <matrixObj> children */
    OSMatrixWithMatrixObjIdx** matrixObj;

    /** The MatrixVariableSolution class constructor */
    MatrixObjectiveSolution();

    /** The MatrixVariableSolution class destructor */
    ~MatrixObjectiveSolution();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixObjectiveSolution *that);
}; // MatrixObjectiveSolution


/*! \class MatrixConstraintSolution
 * \brief The in-memory representation of the
 * <b><MatrixConstraintSolution></b> element.
 */
class MatrixConstraintSolution
{
public:
    /** numberOfOtherMatrixConstraintResults gives the number of <other> children */
    int numberOfOtherMatrixConstraintResults;

    /** matrixCon is an array of pointers to the <matrixCon> children */
    OSMatrixWithMatrixConIdx** matrixCon;

    /** The MatrixConstraintSolution class constructor */
    MatrixConstraintSolution();

    /** The MatrixConstraintSolution class destructor */
    ~MatrixConstraintSolution();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixConstraintSolution *that);
}; // MatrixConstraintSolution


class OtherSolutionResult; //forward declaration

/*! \class MatrixProgrammingSolution
 * \brief The in-memory representation of the 
   <b><MatrixProgrammingSolution></b> element.
 */
class MatrixProgrammingSolution
{
public:
    /** the number of <other> child elements> */
    int numberOfOtherMatrixProgrammingResults;

    /** a pointer to the array of <other> children */
    OtherSolutionResult** other;

    /** a pointer to the matrixVariables object */
    MatrixVariableSolution* matrixVariables;

    /** a pointer to the matrixObjectives object */
    MatrixObjectiveSolution* matrixObjectives;

    /** a pointer to the matrixConstraints object */
    MatrixConstraintSolution* matrixConstraints;


    /** The MatrixProgramming class constructor */
    MatrixProgrammingSolution();

    /** The MatrixProgramming class destructor */
    ~MatrixProgrammingSolution();

    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(MatrixProgrammingSolution *that);

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
    bool deepCopyFrom(MatrixProgrammingSolution *that);
}; // MatrixProgrammingSolution


/*! \class OtherSolutionResult
 *  \brief The OtherSolutionResult  Class.
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that allows the solver to report an
 * arbitrary result associated with the solution.
 */
class OtherSolutionResult
{
public:

    /** the name of the result the user is defining */
    std::string name;

    /** the value associated with the result the user is defining */
    std::string value;

    /** this element allows a specific category to be associated with this
        particular type of result
     */
    std::string category;

    /** a brief description of the type of result */
    std::string description;

    /** the number of items contained in this otherSolutionResult
     */
    int numberOfItems;

    /** an array of items (string-valued)
     */
    std::string *item;

    /**
     *
     * Default constructor.
     */
    OtherSolutionResult();

    /**
     *
     * Class destructor.
     */
    ~OtherSolutionResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherSolutionResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherSolutionResult




/*! \class OtherSolutionResults
 *  \brief The OtherSolutionResults  Class.
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that allows the solver to report an
 * arbitrary result associated with the solution.
 */
class OtherSolutionResults
{
public:

    /** the number of elements in the pointer of OtherSolutionResult objects */
    int numberOfOtherSolutionResults;

    /** otherSolutionResult is a pointer to an array of OtherSolutionResult
     * objects
     */
    OtherSolutionResult **otherSolutionResult;

    /**
     *
     * Default constructor.
     */
    OtherSolutionResults();

    /**
     *
     * Class destructor.
     */
    ~OtherSolutionResults();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherSolutionResults *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherSolutionResults



/*! \class OptimizationSolution
 *  \brief The OptimizationSolution Class.
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0




 *
 * \remarks
 * A class for reporting the various components of
 * an optimization solution.
 */
class OptimizationSolution
{
public:

    /** the index of the objective function for which we are
     *  reporting solution information
     */
    int targetObjectiveIdx;

    /** an optional name of the objective function for which we are
     *  reporting solution information
     */
    std::string targetObjectiveName;

    /** a marker to track whether the objectives are weighted */
    bool weightedObjectives;

    /** status is a pointer to an OptimizationSolutionStatus
     *  object associated with this optimization solution
     */
    OptimizationSolutionStatus *status;

    /** a message associated with this solution */
    std::string message;

    /** variables holds the solution information for
     *  the variables
     */
    VariableSolution *variables;
    /** constraints holds the solution information
     *  for the constraints
     */
    ConstraintSolution *constraints;

    /** objectives holds the solution information
     *  for the objectives
     */
    ObjectiveSolution *objectives;

    /** matrixProgramming holds the solution information
     *  for the matrix programming components: matrix variables, 
     *  matrix objectives and matrix constraints
     */
    MatrixProgrammingSolution *matrixProgramming;

    /** otherSolutionResults is a pointer to an OtherSolutionResults
    * object that is associated with this optimization solution
     */
    OtherSolutionResults *otherSolutionResults;

    /**
     *
     * Default constructor.
     */
    OptimizationSolution();

    /**
     *
     * Class destructor.
     */
    ~OptimizationSolution();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OptimizationSolution *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};// class OptimizationSolution


/*! \class SolverOutput
 *  \brief The SolverOutput  Class.
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that allows the solver to report an
 * arbitrary result associated with the solution.
 */
class SolverOutput
{
public:

    /** the name of the result the user is defining */
    std::string name;

    /** this element allows a specific category to be associated with this
        particular type of result
     */
    std::string category;

    /** a brief description of the type of result */
    std::string description;

    /** the number of items contained in this otherSolutionResult
     */
    int numberOfItems;

    /** an array of items (string-valued)
     */
    std::string *item;

    /**
     *
     * Default constructor.
     */
    SolverOutput();

    /**
     *
     * Class destructor.
     */
    ~SolverOutput();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(SolverOutput *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//SolverOutput



/*! \class OtherSolverOutput
 *  \brief The OtherSolverOutput Class.
 *
 * @author Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class that allows the solver to report an
 * arbitrary result associated with the solution.
 */
class OtherSolverOutput
{
public:

    /** the number of elements in the pointer of SolverOutput objects
     */
    int numberOfSolverOutputs;

    /** solverOutput is a pointer to an array of SolverOutput objects
     */
    SolverOutput **solverOutput;

    /**
     *
     * Default constructor.
     */
    OtherSolverOutput();

    /**
     *
     * Class destructor.
     */
    ~OtherSolverOutput();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherSolverOutput *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};//OtherSolverOutput



/*! \class OptimizationResult
 *  \brief The OptimizationResult Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for holding information that might be associated
 * with an optimization solution.
 */
class OptimizationResult
{
public:

    /** numberOfSolutions is the number of objective functions
     * reported.
     */
    int numberOfSolutions;

    /** numberOfVariables is the number of variables reported
     * in the solution.
     */
    int numberOfVariables;

    /** numberOfObjectives is the number of objective functions
     * reported in the solution.
     */
    int numberOfObjectives;

    /** numberOfConstrants is the number of constraint functions
     * reported in the solution.
     */
    int numberOfConstraints;

    /** solution is an array of pointers to OptimizationSolution
     * objects
     */
    OptimizationSolution **solution;

    /** otherSolverOutput is a pointer to an OtherSolverOutput
     *  object
     */
    OtherSolverOutput *otherSolverOutput;

    /**
     *
     * Default constructor.
     */
    OptimizationResult();

    /**
     *
     * Class destructor.
     */
    ~OptimizationResult();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OptimizationResult *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

};// class OptimizationResult



/*! \class OSResult
 *  \brief The Result Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 03/14/2004
 * @since OS 1.0
 *
 * \remarks
 * A class for holding all the solution information.
 */
class OSResult
{

public:

    /**
     * header information
     */
    GeneralFileHeader *resultHeader;

    /**
     * general holds the first child of the OSResult specified by the OSrL Schema.
     */
    GeneralResult *general;

    /**
     * system holds the second child of the OSResult specified by the OSrL Schema.
     */
    SystemResult *system;

    /**
     * service holds the third child of the OSResult specified by the OSrL Schema.
     */
    ServiceResult *service;

    /**
     * job holds the fourth child of the OSResult specified by the OSrL Schema.
     */
    JobResult *job;

    /**
     * optimization holds the fifth child of the OSResult specified by the OSrL Schema.
     */
    OptimizationResult *optimization;

    /**
     *
     * Default constructor.
     */
    OSResult();

    /**
     *
     * Class destructor.
     */
    ~OSResult();


    /**
     *
     * A function to populate an instance of the result header element
     * @param name: the name of this file or instance
     * @param source: the source (e.g., in BiBTeX format)

     * @param fileCreator: the creator of this file
     * @param description: further description about this file and/or its contents
     * @param licence: licence information if applicable
     */
    bool setHeader(std::string name, std::string source,
                         std::string fileCreator, std::string description, std::string licence);

public:

    /**
     * m_iVariableNumber holds the variable number.
     */
    int m_iVariableNumber;

    /**
     * m_iObjectiveNumber holds the objective number.
     */
    int m_iObjectiveNumber;

    /**
     * m_iConstraintNumber holds the constraint number.
     */
    int m_iConstraintNumber;

    /**
     * m_iNumberOfOtherVariableResults holds the number of OtherVariableResult objects.
     */
    int m_iNumberOfOtherVariableResults;

    /**
     * m_mdPrimalValues a vector of primal variables.
     */
    double *m_mdPrimalValues;

    /**
     * m_mdDualValues a vector of dual variables.
     */
    double *m_mdDualValues;


    std::vector<IndexValuePair*>  primalVals;

    std::vector<IndexValuePair*>  dualVals;


public:
    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OSResult *that);


    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);



public:

    /**
     * Get the general status.
     *
     * @return the general status.
     */
    GeneralStatus* getGeneralStatus();

    /**
     * Get the general status type, which can be:
     * success, error, warning.
     *
     * @return the general status type, null if none.
     */
    std::string getGeneralStatusType();

    /**
     * Get the general status description.
     *
     * @return the general status description, null or empty std::string if none.
     */
    std::string getGeneralStatusDescription();

    /**
     * Get the number of substatuses.
     *
     * @return the number of substatuses, -1 if general status does not exist.
     */
    int getNumberOfGeneralSubstatuses();

    /**
     * Get the i_th general substatus name
     *
     * @param i the number of the substatus (must be between 0 and numberOfSubstatuses)
     * @return the general substatus name, null if none.
     */
    std::string getGeneralSubstatusName(int i);

    /**
     * Get the i_th general substatus description.
     *
     * @param i the number of the substatus (must be between 0 and numberOfSubstatuses)
     * @return the general substatus description, null or empty std::string if none.
     */
    std::string getGeneralSubstatusDescription(int i);

    /**
     * Get the general message.
     *
     * @return the general message.
     */
    std::string getGeneralMessage();

    /**
     * Get service name.
     *
     * @return the service name.
     */
    std::string getServiceName();

    /**
     * Get service uri.
     *
     * @return the service uri.
     */
    std::string getServiceURI();

    /**
     * Get instance name.
     *
     * @return the instance name.
     */
    std::string getInstanceName();

    /**
     * Get the job id.
     *
     * @return the job id.
     */
    std::string getJobID();

    /**
     * Get the solver invoked.
     *
     * @return the solver invoked.
     */
    std::string getSolverInvoked();

    /**
     * Get the time stamp.
     *
     * @return the time stamp.
     */
    std::string getTimeStamp();

    /**
     *  Get the number of other results in the <general> element.
     *
     *  @return the number of other <general> results.
     */
    int getNumberOfOtherGeneralResults();

    /**
     *  Get the name of the i-th other result in the <general> element.
     *
     *  @param i holds the number of the result whose name is saught.
     *  @return the name of the other <general> result.
     */
    std::string getOtherGeneralResultName(int idx);

    /* */
    std::string getOtherGeneralResultValue(int idx);
    /* */
    std::string getOtherGeneralResultDescription(int idx);
    /* */
    std::string getSystemInformation();
    /* */
    std::string getAvailableDiskSpaceUnit();
    /* */
    std::string getAvailableDiskSpaceDescription();
    /* */
    double getAvailableDiskSpaceValue();
    /* */
    std::string getAvailableMemoryUnit();
    /* */
    std::string getAvailableMemoryDescription();
    /* */
    double getAvailableMemoryValue();
    /* */
    std::string getAvailableCPUSpeedUnit();
    /* */
    std::string getAvailableCPUSpeedDescription();
    /* */
    double getAvailableCPUSpeedValue();
    /* */
    std::string getAvailableCPUNumberDescription();
    /* */
    int getAvailableCPUNumberValue();
    /* */
    int getNumberOfOtherSystemResults();
    /* */
    std::string getOtherSystemResultName(int idx);
    /* */
    std::string getOtherSystemResultValue(int idx);
    /* */
    std::string getOtherSystemResultDescription(int idx);
    /* */
    std::string getCurrentState();
    /* */
    int getCurrentJobCount();
    /* */
    int getTotalJobsSoFar();
    /* */
    std::string getTimeServiceStarted();
    /* */
    double getServiceUtilization();
    /* */
    int getNumberOfOtherServiceResults();
    /* */
    std::string getOtherServiceResultName(int idx);
    /* */
    std::string getOtherServiceResultValue(int idx);
    /* */
    std::string getOtherServiceResultDescription(int idx);
    /* */
    std::string getJobStatus();
    /* */
    std::string getJobSubmitTime();
    /* */
    std::string getScheduledStartTime();
    /* */
    std::string getActualStartTime();
    /* */
    std::string getJobEndTime();

    /**
     *  Get the number of time measurements.
     *
     *  @return the number of time measurements
     */
    int getTimeNumber();

    /**
     *  Get the time measurement. In the first instance, assume that there is only
     *  a single measure, which is the total elapsed time in seconds
     *
     *  @return the time measurement
     */
    double getTimeValue();

    /* */
    int getNumberOfTimes();
    /* */
    std::string getTimingInfoUnit(int idx);
    /* */
    std::string getTimingInfoType(int idx);
    /* */
    std::string getTimingInfoCategory(int idx);
    /* */
    std::string getTimingInfoDescription(int idx);
    /* */
    double getTimingInfoValue(int idx);
    /* */
    std::string getUsedDiskSpaceUnit();
    /* */
    std::string getUsedDiskSpaceDescription();
    /* */
    double getUsedDiskSpaceValue();
    /* */
    std::string getUsedMemoryUnit();
    /* */
    std::string getUsedMemoryDescription();
    /* */
    double getUsedMemoryValue();
    /* */
    std::string getUsedCPUSpeedUnit();
    /* */
    std::string getUsedCPUSpeedDescription();
    /* */
    double getUsedCPUSpeedValue();
    /* */
    std::string getUsedCPUNumberDescription();
    /* */
    int getUsedCPUNumberValue();
    /* */
    int getNumberOfOtherJobResults();
    /* */
    std::string getOtherJobResultName(int idx);
    /* */
    std::string getOtherJobResultValue(int idx);
    /* */
    std::string getOtherJobResultDescription(int idx);

    /**
     * Get variable number.
     *
     * @return variable number, -1 if no information.
     */
    int getVariableNumber();

    /**
     * Get objective number.
     *
     * @return objective number, -1 if no information.
     */
    int getObjectiveNumber();

    /**
     * Get constraint number.
     *
     * @return constraint number, -1 if no information.
     */
    int getConstraintNumber();

    /**
     * get the number of solutions.
     *
     * @return the number of solutions, 0 if none.
     */
    int getSolutionNumber();


    /**
     * Get the [i]th optimization solution status, where i equals the given solution index.
     * The solution status includes the status type, optional descriptions and possibly substatuses.
     *
     * @param solIdx holds the solution index to get the solution status.
     * @return the optimization solution status that corresponds to solIdx, null if none.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OptimizationSolutionStatus
     */
    OptimizationSolutionStatus* getSolutionStatus( int solIdx);

    /**
     * Get the [i]th optimization solution status type, where i equals the given solution index.
     * The solution status type can be:
     * unbounded, globallyOptimal, locallyOptimal, optimal, bestSoFar, feasible, infeasible,
     * stoppedByLimit, unsure, error, other
     *
     * @param solIdx holds the solution index to get the solution status type.
     * @return the optimization solution status type that corresponds to solIdx, null or empty std::string if none.
     */
    std::string getSolutionStatusType(int solIdx);

    /**
     * Get the [i]th optimization solution status description, where i equals the given solution index.
     *
     * @param solIdx holds the solution index to get the solution status description.
     * @return the optimization solution status description that corresponds to solIdx, null or empty std::string if none.
     */
    std::string getSolutionStatusDescription(int solIdx);

    /* */
    int getNumberOfSolutionSubstatuses(int solIdx);
    /* */
    std::string getSolutionSubstatusType(int solIdx, int substatusIdx);
    /* */
    std::string getSolutionSubstatusDescription(int solIdx, int substatusIdx);
    /* */
    int getSolutionTargetObjectiveIdx(int solIdx);
    /* */
    std::string getSolutionTargetObjectiveName(int solIdx);

    /**
     * Get the [i]th optimization solution form of the objective
     *
     * @param solIdx holds the solution index to get the solution status description.
     * @return whether weighting is applied to the objective.
     */
    bool getSolutionWeightedObjectives(int solIdx);

    /**
     * Get the [i]th optimization solution message, where i equals the given solution index.
     *
     * @param solIdx holds the solution index to get the solution message.
     * @return the optimization solution message that corresponds to solIdx, null or empty if none.
     */
    std::string getSolutionMessage(int solIdx);

    /* */
    int getNumberOfPrimalVariableValues(int solIdx);
    /* */
    int getNumberOfVarValues(int solIdx);
    /* */
    int getVarValueIdx(int solIdx, int varIdx);
    /* */
    std::string getVarValueName(int solIdx, int varIdx);
    /* */
    double getVarValue(int solIdx, int varIdx);

    /**
     * Get one solution of optimal primal variable values.
     *
     * @param solIdx holds the solution index the optimal solution corresponds to.
     * @return a vector of variable indexes and values, an empty vector if no optimal value.
     */
    std::vector<IndexValuePair*> getOptimalPrimalVariableValues(int solIdx);

    /* */
    int getNumberOfVarValuesString(int solIdx);
    /* */
    int getVarValueStringIdx(int solIdx, int varIdx);
    /* */
    std::string getVarValueStringName(int solIdx, int varIdx);
    /* */
    std::string getVarValueString(int solIdx, int varIdx);

    /**
     * Get the number of indices that belong to a particular basis status
     *
     * @param solIdx holds the solution index for the current solution
     * @param object describes the kind of indices to be retrieved
     *	(legal values are described in ENUM_BASIS_STATUS --- see OSGeneral.h)
     * @param status gives the basis status type
     */
    int getBasisStatusNumberOfEl(int solIdx, int object, int status);

    /**
     * Get an entry in the array of indices that belong to a particular basis status
     *
     * @param solIdx holds the solution index for the current solution
     * @param object describes the kind of indices to be retrieved
     *	(legal values are described in ENUM_BASIS_STATUS --- see OSGeneral.h)
     * @param status gives the basis status (basic, atLower, atUpper, etc.)
     * @param j is the (zero-based) position of the desired entry within the index array
     */
    int getBasisStatusEl(int solIdx, int object, int status, int j);


    /** Get the basis information associated with the variables, objectives or constraints
     *  for some solution
     *  @param solIdx is the solution index
     *  @param object describes the kind of indices to be retrieved
     *	 (legal values are described in ENUM_PROBLEM_COMPONENT --- see OSGeneral.h)
     *  @param resultArray is the array that returns the basis information
     *  @param dim is the dimension of the resultArray
     *
     *  @return whether the operation was successful: 
     *     < 0: error condition 
     *     = 0: no data encountered
     *     > 0: success
     */
    int getBasisInformationDense(int solIdx, int object, int* resultArray, int dim);

    /**
     * Get numberOfOtherVariableResult.
     *
     * @return numberOfOtherVariableResult, -1 if no information.
     */
    int getNumberOfOtherVariableResults( int solIdx);

    /**
     * Get getAnOtherVariableResultNumberOfVar.
     *
     * @return the number of variables in the i'th other variable result, -1 if no information.
     */
    int getAnOtherVariableResultNumberOfVar(int solIdx, int iOther);

    /* */
    std::string getOtherVariableResultName(int solIdx, int otherIdx);
    /* */
    std::string getOtherVariableResultType(int solIdx, int otherIdx);
    /* */
    std::string getOtherVariableResultValue(int solIdx, int otherIdx);
    /* */
    std::string getOtherVariableResultDescription(int solIdx, int otherIdx);
    /* */
    int getOtherVariableResultNumberOfVar(int solIdx, int otherIdx);
    /* */
    int getOtherVariableResultNumberOfEnumerations(int solIdx, int otherIdx);
    /* */
    int getOtherVariableResultVarIdx(int solIdx, int otherIdx, int varIdx);
    /* */
    std::string getOtherVariableResultVar(int solIdx, int otherIdx, int varIdx);

    /** Get the type of values contained in the <var> or <enumeration> array
     *  associated with an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *
     * @return the array type
     */
    std::string getOtherVariableResultArrayType(int solIdx,int otherIdx);

    /** Get the value of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     *
     * @return the value
     */
    std::string getOtherVariableResultEnumerationValue(int solIdx,int otherIdx, int enumIdx);

    /** Get the description of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     *
     * @return the description
     */
    std::string getOtherVariableResultEnumerationDescription(int solIdx,int otherIdx, int enumIdx);

    /** Get the size of an enum associated with an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *  @enumIdx is the index of the current enumeration level
     *
     *  @return the number of indices that assume this level
     */
    int  getOtherVariableResultEnumerationNumberOfEl(int solIdx, int otherIdx, int enumIdx);

    /** Get one index of an enum associated with an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *  @param enumIdx is the index of the current enumeration level
     *  @param j is the (zero-based) position of the index within the index array
     *
     *  @return the array of indices
     */
    int getOtherVariableResultEnumerationEl(int solIdx,int otherIdx, int enumIdx, int j);

    /** Get the values of a <var> array or an <enumeration> associated with 
     *  an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *  @param resultArray is the array that returns the content of the <var> or <enumeration> array
     *  @param dim is the array dimension
     *
     *  @return whether the operation was successful: 
     *     < 0: error condition 
     *     = 0: no data encountered
     *     > 0: number of data items set
     */
    int getOtherVariableResultArrayDense(int solIdx, int otherIdx, std::string* resultArray, int dim);


    /* */
    int getNumberOfObjValues(int solIdx);
    /* */
    int getObjValueIdx(int solIdx, int objIdx);
    /* */
    std::string getObjValueName(int solIdx, int objIdx);
    /* */
    double getObjValue(int solIdx, int objIdx);

    /**
     * Get one solution's optimal objective value.
     *
     * @param objIdx holds the objective index the optimal value corresponds to.
     * @param solIdx holds the solution index the optimal value corresponds to.
     * @return a double with the optimal objective function value.
     */
    double getOptimalObjValue(int objIdx, int solIdx);

    /* */
    int getNumberOfOtherObjectiveResults(int solIdx);
    /* */
    std::string getOtherObjectiveResultName(int solIdx, int otherIdx);
    /* */
    std::string getOtherObjectiveResultType(int solIdx, int otherIdx);
    /* */
    std::string getOtherObjectiveResultValue(int solIdx, int otherIdx);
    /* */
    std::string getOtherObjectiveResultDescription(int solIdx, int otherIdx);
    /* */
    int getOtherObjectiveResultNumberOfObj(int solIdx, int otherIdx);
    /* */
    int getOtherObjectiveResultNumberOfEnumerations(int solIdx, int otherIdx);
    /* */
    int getOtherObjectiveResultObjIdx(int solIdx, int otherIdx, int objIdx);
    /* */
    std::string getOtherObjectiveResultObj(int solIdx, int otherIdx, int objIdx);

    /** Get the type of values contained in the <obj> or <enumeration> array
     *  associated with an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *
     * @return the array type
     */
    std::string getOtherObjectiveResultArrayType(int solIdx,int otherIdx);

    /** Get the value of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     *
     * @return the value
     */
    std::string getOtherObjectiveResultEnumerationValue(int solIdx,int otherIdx, int enumIdx);

    /** Get the description of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     *
     * @return the description
     */
    std::string getOtherObjectiveResultEnumerationDescription(int solIdx,int otherIdx, int enumIdx);

    /** Get the size of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     *
     * @return the number of indices that assume this level
     */
    int  getOtherObjectiveResultEnumerationNumberOfEl(int solIdx,int otherIdx, int enumIdx);

    /** Get one index of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     * @param j is the (zero-based) position of the index in the array
     *
     * @return the array of indices
     */
    int getOtherObjectiveResultEnumerationEl(int solIdx,int otherIdx, int enumIdx, int j);

    /** Get the values of an <obj> array or an <enumeration> associated with 
     *  an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *  @param resultArray is the array that returns the content of the <obj> or <enumeration> array
     *  @param dim is the array dimension
     *
     *  @return whether the operation was successful: 
     *     < 0: error condition 
     *     = 0: no data encountered
     *     > 0: number of data items set
     */
    int getOtherObjectiveResultArrayDense(int solIdx, int otherIdx, std::string* resultArray, int dim);


    /* */
    int getNumberOfDualValues(int solIdx);
    /* */
    int getDualValueIdx(int solIdx, int conIdx);
    /* */
    std::string getDualValueName(int solIdx, int objIdx);
    /* */
    double getDualValue(int solIdx, int conIdx);

    /**
     * Get one solution of optimal dual variable values.
     *
     * @param solIdx holds the solution index the optimal solution corresponds to.
     * @return a vector of variable indexes and values, an empty vector if no optimal value.
     */
    std::vector<IndexValuePair*> getOptimalDualVariableValues(int solIdx);

    /* */
    int getNumberOfOtherConstraintResults(int solIdx);
    /* */
    std::string getOtherConstraintResultName(int solIdx, int otherIdx);
    /* */
    std::string getOtherConstraintResultType(int solIdx, int otherIdx);
    /* */
    std::string getOtherConstraintResultValue(int solIdx, int otherIdx);
    /* */
    std::string getOtherConstraintResultDescription(int solIdx, int otherIdx);
    /* */
    int getOtherConstraintResultNumberOfCon(int solIdx, int otherIdx);
    /* */
    int getOtherConstraintResultNumberOfEnumerations(int solIdx, int otherIdx);
    /* */
    int getOtherConstraintResultConIdx(int solIdx, int otherIdx, int conIdx);
    /* */
    std::string getOtherConstraintResultCon(int solIdx, int otherIdx, int conIdx);


    /** Get the type of values contained in the <con> or <enumeration> array
     *  associated with an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *
     *  @return the array type
     */
    std::string getOtherConstraintResultArrayType(int solIdx,int otherIdx);

    /** Get the value of an enum associated with an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *  @param enumIdx is the index of the current enumeration level
     *
     *  @return the value
     */
    std::string getOtherConstraintResultEnumerationValue(int solIdx,int otherIdx, int enumIdx);

    /** Get the description of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     *
     * @return the description
     */
    std::string getOtherConstraintResultEnumerationDescription(int solIdx,int otherIdx, int enumIdx);

    /** Get the size of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     *
     * @return the number of indices that assume this level
     */
    int  getOtherConstraintResultEnumerationNumberOfEl(int solIdx,int otherIdx, int enumIdx);

    /** Get one index of an enum associated with an <other> result for some solution
     * @param solIdx is the solution index
     * @param otherIndex is the index of the current <other> result
     * @enumIdx is the index of the current enumeration level
     * @param j is the (zeo-based) position of the entry in the array
     *
     * @return the array of indices
     */
    int getOtherConstraintResultEnumerationEl(int solIdx,int otherIdx, int enumIdx, int j);

    /** Get the values of a <con> array or an <enumeration> associated with 
     *  an <other> result for some solution
     *  @param solIdx is the solution index
     *  @param otherIndex is the index of the current <other> result
     *  @param resultArray is the array that returns the content of the <con> or <enumeration> array
     *  @param dim is the array dimension
     *
     *  @return whether the operation was successful: 
     *     < 0: error condition 
     *     = 0: no data encountered
     *     > 0: number of data items set
     */



    int getOtherConstraintResultArrayDense(int solIdx, int otherIdx, std::string* resultArray, int dim);



    /* */
    int getNumberOfOtherSolutionResults(int solIdx);
    /* */
    std::string getOtherSolutionResultName(int solIdx, int otherIdx);
    /* */
    std::string getOtherSolutionResultValue(int solIdx, int otherIdx);
    /* */
    std::string getOtherSolutionResultCategory(int solIdx, int otherIdx);
    /* */
    std::string getOtherSolutionResultDescription(int solIdx, int otherIdx);
    /* */
    int getOtherSolutionResultNumberOfItems(int solIdx, int otherIdx);
    /* */
    std::string getOtherSolutionResultItem(int solIdx, int otherIdx, int itemIdx);
    /* */
    int getNumberOfSolverOutputs();
    /* */
    std::string getSolverOutputName(int otherIdx);
    /* */
    std::string getSolverOutputCategory(int otherIdx);
    /* */
    std::string getSolverOutputDescription(int otherIdx);
    /* */
    int getSolverOutputNumberOfItems(int otherIdx);
    /* */
    std::string getSolverOutputItem(int otherIdx, int itemIdx);

    // set() methods
    //

    /**
     * Set the general status
     *
     * @param status holds the general status.
     * @return whether the general status is set successfully.
     */
    bool setGeneralStatus(GeneralStatus *status);

    /**
     * Set the general status type, which can be:
     * success, error, warning.
     *
     * @param type holds the general status type
     * @return whether the general status type is set successfully or not.
     */
    bool setGeneralStatusType(std::string type);

    /**
     * Set the number of substatus elements
     *
     * @param num holds the number of substatuses (a nonegative integer)
     * @return whether the number of substatuses is set successfully or not.
     */
    bool setNumberOfGeneralSubstatuses(int num);

    /**
     * Set the general status description.
     *
     * @param description holds the general status description.
     * @return whether the general status description is set successfully or not.
     */
    bool setGeneralStatusDescription(std::string description);

    /**
     * Set the general substatus name
     *
     * @param name holds the general substatus name
     * @param idx holds the index of the substatus in the array
     * @return whether the general substatus name is set successfully or not.
     */
    bool setGeneralSubstatusName(int idx, std::string name);

    /**
     * Set the general substatus description.
     *
     * @param description holds the general substatus description.
     * @param idx holds the index of the substatus in the array
     * @return whether the general status description is set successfully or not.
     */
    bool setGeneralSubstatusDescription(int idx, std::string description);

    /**
     * Set the general message.
     *
     * @param message holds the general message.
     * @return whether process message is set successfully.
     */
    bool setGeneralMessage(std::string message);

    /**
     * Set service name.
     *
     * @param serviceName holds the name of the service.
     * @return whether the service name is set successfully.
     */
    bool setServiceName(std::string serviceName);

    /**
     * Set service uri.
     *
     * @param serviceURI holds the uri of the service.
     * @return whether the service uri is set successfully.
     */
    bool setServiceURI(std::string serviceURI);

    /**
     * Set instance name.
     *
     * @param instanceName holds the name of the instance.
     * @return whether the instance name is set successfully.
     */
    bool setInstanceName(std::string instanceName);

    /**
     * Set job id.
     *
     * @param jobID holds the job id.
     * @return whether the job id is set successfully.
     */
    bool setJobID(std::string jobID);

    /**
     * Set solver invoked.
     *
     * @param solverInvoked holds the solver invoked.
     * @return whether the solver invoked is set successfully.
     */
    bool setSolverInvoked(std::string solverInvoked);

    /**
     * Set time stamp.
     *
     * @param time holds the time stamp.
     * @return whether the time stamp is set successfully.
     */
    bool setTimeStamp(std::string timeStamp);

    /**
     * Set number of other general results.
     *
     * @param num holds the number of other general results.
     * @return whether the number was set successfully.
     */
    bool setNumberOfOtherGeneralResults(int num);

    /**
     * Set the general otherResult name
     *
     * @param name holds the general otherResult name
     * @param idx holds the index of the otherResult in the array
     * @return whether the general otherResult name is set successfully or not.
     */
    bool setOtherGeneralResultName(int idx, std::string name);

    /**
     * Set the general otherResult value
     *
     * @param name holds the general otherResult value
     * @param idx holds the index of the otherResult in the array
     * @return whether the general otherResult value is set successfully or not.
     */
    bool setOtherGeneralResultValue(int idx, std::string value);

    /**
     * Set the general otherResult description
     *
     * @param name holds the general otherResult description
     * @param idx holds the index of the otherResult in the array
     * @return whether the general otherResult description is set successfully or not.
     */
    bool setOtherGeneralResultDescription(int idx, std::string description);

    /**
     * Set the system information
     *
     * @param systemInformation holds the system information
     * @return whether the system information was set successfully or not.
     */
    bool setSystemInformation(std::string systemInformation);

    /**
     * Set the unit in which available disk space is measured
     *
     * @param unit holds unit (byte, kilobyte, megabtye, gigabyte, terabyte, petabyte)
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableDiskSpaceUnit(std::string unit);

    /**
     * Set the description of available disk space
     *
     * @param description holds further information about available disk space
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableDiskSpaceDescription(std::string description);

    /**
     * Set the amount of available disk space
     *
     * @param value holds the number of disk space units
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableDiskSpaceValue(double value);

    /**
     * Set the unit in which available memory is measured
     *
     * @param unit holds unit (byte, kilobyte, megabtye, gigabyte, terabyte)
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableMemoryUnit(std::string unit);

    /**
     * Set the description of available memory
     *
     * @param description holds further information about available memory
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableMemoryDescription(std::string description);

    /**
     * Set the amount of available memory
     *
     * @param value holds the number of memory units
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableMemoryValue(double value);

    /**
     * Set the unit in which available CPU speed is measured
     *
     * @param unit holds unit
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableCPUSpeedUnit(std::string unit);

    /**
     * Set the description of available CPU speed
     *
     * @param description holds further information about the CPU speed
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableCPUSpeedDescription(std::string description);

    /**
     * Set the available CPU speed
     *
     * @param value holds the available CPU speed
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableCPUSpeedValue(double value);

    /**
     * Set the description of available number of CPUs
     *
     * @param description is used to impart further info about the CPUs
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableCPUNumberDescription(std::string description);

    /**
     * Set the available number of CPUs
     *
     * @param value holds the available number of CPUs
     * @return whether the system information was set successfully or not.
     */
    bool setAvailableCPUNumberValue(int value);

    /**
     * Set number of other system results.
     *
     * @param num holds the number of other system results.
     * @return whether the number was set successfully.
     */
    bool setNumberOfOtherSystemResults(int num);

    /**
     * Set the system otherResult name
     *
     * @param name holds the system otherResult name
     * @param idx holds the index of the otherResult in the array
     * @return whether the system otherResult name is set successfully or not.
     */
    bool setOtherSystemResultName(int idx, std::string name);

    /**
     * Set the system otherResult value
     *
     * @param name holds the system otherResult value
     * @param idx holds the index of the otherResult in the array
     * @return whether the system otherResult value is set successfully or not.
     */
    bool setOtherSystemResultValue(int idx, std::string value);

    /**
     * Set the system otherResult description
     *
     * @param name holds the system otherResult description
     * @param idx holds the index of the otherResult in the array
     * @return whether the system otherResult description is set successfully or not.
     */
    bool setOtherSystemResultDescription(int idx, std::string description);

    /**
     * Set the current state of the service
     *
     * @param currentState holds the current state
     * @return whether the service information was set successfully or not.
     */
    bool setCurrentState(std::string currentState);

    /**
     * Set the current job count
     *
     * @param jobCount holds the current job count
     * @return whether the service information was set successfully or not.
     */
    bool setCurrentJobCount(int jobCount);

    /**
     * Set the total number of jobs so far
     *
     * @param number holds the total number of jobs
     * @return whether the service information was set successfully or not.
     */
    bool setTotalJobsSoFar(int number);

    /**
     * Set the time the service was started
     *
     * @param startTime holds the starting time
     * @return whether the service information was set successfully or not.
     */
    bool setTimeServiceStarted(std::string startTime);

    /**
     * Set the service utilitzation
     *
     * @param value holds the service utilitzation

     * @return whether the service information was set successfully or not.
     */
    bool setServiceUtilization(double value);


    /**
     * Set number of other service results.
     *
     * @param num holds the number of other service results.
     * @return whether the number was set successfully.
     */
    bool setNumberOfOtherServiceResults(int num);

    /**
     * Set the service otherResult name
     *
     * @param name holds the service otherResult name
     * @param idx holds the index of the otherResult in the array
     * @return whether the service otherResult name is set successfully or not.
     */
    bool setOtherServiceResultName(int idx, std::string name);

    /**
     * Set the service otherResult value
     *
     * @param name holds the service otherResult value
     * @param idx holds the index of the otherResult in the array
     * @return whether the service otherResult value is set successfully or not.
     */
    bool setOtherServiceResultValue(int idx, std::string value);

    /**
     * Set the service otherResult description
     *
     * @param name holds the service otherResult description
     * @param idx holds the index of the otherResult in the array
     * @return whether the service otherResult description is set successfully or not.
     */
    bool setOtherServiceResultDescription(int idx, std::string description);


    /**
     * Set the job status
     *
     * @param status holds the job status
     * @return whether the job status was set successfully or not.
     */
    bool setJobStatus(std::string status);

    /**
     * Set the time when the job was submitted
     *
     * @param submitTime holds the time when the job was submitted
     * @return whether the information was set successfully or not.
     */
    bool setJobSubmitTime(std::string submitTime);

    /**
     * Set the job's scheduled start time
     *
     * @param scheduledStartTime holds the scheduled start time
     * @return whether the information was set successfully or not.
     */
    bool setScheduledStartTime(std::string scheduledStartTime);

    /**
     * Set the job's actual start time
     *
     * @param actualStartTime holds the actual start time
     * @return whether the information was set successfully or not.
     */
    bool setActualStartTime(std::string actualStartTime);

    /**
     * Set the time when the job finished
     *
     * @param endTime holds the time when the job finished
     * @return whether the information was set successfully or not.
     */
    bool setJobEndTime(std::string endTime);


    /**
     * Set time.
     *
     * @param time holds the time.
     * @return whether the time is set successfully.
     */
    bool setTime(double time);

    /**
     * Add timing information.
     *
     * @param type holds the timer type (cpuTime/elapsedTime/other).
     * @param category holds the timer category (total/input/preprocessing, etc.)
     * @param unit holds the timer unit (tick/milliscond/second/minute/etc.)
     * @param description holds further information about the timer.
     * @param value holds the time measurement.
     * @return whether the time is set successfully.
     */
    bool addTimingInformation(std::string type, std::string category,
                              std::string unit, std::string description, double value);

    /**
     * Set timing information.
     *
     * @param idx holds the index within the time array of the item to be set
     * @param type holds the timer type (cpuTime/elapsedTime/other).
     * @param category holds the timer category (total/input/preprocessing, etc.)
     * @param unit holds the timer unit (tick/milliscond/second/minute/etc.)
     * @param description holds further information about the timer.

     * @param value holds the time measurement.
     * @return whether the time is set successfully.
     */
    bool setTimingInformation(int idx, std::string type, std::string category,
                              std::string unit, std::string description, double value);

    /**
     * Set the number of time measurements and initial the time array
     *
     * @param numberOfTimes holds the number of measurements
     * @return whether the function completed successfully or not.
     */
    bool setNumberOfTimes(int numberOfTimes);

    /**
     * Set the number of time measurements.
     *
     * @param timeNumber holds the number of measurements
     * @return whether the time number is set successfully or not.
     */
    bool setTimeNumber(int timeNumber);

    /**
     * Set the unit in which used disk space is measured
     *
     * @param unit holds unit (byte, kilobyte, megabtye, gigabyte, terabyte, petabyte)
     * @return whether the information was set successfully or not.
     */
    bool setUsedDiskSpaceUnit(std::string unit);

    /**
     * Set the description of used disk space
     *
     * @param description holds further information about used disk space
     * @return whether the information was set successfully or not.
     */
    bool setUsedDiskSpaceDescription(std::string description);

    /**
     * Set the amount of used disk space
     *
     * @param value holds the number of disk space units
     * @return whether the information was set successfully or not.
     */
    bool setUsedDiskSpaceValue(double value);

    /**
     * Set the unit in which used memory is measured
     *
     * @param unit holds unit (byte, kilobyte, megabtye, gigabyte, terabyte)
     * @return whether the information was set successfully or not.
     */
    bool setUsedMemoryUnit(std::string unit);

    /**
     * Set the description of used memory
     *
     * @param description holds further information about used memory
     * @return whether the information was set successfully or not.
     */
    bool setUsedMemoryDescription(std::string description);

    /**
     * Set the amount of used memory
     *
     * @param value holds the number of memory units
     * @return whether the information was set successfully or not.
     */
    bool setUsedMemoryValue(double value);

    /**
     * Set the unit in which used CPU speed is measured
     *
     * @param unit holds unit
     * @return whether the information was set successfully or not.
     */
    bool setUsedCPUSpeedUnit(std::string unit);

    /**
     * Set the description of used CPU speed
     *
     * @param description holds further information about the CPU speed
     * @return whether the information was set successfully or not.
     */
    bool setUsedCPUSpeedDescription(std::string description);

    /**
     * Set the used CPU speed
     *
     * @param value holds the used CPU speed
     * @return whether the information was set successfully or not.
     */
    bool setUsedCPUSpeedValue(double value);

    /**
     * Set the description of used number of CPUs
     *
     * @param description is used to impart further info about the CPUs
     * @return whether the system information was set successfully or not.
     */
    bool setUsedCPUNumberDescription(std::string description);

    /**
     * Set the used number of CPUs
     *
     * @param value holds the used number of CPUs
     * @return whether the information was set successfully or not.
     */
    bool setUsedCPUNumberValue(int value);

    /**
     * Set number of other job results.
     *
     * @param num holds the number of other job results.
     * @return whether the number was set successfully.
     */
    bool setNumberOfOtherJobResults(int num);

    /**
     * Set the job otherResult name
     *
     * @param name holds the job otherResult name
     * @param idx holds the index of the otherResult in the array
     * @return whether the job otherResult name is set successfully or not.
     */
    bool setOtherJobResultName(int idx, std::string name);

    /**
     * Set the job otherResult value
     *
     * @param name holds the job otherResult value
     * @param idx holds the index of the otherResult in the array
     * @return whether the job otherResult value is set successfully or not.
     */
    bool setOtherJobResultValue(int idx, std::string value);

    /**
     * Set the job otherResult description
     *
     * @param name holds the job otherResult description
     * @param idx holds the index of the otherResult in the array
     * @return whether the job otherResult description is set successfully or not.
     */
    bool setOtherJobResultDescription(int idx, std::string description);


    /**
     * Set the variable number.
     *
     * @param variableNumber holds the number of variables
     * @return whether the variable number is set successfully or not.
     */
    bool setVariableNumber(int variableNumber);

    /**
     * Set the objective number.
     *
     * @param objectiveNumber holds the number of objectives
     * @return whether the objective number is set successfully or not.
     */
    bool setObjectiveNumber(int objectiveNumber);

    /**
     * Set the constraint number.
     *
     * @param constraintNumber holds the number of constraints
     * @return whether the constraint number is set successfully or not.
     */
    bool setConstraintNumber(int constraintNumber); 

    /**
     * set the number of solutions. This method must be called before setting other optimization solution
     * related results.
     * Before this method is called, the setVariableNumber(int), setObjectiveNumber(int),
     * setConstraintNumber(int) methods have to be called first.
     *
     * @param number holds the number of solutions to set.
     * @return whether the solution number is set successfully.
     * @see #setVariableNumber(int)
     * @see #setObjectiveNumber(int)
     * @see #setConstraintNumber(int)
     */
    bool setSolutionNumber(int number);


    /**
     * Set the [i]th optimization solution status, where i equals the given solution index.
     * The solution status includes the status type, optional descriptions and possibly substatuses.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     *
     * @param solIdx holds the solution index to set the solution status.
     * @param status holds the optimization solution status to set.
     *
     * @return whether the optimization solution status is set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OptimizationSolutionStatus
     * @see #setSolutionNumber(int)
     */
    bool setSolutionStatus(int solIdx, std::string type, std::string description);

    /**
     * Set the [i]th optimization solution status type
     *
     * @param solIdx holds the solution index whose status to set.
     * @param type holds the solution status type
     * @return whether the solution status type is set successfully or not.
     */
    bool setSolutionStatusType(int solIdx, std::string type);

    /**
     * Set the [i]th optimization solution's number of substatus elements
     *
     * @param solIdx holds the solution index whose status to set.
     * @param num holds the number of substatuses (a nonegative integer)
     * @return whether the number of substatuses is set successfully or not.
     */
    bool setNumberOfSolutionSubstatuses(int solIdx, int num);

    /**
     * Set the [i]th optimization solution status description.
     *
     * @param solIdx holds the solution index whose status to set.
     * @param description holds the solution status description.
     * @return whether the solution status description is set successfully or not.
     */
    bool setSolutionStatusDescription(int solIdx, std::string description);

    /**
     * Set the solution substatus type
     *
     * @param solIdx holds the solution index whose status to set.
     * @param substatusIdx holds the index of the substatus in the array
     * @param type holds the general substatus type
     * @return whether the general substatus type is set successfully or not.
     */
    bool setSolutionSubstatusType(int solIdx, int substatusIdx, std::string type);

    /**
     * Set the solution substatus description.
     *
     * @param solIdx holds the solution index whose status to set.
     * @param substatusIdx holds the index of the substatus in the array
     * @param description holds the general substatus description.
     * @return whether the solution status description is set successfully or not.
     */
    bool setSolutionSubstatusDescription(int solIdx, int substatusIdx, std::string description);


    /**
     * Set the [i]th optimization solution's objective index, where i equals the given solution index.
     * The first objective's index should be -1, the second -2, and so on.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.

     *
     * @param solIdx holds the solution index to set the objective index.
     * @param objectiveIdx holds the objective index to set.
     * All the objective indexes are negative starting from -1 downward.
     *
     * @return whether the optimization objective index is set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setSolutionTargetObjectiveIdx(int solIdx, int objectiveIdx);

    /**
     * Set the [i]th optimization solution's objective name, where i equals the given solution index.
     * The first objective's index should be -1, the second -2, and so on.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     *
     * @param solIdx holds the solution index to set the objective index.
     * @param objectiveName holds the objective indexname to set.
     *
     * @return whether the optimization objective name is set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setSolutionTargetObjectiveName(int solIdx, std::string objectiveName);

    /**
     * Record whether the [i]th optimization solution uses weighted objectives,
     * where i equals the given solution index.
     *
     * @param solIdx holds the solution index to set the objective index.
     * @param weightedObjectives holds the value "true" or "false".
     *
     * @return whether the information was set successfully or not.
     */
    bool setSolutionWeightedObjectives(int solIdx, bool weightedObjectives);

    /**
     * Set the [i]th optimization solution's message, where i equals the given solution index.
     * The first objective's index should be -1, the second -2, and so on.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     *
     * @param solIdx holds the solution index to set the objective index.
     * @param msg holds the solution message to set.
     *
     * @return whether the optimization objective index is set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setSolutionMessage(int solIdx, std::string msg);

    /**
     * Set the [i]th optimization solution's number of primal variable values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the primal variable values.
     * @param n holds the number of elements in the array x
     *
     * @return whether primal variable values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfPrimalVariableValues(int solIdx, int n);

    /**
     * Set the [i]th optimization solution's primal variable values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the primal variable values.
     * @param x holds a vector of type IndexValuePair; the idx component holds the index of the variable;
     * the value component holds its value. The vector could be null if all variables are 0.
     *
     * @return whether primal variable values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setPrimalVariableValuesSparse(int solIdx, std::vector<IndexValuePair*> x);

    /**
     * Set the [i]th optimization solution's primal variable values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the primal variable values.
     * @param x holds a double dense array of variable values to set; it could be null if all variables are 0.
     *
     * @return whether primal variable values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setPrimalVariableValuesDense(int solIdx, double *x);

    /**
     * Set the number of primal variables to be given a value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the primal variable values.
     * @param numberOfVar holds the number of primal variables that are to be set
     *
     * @return whether the information was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfVarValues(int solIdx, int numberOfVar);

    /**
     * Set a primal variable value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the primal variable values.
     * @param number holds the location within the sparse array var that is to be used
     * @param idx holds the index of the primal variable that is to be set
     * @param name holds the variable name (or an empty string).
     * @param val holds the variable value to set.
     *
     * @return whether primal variable value was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setVarValue(int solIdx, int number, int idx, std::string name, double val);

    /**
     * Set the number of string-valued primal variables to be given a value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the primal variable values.
     * @param numberOfVar holds the number of primal variables that are to be set
     *
     * @return whether the information was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfVarValuesString(int solIdx, int numberOfVar);

    /**
     * Set a string-valued primal variable value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the primal variable values.
     * @param number holds the location within the sparse array var that is to be used
     * @param idx holds the index of the primal variable that is to be set
     * @param name holds the variable name (or an empty string).
     * @param str holds the variable value to set.
     *
     * @return whether primal variable value was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setVarValueString(int solIdx, int number, int idx, std::string name, std::string str);


    /**
     * Set the basis status of a number of variables/constraints/objectives.
     * @param solIdx holds the index of the solution to which the basis values belong.
     * @param object holds the type of basis object to be used
     *     (legal values are taken from the ENUM_PROBLEM_COMPONENT enumeration --- see OSGeneral.h))
     * @param status holds the status which is to be used
     *     (legal values are taken from the ENUM_BASIS_STATUS enumeration --- see OSGeneral.h)
     * @param i holds the integer array whose values are to be transferred.

     *     (NOTE WELL: This method does not handle individual variables --- the entire basis must be processed at once.)
     * @param ni holds the number of elements of i
     *
     * @return whether basis status was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setBasisStatus(int solIdx, int object, int status, int *i, int ni);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific) variable-related results,
     * where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * This method then allocates the memory for the new OtherVariableResult objects
     * @param solIdx is the solution index
     * @param numberOfOtherVariableResult holds the number of OtherVariableResult objects
     * Each other variable result contains the name (required), an optional description (std::string) and an optional
     * value (std::string). Each other variable result can also optionally contain an array OtherVarResult for each variable.
     * The OtherVarResult contains a variable idx (required) and an optional std::string value.
     *
     * @return whether the other variable results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfOtherVariableResults(int solIdx, int numberOfOtherVariableResults);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific)variable-related results,
     * where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the new OtherVariableResult object
     * @param name holds the name of the other element
     * @param value holds the value of the other element
     * @param idx holds a pointer to the indexes of the var element
     * @param s holds a pointer to the array of values of the var element
     * @param n holds the number of elements of the array
     *
     * @return whether the other variable results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setAnOtherVariableResultSparse(int solIdx, int otherIdx, std::string name, std::string value, std::string description, int *idx,  std::string *s, int n);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific)variable-related results,
     * where i equals the given solution index.
     *
     * This alternate signature sets the type of the value attribute and the <var> and <enumeration> arrays
     *
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the new OtherVariableResult object
     * @param name holds the name of the other element
     * @param value holds the value of the other element
     * @param idx holds a pointer to the indexes of the var element
     * @param s holds a pointer to the array of values of the var element
     * @param n holds the number of elements of the array
     * @param type holds the type of the <other> element's value attribute
     * @param varType holds the type of the <other> element's <var> array
     * @param enumType holds the type of the <other> element's <enumeration> array
     *
     * @return whether the other variable results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setAnOtherVariableResultSparse(int solIdx, int otherIdx, std::string name, std::string value, std::string description, int *idx,  std::string *s, int n, std::string type, std::string varType, std::string enumType);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific)variable-related results,
     * where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the new OtherVariableResult object
     * @param name holds the name of the other element
     * @param value holds the value of the other element
     * @param s holds a pointer to the array of values of the var element
     *
     * @return whether the other variable results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setAnOtherVariableResultDense(int solIdx, int otherIdx, std::string name, std::string value, std::string description, std::string *s);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific)variable-related results,
     * where i equals the given solution index.
     *
     * This alternate signature sets the type of the value attribute and the <var> and <enumeration> arrays
     *
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the new OtherVariableResult object
     * @param name holds the name of the other element
     * @param value holds the value of the other element
     * @param s holds a pointer to the array of values of the var element
     * @param type holds the type of the <other> element's value attribute
     * @param varType holds the type of the <other> element's <var> array
     * @param enumType holds the type of the <other> element's <enumeration> array
     *
     * @return whether the other variable results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setAnOtherVariableResultDense(int solIdx, int otherIdx, std::string name, std::string value, std::string description, std::string *s, std::string type, std::string varType, std::string enumType);

    /**
     * Set the number of <var> children of another (non-standard/solver specific)
     * variable-related result, for the [i]th solution.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param numberOfVar holds the number of <var> children
     *
     * @return whether the other variable result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultNumberOfVar(int solIdx, int otherIdx, int numberOfVar);

    /**
     * Set the number of <enumeration> children of another (non-standard/solver specific)
     * variable-related result, for the [i]th solution.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param numberOfEnumerations holds the number of <enumeration> children
     *
     * @return whether the other variable result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultNumberOfEnumerations(int solIdx, int otherIdx, int numberOfEnumerations);

    /**
     * Set the name of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param name holds the name of the other element
     *
     * @return whether the other variable result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultName(int solIdx, int otherIdx, std::string name);

    /**
     * Set the type of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param type holds the type of the other element
     *
     * @return whether the other variable result's type was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultType(int solIdx, int otherIdx, std::string type);

    /**
     * Set the varType of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param varType holds the data type of the <var> array of the <other> element
     *
     * @return whether the other variable result's varType was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultVarType(int solIdx, int otherIdx, std::string varType);

    /**
     * Set the enumType of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param enumType holds the data type of the <enumeration> array of the <other> element
     *
     * @return whether the other variable result's enumType was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultEnumType(int solIdx, int otherIdx, std::string enumType);

    /**
     * Set the value of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param value holds the name of the other element
     *
     * @return whether the other variable result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultValue(int solIdx, int otherIdx, std::string value);

    /**
     * Set the description of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param description holds the name of the other element
     *
     * @return whether the other variable result's description was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultDescription(int solIdx, int otherIdx, std::string description);

    /**
     * Set the solver of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param solver holds the type of the other element
     *
     * @return whether the other variable result's solver was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultSolver(int solIdx, int otherIdx, std::string solver);

    /**
     * Set the category of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param category holds the type of the other element
     *
     * @return whether the other variable result's category was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultCategory(int solIdx, int otherIdx, std::string category);

    /**
     * Set the index of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param varIdx holds the index of the location to which the information is stored
     * @param idx holds the index of the variable to which the information belongs
     *
     * @return whether the other variable result's index was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultVarIdx(int solIdx, int otherIdx, int varIdx, int idx);

    /**
     * Set the name of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param varIdx holds the index of the location to which the information is stored
     * @param name holds the name of the variable to which the information belongs
     *
     * @return whether the other variable result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultVarName(int solIdx, int otherIdx, int varIdx, std::string name);

    /**
     * Set the value of another (non-standard/solver specific) variable-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param varIdx holds the index of the location to which the information is stored
     * @param value holds the value of the variable to which the information belongs
     *
     * @return whether the other variable result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherVariableResultVar(int solIdx, int otherIdx, int varIdx, std::string value);

    /**
     * Set the value and corresponding indices of another (non-standard/solver specific) variable-related result,
     * for the [k]th solution, where k equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherVariableResult object
     * @param object holds the object to which this enumeration pertains
     *     (legal values are taken from the ENUM_PROBLEM_COMPONENT enumeration --- see OSGeneral.h))
     * @param enumIdx holds the index of the OtherOptionOrResultEnumeration object
     * @param value holds the value of this result
     * @param description holds a description of this result
     * @param i holds the indices of the variables that take on this value
     * @param ni holds the dimension of the index vector i
     *
     * @return whether the other variable result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVariableResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherVarResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherOptionOrResultEnumeration(int solIdx, int otherIdx, int object, int enumIdx, std::string value, std::string description, int *i, int ni);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific) objective-related results,
     * where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * This method then allocates the memory for the new OtherObjectiveResult objects
     * @param solIdx is the solution index
     * @param numberOfOtherObjectiveResult holds the number of OtherObjectiveResult objects
     * Each other objective result contains the name (required), an optional description (std::string) and an optional
     * value (std::string). Each other objective result can also optionally contain an array OtherObjResult for each objective.
     * The OtherObjResult contains an objective idx (required) and an optional std::string value.
     *
     * @return whether the other objective results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfOtherObjectiveResults(int solIdx, int numberOfOtherObjectiveResults);

    /**
     * Set the number of objectives to be given a value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the objective values.
     * @param numberOfObj holds the number of objectives that are to be set
     *
     * @return whether the information was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfObjValues(int solIdx, int numberOfObj);

    /**
     * Set the [i]th optimization solution's number of objective values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param n holds the number of elements in the array x
     *
     * @return whether objctive values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfObjectiveValues(int solIdx, int n);


    /**
     * Set the [i]th optimization solution's objective values, where i equals the given solution index.
     * Usually one of the objective is what the solution was solved for (or based on). Its index should be indicated
     * in the solution's objectiveIdx attribute. Based on this objective's solution, the rest of the objective
     * values are (optionally) calculated.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the objective values.
     * @param x holds a vector of type IndexValuePair; the idx component holds the index of the objective;
     * the value component holds its value. The vector could be null if all objectives are 0.
     * Possibly only the objective that the solution is based on has the value, and the rest of the objective
     * values all get a Double.NaN value, meaning that they are not calculated.
     *
     * @return whether objective values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setObjectiveValuesSparse(int solIdx, std::vector<IndexValuePair*> x);

    /**
     * Set the [i]th optimization solution's objective values, where i equals the given solution index.
     * Usually one of the objective is what the solution was solved for (or based on). Its index should be indicated
     * in the solution's objectiveIdx attribute. Based on this objective's solution, the rest of the objective
     * values are (optionally) calculated.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the objective values.
     * @param objectiveValues holds the double sparse array of objective values to set.
     * Possibly only the objective that the solution is based on has the value, and the rest of the objective
     * values all get a Double.NaN value, meaning that they are not calculated.
     *
     * @return whether objective values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setObjectiveValuesDense(int solIdx, double *objectiveValues);

    /**
     * Set an objective value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the objective values.
     * @param number holds the location within the sparse array obj that is to be used
     * @param idx holds the index of the objective that is to be set
     * @param name holds the objective name (or an empty string).
     * @param val holds the objective value to set.
     *
     * @return whether primal variable value was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setObjValue(int solIdx, int number, int idx, std::string name, double val);

    /**
     * Set the number of <obj> children of another (non-standard/solver specific)
     * objective-related result, for the [i]th solution.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param numberOfObj holds the number of <obj> children
     *
     * @return whether the other objective result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultNumberOfObj(int solIdx, int otherIdx, int numberOfObj);

    /**
    * Set the number of <enumeration> children of another (non-standard/solver specific)
    * objective-related result, for the [i]th solution.
    * Before this method is called, the setSolutionNumber(int) method has to be called first.
    * @param solIdx holds the solution index
    * @param otherIdx holds the index of the OtherObjectiveResult object
    * @param numberOfObj holds the number of <obj> children
    *
    * @return whether the other objective result's name was set successfully or not.
    * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
    * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
    * @see #setSolutionNumber(int)
    */
    bool setOtherObjectiveResultNumberOfEnumerations(int solIdx, int otherIdx, int numberOfObj);

    /**
     * Set the name of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param name holds the name of the other element
     *
     * @return whether the other objective result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultName(int solIdx, int otherIdx, std::string name);

    /**
     * Set the type of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param name holds the type of the <other> element
     *
     * @return whether the other objective result's type was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultType(int solIdx, int otherIdx, std::string type);

    /**
     * Set the objType of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param name holds the data type of the <other> element's <var> array
     *
     * @return whether the other objective result's objType was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultObjType(int solIdx, int otherIdx, std::string objType);

    /**
     * Set the enumType of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param name holds the data type of the <other> element's <enumeration> array
     *
     * @return whether the other objective result's enumType was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultEnumType(int solIdx, int otherIdx, std::string enumType);

    /**
     * Set the value of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param value holds the name of the other element
     *
     * @return whether the other objective result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultValue(int solIdx, int otherIdx, std::string value);

    /**
     * Set the description of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param description holds the name of the other element
     *
     * @return whether the other objective result's description was set successfully or not.

     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultDescription(int solIdx, int otherIdx, std::string description);

    /**
     * Set the solver of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param solver holds the solver of the other element
     *
     * @return whether the other Objective result's solver was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultSolver(int solIdx, int otherIdx, std::string solver);

    /**
     * Set the category of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param category holds the category of the other element
     *
     * @return whether the other objective result's category was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultCategory(int solIdx, int otherIdx, std::string category);

    /**
     * Set the index of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param objIdx holds the index of the location to which the information is stored
     * @param idx holds the index of the objective to which the information belongs
     *
     * @return whether the other variable result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultObjIdx(int solIdx, int otherIdx, int objIdx, int idx);

    /**
     * Set the name of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param objIdx holds the index of the location to which the information is stored
     * @param name holds the name of the objective to which the information belongs
     *
     * @return whether the other variable result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultObjName(int solIdx, int otherIdx, int objIdx, std::string name);

    /**
     * Set the value of another (non-standard/solver specific) objective-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherObjectiveResult object
     * @param objIdx holds the index of the location to which the information is stored
     * @param value holds the value of the objective to which the information belongs
     *
     * @return whether the other objective result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjectiveResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherObjResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherObjectiveResultObj(int solIdx, int otherIdx, int objIdx, std::string value);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific) constraint-related results,
     * where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * This method then allocates the memory for the new OtherConstraintResult objects
     * @param solIdx is the solution index
     * @param numberOfOtherConstraintResults holds the number of OtherConstraintResult objects
     * Each other objective result contains the name (required), an optional description (std::string) and an optional
     * value (std::string). Each other constraint result can also optionally contain an array OtherConResult for each constraint.
     * The OtherConResult contains a constraint idx (required) and an optional std::string value.
     *
     * @return whether the other objective results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfOtherConstraintResults(int solIdx, int numberOfOtherConstraintResults);

    /**
     * Set the number of constraints to be given a value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param numberOfCon holds the number of constraint that are to be set
     *
     * @return whether the information was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfDualValues(int solIdx, int numberOfCon);


    /**
     * Set the [i]th optimization solution's number of dual variable values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the dual variable values.
     * @param n holds the number of elements in the array x
     *
     * @return whether dual variable values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfDualVariableValues(int solIdx, int n);


    /**
     * Set the [i]th optimization solution's dual variable values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the dual variable values.
     * @param x holds a vector of type IndexValuePair; the idx component holds the index of the constraint;
     * the value component holds its value. The vector could be null if all dual variables are 0.
     *
     * @return whether dual variable values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setDualVariableValuesSparse(int solIdx, std::vector<IndexValuePair*> x);


    /**
     * Set the [i]th optimization solution's dual variable values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the dual variable values.
     * @param y holds a double dense array of variable dual values; it could be NULL if all values are 0.
     * @return whether dual variable values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setDualVariableValuesDense(int solIdx, double *y);

    /**
     * Set the [i]th optimization solution's constraint values, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param constraintValues holds the a double dense array of constraint values to set; it could be null if all constraint values are 0.
     *
     * @return whether constraint values are set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setConstraintValuesDense(int solIdx, double *constraintValues);

    /**
     * Set a dual value.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param number holds the location within the sparse array con that is to be used
     * @param idx holds the index of the constraint that is to be set
     * @param name holds the constraint name (or an empty string).
     * @param val holds the constraint value to set.
     *
     * @return whether dual variable value was set successfully or not.
     * @see #setSolutionNumber(int)
     */
    bool setDualValue(int solIdx, int number, int idx, std::string name, double val);

    /**
     * Set the number of <con> children of another (non-standard/solver specific)
     * constraint-related result, for the [i]th solution.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param numberOfCon holds the number of <con> children
     *
     * @return whether the other constraint result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultNumberOfCon(int solIdx, int otherIdx, int numberOfCon);

    /**
     * Set the number of <enumeration> children of another (non-standard/solver specific)
     * constraint-related result, for the [i]th solution.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param numberOfCon holds the number of <con> children
     *
     * @return whether the other constraint result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultNumberOfEnumerations(int solIdx, int otherIdx, int numberOfCon);

    /**
     * Set the name of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param name holds the name of the other element
     *
     * @return whether the other constraint result's name was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultName(int solIdx, int otherIdx, std::string name);

    /**
     * Set the type of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param name holds the type of the <other> element
     *
     * @return whether the other constraint result's type was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultType(int solIdx, int otherIdx, std::string type);

    /**
     * Set the conType of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param name holds the type of the <other> element's <con> array
     *
     * @return whether the other constraint result's conType was set successfully or not.

     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultConType(int solIdx, int otherIdx, std::string conType);

    /**
     * Set the enumType of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param name holds the type of the <other> element's <enumeration> array
     *
     * @return whether the other constraint result's enumType was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultEnumType(int solIdx, int otherIdx, std::string enumType);

    /**
     * Set the value of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param value holds the name of the other element
     *
     * @return whether the other constraint result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultValue(int solIdx, int otherIdx, std::string value);

    /**
     * Set the description of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param description holds the name of the other element
     *
     * @return whether the other constraint result's description was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultDescription(int solIdx, int otherIdx, std::string description);

    /**
     * Set the solver of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param solver holds the solver of the other element
     *
     * @return whether the other constraint result's solver was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultSolver(int solIdx, int otherIdx, std::string solver);

    /**
     * Set the category of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param category holds the category of the other element
     *
     * @return whether the other constraint result's category was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultCategory(int solIdx, int otherIdx, std::string category);

    /**
     * Set the index of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param conIdx holds the index of the location to which the information is stored
     * @param idx holds the index of the onstraint to which the information belongs
     *
     * @return whether the other variable result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultConIdx(int solIdx, int otherIdx, int conIdx, int idx);

    /**
     * Set the name of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param conIdx holds the index of the location to which the information is stored
     * @param name holds the name of the constraint to which the information belongs
     *
     * @return whether the other variable result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultConName(int solIdx, int otherIdx, int conIdx, std::string name);

    /**
     * Set the value of another (non-standard/solver specific) constraint-related result,
     * for the [i]th solution, where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index
     * @param otherIdx holds the index of the OtherConstraintResult object
     * @param conIdx holds the index of the location to which the information is stored
     * @param value holds the value of the constraint to which the information belongs
     *
     * @return whether the other constraint result's value was set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConstraintResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherConResult
     * @see #setSolutionNumber(int)
     */
    bool setOtherConstraintResultCon(int solIdx, int otherIdx, int conIdx, std::string value);

    /**
     * Set the [i]th optimization solution's MatrixVariableSolution, 
     * where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * This method then allocates the memory for the new MatrixVariableSolution objects.
     * Further methods then store the values into the data structure.
     *
     * @param solIdx is the solution index
     * @param numberOfMatrixVar_ holds the number of matrixVar elements 
     *        for which values are to be provided
     * @param numberOfOtherMatrixVariableResults_ holds the number of <other> elements 
     *        for which values are to be provided
     *
     * @return whether the matrix variable results are set successfully or not.  
     */
    bool setMatrixVariableSolution(int solIdx, int numberOfMatrixVar_, 
                                               int numberOfOtherMatrixVariableResults_);

    /**
     * A method to set general attributes for a matrixVar in the [i]th optimization solution,
     * where i equals the given solution index.
     * Before this method is called, the setMatrixVariableSolution() method has to be called first.
     * This method then sets the matrix dimensions and other basic attributes.
     * Further methods then store the values into the data structure.
     *
     * @param solIdx is the solution index
     * @param idx holds the index of the matrixVar (in the MatrixVariableValues array)
     * @param matrixVarIdx holds the index of the matrixVar (as defined in the OS instance)
     * @param numberOfRows holds the number of rows in the matrixVar
     * @param numberOfColumns holds the number of columns in the matrixVar
     * @param symmetry (optional) holds the type of symmetry
     *        (if not present, the default value is "none" 
     * @param type (optional) holds the type of values in the nonzeroes of the matrix
     *        (if not present, the default value is "unknown")
     * @param name (optional) holds the name of this matrixVar (the default is the empty string) 
     *        for which values are to be provided
     * @return whether the matrix variable attributes are set successfully or not.  
     */
    bool setMatrixVarValuesAttributes(int solIdx, int idx, int matrixVarIdx, int numberOfRows, 
            int numberOfColumns, ENUM_MATRIX_SYMMETRY symmetry=ENUM_MATRIX_SYMMETRY_none, 
            ENUM_MATRIX_TYPE type=ENUM_MATRIX_TYPE_unknown, std::string name="");

    /**
     * A method to set the block structure for the values of a matrixVar
     * in the [i]th optimization solution, where i equals the given solution index.
     * Before this method is called, the setMatrixVariableSolution() method has to be called first.
     * This method then allocates the block structure for the new MatrixVar objects in the solution.
     * Further methods then store the values into the data structure.
     *
     * @param solIdx is the solution index
     * @param idx holds the index of the matrixVar for which values are to be provided
     *        (as derived from the <values> element in matrixProgramming
     * @param colOffset is an array of column offsets that define 
     *        the partition of the columns within the block structure
     * @param colOffsetSize gives the size of the colOffset array
     * @param rowOffset is an array of row offsets that define 
     *        the partition of the rows within the block structure
     * @param rowOffsetSize gives the size of the rowOffset array
     * @param numberOfBlocks gives the number of blocks 
     * @param blocksConstructorIdx gives the index of the MatrixBlocks node 
     *        in the array of constructors of the matrixVar. The default is 0.
     *
     * @return whether the matrix variable block structure was set successfully or not.  
     */
    bool setMatrixVarValuesBlockStructure(int solIdx, int idx, int* colOffset, int colOffsetSize,
            int* rowOffset, int rowOffsetSize, int numberOfBlocks, int blocksConstructorIdx=0);

    /**
     * A method to set the elements within a block of a matrixVar in the [i]th optimization solution,
     * where i equals the given solution index.
     * Before this method is called, the setMatrixVarBlockStructure() method has to be called first.
     * This method then allocates the elements for the new MatrixVar blocks in the solution.
     *
     * @param solIdx is the solution index
     * @param idx holds the index of the matrixVar for which values are to be provided
     *        (as derived from the <values> element in matrixProgramming
     * @param blkno is the number of the block for which elements are provided
     *        the partition of the columns within the block structure
     * @param blkRowIdx gives the index of the block row in which the block is located
     * @param blkColIdx gives the index of the block column in which the block is located
     * @param nz gives the number of (nonzero) values 
     * @param start gives the start elements (column or row starts, depending on
     *        whether rowMajor is false or true)
     * @param index gives the array of row or column (depending on rowMajor) indices
     * @param value gives the data structure for (nonzero) values that need to be stored
     * @param valueType gives the type of values (see OSParameters.h)
     * @param symmetry gives the form of symmetry. (The default is NONE.)
     * @param rowMajor indicates whther the elements are stored column by column (if rowMajor is false)
     *        or row by row (if rowMajor is true). The default is rowMajor = false.
     *
     * @remark each block object can handle only one type of elements, 
     *         although different blocks may contain different types of values
     *
     * @return whether the matrix variable block structure was set successfully or not.  
     */
    bool setMatrixVarValuesBlockElements(int solIdx, int idx, int blkno, int blkRowIdx, int blkColIdx,
            int nz, int* start, int* index, MatrixElementValues* value, ENUM_MATRIX_TYPE valueType,
            ENUM_MATRIX_SYMMETRY symmetry=ENUM_MATRIX_SYMMETRY_none, bool rowMajor=false);

    /**
     * A method to set general attributes for another (non-standard/solver specific) result
     * associated with the matrix variables in the [i]th optimization solution,
     * where i equals the given solution index.
     * Before this method is called, the setMatrixVariableSolution() method has to be called first.
     * This method then sets the matrix dimensions and other basic attributes.
     * Further methods then store the values into the data structure.
     *
     * @param solIdx is the solution index
     * @param idx holds the index of the other result (in the array of <other> solutions)
     * @param name holds the name of the <other> result
     * @param description can be used to hold a further description of the result
     * @param value holds a scalar value associated with the <other> result
     * @param type describes the type of value represented by the scalar result
     * @param solver gives the solver with which this result is associated
     * @param category can be used to specify a further category within the solver
     * @param numberOfMatrixVar gives the number of matrixVar elements associated with this result.
     *        This argument is optional and defaults to 0.
     * @param matrixType can be used to associate a type with the values of the matrixVar elements. 
     *        This argument is optional and defaults to the empty string ("").
     * @param numberOfEnumerations gives the number of levels associated with an enumeration of matrixVar
     *        elements pertaining to this result. The argument is optional and defaults to 0.
     * @param enumType can be used to associate a type with the values of the enumeration. 
     *        This argument is optional and defaults to the empty string ("").
     * @return whether the attributes are set successfully or not.  
     */
    bool setMatrixVariablesOtherResultGeneralAttributes(int solIdx, int idx, std::string name, 
                            std::string description, std::string value, std::string type,
                            std::string solver, std::string category,
                            int numberOfMatrixVar=0,    std::string matrixType="", 
                            int numberOfEnumerations=0, std::string enumType="");

    /**
     * A method to set attributes for a matrixVar in the [j]th other result associated with
     * matrix variables in the the [i]th optimization solution, where i equals the given solution index
     * and j equals a given other result index. 
     * Before this method is called, the setMatrixVariableSolution() and  
     * setMatrixVariablesOtherResultGeneralAttributes() methods have to be called. 
     * This method then sets the matrix dimensions and other basic attributes.
     * Further methods then store the values into the data structure.
     *
     * @param solIdx is the solution index
     * @param otherIdx holds the index of the other matrix variables result
     * @param matrixVarIdx holds the index of the matrixVar (as defined in the OS instance)
     * @param numberOfRows holds the number of rows in the matrixVar
     * @param numberOfColumns holds the number of columns in the matrixVar
     * @param symmetry (optional) holds the type of symmetry
     *        (if not present, the default value is "none" 
     * @param type (optional) holds the type of values in the nonzeroes of the matrix
     *        (if not present, the default value is "unknown")
     * @param name (optional) holds the name of this matrixVar (the default is the empty string) 
     *        for which values are to be provided
     * @return whether the matrix variable attributes are set successfully or not.  
     */
    bool setMatrixVariablesOtherResultMatrixAttributes(int solIdx, int otherIdx, int matrixVarIdx, 
            int numberOfRows, int numberOfColumns, 
            ENUM_MATRIX_SYMMETRY symmetry=ENUM_MATRIX_SYMMETRY_none, 
            ENUM_MATRIX_TYPE type=ENUM_MATRIX_TYPE_unknown, std::string name="");

    /**
     * A method to set the block structure for the values of a matrixVar associated with the [j]th
     * "other" result of the [i]th optimization solution, where i equals the given solution index,
     * and j equals the index of the other result.
     * Before this method is called, the setMatrixVariableSolution() and  
     * setMatrixVariablesOtherSolutionAttributes() methods have to be called. 
     * This method then allocates the block structure for the new MatrixVar objects in the solution.
     * Further methods then store the values into the data structure.
     *
     * @param solIdx is the solution index
     * @param otherIdx holds the index of the other solution
     * @param matrixVarIdx holds the index of the matrixVar for which values are to be provided
     * @param colOffset is an array of column offsets that define 
     *        the partition of the columns within the block structure
     * @param colOffsetSize gives the size of the colOffset array
     * @param rowOffset is an array of row offsets that define 
     *        the partition of the rows within the block structure
     * @param rowOffsetSize gives the size of the rowOffset array
     * @param numberOfBlocks gives the number of blocks 
     * @param blocksConstructorIdx gives the index of the MatrixBlocks node 
     *        in the array of constructors of the matrixVar. The default is 0.
     *
     * @return whether the matrix variable block structure was set successfully or not.  
     */
    bool setMatrixVariablesOtherResultBlockStructure(int solIdx, int otherIdx, int matrixVarIdx, 
            int* colOffset, int colOffsetSize, int* rowOffset, int rowOffsetSize, 
            int numberOfBlocks, int blocksConstructorIdx=0);

    /**
     * A method to set the elements within a block of a matrixVar associated with the [j]th "other"
     * result in the [i]th optimization solution, where i equals the given solution index and
     * j equals the index of the other result.
     * Before this method is called, the setMatrixVariablesOtherResultBlockStructure() method 
     * has to be called first.
     * This method then allocates the elements for the new MatrixVar blocks in the solution.
     *
     * @param solIdx is the solution index
     * @param otherIdx holds the index of the other solution
     * @param matrixVarIdx holds the index of the matrixVar for which values are to be provided
     * @param blkno is the number of the block for which elements are provided
     *        the partition of the columns within the block structure
     * @param blkRowIdx gives the index of the block row in which the block is located
     * @param blkColIdx gives the index of the block column in which the block is located
     * @param nz gives the number of (nonzero) values 
     * @param start gives the start elements (column or row starts, depending on
     *        whether rowMajor is false or true)
     * @param index gives the array of row or column (depending on rowMajor) indices
     * @param value gives the data structure for (nonzero) values that need to be stored
     * @param valueType gives the type of values (see OSParameters.h)
     * @param symmetry gives the form of symmetry. (The default is NONE.)
     * @param rowMajor indicates whther the elements are stored column by column (if rowMajor is false)
     *        or row by row (if rowMajor is true). The default is rowMajor = false.
     *
     * @remark each block object can handle only one type of elements, 
     *         although different blocks may contain different types of values
     *
     * @return whether the matrix variable block structure was set successfully or not.  
     */
    bool setMatrixVariablesOtherResultBlockElements(int solIdx, int otherIdx, int matrixVarIdx, 
            int blkno, int blkRowIdx, int blkColIdx, int nz, int* start, int* index,
            MatrixElementValues* value, ENUM_MATRIX_TYPE valueType,
            ENUM_MATRIX_SYMMETRY symmetry=ENUM_MATRIX_SYMMETRY_none, bool rowMajor=false);

    /**
     * Set the [i]th optimization solution's other (non-standard/solver specific) solution-related results,
     * where i equals the given solution index.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * This method then allocates the memory for the new OtherSolutionResult objects
     *
     * @param solIdx is the solution index
     * @param numberOfOtherSolutionResults holds the number of OtherSolutionResult objects
     * Each other objective result contains the name (required), an optional
     * description (std::string) and an optional category (std::string).
     * Each other solution result can also optionally contain an array Item for each result.
     * The Item content is string-valued.
     *
     * @return whether the other objective results are set successfully or not.
     * @see org.optimizationservices.oscommon.datastructure.osresult.OtherSolutionResult
     * @see org.optimizationservices.oscommon.datastructure.osresult.Item
     * @see #setSolutionNumber(int)
     */
    bool setNumberOfOtherSolutionResults(int solIdx, int numberOfOtherSolutionResults);

    /**
     * Set the name associated with the [j]th other solution result of solution [i].
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param otherIdx holds the index of the otherSolutionResult
     * @param name holds the name of the otherSolutionResult
     *
     * @return whether the other solution result was set successfully or not.
     */
    bool setOtherSolutionResultName(int solIdx, int otherIdx, std::string name);

    /**

     * Set the value associated with the [j]th other solution result of solution [i].
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param otherIdx holds the index of the otherSolutionResult
     * @param value holds the value of the otherSolutionResult
     *
     * @return whether the other solution result was set successfully or not.
     */
    bool setOtherSolutionResultValue(int solIdx, int otherIdx, std::string value);

    /**
     * Set the category associated with the [j]th other solution result of solution [i].
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param otherIdx holds the index of the otherSolutionResult
     * @param category holds the category of the otherSolutionResult
     *
     * @return whether the other solution result was set successfully or not.
     */
    bool setOtherSolutionResultCategory(int solIdx, int otherIdx, std::string category);

    /**
     * Set the description associated with the [j]th other solution result of solution [i].
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param otherIdx holds the index of the otherSolutionResult
     * @param category holds the description of the otherSolutionResult
     *
     * @return whether the other solution result was set successfully or not.
     */
    bool setOtherSolutionResultDescription(int solIdx, int otherIdx, std::string description);

    /**
     * Set the number of items associated with the [j]th other solution result of solution [i].
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param otherIdx holds the index of the otherSolutionResult
     * @param numberOfItems holds the number of items
     *
     * @return whether the other solution result was set successfully or not.
     */
    bool setOtherSolutionResultNumberOfItems(int solIdx, int otherIdx, int numberOfItems);

    /**
     * Set one item associated with the [j]th other solution result of solution [i].
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param solIdx holds the solution index to set the constraint values.
     * @param otherIdx holds the index of the otherSolutionResult
     * @param itemIdx holds the index of the item
     * @param item holds the value of the item
     *
     * @return whether the other solution result item was set successfully or not.
     */
    bool setOtherSolutionResultItem(int solIdx, int otherIdx, int itemIdx, std::string item);

    /**
     * Set another solution result of solution [i].
     * @param solIdx holds the solution index i.
     * @param name holds the name of the other solution result
     * @param value holds the value of the other solution result
     * @param category holds the category of the result
     * @param description holds a description of the result
     * @param numberOfItems holds the number of items
     * @param item holds a pointer to the array of items (can be NULL if numberOfItems is 0)
     *
     * @return whether the other solution result was set successfully or not.
     */
    bool setAnOtherSolutionResult(int solIdx, std::string name, std::string value, std::string category, 
        std::string description, int numberOfItems, std::string* item);

    /**
     * Set the number of other solver outputs
     * @param numberOfOtherSolverOutputs holds the number of SolverOutput objects
     * Each solver output can also optionally contain an array Item for each result.
     * The Item content is string-valued.
     *
     * @return whether the results were set successfully or not.
     */
    bool setNumberOfSolverOutputs(int numberOfSolverOutputs);

    /**
     * Set the name associated with the [j]th solver output.
     * @param otherIdx holds the index of the solverOutput object
     * @param name holds the name of the solver output
     *
     * @return whether the solver output was set successfully or not.
     */
    bool setSolverOutputName(int otherIdx, std::string name);

    /**
     * Set the category associated with the [j]th solver output.
     * @param otherIdx holds the index of the solverOutput object
     * @param name holds the category of the solver output
     *
     * @return whether the solver output was set successfully or not.
     */
    bool setSolverOutputCategory(int otherIdx, std::string category);

    /**
     * Set the description associated with the [j]th solver output.
     * @param otherIdx holds the index of the solverOutput object
     * @param name holds the description of the solver output
     *
     * @return whether the solver output was set successfully or not.
     */
    bool setSolverOutputDescription(int otherIdx, std::string description);

    /**
     * Set the number of items associated with the [j]th solver output.
     * Before this method is called, the setSolutionNumber(int) method has to be called first.
     * @param otherIdx holds the index of the solverOutput object
     * @param numberOfItems holds the number of items
     *
     * @return whether the information was set successfully or not.
     */
    bool setSolverOutputNumberOfItems(int otherIdx, int numberOfItems);

    /**
     * Set one item associated with the [j]th solver output.
     * @param otherIdx holds the index of the otherSolutionResult
     * @param itemIdx holds the index of the item
     * @param item holds the value of the item
     *
     * @return whether the information was set successfully or not.
     */
    bool setSolverOutputItem(int otherIdx, int itemIdx, std::string item);


};
#endif

