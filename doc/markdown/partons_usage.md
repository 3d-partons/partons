# PARTONS usage {#usage}

# Introduction {#usage_intro}

This wiki page demonstrates how to use PARTONS. At this point you should have your own version of PARTONS available: either compiled at own your system ([Linux](@ref linux) or [Mac](@ref mac)), or accessible through [our virtual machine ](@ref vm). To run PARTONS properly, make sure to set up properly [the configuration files](@ref config). Note also, that some tasks described here require [MySQL server](@ref database) to be available and pre-configured to work with PARTONS. 

To perform all kind of tasks you should use __services__, which have been developed in order to make your life easier with PARTONS. You may consider services as a toolbox, which allows you to write a simple task and get back powerful C++ objects. In particular, you do not need to worry about how to use low level objects and functions - services configure and use them for you. 

We provide two ways of using services (and therefore of using PARTONS). The first and the most preferred one is to use XML files. A great advantage of this way is a possibility to perform complex tasks in a simple and generic way without writing a line of C++ code and without a need of rebuilding PARTONS-related projects. In addition, both an input XML file and results produced with this file can be stored in the database, so one can easily keep track of all computations done so far and retrieve specific data (e.g. to make a plot) at a convenient time. The second way of using PARTONS is to use its library and its headers to write a standalone program - we recommend this way for only the most complex and sophisticated tasks. If you wish to explore this way of using PARTONS, we strongly recommend to use [Eclipse CDT IDE](@ref eclipse) for any code writing. 

One can distinguish three types of services. 1) System services are used to perform basic operations, like calling for new objects, parsing XML scenarios, handling threads, etc. These services are used mainly by the developers and will not be discussed here any further. 2) Database services are used to handle the database and perform such operations as insertion, selection and deletion of data. These services are used mainly by the developers and they are explained in [this wiki page](@ref database). 3) Computation services are used to perform all kinds of computational tasks. They have been designed to be used by PARTONS users and they are explained here. 

PARTONS benefit from a layered structure corresponding to the factorized nature of GPD-oriented computations. We distinguish three layers - each one coming with its own computation service. These are: 1) GPD layer with GPDService 2) CFF layer with ConvolCoeffFunctionService and 3) observable layer with ObservableService. When a computation is performed, higher layers call lower ones automatically. The responsibility of PARTONS user is to only set all required physics assumptions, as GPD model, order of pQCD approximation in CFF calculation, evolution type, etc.

# XML interface remarks {#usage_xml}

We refer to a set of physics assumptions as __scenario__. In this section we demonstrate how a single scenario can be encoded in an input XML file to be evaluated by PARTONS. This will be achieved with the help of this working scenario, which can be used to evaluate one of Fourier moments of DVCS beam charge asymmetry, \f$A_{C}^{\cos 2\phi}\f$, in a single kinematic point:
~~~~~~~~~~~~~{.xml}
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<scenario date="2017-06-15" description="How to compute an observable">

    <task service="ObservableService" method="computeObservable" storeInDB="0">

        <kinematics type="ObservableKinematic">
            <param name="xB" value="0.1763" />
            <param name="t" value="-0.1346" />
            <param name="Q2" value="1.3651" />
            <param name="E" value="5.77" />
        </kinematics>

        <computation_configuration>

            <module type="Observable" name="AcCos2phi">

                <module type="ProcessModule" name="GV2008Model">

                    <module type="ScaleModule" name="Q2Multiplier">
                        <param name="lambda" value="1" />
                    </module>

                    <module type="XiConverterModule" name="XBToXi">
                    </module>

                    <module type="ConvolCoeffFunctionModule" name="DVCSCFFModel">

                        <param name="qcd_order_type" value="LO" />

                        <module type="GPDModule" name="GK11Model">
                        </module>
                    </module>
                </module>
            </module>
        </computation_configuration>
    </task>

    <task service="ObservableService" method="printResults">
    </task>

