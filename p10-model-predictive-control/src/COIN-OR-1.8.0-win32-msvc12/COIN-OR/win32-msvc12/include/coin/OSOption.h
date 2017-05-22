/* $Id: OSOption.h 5019 2015-06-03 20:23:51Z Gassmann $ */
/** @file OSOption.h
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

#ifndef OSOPTION_H
#define OSOPTION_H

#include <string>
#include <vector>
#include "OSGeneral.h"
#include "OSMathUtil.h"



//#define DEBUG


/*! \class InstanceLocationOption
 *  \brief the InstanceLocationOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to the instanceLocation
 * element in the OSoL schema.
 */
class InstanceLocationOption
{

public:
    /** the type of the location */
    std::string locationType;

    /** the value of the <instanceLocation> element */
    std::string value;

    /**
     *
     * Default constructor.
     */
    InstanceLocationOption();

    /**
     *
     * Class destructor.
     */
    ~InstanceLocationOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InstanceLocationOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InstanceLocationOption *that);
}; //InstanceLocationOption


/*! \class ContactOption
 *  \brief the ContactOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to the contact method
 * in the OSoL schema.
 */
class ContactOption
{

public:
    /** the contact mechanism */
    std::string transportType;

    /** the value of the <contact> element */
    std::string value;

    /**
     *
     * Default constructor.
     */
    ContactOption();
    /**
     *
     * Class destructor.
     */
    ~ContactOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ContactOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(ContactOption *that);
}; //ContactOption


/*! \class OtherOption
 *  \brief the OtherOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to the OtherOption
 * element in the OSoL schema.
 */
class OtherOption
{

public:
    /** the name of the option */
    std::string name;

    /** the value of the option */
    std::string value;

    /** the description of the option */
    std::string description;

    /**
     *
     * Default constructor.
     */
    OtherOption();
    /**
     *
     * Class destructor.
     */
    ~OtherOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherOption *that);
}; //OtherOption


/*! \class OtherOptions
 *  \brief the OtherOptions class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to the OtherOptions
 * element in the OSoL schema.
 */
class OtherOptions
{

public:
    /** the number of other options */
    int numberOfOtherOptions;

    /** the list of other options */
    OtherOption **other;

    /**
     *
     * Default constructor.
     */
    OtherOptions();
    /**
     *
     * Class destructor.
     */
    ~OtherOptions();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherOptions *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherOptions *that);

    /**
     *
     * A function to set an array of <other> elements
     * @param numberOfOptions: number of <other> elements to be set
     * @param other: the array of <other> elements that are to be set
     */
    bool setOther(int numberOfOptions, OtherOption** other);

    /**
     *
     * A function to add an <other> element
     * @param name: the name of the <other> element to be added (required)
     * @param value: the value of the <other> element to be added (optional)
     * @param description: a description of the <other> element (optional)
     */
    bool addOther(std::string name, std::string value, std::string description);
}; //OtherOptions


/*! \class GeneralOption
 *  \brief The GeneralOption Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class GeneralOption
{

public:

    /** the service URI */
    std::string serviceURI;

    /** the name of the service */
    std::string serviceName;

    /** the name of the instance */
    std::string instanceName;

    /** the location of the instance */
    InstanceLocationOption *instanceLocation;

    /** the job ID */
    std::string jobID;

    /** the solver to invoke */
    std::string solverToInvoke;

    /** the license information */
    std::string license;

    /** the username */
    std::string userName;

    /** the password */
    std::string password;

    /** the contact method */
    ContactOption *contact;

    /** the list of other general options */
    OtherOptions *otherOptions;

    /**
     *
     * Default constructor.
     */
    GeneralOption();
    /**
     *
     * Class destructor.
     */
    ~GeneralOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(GeneralOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(GeneralOption *that);
};//GeneralOption

/*! \class MinDiskSpace
 *  \brief the MinDiskSpace class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 * This class has been superseded as of version 2.3
 * by the class StorageCapacity (see OSGeneral.h)
 */
class MinDiskSpace
{

public:
    /** the unit in which disk space is measured */
    std::string unit;

    /** additional description about the disk space */
    std::string description;

    /** the minimum disk space required */
    double value;


    /**
     *
     * Default constructor.
     */
    MinDiskSpace();
    /**
     *
     * Class destructor.
     */
    ~MinDiskSpace();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(MinDiskSpace *that);
}; //MinDiskSpace

/*! \class MinMemorySize
 *  \brief the MinMemorySize class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 * This class has been superseded as of version 2.3
 * by the class StorageCapacity (see OSGeneral.h)
 */
class MinMemorySize
{

public:
    /** the unit in which memory size is measured */
    std::string unit;

    /** additional description about the memory */
    std::string description;

    /** the minimum memory size required */
    double value;


    /**
     *
     * Default constructor.
     */
    MinMemorySize();
    /**
     *
     * Class destructor.
     */
    ~MinMemorySize();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(MinMemorySize *that);
}; //MinMemorySize

/*! \class MinCPUSpeed
 *  \brief the MinCPUSpeed class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 * This class has been superseded as of version 2.3
 * by the class CPUSpeed (see OSGeneral.h)
 */
class MinCPUSpeed
{

public:
    /** the unit in which CPU speed is measured */
    std::string unit;

    /** additional description about the CPU speed */
    std::string description;

    /** the minimum CPU speed required */
    double value;


    /**
     *
     * Default constructor.
     */
    MinCPUSpeed();
    /**
     *
     * Class destructor.
     */
    ~MinCPUSpeed();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(MinCPUSpeed *that);
}; //MinCPUSpeed

/*! \class MinCPUNumber
 *  \brief the MinCPUNumber class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 * This class has been superseded as of version 2.3
 * by the class CPUNumber (see OSGeneral.h)
 */
class MinCPUNumber
{

public:
    /** additional description about the CPU */
    std::string description;

    /** the minimum number of CPUs required */
    int value;


    /**
     *
     * Default constructor.
     */
    MinCPUNumber();
    /**
     *
     * Class destructor.
     */
    ~MinCPUNumber();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(MinCPUNumber *that);
}; //MinCPUNumber


/*! \class SystemOption
 *  \brief the SystemOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class SystemOption
{

public:
    /** the minimum disk space required */
    StorageCapacity *minDiskSpace;

    /** the minimum memory required */
    StorageCapacity *minMemorySize;

    /** the minimum CPU speed required */
    CPUSpeed *minCPUSpeed;

    /** the minimum number of processors required */
    CPUNumber *minCPUNumber;

    /** the list of other system options */
    OtherOptions *otherOptions;

    /**
     *
     * Default constructor.
     */
    SystemOption();
    /**
     *
     * Class destructor.
     */
    ~SystemOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(SystemOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(SystemOption *that);
}; //SystemOption


/*! \class ServiceOption
 *  \brief the ServiceOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class ServiceOption
{

public:
    /** the service type */
    std::string type;

    /** the list of other service options */
    OtherOptions *otherOptions;

    /**
     *
     * Default constructor.
     */
    ServiceOption();
    /**
     *
     * Class destructor.
     */
    ~ServiceOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ServiceOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(ServiceOption *that);
}; //ServiceOption


/*! \class MaxTime
 *  \brief the MaxTime class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 * This class has been superseded as of version 2.3
 * by the class TimeSpan (see OSGeneral.h)
 */
class MaxTime
{

public:
    /** the unit in which time is measured */
    std::string unit;

    /** the maximum time allowed */
    double value;


    /**
     *
     * Default constructor.
     */
    MaxTime();
    /**
     *
     * Class destructor.
     */
    ~MaxTime();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(MaxTime *that);
}; //MaxTime


/*! \class JobDependencies
 *  \brief the JobDependencies class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class JobDependencies
{

public:
    /** the number of entries in the list of job dependencies */
    int numberOfJobIDs;

    /** the list of job IDs */
    std::string *jobID;

    /**
     *
     * Default constructor.
     */
    JobDependencies();
    /**
     *
     * Class destructor.
     */
    ~JobDependencies();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(JobDependencies *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(JobDependencies *that);

    /**
     *
     * A function to set an array of <jobID> elements
     * @param numberOfJobIDs: number of <jobID> elements to be set
     * @param jobID: the array of <jobID> elements that are to be set
     */
    bool setJobID(int numberOfJobIDs, std::string *jobID);

    /**
     *
     * A function to add an <jobID> element
     * @param jobID: the name of the <jobID> element to be added
     */
    bool addJobID(std::string jobID);
}; //JobDependencies


/*! \class DirectoriesAndFiles
 *  \brief the DirectoriesAndFiles class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class DirectoriesAndFiles
{

public:
    /** the number of <path> children */
    int numberOfPaths;

    /** the list of directory and file paths */
    std::string *path;

    /**
     *
     * Default constructor.
     */
    DirectoriesAndFiles();
    /**
     *
     * Class destructor.
     */
    ~DirectoriesAndFiles();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(DirectoriesAndFiles *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(DirectoriesAndFiles *that);

    /**
     *
     * A function to set an array of <path> elements
     * @param numberOfPaths: number of <path> elements to be set
     * @param path: the array of <path> elements that are to be set
     */
    bool setPath(int numberOfPaths, std::string *path);
    /**
     *
     * A function to add a <path> element
     * @param path: the path to be added
     */
    bool addPath(std::string path);
}; //DirectoriesAndFiles



/*! \class PathPair
 *  \brief the PathPair class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class PathPair
{

public:
    /** the file or directory to move/copy from */
    std::string from;

    /** the file or directory to move/copy to */
    std::string to;

    /** record whether a copy is to be made */
    bool makeCopy;

    /**
     *
     * Default constructor.
     */
    PathPair();
    /**
     *
     * Class destructor.
     */
    ~PathPair();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(PathPair *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(PathPair *that);
}; //PathPair


/*! \class PathPairs
 *  \brief the PathPairs class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class PathPairs
{

public:
    /** the number of <path> children */
    int numberOfPathPairs;

    /** the list of directory and file paths */
    PathPair **pathPair;

    /**
     *
     * Default constructor.
     */
    PathPairs();
    /**
     *
     * Class destructor.
     */
    ~PathPairs();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(PathPairs *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(PathPairs *that);

    /**
     *
     * A function to set an array of <pathPair> elements
     * @param numberOfPathPairs: number of <pathPair> elements to be set
     * @param path: the array of <pathPair> elements that are to be set
     */
    bool setPathPair(int numberOfPathPairs, PathPair **pathPair);

    /**
     *
     * Alternate signature for this function
     * @param from: array containing a list of objects to be moved
     * @param to: array containing a list of destinations
     * @param makeCopy: records whether each object is to be moved or copied
     * @param numberOfPathPairs: number of <pathPair> elements to be set
     */
    bool setPathPair(std::string *from, std::string *to, bool *makeCopy, int numberOfPathPairs);

    /**
     *
     * A function to add a <pathPair> element
     * @param fromPath: the path from which to copy or move
     * @param toPath: the path to which to copy or move
     * @param makecopy: tracks whether a copy is to be made
     */
    bool addPathPair(std::string fromPath, std::string toPath, bool makeCopy);
}; //PathPairs


