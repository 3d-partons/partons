# Using %PARTONS {#usage}

[TOC]

# Introduction {#usage_intro}

This wiki page demonstrates how to use %PARTONS. At this point you should have your own version of %PARTONS available: either compiled at own your system ([Linux](@ref linux) or [Mac](@ref mac)), or accessible through [our virtual machine ](@ref vm). To run %PARTONS properly, make sure to set up properly [the configuration files](@ref config). Note also, that some tasks described here require a [MySQL server](@ref database) to be available and pre-configured to work with %PARTONS. 

To perform all kind of tasks you should use Services, which have been developed in order to make your life easier with %PARTONS. You may consider Services as a toolbox, which allows you to write a simple task and get back powerful C++ objects. You do not need to worry about how to use low level objects and functions ; Services configure and use them for you. 

We provide two ways of using Services (and therefore of using %PARTONS):
* The first and the most preferred one is to use XML files. A great advantage of this way is a possibility to perform complex tasks in a simple and generic way without writing a line of C++ code and without a need of rebuilding %PARTONS-related projects. In addition, both an input XML file and the result produced with this file can be stored in the database, so one can easily keep track of all computations done so far and retrieve specific data (e.g. to make a plot) at a convenient time.
* The second way of using %PARTONS is to use its library and its headers to write a standalone program - we recommend this way for only the most complex and sophisticated tasks. If you wish to explore this way of using %PARTONS, we strongly recommend you to use [Eclipse CDT IDE](@ref eclipse) for any code writing. 

One can distinguish three types of Services:
1. System services are used to perform basic operations, like calling for new objects, parsing XML scenarios, handling threads, etc. These services are used mainly by the developers and will not be discussed here any further.
2. Database services are used to handle the database and perform such operations as insertion, selection and deletion of data. These services are used mainly by the developers and they are described in [this wiki page](@ref database).
3. %Computation services are used to perform all kinds of computational tasks. They have been designed to be used by %PARTONS users and they are explained here. 

%PARTONS benefits from a layered structure corresponding to the factorized nature of a GPD-oriented computation. We distinguish currently three main layers, each one coming with its own computation service. These are: 
1. GPD layer with GPDService ;
2. CFF layer with ConvolCoeffFunctionService ; and
3. %Observable layer with ObservableService.

When a computation is performed, higher layers call lower ones automatically. The responsibility of a %PARTONS user is only to set all required physical assumptions, such as GPD model, order of pQCD approximation, etc.

The [first section](@ref usage_xml) explains how to write an XML file, whereas the practical aspects of running these files is left to [another tutorial](@ref use_xml_interface). The second section explains how to make use of the C++ library directly while again the practical aspects of writing an external program are left to the aforementioned [tutorial](@ref write_external_program).

<hr>

# XML interface {#usage_xml}

We refer to a set of physics assumptions as scenario. In this section we demonstrate how a single scenario can be encoded in an input XML file to be evaluated by %PARTONS.

## Example {#usage_xml_example}

This will be achieved with the help of this example, which is used to evaluate one of the Fourier moments of a DVCS beam charge asymmetry, \f$A_{C}^{\cos 2\phi}\f$, for a single kinematic point:

```xml
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
```
Let us analyze the structure of this scenario step-by-step:
* The scenario starts with a typical XML preamble encoded between `<? ... ?>` tags. Keep it in each of your XML scenarios and do not modify it - the preamble is used exclusively by XML parsers. 
* The scenario is defined between `<scenario></scenario>` tags. For your convenience and for bookkeeping, set both date when the scenario was created and your own, unique description.
* The scenario contains two tasks - each one with input data encoded between `<task></task>` tags. The information in the opening tags defines the target Service (`service = ""`) and the method (`method = ""`) to be called. In our example, the first task is for the computation, while the second one prints out its result to the standard output. Available tasks for all Services are summarized in [the following subsection](@ref usage_tasks).
* In the opening tag defining the computational task, a switch is available, `storeInDB = ""`, to store the result in the database. When this switch is active, both the scenario file and the result are stored in the database. You may refer to the stored data by a unique `computation.id` value returned by one of the involved database services:
~~~~~~~~~~~~~{.xml}
26-06-2017 03:48:04 [INFO] (ObservableService::computeTask) ObservableResultList object has been stored in database with computation_id = 2
~~~~~~~~~~~~~
* In the computational task, input kinematics of a given type are defined between `<kinematics></kinematics>` tags. Typically, you will encounter three types of kinematics - each one corresponding to a specific layer of the computation. These are: `GPDKinematics`, `ConvolCoeffFunctioKinematics` and `ObservableKinematics`. See examples provided in [the following subsection](@ref usage_tasks) to learn how to define these objects. Note, that they can be defined either via XML file (as in the analyzed example), or via external text files (for a more convenient handling of lists).
* Physics assumptions are defined between `<computation_configuration></computation_configuration>` tags. It is a nested structure that indicates %PARTONS modules to be used. The structure corresponds to the following computation structure: 
![](../images/module_structure.png "Module structure used by PARTONS")
* Each module can be configured by a set of `<param/>` self-closing tags. This is our way of transferring parameters from the XML scenario to the specific %PARTONS module. 

## Services and available tasks {#usage_tasks}

This table summarizes all tasks available in computation services. For a given task, click on the corresponding link in the `Details` column for more information. 