</scenario>
~~~~~~~~~~~~~
Let us analyze now step-by-step the structure of this scenario:
* The scenario starts with a typical XML preamble encoded between `<? ... ?>` tags. Keep it in each of your XML scenarios and do not modify it - the preamble is used exclusively by XML parsers. 
* The scenario is defined between `<scenario></scenario>` tags. For your convenience and for bookkeeping, you can set both date when the scenario was created and your own, unique description.
* The scenario contains two tasks - each one with input data encoded between `<task></task>` tags. Information in the opening tags define the target service (`service = ""`) and its method (`method = ""`) to be called. In our example, the first task is for the computation, while the second one prints out a result of the last computation to the standard output. Available tasks for all services are summarized in [this section](#usage_tasks).
* In the opening tag defining the computational task a switch is available, `storeInDB = ""`, to store the result of this task in the database. When this switch is active, both the scenario file and the result are stored in the database. You may refer to the stored data by unique `computation.id` returned by one of the involved database services:
~~~~~~~~~~~~~{.xml}
26-06-2017 03:48:04 [INFO] (ObservableService::computeTask) ObservableResultList object has been stored in database with computation_id = 2
~~~~~~~~~~~~~
* In the computational task, input kinematics of a given type is defined between `<kinematics></kinematics>` tags. Typically, you will encounter three types of objects defining kinematics, with each one corresponding to a specific layer of the computation. These are: `GPDKinematics`, `ConvolCoeffFunctioKinematics` and `ObservableKinematics`. See the examples provided in [this section](#usage_tasks) to learn how to define these objects. Note, that they can be defined both via XML file (as in the analyzed example), but they can be also defined via external text files (lists).
* Physics assumptions are defined between `<computation_configuration></computation_configuration>` tags. Namely, this is a nested structure of module names corresponding to the following computation structure: 
   * Observable (evaluation of observable)
      * ProcessModule (evaluation of cross-sections)
          * ScaleModule (evaluation of factorization and renormalization scales)
          * XiConverterModule (evaluation of \f$\xi\f$ GPD variable)
          * ConvolCoeffFunctionModule (evaluation of CFFs)
             * GPDModule (evaluation of GPDs)
* Each module can be configured by a set of `<param/>` self-closing tags. This is our way of transferring data from XML scenario to a specific PARTONS module. 

# C++ interface remarks {#usage_cpp}
These are the most important remarks on the usage of PARTONS C++ interface. Due to a complexity of this subject, we recommend you to study examples provided in [this section](#usage_tasks) and in `parons-exe` project. 
* For any code writing and further development we strongly recommend to use Eclipse. See [this wiki page](#eclipse) for a tutorial and more information.
* PARTONS uses the registry mechanism. The registry is the analog of a phone book, which lists all available modules. From the point of view of software engineering, it corresponds to the singleton design pattern which ensures that it is unique. When a new module is created, the first thing to do is to call this unique instance, and to register the new module with the class name provided by the developer of the module. In return the registry gives a unique identifier encoded in an integer variable for performance purposes. Therefore, the class appliance may be returned by the registry by the class name of by the unique id. E.g. to get a new appliance of `GK11Model` we use:
~~~~~~~~~~~~~{.cpp}
GPDModule* pGK11Module1 =
   Partons::getInstance()->getModuleObjectFactory()->newGPDModule("GK11Model");
//or (recomended!)
GPDModule* pGK11Module2 =
   Partons::getInstance()->getModuleObjectFactory()->newGPDModule(GK11Model::classId);
~~~~~~~~~~~~~
* PARTONS uses the logger mechanism. Use it whenever needed, instead of functions like `printf()`, `std::cout`, etc. This will allow to create a consistent output and it is crucial to keep the continuity of the information stream - since Logger uses a different thread than the computation, information send by Logger and by e.g. `printf()` function will not be aligned. To use Logger run one of the following functions: 
~~~~~~~~~~~~~{.cpp}
debug(__func__, ElemUtils::Formatter() << "Debugging information");
info(__func__, ElemUtils::Formatter() << "Information");
warn(__func__, ElemUtils::Formatter() << "Warning");
throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Error");
~~~~~~~~~~~~~
Here, we have assumed that the functions are called from a member of a class that inherits from BaseObject, which is true for e.g. all modules. Let us name this class 'MyClass' and the member 'someFunction()'. Then, the output returned by Logger will be similar to that one: 
~~~~~~~~~~~~~{.py}
26-06-2017 12:11:52 [DEBUG] (MyClass::someFunction) Debugging information
26-06-2017 12:11:52 [INFO] (MyClass::someFunction) Information
26-06-2017 12:11:52 [WARN] (MyClass::someFunction) Warning
26-06-2017 12:11:52 [ERROR] (MyClass::someFunction) Error
~~~~~~~~~~~~~
The last is an exception that stops the excecution (if appropriatly catched and processed by the excecution program). The appearence of each information type in the PARTONS output depends on [Logger's configuration](@ref config_logger). `ElemUtils::Formatter()` is a stream buffer that allows to build sophisticated strings of characters out of simple types, e.g.:
~~~~~~~~~~~~~{.cpp}
double a = 1.12;
int b = 2;
bool c = false;
std::string d = "ddd";

ElemUtils::LoggerManager::getInstance()->info("SomeClass", "someFunction", ElemUtils::Formatter() << "We have: " << a << " " << b << " " << c << " " << d);
~~~~~~~~~~~~~
which gives:
~~~~~~~~~~~~~{.py}
26-06-2017 02:20:28 [INFO] (SomeClass::someFunction) We have: 1.12 2 0 ddd
~~~~~~~~~~~~~
Here we have shown how to use Logger outside a class that inherits from BaseObject.

# Services and available tasks {#usage_tasks}

This table summarizes all tasks available in computation services. For a given task, click on the corresponding link in the `Details` column for more information. 

| Service         | Name                                 | Short description                                | Details 						| 
| :-------------- | :----------------------------------- | :------------------------------------------------| :---------------------------- |
| `GPDService`    | `computeGPDModel`                    | Evaluate GPD model for single kinematic point    | [link](@ref usage_gpd_1)    	|
| `GPDService`    | `computeManyKinematicOneModel`       | Evaluate GPD model for many kinematic points     | [link](@ref usage_gpd_2)    	|
| `GPDService`    | `printResults`                       | Print out result to std ouput                    | [link](@ref usage_gpd_3)    	|
| `GPDService`    | `generatePlotFile`                   | Generate plot file from data stored in database  | [link](@ref usage_gpd_4)    	|
|                 |                                      |                                                  |         						|
| `ConvolCoeffFunctionService` | ...                     | ...                                              | ...     						|                                        

# Available modules {#usage_modules} 

This table summarizes all modules available in PARTONS. For a given module, click on its name for more information. 