/*! \class Processes
 *  \brief the Processes class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class Processes
{

public:
    /** the number of <process> children */
    int numberOfProcesses;

    /** the list of processes */
    std::string *process;

    /**
     *
     * Default constructor.
     */
    Processes();
    /**
     *
     * Class destructor.
     */
    ~Processes();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(Processes *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(Processes *that);

    /**
     *
     * A function to set an array of <process> elements
     * @param numberOfProcesses: number of <process> elements to be set
     * @param path: the array of <process> elements that are to be set
     */
    bool setProcess(int numberOfProcesses, std::string *process);

    /**
     *
     * A function to add a <process> element
     * @param process: the ID of the process to be added
     */
    bool addProcess(std::string process);
}; //Processes


/*! \class JobOption
 *  \brief the JobOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class JobOption
{

public:
    /** the maximum time allowed */
    TimeSpan *maxTime;

    /** the requested time to start the job */
    std::string requestedStartTime;

    /** the dependency set */
    JobDependencies *dependencies;

    /** directories  required to run the job */
    DirectoriesAndFiles *requiredDirectories;

    /** files required to run the job */
    DirectoriesAndFiles *requiredFiles;

    /** directories to make during the job */
    DirectoriesAndFiles *directoriesToMake;

    /** files to make during the job */
    DirectoriesAndFiles *filesToMake;

    /** input directories to move or copy */
    PathPairs *inputDirectoriesToMove;

    /** input files to move or copy */
    PathPairs *inputFilesToMove;

    /** output files to move or copy */
    PathPairs *outputFilesToMove;

    /** output directories to move or copy */
    PathPairs *outputDirectoriesToMove;

    /** files to delete upon completion */
    DirectoriesAndFiles *filesToDelete;

    /** directories to delete upon completion */
    DirectoriesAndFiles *directoriesToDelete;

    /** processes to kill upon completion */
    Processes *processesToKill;

    /** list of other job options */
    OtherOptions *otherOptions;

    /**
     *
     * Default constructor.
     */
    JobOption();
    /**
     *
     * Class destructor.
     */
    ~JobOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(JobOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(JobOption *that);
}; //JobOption


/*! \class InitVarValue
 *  \brief the InitVarValue class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitVarValue
{

public:
    /** variable index */
    int idx;

    /** optional variable name */
    std::string name;

    /** initial value */
    double value;

    /**
     *
     * Default constructor.
     */
    InitVarValue();
    /**
     *
     * Class destructor.
     */
    ~InitVarValue();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitVarValue *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitVarValue *that);
}; //InitVarValue


/*! \class InitVariableValues
 *  \brief the InitVariableValues class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitVariableValues
{

public:
    /** number of <var> children */
    int numberOfVar;

    /** initial value for each variable */
    InitVarValue **var;

    /**
     *
     * Default constructor.
     */
    InitVariableValues();
    /**
     *
     * Class destructor.
     */
    ~InitVariableValues();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitVariableValues *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitVariableValues *that);

    /**
     *
     * A function to set an array of <var> elements
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     */
    bool setVar(int numberOfVar, InitVarValue **var);

    /**
     *
     * Alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     * @param disp: method of disposition if previous data exist
     */
    bool setVar(int numberOfVar, InitVarValue **var, ENUM_COMBINE_ARRAYS disp);


    /**
     *
     * Another alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param idx: the array of indices
     * @param value: the array of corresponding values
     * @param name: the array of corresponding names
     */
    bool setVar(int numberOfVar, int *idx, double *value, std::string *name);

    /**
     *
     * A function to add a <var> element
     * @param idx: the index of the variable to be given an initial value
     * @param value: the initial variable value to be added
     */
    bool addVar(int idx, double value);

    /**
     *
     * Alternative signature for this function
     *
     * A function to add an array of <var> elements simultaneously
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     */
    bool addVar(int numberOfVar, InitVarValue **var);
}; //InitVariableValues


/*! \class InitVarValueString
 *  \brief the InitVarValueString class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitVarValueString
{

public:
    /** variable index */
    int idx;

    /** optional variable name */
    std::string name;

    /** initial value */
    std::string value;

    /**
     *
     * Default constructor.
     */
    InitVarValueString();
    /**
     *
     * Class destructor.
     */
    ~InitVarValueString();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitVarValueString *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitVarValueString *that);
}; //InitVarValueString


/*! \class InitVariableValuesString
 *  \brief the InitVariableValuesString class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitVariableValuesString
{

public:
    /** number of <var> children */
    int numberOfVar;

    /** initial value for each variable */
    InitVarValueString **var;

    /**
     *
     * Default constructor.
     */
    InitVariableValuesString();
    /**
     *
     * Class destructor.
     */
    ~InitVariableValuesString();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitVariableValuesString *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitVariableValuesString *that);

    /**
     *
     * A function to set an array of <var> elements
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     */
    bool setVar(int numberOfVar, InitVarValueString **var);

    /**
     *
     * Alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     * @param disp: method of disposition if previous data exist
     */
    bool setVar(int numberOfVar, InitVarValueString **var, ENUM_COMBINE_ARRAYS disp);


    /**
     *
     * Another alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param idx: the array of indices
     * @param value: the array of corresponding values
     * @param name: the array of corresponding names
     */
    bool setVar(int numberOfVar, int *idx, std::string *value, std::string* name);

    /**
     *
     * A function to add a <var> element
     * @param idx: the index of the variable to be given an initial value
     * @param value: the initial string value to be added
     */
    bool addVar(int idx, std::string value);

    /**
     *
     * Alternative signature for this function
     *
     * A function to add an array of <var> elements simultaneously
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     */
    bool addVar(int numberOfVar, InitVarValueString **var);
}; //InitVariableValuesString

/*! \class InitBasStatus
 *  \brief the InitBasStatus class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 * This class has been made redundant since OS version 2.3.
 */
class InitBasStatus
{

public:
    /** variable index */
    int idx;

    /** initial value */
    std::string value;

    /**
     *
     * Default constructor.

     */
    InitBasStatus();
    /**
     *
     * Class destructor.
     */
    ~InitBasStatus();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitBasStatus *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitBasStatus *that);
}; //InitBasStatus


/*! \class InitialBasisStatus
 *  \brief the InitialBasisStatus class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 * As of OS version 2.3 this class has been superseded by
 * the class BasisStatus (see OSGeneral.h)
 */
class InitialBasisStatus
{

public:
    /** number of <var> children */
    int numberOfVar;

    /** initial value for each variable */
    InitBasStatus **var;

    /**
     *
     * Default constructor.
     */
    InitialBasisStatus();
    /**
     *
     * Class destructor.
     */
    ~InitialBasisStatus();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitialBasisStatus *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitialBasisStatus *that);

    /**
     *
     * A function to set an array of <var> elements
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements to be that are to be set
     */
    bool setVar(int numberOfVar, InitBasStatus **var);

    /**
     *
     * A function to add a <var> element
     * @param idx: the index of the variable to be given an initial basis status
     * @param value: the initial basis status to be added
     */
    bool addVar(int idx, std::string value);
}; //InitialBasisStatus

/*! \class BranchingWeight
 *  \brief the BranchingWeight class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/11/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class BranchingWeight
{

public:
    /** index of the variable */
    int idx;

    /** optional variable name */
    std::string name;

    /** branching weight */
    double value;

    /**
     *
     * Default constructor.
     */
    BranchingWeight();
    /**
     *
     * Class destructor.
     */
    ~BranchingWeight();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(BranchingWeight *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(BranchingWeight *that);
}; //BranchingWeight



/*! \class IntegerVariableBranchingWeights
 *  \brief the IntegerVariableBranchingWeights class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/11/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class IntegerVariableBranchingWeights
{

public:
    /** number of <var> children */
    int numberOfVar;

    /** branching weight for each variable */
    BranchingWeight **var;

    /**
     *
     * Default constructor.
     */
    IntegerVariableBranchingWeights();
    /**
     *
     * Class destructor.
     */
    ~IntegerVariableBranchingWeights();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(IntegerVariableBranchingWeights *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(IntegerVariableBranchingWeights *that);

    /**
     *
     * A function to set an array of <var> elements
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements to be that are to be set
     */
    bool setVar(int numberOfVar, BranchingWeight **var);

    /**
     *
     * Alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     * @param disp: method of disposition if previous data exist
     */
    bool setVar(int numberOfVar, BranchingWeight **var, ENUM_COMBINE_ARRAYS disp);

    /**
     *
     * Another alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param idx: the array of indices
     * @param value: the array of corresponding values
     * @param name: the array of corresponding names
     */
    bool setVar(int numberOfVar, int *idx, double *value, std::string* name);

    /**
     *
     * A function to add a <var> element
     * @param idx: the index of the variable to be given a branching weight
     * @param value: the branching weight to be added
     */
    bool addVar(int idx, double value);

    /**
     *
     * Alternative signature for this function
     *
     * A function to add an array of <var> elements simultaneously
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     */
    bool addVar(int numberOfVar, BranchingWeight **var);
}; //IntegerVariableBranchingWeights

/*! \class SOSWeights
 *  \brief the SOSWeights class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/11/2008
 * @since OS 1.1
 *
 * \remarks




 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class SOSWeights
{

public:
    /** index of the SOS (to match the OSiL file) */
    int sosIdx;

    /** branching weight for the entire SOS */
    double groupWeight;

    /** number of <var> children */
    int numberOfVar;

    /** branching weights for individual variables */
    BranchingWeight **var;

    /**
     *
     * Default constructor.
     */
    SOSWeights();
    /**
     *
     * Class destructor.
     */
    ~SOSWeights();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(SOSWeights *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(SOSWeights *that);

    /**
     *
     * A function to set an array of <var> elements
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     */
    bool setVar(int numberOfVar, BranchingWeight **var);

    /**
     *
     * A function to add a <var> element
     * @param idx: the index of the variable to be given a branching weight
     * @param value: the branching weight to be added
     */
    bool addVar(int idx, double value);
}; //SOSWeights


/*! \class SOSVariableBranchingWeights
 *  \brief the SOSVariableBranchingWeights class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/11/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class SOSVariableBranchingWeights
{

public:
    /** number of <sos> children */
    int numberOfSOS;

    /** branching weights for the SOS */
    SOSWeights **sos;

    /**
     *
     * Default constructor.
     */
    SOSVariableBranchingWeights();
    /**
     *
     * Class destructor.
     */
    ~SOSVariableBranchingWeights();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(SOSVariableBranchingWeights *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(SOSVariableBranchingWeights *that);

    /**
     *
     * A function to set an array of <sos> elements
     * @param numberOfSOS: number of <sos> elements to be set
     * @param sos: the array of <sos> elements that are to be set
     */

    bool setSOS(int numberOfSOS, SOSWeights **sos);
    /**
     *
     * A function to add an <sos> element
     * @param sosIdx: the index of the SOS that is to be added (refer back to OSiL file)
     * @param nvar: the number of variables in this SOS that are to be given weights
     * @param weight: a selection weight for the entire group of variables
     * @param idx: an array of variable indices
     * @param value: the array of corresponding selection weights
     */
    bool addSOS(int sosIdx, int nvar, double weight, int* idx, double* value, std::string* name);
}; //SOSVariableBranchingWeights