| Service         | Name                                 | Short description                                | Details                       | 
| :-------------- | :----------------------------------- | :------------------------------------------------| :---------------------------- |
| `GPDService`    | `computeGPDModel`                    | Evaluate GPD model for single kinematic point    | [Example](@ref usage_gpd_1)      |
| `GPDService`    | `computeManyKinematicOneModel`       | Evaluate GPD model for many kinematic points     | [Example](@ref usage_gpd_2)      |
| `GPDService`    | `printResults`                       | Print out result to std ouput                    | [Example](@ref usage_gpd_3)      |
| `GPDService`    | `generatePlotFile`                   | Generate plot file from data stored in database  | [Example](@ref usage_gpd_4)      |
|                 |                                      |                                                  |                               |
| `ConvolCoeffFunctionService` | ...                     | ...                                              | ...                           |

<hr>

# C++ interface {#usage_cpp}

The recommended way of using PARTONs is through Services, even with the C++ interface. 

## Example {#usage_cpp_example}

We will consider again a simple example (computing a GPD at single kinematics):

```cpp
// Get service
GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

// Create GPD module with the ModuleObjectFactory, here it is a specific MMS13 model
GPDModule* pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

// Create a GPDKinematic(x, xi, t, MuF, MuR) object
GPDKinematic gpdKinematic(-0.99, 0.99, 0., 1., 1.);

// Compute GPD
GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel);
```

GPDService has many other useful methods that mirror those of the XML interface (for instance, if you are considering several models, or a list of kinematics). See its documentation for more details.

The same can be said for the other Services, such as ConvolCoeffFunctionService (used for perturbative coefficient functions) or ObservableService.

## General remarks {#usage_cpp_remarks}

These are the most important remarks on the usage of %PARTONS C++ interface. Due to a complexity of this subject, we recommend you to study examples provided in [this section](@ref usage_tasks) and in `partons-example` project. 
* For any code writing and further development we strongly recommend you to use Eclipse. See [this wiki page](@ref eclipse) for a tutorial and more information.
* %PARTONS uses the registry/factory mechanism. The registry is the analog of a phone book, which lists all available modules. From the software engineering point of view, it corresponds to the singleton design pattern, which ensures that it is unique. When during the execution a new module is created, the first thing to do is to call this unique instance, and to register this new module with the class name provided by the developer. In return, the Registry gives a unique identifier encoded in an integer variable (for illustration, see one of the examples listed in the *New module* [tutorial](@ref newmodule)). When the user creates a new instance of this class, he calls this time the Factory which will clone the unique original object from the Registry and return it to the user. This clone may be returned by the Factory either by using its class name or by the unique id. To get *e.g.* a new instance of `GK11Model` we use:
~~~~~~~~~~~~~{.cpp}
// To be avoided
GPDModule* pGK11Module1 =
   Partons::getInstance()->getModuleObjectFactory()->newGPDModule("GK11Model");
// Recommended as it is faster and allows to avoid hard-coding of character strings
GPDModule* pGK11Module2 =
   Partons::getInstance()->getModuleObjectFactory()->newGPDModule(GK11Model::classId);
~~~~~~~~~~~~~
* %PARTONS uses the logger mechanism. Use it whenever needed, instead of functions like `printf()`, `std::cout`, etc. This will allow to create a consistent output and it is crucial to keep the continuity of the information stream - since Logger uses a different thread than the computation (with the advantage of not slowing down a computation with parasitic printing), information sent by the Logger and by e.g. `printf()` function will not be aligned properly. To use the Logger, run one of the following functions: 
~~~~~~~~~~~~~{.cpp}
debug(__func__, ElemUtils::Formatter() << "Debugging information");
info(__func__, ElemUtils::Formatter() << "Information");
warn(__func__, ElemUtils::Formatter() << "Warning");
throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Error");
~~~~~~~~~~~~~
Here, we have assumed that the functions are called from a member of a class that inherits from BaseObject, which is true for e.g. all modules. Let us name this class 'MyClass' and the aforementioned member 'someFunction()'. Then, the output returned by Logger will be similar to that one: 
~~~~~~~~~~~~~{.sh}
26-06-2017 12:11:52 [DEBUG] (MyClass::someFunction) Debugging information
26-06-2017 12:11:52 [INFO] (MyClass::someFunction) Information
26-06-2017 12:11:52 [WARN] (MyClass::someFunction) Warning
26-06-2017 12:11:52 [ERROR] (MyClass::someFunction) Error
~~~~~~~~~~~~~
The last is an exception that stops the execution (if appropriately caught and processed by the execution program). The appearance of each information type in the %PARTONS output depends on the [Logger's configuration](@ref config_logger). 
* Formatter, `ElemUtils::Formatter`, is a stream buffer that allows to build sophisticated character strings out of simple types, e.g.:
~~~~~~~~~~~~~{.cpp}
double a = 1.12;
int b = 2;
bool c = false;
std::string d = "ddd";

ElemUtils::LoggerManager::getInstance()->info("SomeClass", "someFunction", ElemUtils::Formatter() << "We have: " << a << " " << b << " " << c << " " << d);
~~~~~~~~~~~~~
which gives:
~~~~~~~~~~~~~{.sh}
26-06-2017 02:20:28 [INFO] (SomeClass::someFunction) We have: 1.12 2 0 ddd
~~~~~~~~~~~~~
Here we have shown how to use the Logger outside a class that inherits from BaseObject.

<hr>

# Available modules {#usage_modules} 

This table summarizes all modules available in %PARTONS. For a given module, click on its name for more information. 