/*! \class OtherVarOption
 *  \brief the OtherVarOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class OtherVarOption
{

public:
    /** variable index*/
    int idx;

    /** optional variable name */
    std::string name;

    /** value of the option */
    std::string value;

    /** lower bound on the value */
    std::string lbValue;

    /** lower bound on the value */
    std::string ubValue;

    /**
     *
     * Default constructor.
     */
    OtherVarOption();
    /**
     *
     * Class destructor.
     */
    ~OtherVarOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherVarOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherVarOption *that);
}; //OtherVarOption


/*! \class OtherVariableOption
 *  \brief the OtherVariableOption class.
 *
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class OtherVariableOption
{

public:
    /** number of <var> child elements */
    int numberOfVar;

    /** number of <enumeration> child elements */
    int numberOfEnumerations;

    /** name of the option */
    std::string name;

    /** value of the option */
    std::string value;

    /** name of the solver to which this option applies */
    std::string solver;

    /** name of the category into which this option falls */
    std::string category;

    /** type of the option value (integer, double, boolean, string) */
    std::string type;

    /** description of the option */
    std::string description;

    /** array of option values */
    OtherVarOption **var;

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
    OtherVariableOption();
    /**
     *
     * Class destructor.
     */
    ~OtherVariableOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherVariableOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherVariableOption *that);

    /**
     *
     * A function to set an array of <var> elements
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     */
    bool setVar(int numberOfVar, OtherVarOption **var);

    /**
     *
     * A function to add a <var> element
     * @param idx: the index of the variable
     * @param value: the value associated with this variable
     * @param lbValue: a lower bound associated with this variable
     * @param ubValue: an upper bound associated with this variable
     */
    bool addVar(int idx, std::string value, std::string lbValue, std::string ubValue);
}; //OtherVariableOption


/*! \class VariableOption
 *  \brief the VariableOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class VariableOption
{

public:
    /** number of <other> child elements */
    int numberOfOtherVariableOptions;

    /** initial values for the variables */
    InitVariableValues *initialVariableValues;

    /** initial values for string-valued variables */
    InitVariableValuesString *initialVariableValuesString;

    /** initial basis information*/
    BasisStatus *initialBasisStatus;

    /** branching weights for integer variables */
    IntegerVariableBranchingWeights *integerVariableBranchingWeights;

    /** branching weights for SOS variables and groups */
    SOSVariableBranchingWeights *sosVariableBranchingWeights;

    /** other variable options */
    OtherVariableOption **other;

    /**
     *
     * Default constructor.
     */
    VariableOption();
    /**
     *
     * Class destructor.
     */
    ~VariableOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(VariableOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(VariableOption *that);

    /**
     *
     * A function to set an array of <other> elements
     * @param numberOfOptions: number of <other> elements to be set
     * @param other: the array of <other> elements that are to be set
     */
    bool setOther(int numberOfOptions, OtherVariableOption  **other);

    /**
     *
     * A function to add an <other> element
     * @param other: the content of the <other> element to be added
     */
    bool addOther(OtherVariableOption *other);
}; //VariableOption


/*! \class InitObjValue
 *  \brief the InitObjValue class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitObjValue
{

public:
    /** objective index */
    int idx;

    /** optional objective name */
    std::string name;

    /** initial value */
    double value;

    /**
     *
     * Default constructor.
     */
    InitObjValue();
    /**
     *
     * Class destructor.
     */
    ~InitObjValue();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitObjValue *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitObjValue *that);
}; //InitObjValue


/*! \class InitObjectiveValues
 *  \brief the InitObjectiveValues class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitObjectiveValues
{

public:
    /** number of <obj> children */
    int numberOfObj;

    /** initial value for each objective */
    InitObjValue **obj;

    /**
     *
     * Default constructor.
     */
    InitObjectiveValues();
    /**
     *
     * Class destructor.
     */
    ~InitObjectiveValues();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitObjectiveValues *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitObjectiveValues *that);

    /**
     *
     * A function to set an array of <obj> elements
     * @param numberOfObj: number of <obj> elements to be set
     * @param obj: the array of <obj> elements that are to be set
     */
    bool setObj(int numberOfObj, InitObjValue **obj);

    /**
     *
     * Alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     * @param disp: method of disposition if previous data exist
     */
    bool setObj(int numberOfObj, InitObjValue **obj, ENUM_COMBINE_ARRAYS disp);


    /**
     *
     * Another alternative signature for this function
	 * @param numberOfObj: number of <obj> elements to be set
     * @param idx: the array of indices
     * @param value: the array of corresponding values
     * @param name: the array of objective names
     */
    bool setObj(int numberOfObj, int *idx, double *value, std::string *name);

    /**
     *
     * A function to add a <obj> element
     * @param idx: the index of the objective to be given an initial value
     * @param value: the initial value to be added
     */
    bool addObj(int idx, double value);

    /**
     *
     * Alternative signature for this function
     *
     * A function to add an array of <obj> elements simultaneously
     * @param numberOfObj: number of <obj> elements to be set
     * @param obj: the array of <obj> elements that are to be set
     */
    bool addObj(int numberOfObj, InitObjValue **obj);
}; //InitObjectiveValues


/*! \class InitObjBound
 *  \brief the InitObjBound class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitObjBound
{

public:
    /** objective index */
    int idx;

    /** optional variable name */
    std::string name;

    /** initial lower bound */
    double lbValue;

    /** initial upper bound */
    double ubValue;

    /**
     *
     * Default constructor.
     */
    InitObjBound();
    /**
     *
     * Class destructor.
     */
    ~InitObjBound();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitObjBound *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitObjBound *that);
}; //InitObjBound


/*! \class InitObjectiveBounds
 *  \brief the InitObjectiveBounds class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitObjectiveBounds
{

public:
    /** number of <obj> children */
    int numberOfObj;

    /** initial bounds for each objective */
    InitObjBound **obj;

    /**
     *
     * Default constructor.
     */
    InitObjectiveBounds();
    /**
     *
     * Class destructor.
     */
    ~InitObjectiveBounds();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitObjectiveBounds *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitObjectiveBounds *that);

    /**
     *
     * A function to set an array of <obj> elements
     * @param numberOfObj: number of <obj> elements to be set
     * @param obj: the array of <obj> elements that are to be set
     */
    bool setObj(int numberOfObj, InitObjBound **obj);

    /**
     *
     * Alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     * @param disp: method of disposition if previous data exist
     */
    bool setObj(int numberOfObj, InitObjBound **obj, ENUM_COMBINE_ARRAYS disp);


    /**
     *
     * Another alternative signature for this function
     * @param numberOfObj: number of <obj> elements to be set
     * @param idx: the array of indices
     * @param lbValue: the array of corresponding lower bounds
     * @param ubValue: the array of corresponding upper bounds
     * @param name: the array of objective names
     */
    bool setObj(int numberOfObj, int *idx, double *lbValue, double *ubValue, std::string *name);

    /**
     *
     * A function to add a <obj> element
     * @param idx: the index of the objective to be given initial bounds
     * @param lbValue: the initial lower bound for the objective
     * @param ubValue: the initial upper bound for the objective
     */
    bool addObj(int idx, double lbValue, double ubValue);

    /**
     *
     * Alternative signature for this function
     *
     * A function to add an array of <obj> elements simultaneously
     * @param numberOfObj: number of <obj> elements to be set
     * @param obj: the array of <obj> elements that are to be set
     */
    bool addObj(int numberOfObj, InitObjBound **obj);
}; //InitObjectiveBounds


/*! \class OtherObjOption
 *  \brief the OtherObjOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class OtherObjOption
{

public:
    /** variable index*/
    int idx;

    /** optional variable name */
    std::string name;

    /** value of the option */
    std::string value;


    /** lower bound on the value */
    std::string lbValue;

    /** lower bound on the value */
    std::string ubValue;
    /**
     *
     * Default constructor.
     */
    OtherObjOption();
    /**
     *
     * Class destructor.
     */
    ~OtherObjOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherObjOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherObjOption *that);
}; //OtherObjOption


/*! \class OtherObjectiveOption
 *  \brief the OtherObjectiveOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class OtherObjectiveOption
{

public:
    /** number of <obj> children */
    int numberOfObj;

    /** number of <enumeration> child elements */
    int numberOfEnumerations;

    /** name of the option */
    std::string name;

    /** value of the option */
    std::string value;

    /** name of the solver to which this option applies */
    std::string solver;

    /** name of the category into which this option falls */
    std::string category;

    /** type of the option value (integer, double, boolean, string) */
    std::string type;

    /** description of the option */
    std::string description;

    /** array of option values */
    OtherObjOption **obj;

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
    OtherObjectiveOption();
    /**
     *
     * Class destructor.
     */
    ~OtherObjectiveOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherObjectiveOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherObjectiveOption *that);

    /**
     *
     * A function to set an array of <obj> elements
     * @param numberOfObj: number of <obj> elements to be set
     * @param obj: the array of <obj> elements that are to be set
     */
    bool setObj(int numberOfObj, OtherObjOption **obj);

    /**
     *
     * A function to add a <obj> element
     * @param idx: the index of the objective
     * @param value: the value associated with this objective
     * @param lbValue: a lower bound associated with this objective
     * @param ubValue: an upper bound associated with this objective
     */
    bool addObj(int idx, std::string value, std::string lbValue, std::string ubValue);
}; //OtherObjectiveOption


/*! \class ObjectiveOption
 *  \brief the ObjectiveOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class ObjectiveOption
{

public:
    /** number of <other> child elements */
    int numberOfOtherObjectiveOptions;

    /** initial values for the objectives */
    InitObjectiveValues *initialObjectiveValues;

    /** initial bounds for the objectives */
    InitObjectiveBounds *initialObjectiveBounds;

    /** initial basis status for the objectives */
    BasisStatus *initialBasisStatus;

    /** other information about the objectives */
    OtherObjectiveOption **other;

    /**
     *
     * Default constructor.
     */
    ObjectiveOption();
    /**
     *
     * Class destructor.
     */
    ~ObjectiveOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ObjectiveOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(ObjectiveOption *that);

    /**
     *
     * A function to set an array of <other> elements
     * @param numberOfOptions: number of <other> elements to be set
     * @param other: the array of <other> elements that are to be set
     */
    bool setOther(int numberOfOptions, OtherObjectiveOption  **other);



    /**
     *
     * A function to add an <other> element
     * @param other: the content of the <other> element to be added
     */
    bool addOther(OtherObjectiveOption *other);
}; //ObjectiveOption


/*! \class InitConValue
 *  \brief the InitConValue class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitConValue
{

public:
    /** constraint index */
    int idx;

    /** optional variable name */
    std::string name;

    /** initial value */
    double value;

    /**
     *
     * Default constructor.
     */
    InitConValue();
    /**
     *
     * Class destructor.
     */
    ~InitConValue();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitConValue *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitConValue *that);
}; //InitConValue


/*! \class InitConstraintValues
 *  \brief the InitConstraintValues class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitConstraintValues
{

public:
    /** number of <con> children */
    int numberOfCon;

    /** initial value for each constraint */
    InitConValue **con;

    /**
     *
     * Default constructor.
     */
    InitConstraintValues();
    /**
     *
     * Class destructor.
     */
    ~InitConstraintValues();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitConstraintValues *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitConstraintValues *that);

    /**
     *
     * A function to set an array of <con> elements
     * @param numberOfCon: number of <con> elements to be set
     * @param con: the array of <con> elements that are to be set
     */
    bool setCon(int numberOfCon, InitConValue **con);

    /**
     *
     * Alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     * @param disp: method of disposition if previous data exist
     */
    bool setCon(int numberOfCon, InitConValue **con, ENUM_COMBINE_ARRAYS disp);


    /**
     *
     * Another alternative signature for this function
     * @param numberOfCon: number of <con> elements to be set
     * @param idx: the array of indices
     * @param value: the array of corresponding values
     * @param name: the array of constraint names
     */
    bool setCon(int numberOfCon, int *idx, double *value, std::string *name);

    /**
     *
     * A function to add a <con> element
     * @param idx: the index of the constraint to be given an initial value
     * @param value: the initial value to be added
     */
    bool addCon(int idx, double value);

    /**
     *
     * Alternative signature for this function
     *
     * A function to add an array of <con> elements simultaneously
     * @param numberOfCon: number of <con> elements to be set
     * @param obj: the array of <con> elements that are to be set
     */
    bool addCon(int numberOfCon, InitConValue **con);
}; //InitConstraintValues


/*! \class InitDualVarValue
 *  \brief the InitDualVarValue class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitDualVarValue
{

public:
    /** constraint index */
    int idx;

    /** optional variable name */
    std::string name;

    /** initial lower bound */
    double lbDualValue;

    /** initial upper bound */
    double ubDualValue;

    /**
     *
     * Default constructor.
     */
    InitDualVarValue();

    /**
     *
     * Class destructor.
     */
    ~InitDualVarValue();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitDualVarValue *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitDualVarValue *that);
}; //InitDualVarValue


/*! \class InitDualVariableValues
 *  \brief the InitDualVariableValues class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class InitDualVariableValues
{

public:
    /** number of <con> children */
    int numberOfCon;

    /** initial dual values for each constraint */
    InitDualVarValue **con;

    /**
     *
     * Default constructor.
     */
    InitDualVariableValues();
    /**
     *
     * Class destructor.
     */
    ~InitDualVariableValues();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(InitDualVariableValues *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(InitDualVariableValues *that);

    /**
     *
     * A function to set an array of <con> elements
     * @param numberOfCon: number of <con> elements to be set
     * @param con: the array of <con> elements that are to be set
     */
    bool setCon(int numberOfCon, InitDualVarValue **con);

    /**
     *
     * Alternative signature for this function
     * @param numberOfVar: number of <var> elements to be set
     * @param var: the array of <var> elements that are to be set
     * @param disp: method of disposition if previous data exist
     */
    bool setCon(int numberOfCon, InitDualVarValue **con, ENUM_COMBINE_ARRAYS disp);


    /**
     *
     * Another alternative signature for this function
     * @param numberOfCon: number of <con> elements to be set
     * @param idx: the array of indices
     * @param lbValue: the array of dual values for the lower bound
     * @param ubValue: the array of dual values for the upper bound
     * @param name: the array of constraint names
     */
    bool setCon(int numberOfCon, int *idx, double *lbValue, double *ubValue, std::string *name);

    /**
     *
     * A function to add a <con> element
     * @param idx: the index of the constraint to be given initial dual variables
     * @param lbDualValue: an initial value for the dual variable associated with the lower bound
     * @param ubDualValue: an initial value for the dual variable associated with the upper bound
     */
    bool addCon(int idx, double lbDualValue, double ubDualValue);

    /**
     *
     * Alternative signature for this function
     *
     * A function to add an array of <con> elements simultaneously
     * @param numberOfCon: number of <con> elements to be set
     * @param obj: the array of <con> elements that are to be set
     */
    bool addCon(int numberOfCon, InitDualVarValue **con);
}; //InitDualVariableValues


/*! \class OtherConOption
 *  \brief the OtherConOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class OtherConOption
{

public:
    /** variable index*/
    int idx;

    /** optional variable name */
    std::string name;

    /** value of the option */
    std::string value;

    /** lower bound of the option */
    std::string lbValue;

    /** upper bound of the option */
    std::string ubValue;

    /**
     *
     * Default constructor.
     */
    OtherConOption();
    /**
     *
     * Class destructor.
     */
    ~OtherConOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherConOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherConOption *that);
}; //OtherConOption


/*! \class OtherConstraintOption
 *  \brief the OtherConstraintOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class OtherConstraintOption
{

public:
    /** number of <con> children */
    int numberOfCon;

    /** number of <enumeration> child elements */
    int numberOfEnumerations;

    /** name of the option */
    std::string name;

    /** value of the option */
    std::string value;

    /** name of the solver to which this option applies */
    std::string solver;

    /** name of the category into which this option falls */
    std::string category;

    /** type of the option value (integer, double, boolean, string) */
    std::string type;

    /** description of the option */
    std::string description;

    /** array of option values */
    OtherConOption **con;

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
    OtherConstraintOption();
    /**
     *
     * Class destructor.
     */
    ~OtherConstraintOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OtherConstraintOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OtherConstraintOption *that);

    /**
     *
     * A function to set an array of <con> elements
     * @param numberOfCon: number of <con> elements to be set
     * @param obj: the array of <con> elements that are to be set
     */
    bool setCon(int numberOfCon, OtherConOption **con);

    /**
     *
     * A function to add a <con> element
     * @param idx: the index of the constraint
     * @param value: the value associated with this constraint
     * @param lbValue: a lower bound associated with this constraint
     * @param ubValue: an upper bound associated with this constraint
     */
    bool addCon(int idx, std::string value, std::string lbValue, std::string ubValue);
}; //OtherConstraintOption


/*! \class ConstraintOption
 *  \brief the ConstraintOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class ConstraintOption
{

public:
    /** number of <other> child elements */
    int numberOfOtherConstraintOptions;

    /** initial values for the constraints */
    InitConstraintValues *initialConstraintValues;

    /** initial dual values for the constraints */
    InitDualVariableValues *initialDualValues;

    /** initial basis status for the slack variables */
    BasisStatus *initialBasisStatus;

    /** other information about the constraints */
    OtherConstraintOption **other;

    /**
     *
     * Default constructor.
     */
    ConstraintOption();
    /**
     *
     * Class destructor.
     */
    ~ConstraintOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(ConstraintOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(ConstraintOption *that);

    /**
     *
     * A function to set an array of <other> elements
     * @param numberOfOptions: number of <other> elements to be set
     * @param other: the array of <other> elements that are to be set
     */
    bool setOther(int numberOfOptions, OtherConstraintOption  **other);


    /**
     *
     * A function to add an <other> element
     * @param other: the content of the <other> element to be added
     */
    bool addOther(OtherConstraintOption *other);
}; //ConstraintOption


/*! \class SolverOption
 *  \brief the SolverOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class SolverOption
{

public:
    /** the name of the option */
    std::string name;

    /** the value of the option */
    std::string value;

    /** the solver to which the option applies */
    std::string solver;

    /** the category to which the option belongs */
    std::string category;

    /** the type of the option value (integer, double, boolean, string) */
    std::string type;

    /** the description of the option */
    std::string description;

    /** the number of items (additional pieces of data) of the option */
    int numberOfItems;

    /** the list of items of the option */
    std::string* item;

    /**
     *
     * Default constructor.
     */
    SolverOption();
    /**
     *
     * Class destructor.
     */
    ~SolverOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(SolverOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(SolverOption *that);
}; //SolverOption


/*! \class SolverOptions
 *  \brief the SolverOptions class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class SolverOptions
{

public:
    /** the number of solver options */
    int numberOfSolverOptions;

    /** the list of solver options */
    SolverOption **solverOption;

    /**
     *
     * Default constructor.
     */
    SolverOptions();
    /**
     *
     * Class destructor.
     */
    ~SolverOptions();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(SolverOptions *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(SolverOptions *that);

    /**
     *
     * A function to set an array of solver options
     * @param numberOfOptions: number of solver options to be set
     * @param solverOption: the array of solver options that are to be set
     */
    bool setSolverOptions(int numberOfOptions, SolverOption **solverOption);

    /**
     *
     * A function to add a solver option
     * @param name: the name of the solver option (required)
     * @param value: a value associated with the option (optional)
     * @param solver: the solver to which the option applies (optional)
     * @param category: the category (and subcategories) of the option (optional)
     * @param type: the type of the option (optional)
     * @param description: a description associated with the option (optional)
     */
    bool addSolverOption(std::string name, std::string value, std::string solver,
                         std::string category, std::string type, std::string description);
}; //SolverOptions


/*! \class OptimizationOption
 *  \brief the OptimizationOption class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A data structure class that corresponds to an xml element in
 * the OSoL schema.
 */
class OptimizationOption
{

public:
    /** the number of variables */
    int numberOfVariables;

    /** the number of objectives */
    int numberOfObjectives;

    /** the number of constraints */
    int numberOfConstraints;

    /** the options for the variables */
    VariableOption *variables;

    /** the options for the objectives */
    ObjectiveOption *objectives;

    /** the options for the constraints */
    ConstraintOption *constraints;

    /** other solver options */
    SolverOptions *solverOptions;
    /**
     *
     * Default constructor.
     */
    OptimizationOption();
    /**
     *
     * Class destructor.
     */
    ~OptimizationOption();

    /**
     *
     * A function to check for the equality of two objects
     */
    bool IsEqual(OptimizationOption *that);

    /**
     *
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an instance of this class
     * @param that: the instance from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OptimizationOption *that);
}; //OptimizationOption


/*! \class OSOption
 *  \brief The Option Class.
 *
 * @author Robert Fourer, Horand Gassmann, Jun Ma, Kipp Martin
 * @version 1.0, 21/07/2008
 * @since OS 1.1
 *
 * \remarks
 * A class for holding all the solver options information.
 */
class OSOption
{

public:

    /**
     * OSOption has a header and five other children: general, system, service, job, and optimization.
     */

    /**
     * header information
     */
    GeneralFileHeader *optionHeader;
    /**
     * generalOption holds the first child of the OSOption specified by the OSoL Schema.
     */
    GeneralOption *general;
    /**
     * systemOption holds the second child of the OSOption specified by the OSoL Schema.
     */
    SystemOption *system;
    /**
     * serviceOption holds the third child of the OSOption specified by the OSoL Schema.
     */
    ServiceOption *service;
    /**
     * jobOption holds the fourth child of the OSOption specified by the OSoL Schema.
     */
    JobOption *job;
    /**
     * optimizationOption holds the fifth child of the OSOption specified by the OSoL Schema.
     */
    OptimizationOption *optimization;

    /**
     *
     * Default constructor.
     */
    OSOption();
    /**
     *
     * Class destructor.
     */
    ~OSOption();

    /**
     *
     * A function to populate an instance of the option header element
     * @param name: the name of this file or instance
     * @param source: the source (e.g., in BiBTeX format)
     * @param fileCreator: the creator of this file
     * @param description: further description about this file and/or its contents
     * @param licence: licence information if applicable
     */
    bool setHeader(std::string name, std::string source,
                         std::string description, std::string fileCreator, std::string licence);

private:
    /**
     *  m_mdInitVarValuesDense holds the initial variable values in dense form
     */
    double* m_mdInitVarValuesDense;

    /**
     *  m_mdInitVarValuesStringDense holds the initial values for string-valued variables
     */
    std::string* m_mdInitVarValuesStringDense;

    /**
     *  m_mdInitBasisStatusDense holds the initial basis status in dense form
     */
    int* m_mdInitBasisStatusDense;

    /**
     *  m_mdIntegerVariableBranchingWeightsDense holds the branching weights for integer variables
     */
    double* m_mdIntegerVariableBranchingWeightsDense;

    /**
     *  m_mdInitObjValuesDense holds initial objective values in dense form
     */
    double* m_mdInitObjValuesDense;

    /**
     *  m_mdInitObjLowerBoundsDense holds initial lower bounds for the objectives in dense form
     */
    double* m_mdInitObjLowerBoundsDense;

    /**
     *  m_mdInitObjUpperBoundsDense holds initial upper bounds for the objectives in dense form
     */
    double* m_mdInitObjUpperBoundsDense;

    /**
     *  m_mdInitConValuesDense holds initial constraint values in dense form
     */
    double* m_mdInitConValuesDense;

    /**
     *  m_mdInitDualVarLowerBoundsDense holds initial dual values for the
     *  lower bound of the constraints in dense form
     */
    double* m_mdInitDualVarLowerBoundsDense;

    /**
     *  m_mdInitDualVarUpperBoundsDense holds initial dual values for the
     *  upper bound of the constraints in dense form
     */
    double* m_mdInitDualVarUpperBoundsDense;

public:
    /**
     * A function to check for the equality of two objects
     */
    bool IsEqual(OSOption *that);

    /**
     * A function to make a random instance of this class
     * @param density: corresponds to the probability that a particular child element is created
     * @param conformant: if true enforces side constraints not enforceable in the schema
     *     (e.g., agreement of "numberOfXXX" attributes and <XXX> children)
     */
    bool setRandom(double density, bool conformant);

    /**
     * A function to make a deep copy of an OSOption object
     * @param that: the OSOption object from which information is to be copied
     * @return whether the copy was created successfully
     */
    bool deepCopyFrom(OSOption *that);

public:

    // get() methods...

    /**
     * Get the name of the file
     */
    std::string  getFileName();

    /**
     * Get the source of the file or problem
     */
    std::string  getFileSource();

    /**
     * Get a description for the file or problem
     */
    std::string  getFileDescription();

    /**
     * Get the name of the person who created the file
     */
    std::string  getFileCreator();

    /**
     * Get licence information associated with the file
     */
    std::string  getFileLicence();

    /**
     * Get the service URI
     */
    std::string  getServiceURI();

    /**
     * Get the service name
     */
    std::string  getServiceName();

    /**
     * Get the instance name
     */
    std::string  getInstanceName();

    /**
     * Get the instance location
     */
    std::string  getInstanceLocation();

    /**
     * Get the location type
     */
    std::string  getInstanceLocationType();

    /**
     * Get the job ID

     */
    std::string  getJobID();

    /**
     * Get the solver name
     */
    std::string  getSolverToInvoke();

    /**
     * Get the license string
     */
    std::string  getLicense();

    /**
     * Get the user name
     */
    std::string  getUserName();

    /**
     * Get the password
     */
    std::string  getPassword();

    /**
     * Get the contact information
     */
    std::string  getContact();

    /**
     * Get the transport type
     */
    std::string  getContactTransportType();

    /**
     * Get the disk space unit
     */
    std::string  getMinDiskSpaceUnit();

    /**
     * get the disk space description
     */
    std::string  getMinDiskSpaceDescription();

    /**
     * Get the memory unit
     */
    std::string  getMinMemoryUnit();

    /**
     * get the memory description
     */
    std::string  getMinMemoryDescription();

    /**
     * Get the CPU speed unit
     */
    std::string  getMinCPUSpeedUnit();

    /**
     * Get the CPU speed description
     */
    std::string  getMinCPUSpeedDescription();

    /**
     * Get the CPU description
     */
    std::string  getMinCPUNumberDescription();

    /**
     * Get the service type
     */
    std::string  getServiceType();

    /**
     * Get the time unit
     */
    std::string  getMaxTimeUnit();

    /**
     * Get the requested starting time
     */
    std::string  getRequestedStartTime();

    /**
     * Get any of the string-valued options
     */
    std::string  getOptionStr(std::string optionName);

    /**
     * Get the minimum required disk space
     */
    double  getMinDiskSpace();

    /**
     * Get the minimum required memory
     */
    double  getMinMemorySize();

    /**
     * Get the minimum required CPU speed
     */
    double  getMinCPUSpeed();

    /**
     * Get the maximum allowed time
     */
    double  getMaxTime();

    /**
     * Get any of the double-valued options
     */
    double  getOptionDbl(std::string optionName);

    /**
     * Get the minimum required number of CPUs
     */
    int  getMinCPUNumber();

    /**
     * Get the number of <other> options in the <general> element
     */
    int  getNumberOfOtherGeneralOptions();

    /**
     * Get the number of <other> options in the <system> element
     */
    int  getNumberOfOtherSystemOptions();

    /**
     * Get the number of <other> options in the <service> element
     */
    int  getNumberOfOtherServiceOptions();

    /**
     * Get the number of <other> options in the <job> element
     */
    int  getNumberOfOtherJobOptions();

    /**
     * Get the number of job dependencies
     */
    int  getNumberOfJobDependencies();

    /**
     * Get the number of required directories
     */
    int  getNumberOfRequiredDirectories();

    /**
     * Get the number of required files
     */
    int  getNumberOfRequiredFiles();

    /**
     * Get the number of directories to make
     */
    int  getNumberOfDirectoriesToMake();

    /**
     * Get the number of files to make
     */
    int  getNumberOfFilesToMake();

    /**
     * Get the number of input directories to move
     */
    int  getNumberOfInputDirectoriesToMove();

    /**
     * Get the number of input files to move
     */
    int  getNumberOfInputFilesToMove();

    /**
     * Get the number of output directories to move
     */
    int  getNumberOfOutputDirectoriesToMove();

    /**
     * Get the number of output files to move
     */
    int  getNumberOfOutputFilesToMove();

    /**
     * Get the number of files to delete
     */
    int  getNumberOfFilesToDelete();

    /**
     * Get the number of directories to delete
     */
    int  getNumberOfDirectoriesToDelete();

    /**
     * Get the number of processes to kill
     */
    int  getNumberOfProcessesToKill();

    /**
     * Get the number of variables in the instance
     */
    int  getNumberOfVariables();

    /**
     * Get the number of objectives in the instance
     */
    int  getNumberOfObjectives();

    /**
     * Get the number of constraints in the instance
     */
    int  getNumberOfConstraints();

    /**
     * Get the number of initial variable values.
     *
     * @return the number of initial variable values.
     */
    int getNumberOfInitVarValues();

    /**
     * Get the number of initial variable strings.
     *
     * @return the number of initial variable strings.
     */
    int getNumberOfInitVarValuesString();

    /**

     * Get the number of variables for which integer branching weights are provided.
     *
     * @return the number of variables.
     */
    int getNumberOfIntegerVariableBranchingWeights();

    /**
     * Get the number of special ordered sets for which branching weights are provided.
     *
     * @return the number of variables.
     */
    int getNumberOfSOS();

    /**
     * Get the number of variables for which branching weights are provided in a particular SOS.
     * @param iSOS the number of the SOS
     * @return the number of variables.
     */
    int getNumberOfSOSVarBranchingWeights(int iSOS);

    /**
     * Get the number of other variable options
     *
     * @return the number of other variable options.
     */
    int getNumberOfOtherVariableOptions();

    /**
     * Get the number of initial objective values.
     *
     * @return the number of initial objective values.
     */
    int getNumberOfInitObjValues();

    /**
     * Get the number of initial objective bounds.
     *
     * @return the number of initial objective bound values.
     */
    int getNumberOfInitObjBounds();

    /**
     * Get the number of other objective options
     *
     * @return the number of other objective options.
     */
    int getNumberOfOtherObjectiveOptions();

    /**
     * Get the number of initial constraint values.
     *
     * @return the number of initial constraint values.
     */
    int getNumberOfInitConValues();

    /**
     * Get the number of initial dual variable values.
     *
     * @return the number of initial dual variable values.
     */
    int getNumberOfInitDualVarValues();

    /**
     * Get the number of other constraint options
     *
     * @return the number of other constraint options.
     */
    int getNumberOfOtherConstraintOptions();

    /**
     * Get the number of solver options.
     *
     * @return the number of solver options.
     */
    int getNumberOfSolverOptions();

    /**
     * Get any of the integer-valued options
     */
    int getOptionInt(std::string optionName);


    /**
     * Get the array of other options associated with the <general> element
     * <p>
     *
     * @return a vector of pointers to otherOptions objects
     * associated with the <general> element
     */
    OtherOption** getOtherGeneralOptions();

    /**
     * Get the array of other options associated with the <system> element
     * <p>
     *
     * @return a vector of pointers to otherOptions objects
     * associated with the <system> element
     */
    OtherOption** getOtherSystemOptions();

    /**
     * Get the array of other options associated with the <service> element
     * <p>
     *
     * @return a vector of pointers to otherOptions objects
     * associated with the <service> element
     */
    OtherOption** getOtherServiceOptions();

    /**
     * Get the array of other options associated with the <job> element
     * <p>
     *
     * @return a vector of pointers to otherOptions objects
     * associated with the <job> element
     */
    OtherOption** getOtherJobOptions();

    /**
     * Get the array of other options associated with any element
     * <p>
     *
     * @return a vector of pointers to otherOptions objects
     * associated with the element whose name matches elementName
     */
    OtherOption** getOtherOptions(std::string elementName);


    /**
     * Get the array of all other options associated with the
     ** <general>, <system>, <service> and <job> elements
     * <p>
     *
     * @return a vector of pointers to all otherOptions objects
     */
    OtherOption** getAllOtherOptions();

    /**
     * Get the array of job dependencies
     * <p>
     *
     * @return a vector of pointers to JobDependencies objects
     */
    std::string* getJobDependencies();

    /**
     * Get the array of required directories
     * <p>
     *
     * @return a vector of pointers to DirectoriesAndFiles objects
     * giving the directories that are required by the current job
     */
    std::string* getRequiredDirectories();

    /**
     * Get the array of required files
     * <p>
     *
     * @return a vector of pointers to DirectoriesAndFiles objects
     * giving the files that are required by the current job
     */
    std::string* getRequiredFiles();

    /**
     * Get the array of directories to make
     * <p>
     *
     * @return a vector of pointers to DirectoriesAndFiles objects
     * giving the directories that must be created
     */
    std::string* getDirectoriesToMake();

    /**
     * Get the array of files to make
     * <p>
     *
     * @return a vector of pointers to DirectoriesAndFiles objects
     * giving the files that must be created
     */
    std::string* getFilesToMake();

    /**
     * Get the array of input directories to move
     * <p>
     *
     * @return a vector of pointers to PathPair objects
     * giving the input directories that must be moved
     */
    PathPair** getInputDirectoriesToMove();

    /**
     * Get the array of input files to move
     * <p>
     *
     * @return a vector of pointers to PathPair objects
     * giving the input files that must be moved
     */
    PathPair** getInputFilesToMove();

    /**
     * Get the array of output directories to move
     * <p>
     *
     * @return a vector of pointers to PathPair objects
     * giving the output directories that must be moved
     */
    PathPair** getOutputDirectoriesToMove();

    /**
     * Get the array of output files to move
     * <p>
     *
     * @return a vector of pointers to PathPair objects
     * giving the output files that must be moved
     */
    PathPair** getOutputFilesToMove();

    /**
     * Get the array of directories to delete
     * <p>
     *
     * @return a vector of pointers to DirectoriesAndFiles objects
     * giving the directories that must be deleted
     */
    std::string* getDirectoriesToDelete();

    /**
     * Get the array of files to delete
     * <p>
     *
     * @return a vector of pointers to DirectoriesAndFiles objects
     * giving the files that must be deleted
     */
    std::string* getFilesToDelete();

    /**
     * Get the array of processes to kill
     * <p>
     *
     * @return a vector of pointers to Processes objects
     * giving the processes that must be killed
     */
    std::string* getProcessesToKill();

    /**
     * Get the initial values associated with the variables in sparse form
     * <p>
     *
     * @return a vector of pointers to InitVarValue objects that
     * hold initial values for (some of) the variables
     */
    InitVarValue** getInitVarValuesSparse();

    /**
     * Get the initial values associated with the variables in dense form
     * <p>
     *
     * @return a vector of double that
     * holds initial values (or OSNaN()) for all of the variables
     */
    double* getInitVarValuesDense();

    /**
     * Get the initial values associated with the variables in dense form
     * <p>
     *
     * @param numberOfVariables holds the dimension of the vector
     * @return a vector of double that
     * holds initial values (or OSNaN()) for all of the variables
     */
    double* getInitVarValuesDense(int numberOfVariables);

    /**
     * Get the initial value strings associated with the variables in sparse form
     * <p>
     *
     * @return a vector of pointers to InitVarValueString objects that
     * hold initial value strings for (some of) the variables
     */
    InitVarValueString** getInitVarValuesStringSparse();

    /**
     * Get the initial value strings associated with the variables in dense form
     * <p>
     *
     * @return a vector of strings that
     * holds initial value strings (or "") for all of the variables
     */
    std::string *getInitVarValuesStringDense();

    /**
     * Get the initial value strings associated with the variables in dense form
     * <p>
     *
     * @param numberOfVariables holds the dimension of the vector
     * @return a vector of strings that
     * holds initial value strings (or "") for all of the variables
     */
    std::string *getInitVarValuesStringDense(int numberOfVariables);

    /**
     * Get the initial basis status in sparse form
     * <p>
     *
     * @return a vector of pointers to InitBasStatus objects that
     * hold initial basis status for (some of) the variables
     */
    InitBasStatus** getInitBasisStatusSparse();

    /**
     * Get the initial basis information in dense form
     * <p>
     *
     * @return a vector of strings that
     * holds initial basis status (or "unknown") for all of the variables
     */
    std::string *getInitBasisStatusDense();

    /**
     * Get the initial basis status for all variables in dense form
     * <p>
     *
     * @return an array of int, with values corresponding to ENUM_BASIS_STATUS -- see OSGeneral.h)
     * @note returns ENUM_BASIS_STATUS_unknown for variables that are not initialed
     * @param numberOfVariables is the dimension of the array
     */
    int* getVariableInitialBasisStatusDense(int numberOfVariables);

    /**
     * Get the number of initial basis elements for a particular variable type and basis status
     * <p>
     *
     * @return the number of elements
     * @param type: the type of variable or problem component
     *		(contained in ENUM_PROBLEM_COMPONENT --- see OSGeneral.h)
     * @param status: the basis status
     *		(contained in ENUM_BASIS_STATUS --- see OSGeneral.h)
     */
    int getNumberOfInitialBasisElements(int type, int status);

    /**
     * Get the initial basis elements for a particular variable type and basis status
     * <p>
     *
     * @return whether the operation was successful or not
     * @param type: the type of variable or problem component
     *		(contained in ENUM_PROBLEM_COMPONENT --- see OSGeneral.h)
     * @param status: the basis status
     *		(contained in ENUM_BASIS_STATUS --- see OSGeneral.h)
     * @param elem: pointer to the memory location where the user wants to store the returned values
     */
    bool getInitialBasisElements(int type, int status, int* elem);

    /**
     * Get the integer branching weights in sparse form
     * <p>
     *
     * @return a vector of pointers to BranchingWeight objects that
     * hold branching weights for (some of) the variables
     */
    BranchingWeight**  getIntegerVariableBranchingWeightsSparse();

    /**
     * Get the integer branching weights in dense form
     * <p>
     *
     * @return a vector of double that
     * holds branching weights (or OSNaN()) for all the variables
     */
    double* getIntegerVariableBranchingWeightsDense();

    /**
     * Get the integer branching weights in dense form
     * <p>
     *
     * @param numberOfVariables holds the dimension of the vector
     * @return a vector of double that
     * holds branching weights (or OSNaN()) for all the variables
     */
    double* getIntegerVariableBranchingWeightsDense(int numberOfVariables);

    /**
     * Get the SOS branching weights in sparse form
     * <p>
     *
     * @return a vector of pointers to SOSWeights objects that
     * hold branching weights for (some of) the variables contained in special ordered sets
     */
    SOSWeights** getSOSVariableBranchingWeightsSparse();

    /**
     * Get the <other> variable options associated with a particular solver
     * <p>
     *
     * @param solver_name is the name of the solver whose options we want
     * @return a vector of pointers to OtherVariableOption objects
     * that correspond to the solver named.
     */
    std::vector<OtherVariableOption*> getOtherVariableOptions(std::string solver_name);

    /**
     * Get one particular <other> variable option from the array of options
     * <p>
     *
     * @param optionNumber is the index of the option in the array
     * @return a pointer to one OtherVariableOption object
     */
    OtherVariableOption* getOtherVariableOption(int optionNumber);

    /**
     * Get all <other> variable options
     * <p>
     *
     * @return a pointer to an array of OtherVariableOption objects
     */
    OtherVariableOption** getAllOtherVariableOptions();

    /**
     * Get the initial values associated with the objectives in sparse form
     * <p>
     *
     * @return a vector of pointers to InitObjValue objects that
     * hold initial values for (some of) the objectives
     */
    InitObjValue** getInitObjValuesSparse();

    /**
     * Get the initial values associated with the objectives in dense form
     * <p>
     *
     * @return a vector of double that
     * hold initial values (or OSNaN()) for all of the objectives
     */
    double *getInitObjValuesDense();

    /**
     * Get the initial values associated with the objectives in dense form
     * <p>
     *
     * @param numberOfObjectives holds the dimension of the vector
     * @return a vector of double that
     * hold initial values (or OSNaN()) for all of the objectives
     */
    double *getInitObjValuesDense(int numberOfObjectives);

    /**
     * Get the initial bounds associated with the objectives in sparse form
     * <p>
     *
     * @return a vector of pointers to InitObjBound objects that
     * hold initial bounds for (some of) the objectives
     */
    InitObjBound** getInitObjBoundsSparse();

    /**
     * Get the initial lower bounds associated with the objectives in dense form
     * <p>
     *
     * @return a vector of double that
     * hold initial lower bounds (or OSNaN()) for all of the objectives
     */
    double *getInitObjLowerBoundsDense();

    /**
     * Get the initial lower bounds associated with the objectives in dense form
     * <p>
     *
     * @param numberOfObjectives holds the dimension of the vector
     * @return a vector of double that
     * hold initial lower bounds (or OSNaN()) for all of the objectives
     */
    double *getInitObjLowerBoundsDense(int numberOfObjectives);

    /**
     * Get the initial upper bounds associated with the objectives in dense form
     * <p>
     *
     * @return a vector of double that
     * hold initial upper bounds (or OSNaN()) for all of the objectives
     */
    double *getInitObjUpperBoundsDense();

    /**
     * Get the initial upper bounds associated with the objectives in dense form
     * <p>
     *
     * @param numberOfObjectives holds the dimension of the vector
     * @return a vector of double that
     * hold initial upper bounds (or OSNaN()) for all of the objectives
     */
    double *getInitObjUpperBoundsDense(int numberOfObjectives);

    /**
     * Get the initial basis status for all objectives in dense form
     * <p>
     *
     * @return an array of int, with values corresponding to ENUM_BASIS_STATUS -- see OSGeneral.h)
     * @note returns ENUM_BASIS_STATUS_unknown for objectives that are not initialed
     * @param numberOfObjectives is the dimension of the array
     */
    int* getObjectiveInitialBasisStatusDense(int numberOfObjectives);

    /**
     * Get the array of other objective options
     * <p>
     *
     * @param solver_name is the name of the solver whose options we want
     * @return a vector of pointers to OtherConstraintOption objects
     */
    std::vector<OtherObjectiveOption*> getOtherObjectiveOptions(std::string solver_name);

    /**
     * Get one particular <other> objective option from the array of options
     * <p>
     *
     * @param optionNumber is the index of the option in the array
     * @return a pointer to one OtherObjectiveOption object
     */
    OtherObjectiveOption* getOtherObjectiveOption(int optionNumber);

    /**
     * Get all <other> objective options
     * <p>
     *
     * @return a pointer to an array of OtherObjectiveOption objects
     */
    OtherObjectiveOption** getAllOtherObjectiveOptions();

    /**
     * Get the initial values associated with the constraints in sparse form
     * <p>
     *
     * @return a vector of pointers to InitConValue objects that
     * hold initial values for (some of) the constraints
     */
    InitConValue** getInitConValuesSparse();

    /**
     * Get the initial values associated with the constraints in dense form
     * <p>
     *
     * @return a vector of double that
     * hold initial values for all of the constraints
     */
    double *getInitConValuesDense();

    /**
     * Get the initial values associated with the constraints in dense form
     * <p>
     *
     * @param numberOfConstraints holds the dimension of the vector
     * @return a vector of double that
     * hold initial values for all of the constraints
     */
    double *getInitConValuesDense(int numberOfConstraints);

    /**
     * Get the initial bounds associated with the dual variables in sparse form
     * <p>
     *
     * @return a vector of pointers to InitDualVarValue objects that
     * hold initial bounds for (some of) the dual variables
     */
    InitDualVarValue** getInitDualVarValuesSparse();

    /**
     * Get the initial dual variables associated with the lower bounds in dense form
     * <p>
     *
     * @return a vector of double that
     * hold initial lower bounds for all of the dual variables
     */
    double *getInitDualVarLowerBoundsDense();

    /**
     * Get the initial dual variables associated with the lower bounds in dense form
     * <p>
     *
     * @param numberOfConstraints holds the dimension of the vector
     * @return a vector of double that
     * hold initial lower bounds for all of the dual variables
     */
    double *getInitDualVarLowerBoundsDense(int numberOfConstraints);


    /**
     * Get the initial dual variables associated with the upper bounds in dense form
     * <p>
     *
     * @return a vector of double that
     * hold initial upper bounds for all of the dual variables
     */
    double *getInitDualVarUpperBoundsDense();

    /**
     * Get the initial dual variables associated with the upper bounds in dense form
     * <p>
     *
     * @param numberOfConstraints holds the dimension of the vector
     * @return a vector of double that
     * hold initial upper bounds for all of the dual variables
     */
    double *getInitDualVarUpperBoundsDense(int numberOfConstraints);

    /**
     * Get the initial basis status for all slack variables in dense form
     * <p>
     *
     * @return an array of int, with values corresponding to ENUM_BASIS_STATUS -- see OSGeneral.h)
     * @note returns ENUM_BASIS_STATUS_unknown for slack variables that are not initialed
     * @param numberOfConstraints is the dimension of the array
     */
    int* getSlackVariableInitialBasisStatusDense(int numberOfConstraints);

    /**
     * Get the array of other constraint options
     * <p>
     *
     * @param solver_name is the name of the solver whose options we want
     * @return a vector of pointers to OtherConstraintOption objects
     */
    std::vector<OtherConstraintOption*> getOtherConstraintOptions(std::string solver_name);

    /**
     * Get one particular <other> constraint option from the array of options
     * <p>
     *
     * @param optionNumber is the index of the option in the array
     * @return a pointer to one OtherConstraintOption object
     */
    OtherConstraintOption* getOtherConstraintOption(int optionNumber);

    /**
     * Get all <other> constraint options
     * <p>
     *
     * @return a pointer to an array of OtherConstraintOption objects
     */
    OtherConstraintOption** getAllOtherConstraintOptions();

    /**
     * Get the options associated with a given solver
     * <p>
     *
     * @param solver_name is the name of the solver whose options we want
     * @return a vector of pointers to SolverOption objects that
     * correspond to the solver named.
     */
    std::vector<SolverOption*> getSolverOptions( std::string solver_name);


    /**
     * Get the options associated with a given solver AND options
     * not associated with any solver (if desired)
     * <p>
     *
     * @param solver_name is the name of the solver whose options we want
     * @param getFreeOptions is a boolean set to true if the free
     *        options (not associated with a solver name) should be returned
     * @return a vector of pointers to SolverOption objects that
     * correspond to the solver named.
     */
    std::vector<SolverOption*> getSolverOptions( std::string solver_name, bool getFreeOptions);

    /**
     * Get all solver options
     * <p>
     *
     * @return a pointer to an array SolverOption objects
     */
    SolverOption** getAllSolverOptions();

    /* --------------------------------------------------
    *  set() methods
    * -------------------------------------------------- */

    /**
     * Set the serviceURI
     */
    bool setServiceURI( std::string serviceURI);

    /**
     * Set the service name
     */
    bool setServiceName( std::string serviceName);

    /**
     * Set the instance name
     */
    bool setInstanceName( std::string instanceName);

    /**
     * Set the instance location
     */
    bool setInstanceLocation( std::string instanceLocation);

    /**
     * Alternative signature to set the instance location
     * and location type simultaneously
     */
    bool setInstanceLocation( std::string instanceLocation, std::string locationType);

    /**
     * Set the instance location type
     */
    bool setInstanceLocationType( std::string locationType);

    /**
     * Set the job ID
     */
    bool setJobID( std::string jobID);

    /**
     * Set the solver to be invoked
     */
    bool setSolverToInvoke( std::string solverToInvoke);

    /**
     * Set the license information
     */
    bool setLicense( std::string license);

    /**
     * Set the username
     */
    bool setUserName( std::string userName);

    /**
     * Set the password
     */
    bool setPassword( std::string password);

    /**
     * Set the contact information
     */
    bool setContact( std::string contact);

    /**
     * Alternative signature to set the contact information
     * and transport type simultaneously
     */
    bool setContact( std::string contact, std::string transportType);

    /**
     * Set the transport type for contact
     */
    bool setContactTransportType( std::string transportType);

    /**
     * Set the other general options as an entire array
	 * @param numberOfOptions contains the number of other options to be set
	 * @param other is a pointer to an array of OtherOption objects
     */
    bool setOtherGeneralOptions(int numberOfOptions, OtherOption** other);

    /**
     * Add another general option to the <other> option array
	 * @param name - the identifying anme of the option. This string cannot be empty
	 * @param value - optional value associated with this option
	 * @param description - further information (can be used for documentation)
     */
    bool setAnOtherGeneralOption(std::string name, std::string value, std::string description);

	/**
	 * Set the minimum disk space required for the current job
	 * @param unit - select the unit (Kb, Mb, etc.)
	 * @param description - further description (can be used for documentation)
	 * @param value - number of units of disk space required
	 */
    bool setMinDiskSpace(std::string unit, std::string description, double value);

	/**
	 * Alternate signature to set only the number of units
	 */
    bool setMinDiskSpace(double value);

    bool setMinDiskSpaceUnit(std::string unit);

	/**
	 * Set the minimum memory size required for the current job
	 * @param unit - select the unit (Kb, Mb, etc.)
	 * @param description - further description (can be used for documentation)
	 * @param value - number of units of memory size required
	 */
    bool setMinMemorySize(std::string unit, std::string description, double value);

	/**
	 * Alternate signature to set only the number of units
	 */
    bool setMinMemorySize(double value);

    bool setMinMemoryUnit(std::string unit);

	/**
	 * Set the minimum CPU speed required for the current job
	 * @param unit - select the unit (MHz, GHz, TFlops etc.)
	 * @param description - further description (can be used for documentation)
	 * @param value - number of units of CPU speed required
	 */
    bool setMinCPUSpeed(std::string unit, std::string description, double value);

	/**
	 * Alternate signature to set only the number of units
	 */
    bool setMinCPUSpeed(double value);

    bool setMinCPUSpeedUnit(std::string unit);

	/**
	 * Set the minimum number of CPU cores required for the current job
	 * @param number - number of CPU cores required
	 * @param description - further description (can be used for documentation)
	 */
    bool setMinCPUNumber( int number, std::string description);

	/**
	 * Alternate signature to set only the number of cores
	 */
    bool setMinCPUNumber( int number);

    bool setOtherSystemOptions(int numberOfOptions, OtherOption** other);

    bool setAnOtherSystemOption(std::string name, std::string value, std::string description);


    bool setServiceType( std::string serviceType);

    bool setOtherServiceOptions(int numberOfOptions, OtherOption** other);

    bool setAnOtherServiceOption(std::string name, std::string value, std::string description);


    bool setMaxTime(double value, std::string unit);

    bool setMaxTime(double value);

    bool setMaxTimeUnit(std::string unit);

    bool setRequestedStartTime(std::string time);

    bool setJobDependencies(int numberOfDependencies, std::string* jobDependencies);
    bool setAnotherJobDependency(std::string jobID);

    bool setRequiredDirectories(int numberOfPaths, std::string* paths);
    bool setAnotherRequiredDirectory(std::string path);

    bool setRequiredFiles(int numberOfPaths, std::string* paths);
    bool setAnotherRequiredFile(std::string path);

    bool setDirectoriesToMake(int numberOfPaths, std::string* paths);
    bool setAnotherDirectoryToMake(std::string path);

    bool setFilesToMake(int numberOfPaths, std::string* paths);
    bool setAnotherFileToMake(std::string path);

    /**  setPathPairs
     *   set a number of path pairs into the OSOption object
     *   @param object describes the type of pathpairs
     *       legal values are
     *           ENUM_PATHPAIR_input_dir,
     *           ENUM_PATHPAIR_input_file,
     *           ENUM_PATHPAIR_output_file,
     *           ENUM_PATHPAIR_output_dir
     *    @param from is a pointer to an array of strings containing the
     *           location of the original object
     *    @param to   is a pointer to an array of strings containing the
     *           location of the destination object
     *    @param makeCopy is a pointer to an array of boolean, describing
     *           for each object whether it is to be copied or moved
     *    @param numberOfPathPairs is an integer giving the number of PathPairs
     *           this must equal the number of entries in the from, to and makeCopy arrays
     **/
    bool setPathPairs(int object, std::string *from, std::string *to, bool *makeCopy, int numberOfPathPairs);

    bool setInputDirectoriesToMove(int numberOfPathPairs, PathPair** pathPair);
    bool setAnotherInputDirectoryToMove(std::string fromPath, std::string toPath, bool makeCopy);

    bool setInputFilesToMove(int numberOfPathPairs, PathPair** pathPair);
    bool setAnotherInputFileToMove(std::string fromPath, std::string toPath, bool makeCopy);

    bool setOutputFilesToMove(int numberOfPathPairs, PathPair** pathPair);
    bool setAnotherOutputFileToMove(std::string fromPath, std::string toPath, bool makeCopy);

    bool setOutputDirectoriesToMove(int numberOfPathPairs, PathPair** pathPair);
    bool setAnotherOutputDirectoryToMove(std::string fromPath, std::string toPath, bool makeCopy);

    bool setFilesToDelete(int numberOfPaths, std::string* paths);
    bool setAnotherFileToDelete(std::string path);

    bool setDirectoriesToDelete(int numberOfPaths, std::string* paths);
    bool setAnotherDirectoryToDelete(std::string path);

    bool setProcessesToKill(int numberOfProcesses, std::string* processes);
    bool setAnotherProcessToKill(std::string process);

    bool setOtherJobOptions(int numberOfOptions, OtherOption** other);
    bool setAnOtherJobOption(std::string name, std::string value, std::string description);


    bool setNumberOfVariables(int numberOfVariables);

    bool setNumberOfObjectives(int numberOfObjectives);

    bool setNumberOfConstraints(int numberOfConstraints);

    bool setInitVarValues(int numberOfVar, int* idx, double* value, std::string* name);
    bool setInitVarValuesSparse(int numberOfVar, InitVarValue** var);
    bool setInitVarValuesSparse(int numberOfVar, InitVarValue** var, ENUM_COMBINE_ARRAYS disp);
    bool setInitVarValuesDense(int numberOfVar, double *value);
    bool setAnotherInitVarValue(int idx, double value);

    bool setInitVarValuesString(int numberOfVar, int* idx, std::string* value, std::string* name);
    bool setInitVarValuesStringSparse(int numberOfVar, InitVarValueString** var);
    bool setInitVarValuesStringSparse(int numberOfVar, InitVarValueString** var, ENUM_COMBINE_ARRAYS disp);
    bool setInitVarValuesStringDense(int numberOfVar, std::string *value);
    bool setAnotherInitVarValueString(int idx, std::string value);

    bool setInitBasisStatus(int object, int status, int *i, int ni);
    bool setInitBasisStatusSparse(int numberOfVar, InitBasStatus** var);
    bool setInitBasisStatusSparse(int numberOfVar, InitBasStatus** var, ENUM_COMBINE_ARRAYS disp);
    bool setInitBasisStatusDense(int numberOfVar, std::string *var);

    /**
     *
     * Set the basis status for another variable, objective or constraint/slack
     * @param type: type of this element (see ENUM_PROBLEM_COMPONENT - OSGeneral.h)
     * @param idx: index of this element (nonnegative for variable or constraint,
     *		negative for objective)
     * @param status: basis status (see ENUM_BASIS_STATUS - OSGeneral.h)
     */
    bool setAnotherInitBasisStatus(int type, int idx, int status);

    bool setIntegerVariableBranchingWeights(int numberOfVar, int* idx, double* value, std::string* name);
    bool setIntegerVariableBranchingWeightsSparse(int numberOfVar, BranchingWeight** var);
    bool setIntegerVariableBranchingWeightsSparse(int numberOfVar, BranchingWeight** var, ENUM_COMBINE_ARRAYS disp);
    bool setIntegerVariableBranchingWeightsDense(int numberOfVar, double *value);
    bool setAnotherIntegerVariableBranchingWeight(int idx, double value);

    bool setSOSVariableBranchingWeights(int numberOfSOS, SOSWeights** sos);
    bool setAnotherSOSVariableBranchingWeight(int sosIdx, int nvar, double weight, int* idx, double* value, std::string* name);

    bool setNumberOfOtherVariableOptions(int numberOfOther);
    bool setOtherVariableOptions(int numberOfVar, OtherVariableOption** var);
    bool setAnOtherVariableOption(OtherVariableOption* varOption);

    /**
     *
     * Set the attributes for one particular <other> <variable> option
     * @param iOther: position of this element in the array of <other>
     * @param numberOfVar: number of <var> children contained in this <other> element
     * @param numberOfEnumerations: number of <enumeration> children
     * @param name: name of this <other> element
     * @param value: a value associated with this <other> element
     * @param solver: the solver associated with this <other> element
     * @param category: the category of this <other> element
     * @param type: type of this <other> element
     * @param varType: type of the data in the <var> array
     * @param enumType: type of the data in the <enumeration> array
     * @param description: further description of this <other> element
     */
    bool setOtherVariableOptionAttributes(int iOther, int numberOfVar,
                                          int numberOfEnumerations, std::string name,
                                          std::string value, std::string solver,
                                          std::string category, std::string type,
                                          std::string varType, std::string enumType,
                                          std::string description);


    /**
     *  Set one enumeration associated with an <other> option in
     *  the <variables>, <objectives> or <constraints> element
     *  @param object: the object into which the enumeration is to be stored
     *		(legal values see ENUM_PROBLEM_COMPONENT in OSGeneral.h)
     *  @param otherOptionNumber: number of the <other> option in the list of <other> options (zero-based)
     *  @param enumerationNumber: number of the <enumeration> in the list of enumerations (zero-based)
     *	@param numberOfEl: number of objects sharing the value of this enumeration
     *	@param value: value of the enumeration (as a string)
     *	@param description: further information about the enumeration and its value
     *	@param idxArray: the array of indices for the objects sharing this enumeration
     */
    bool setOtherOptionOrResultEnumeration(int object, int otherOptionNumber, int enumerationNumber,
                                   int numberOfEl, std::string value, std::string description, int* idxArray);

    /**
     *  Set one <var> element associated with an <other> option in
     *  the <variables> element
     *  @param otherOptionNumber: number of the <other> option in the list of <other> options (zero-based)
     *  @param varNumber: number of the <var> in the array (zero-based)
     *	@param idx: index of the variable to which this value belongs
     *	@param value: value of the option (as a string)
     *	@param lbValue: value associated with the lower bound of the variable (as a string)
     *	@param ubValue: value associated with the upper bound of the variable (as a string)
     */
    bool setOtherVariableOptionVar(int otherOptionNumber, int varNumber,
                                   int idx, std::string name, std::string value, std::string lbValue, std::string ubValue);

    bool setInitObjValues(int numberOfObj, int* idx, double* value, std::string* name);
    bool setInitObjValuesSparse(int numberOfObj, InitObjValue** obj);
    bool setInitObjValuesSparse(int numberOfObj, InitObjValue** obj, ENUM_COMBINE_ARRAYS disp);
    bool setInitObjValuesDense(int numberOfObj, double *value);
    bool setAnotherInitObjValue(int idx, double value);

    bool setInitObjBounds(int numberOfObj, int* idx, double* lbValue, double* ubValue, std::string* name);
    bool setInitObjBoundsSparse(int numberOfObj, InitObjBound** obj);
    bool setInitObjBoundsSparse(int numberOfObj, InitObjBound** obj, ENUM_COMBINE_ARRAYS disp);
    bool setInitObjBoundsDense(int numberOfObj, double *lb, double *ub);
    bool setAnotherInitObjBound(int idx, double lbValue, double ubValue);

    bool setNumberOfOtherObjectiveOptions(int numberOfOther);
    bool setOtherObjectiveOptions(int numberOfObj, OtherObjectiveOption** obj);
    //bool setOtherObjectiveOptions(int numberOfObj, OtherObjectiveOption** obj, ENUM_COMBINE_ARRAYS disp);
    bool setAnOtherObjectiveOption(OtherObjectiveOption* objOption);

    /**
     *
     * Set the attributes for one particular <other> <objective> option
     * @param iOther: position of this element in the array of <other>
     * @param numberOfObj: number of <obj> children contained in this <other> element
     * @param numberOfEnumerations: number of <enumeration> children
     * @param name: name of this <other> element
     * @param value: a value associated with this <other> element
     * @param solver: the solver associated with this <other> element
     * @param category: the category of this <other> element
     * @param type: type of this <other> element
     * @param objType: type of the data in the <var> array
     * @param enumType: type of the data in the <enumeration> array
     * @param description: further description of this <other> element
     */
    bool setOtherObjectiveOptionAttributes(int iOther, int numberOfObj,
                                           int numberOfEnumerations, std::string name,
                                           std::string value, std::string solver,
                                           std::string category, std::string type,
                                           std::string objType, std::string enumType,
                                           std::string description);


    /**
     *  Set one <obj> element associated with an <other> option in
     *  the <objectives> element
     *  @param otherOptionNumber: number of the <other> option in the list of <other> options (zero-based)
     *  @param objNumber: number of the <obj> in the array (zero-based)
     *	@param idx: index of the objective to which this value belongs
     *	@param name: name of the objective
     *	@param value: value of the option (as a string)
     *	@param lbValue: value associated with the lower bound of the objective (as a string)
     *	@param ubValue: value associated with the upper bound of the objective (as a string)
     */
    bool setOtherObjectiveOptionObj(int otherOptionNumber, int objNumber,
                                    int idx, std::string name, std::string value, std::string lbValue, std::string ubValue);

    bool setInitConValues(int numberOfCon, int* idx, double* value, std::string* name);
    bool setInitConValuesSparse(int numberOfCon, InitConValue** con);
    bool setInitConValuesSparse(int numberOfCon, InitConValue** con, ENUM_COMBINE_ARRAYS disp);
    bool setInitConValuesDense(int numberOfCon, double *value);
    bool setAnotherInitConValue(int idx, double value);

    bool setInitDualValues(int numberOfCon, int* idx, double* lbValue, double* ubValue, std::string* name);
    bool setInitDualVarValuesSparse(int numberOfCon, InitDualVarValue** con);
    bool setInitDualVarValuesSparse(int numberOfCon, InitDualVarValue** con, ENUM_COMBINE_ARRAYS disp);
    bool setInitDualVarValuesDense(int numberOfCon, double *lb, double *ub);
    bool setAnotherInitDualVarValue(int idx, double lbValue, double ubValue);

    bool setNumberOfOtherConstraintOptions(int numberOfOther);
    bool setOtherConstraintOptions(int numberOfOptions, OtherConstraintOption** other);
    //bool setOtherConstraintOptions(int numberOfOptions, OtherConstraintOption** other, ENUM_COMBINE_ARRAYS disp);
    bool setAnOtherConstraintOption(OtherConstraintOption* optionValue);

    /**
     *
     * Set the attributes for one particular <other> <constraint> option
     * @param iOther: position of this element in the array of <other>
     * @param numberOfCon: number of <con> children contained in this <other> element
     * @param numberOfEnumerations: number of <enumeration> children
     * @param name: name of this <other> element
     * @param value: a value associated with this <other> element
     * @param solver: the solver associated with this <other> element
     * @param category: the category of this <other> element
     * @param type: type of this <other> element
     * @param conType: type of the data in the <var> array
     * @param enumType: type of the data in the <enumeration> array
     * @param description: further description of this <other> element
     */
    bool setOtherConstraintOptionAttributes(int iOther, int numberOfCon,
                                            int numberOfEnumerations, std::string name,
                                            std::string value, std::string solver,
                                            std::string category, std::string type,
                                            std::string conType, std::string enumType,
                                            std::string description);


    /**
     *  Set one <con> element associated with an <other> option in
     *  the <constraints> element
     *  @param otherOptionNumber: number of the <other> option in the list of <other> options (zero-based)
     *  @param conNumber: number of the <obj> in the array (zero-based)
     *	@param idx: index of the constraint to which this value belongs
     *	@param name: name of the constraint
     *	@param value: value of the option (as a string)
     *	@param lbValue: value associated with the lower bound of the constraint (as a string)
     *	@param ubValue: value associated with the upper bound of the constraint (as a string)
     */
    bool setOtherConstraintOptionCon(int otherOptionNumber, int conNumber,
                                     int idx, std::string name, std::string value, std::string lbValue, std::string ubValue);

    bool setNumberOfSolverOptions(int numberOfOptions);

    /**
     *
     * Set the attributes for one particular solver option
     * @param iOption: position of this element in the array of options
     * @param numberOfVar: number of <var> children contained in this <other> element
     * @param name: name of this solver option
     * @param value: a value associated with this option
     * @param solver: the solver to which this option applies
     * @param category: the category of this option (solver specific)
     * @param type: type of this option (e.g., numeric or string)
     * @param description: further description of this option
     * "param itemList: the list of items associated with this option (could be NULL)
     */
    bool setSolverOptionContent(int iOption, int numberOfItems,
                                std::string name,
                                std::string value, std::string solver,
                                std::string category, std::string type,
                                std::string description, std::string *itemList);

    bool setSolverOptions(int numberOfSolverOptions, SolverOption** solverOption);
    bool setAnotherSolverOption(std::string name, std::string value, std::string solver,
                                std::string category, std::string type, std::string description);


    bool setOptionInt(std::string optionName, int optionValue);

    bool setOptionStr(std::string optionName, std::string optionValue);

    bool setOptionDbl(std::string optionName, double value);

};//OSOption

#endif
