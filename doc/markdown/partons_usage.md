# Using %PARTONS {#usage}

[TOC]

# Introduction {#usage_intro}

This page explains how to use %PARTONS. At this point you should have your own version of %PARTONS available: either compiled on your own system ([Linux](@ref linux) or [Mac](@ref mac)), or accessible through [our virtual machine ](@ref vm). To run %PARTONS properly, make sure to set up correctly [the configuration files](@ref config). Note also that some tasks described here require a [MySQL server](@ref database) to be available and pre-configured to work with %PARTONS. 

To perform all kinds of tasks you should use Services, which have been developed in order to make your life easier with %PARTONS. You may consider Services as a toolbox, which allows you to write a simple task and get back powerful C++ objects. You do not need to worry about how to use low level objects and functions - Services configure and use them for you. 

We provide two ways of using Services (and therefore of using %PARTONS):
* The first and the most preferred one is to use XML files. A great advantage of this way is a possibility to perform complex tasks in a simple and generic way without writing a line of C++ code and without rebuilding %PARTONS-related projects. In addition, both the input XML file and the result produced with this file can be stored in the database, so one can easily keep track of all computations done so far and retrieve specific data (e.g. to make a plot) at a convenient time.
* The second way of using %PARTONS is to use its library and the corresponding headers to write a standalone program - we recommend this way for only the most complex and sophisticated tasks. If you wish to explore this way of using %PARTONS, we strongly recommend you to use an IDE such as [Eclipse CDT](@ref eclipse) for any code writing. 

One can distinguish three types of Services:
* System services are used to perform basic operations, like calling for new objects, parsing XML scenarios, handling threads, etc. These services are used mainly by the developers and they will not be discussed here any further.
* Database services are used to handle the database and perform such operations as the insertion, selection and deletion of data. These services are used mainly by the developers and they are described in [this page](@ref database).
* %Computation services are used to perform all kinds of computational tasks. They have been designed to be used by %PARTONS users and they are described here. 

%PARTONS benefits from a layered structure corresponding to the factorized nature of GPD-oriented computations. We distinguish three main layers, each one coming with its own computation service. These are: 
* GPD layer with [GPDService](@ref PARTONS::GPDService);
* CFF layer with [ConvolCoeffFunctionService](@ref PARTONS::ConvolCoeffFunctionService);
* Observable layer with [ObservableService](@ref PARTONS::ObservableService).

When a computation is performed, higher layers call lower ones automatically. The responsibility of a %PARTONS user is to only set all required physical assumptions, such as GPD model, order of pQCD approximation, etc.

<hr>

# PARTONS_example project {#usage_pexample}

It is useful for this tutorial to have our example project called [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) set up and ready to be used. It can serve as an illustration of topics being discussed here and you can base your own program on it for a start. The project is set up to run any XML scenario. The collection of exemplary XML scenarios can be found in `data/examples` directory.

The project comes with `main.cpp` file, which illustrates how to call and handle properly %PARTONS library in a stand alone program (see [this section](@ref usage_exe) for more information). With a minor modification, which is clearly indicated  in `main.cpp`, the project can also serve as a base to run any C++ code based on %PARTONS library. The collection of exemplary C++ functions is included in `examples.h` (header) and `example.cpp` (source) files. 

Read [this short tutorial](@ref quick) to learn how to evaluate (play) a demonstration XML scenario in [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example). Note, that you can use this project to run any XML scenario that you will create during your work with %PARTONS. 

<hr>

# PARTONS executable {#usage_exe}

Here we demonstrate how to create the main function of an executable project, like `main()` of [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example). That is, we show how to initialize and handle the %PARTONS library and how to call its members properly. 

If you wish to work with %PARTONS by using only XML scenarios run through [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) (see [this section](@ref usage_pexample) for more information) and you are not interested in details on how the main function of [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) is built, you may skip this section of the tutorial. 

This is the skeleton for the main function:
```cpp
int main(int argc, char** argv) {

    // Initialize Qt
    QCoreApplication a(argc, argv);
    PARTONS::Partons* pPartons = 0;

    try {

        // Initialize PARTONS application
        pPartons = PARTONS::Partons::getInstance();
        pPartons->init(argc, argv);

        // Your program goes here
        // Organize your code as you wish by calling your own methods and classes making use of PARTONS.  
    } 
    // In a case of PARTONS exception
    catch (const ElemUtils::CustomException &e) {

	// Show why the exception has occurred
        pPartons->getLoggerManager()->error(e);
    } 
    // In a case of standard exception
    catch (const std::exception &e) {

	// Show why the exception has occurred
        pPartons->getLoggerManager()->error("main", __func__, e.what());
    }

    // Close PARTONS application properly
    if (pPartons) {
        pPartons->close();
    }

    return 0;
}
```
Note the following:
* To evaluate a single XML scenario you need to add
```cpp
// Retrieve automation service parse scenario xml file and play it.
PARTONS::AutomationService* pAutomationService = pPartons->getServiceObjectRegistry()->getAutomationService();

// Parse scenarion of a given path
PARTONS::Scenario* pScenario = pAutomationService->parseXMLFile("path_to_scenario");

// Evaluate (play)
pAutomationService->playScenario(pScenario);
```
For your convenience, you may pass `path_to_scenario` as a function argument. 
* It is not mandatory, but still highly recommended to keep the `try`-`catch` mechanism in order to catch exceptions and display with the Logger the associated error messages. Without this mechanism the information on what has caused the termination of your program is lost. 
* [Partons](@ref PARTONS::Partons) is a singleton object that initializes and configures all other singletons, like services and registries. Make sure to have well set up configurations files for the initialization and configuration to work. See [this tutorial](@ref config) for more information.
* The following includes are necessary for the code presented in this section to work:
```cpp
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/Partons.h>
#include <QtCore/qcoreapplication.h> 				//or <QCoreApplication>
#include <partons/services/automation/AutomationService.h> 	//to play XML scenario
#include <partons/ServiceObjectRegistry.h>			//to play XML scenario
```
Be sure to include all the headers required by your code. Eclipse can automatically do it for you with the combination of keys `Ctrl`+`Shift`+`O`, assuming that it has been correctly configured as explained in [this tutorial](@ref eclipse).
* All of %PARTONS' classes are encapsulated in a namespace PARTONS. The examples presented in the tutorials use systematically an explicit namespace, but if you wish you can also handle it implicitly with:
```cpp
using namespace PARTONS;
```

# Using XML interface {#usage_xml}

We refer to a set of physics assumptions as a scenario. In this section we demonstrate how a single scenario can be encoded in an input XML file to be evaluated (played) by %PARTONS. This will be achieved with the help of this example, which is used to evaluate one of the Fourier moments of DVCS beam charge asymmetry, \f$A_{C}^{\cos 2\phi}\f$, in a single kinematic point:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<scenario date="2017-06-15" description="How to compute an observable">

    <task service="ObservableService" method="computeObservable" storeInDB="0">

        <kinematics type="ObservableKinematic">
            <param name="xB" value="0.1" />
            <param name="t" value="-0.1" />
            <param name="Q2" value="2." />
            <param name="E" value="12." />
	    <param name="phi" value="20." />
        </kinematics>

        <computation_configuration>

            <module type="Observable" name="DVCSAcCos2Phi">

                <module type="ProcessModule" name="DVCSProcessGV08">

                    <module type="ScalesModule" name="ScalesQ2Multiplier">
                        <param name="lambda" value="1." />
                    </module>

                    <module type="XiConverterModule" name="XiConverterXBToXi">
                    </module>

                    <module type="ConvolCoeffFunctionModule" name="DVCSCFFStandard">

                        <param name="qcd_order_type" value="LO" />

                        <module type="GPDModule" name="GPDGK11">
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
* The scenario is defined between `<scenario></scenario>` tags. For your convenience and for bookkeeping, set both date when the scenario was created and your own unique description.
* The scenario contains two tasks - each one with input data encoded between `<task></task>` tags. The information in the opening tags defines the target Service (`service = ""`) and the method (`method = ""`) to be called. In our example, the first task is for the computation, while the second one prints out the result to the standard output. Available tasks for all Services are summarized in [the following section](@ref usage_tasks).
* In the opening tag defining the computational task, a switch is available, `storeInDB = ""`, to store the result in the database. When this switch is active, both the scenario file and the result are stored in the database. You may refer to the stored data by a unique `computation.id` value returned to the standard output by one of the involved database services:
```sh
26-06-2017 03:48:04 [INFO] (ObservableService::computeTask) ObservableResultList object has been stored in database with computation_id=2
```
* In the computational task, the input kinematics of a given type is defined between `<kinematics></kinematics>` tags. Typically, you will encounter three types of kinematics - each one corresponding to the specific layer of the computation. These are: `GPDKinematics`, `ConvolCoeffFunctioKinematics` and `ObservableKinematics`. See examples provided in [the following section](@ref usage_tasks) to learn how to define these objects. Note that they can be defined either via XML file (as in the analyzed example), or via external text files (for a more convenient handling of lists).
* Physics assumptions are defined between `<computation_configuration></computation_configuration>` tags. It is a nested structure that indicates %PARTONS modules to be used. The structure corresponds to the following computation structure: 
![](../images/module_structure.png "Module structure used by PARTONS")
The list of all available %PARTONS modules is summarized in [this section](@ref usage_modules).
* Each module can be configured by a set of `<param/>` self-closing tags. It is the way of transferring parameters from XML files to specific %PARTONS modules. 

<hr>

# Using C++ interface {#usage_cpp}

These are the most important remarks on the usage of %PARTONS' C++ interface. Due to the complexity of this subject, we recommend you to study examples provided in [this section](@ref usage_tasks) and in the [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) project. 
* The recommended way of using %PARTONS is through Services, even with the C++ interface. In the [following section](@ref usage_tasks), the three main services are presented (the names of the services redirect to the class documentation), with various possibles tasks. Some of these tasks can be used with the C++ interface (click on their name to be redirected to a page with more details).
* For any code writing and further development we strongly recommend you to use an IDE such as Eclipse CDT. See [this tutorial](@ref eclipse) for more information.
* %PARTONS uses the Registry / Factory mechanism. The Registry is the analog of a phone book, which lists all available modules. From the software engineering point of view, it corresponds to the singleton design pattern, which ensures that it is unique. When during the execution a new module is created, the first thing to do is to call this unique instance, and to register the new module with the class name provided by the developer. In return, the Registry gives a unique identifier encoded in an integer variable. When the user creates a new instance of this class, he calls the Factory, which clones the original object from the Registry and returns it to the user. The clone may be returned by the Factory either by using the class name or the unique id. For example to get an instance of `GPDGK11` we use:
```cpp
// To be avoided (used by XML parser only)
PARTONS::GPDModule* pGK11Module1 =
   PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule("GPDGK11");
// Recommended as it is faster and allows to avoid hard-coding of character strings
PARTONS::GPDModule* pGK11Module2 =
   PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(PARTONS::GPDGK11::classId);
```
* %PARTONS uses the logger mechanism. Use it whenever needed, instead of functions like `printf()`, `std::cout`, etc. This will allow to create a consistent output and it is crucial to keep the continuity of the information stream - since the Logger uses a different thread than the computation (with the advantage of not slowing down the computation with parasitic printing), information sent by the Logger and by e.g. `printf()` function will not be properly synchronized. To use the Logger, run one of the following functions: 
```cpp
debug(__func__, ElemUtils::Formatter() << "Debugging information");
info(__func__, ElemUtils::Formatter() << "Information");
warn(__func__, ElemUtils::Formatter() << "Warning");
throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Error");
```
Here, we have assumed that the functions are called from a method of a class that inherits from `BaseObject`, which is true for e.g. all modules. Let us name this class `MyClass` and the aforementioned method as `someFunction()`. Then, the output returned by the Logger will be similar to that one: 
```sh
26-06-2017 12:11:52 [DEBUG] (MyClass::someFunction) Debugging information
26-06-2017 12:11:52 [INFO] (MyClass::someFunction) Information
26-06-2017 12:11:52 [WARN] (MyClass::someFunction) Warning
26-06-2017 12:11:52 [ERROR] (MyClass::someFunction) Error
```
The last one is an exception that stops the execution (if appropriately caught and processed by the execution program). The appearance of each information type in the %PARTONS output depends on the [Logger's configuration](@ref config_logger). 
* The Formatter, `ElemUtils::Formatter`, is a stream buffer that allows to build sophisticated character strings out of simple types, e.g.:
```cpp
double a = 1.12;
int b = 2;
bool c = false;
std::string d = "ddd";

ElemUtils::LoggerManager::getInstance()->info("SomeClass", "someFunction", ElemUtils::Formatter() << "We have: " << a << " " << b << " " << c << " " << d);
```
which gives:
```sh
26-06-2017 02:20:28 [INFO] (SomeClass::someFunction) We have: 1.12 2 0 ddd
```
Here we have shown how to use the Logger outside a class that inherits from `BaseObject`.

<hr>

# Services and available tasks {#usage_tasks}

This table summarizes all tasks available in computation services. For a given task, click on its name to be directed to a page with more information (such as examples). 

| Service         | Task 				 | Short description                                | 
| :-------------- | :----------------------------------- | :------------------------------------------------|
| [GPDService](@ref PARTONS::GPDService)      | [computeGPDModel](@ref usage_gpd_1)                    	 | Evaluate GPD for single kinematic point    	    |
| [GPDService](@ref PARTONS::GPDService)      | [computeManyKinematicOneModel](@ref usage_gpd_2)         | Evaluate GPD for many kinematic points    	    |
| [GPDService](@ref PARTONS::GPDService)      | [printResults](@ref usage_gpd_3)                         | Print out result to std output                   |
| [GPDService](@ref PARTONS::GPDService)      | [generatePlotFile](@ref usage_gpd_4)                     | Generate plot file from data stored in database  |
|                 |                                      |                                                  |
| [ConvolCoeffFunctionService](@ref PARTONS::ConvolCoeffFunctionService)      | [computeWithGPDModel](@ref usage_cff_1)                	 | Evaluate CFF for single kinematic point          |
| [ConvolCoeffFunctionService](@ref PARTONS::ConvolCoeffFunctionService)      | [computeManyKinematicOneModel](@ref usage_cff_2)         | Evaluate CFF for many kinematic points           |
| [ConvolCoeffFunctionService](@ref PARTONS::ConvolCoeffFunctionService)      | [printResults](@ref usage_cff_3)                         | Print out result to std output                   |
| [ConvolCoeffFunctionService](@ref PARTONS::ConvolCoeffFunctionService)      | [generatePlotFile](@ref usage_cff_4)                     | Generate plot file from data stored in database  |
|                 |                                      |                                                  |
| [ObservableService](@ref PARTONS::ObservableService)      | [computeObservable](@ref usage_obs_1)                    | Evaluate observable for single kinematic point   |
| [ObservableService](@ref PARTONS::ObservableService)      | [computeManyKinematicOneModel](@ref usage_obs_2)         | Evaluate observable for many kinematic points    |
| [ObservableService](@ref PARTONS::ObservableService)      | [printResults](@ref usage_obs_3)                         | Print out result to std output                   |
| [ObservableService](@ref PARTONS::ObservableService)      | [generatePlotFile](@ref usage_obs_4)                     | Generate plot file from data stored in database  |

# Available modules {#usage_modules} 

This table summarizes all modules available in %PARTONS. For a given module, click on the class name for more information. The class name serves also as the module identifier to be used in XML scenarios.
| Module type	| Class name			| Short description							|
| :------------ | :---------------------------- | :------------------------------------------------------------ 	| 
| [GPDModule](@ref PARTONS::GPDModule)	| [GPDGK16](@ref PARTONS::GPDGK16)		| Goloskokov-Kroll model 2016 (analytical DD integration)		| 
| [GPDModule](@ref PARTONS::GPDModule)	| [GPDGK16Numerical](@ref PARTONS::GPDGK16Numerical)	| Goloskokov-Kroll model 2016 (numerical DD integration)		| 
| [GPDModule](@ref PARTONS::GPDModule)	| [GPDMMS13](@ref PARTONS::GPDMMS13)		| Mezrag-Moutarde-Sabatie model 2013 					| 
| [GPDModule](@ref PARTONS::GPDModule)	| [GPDMPSSW13](@ref PARTONS::GPDMPSSW13)		| Moutarde-Pire-Sabatie-Szymanowski-Wagner model 2013 			| 
| [GPDModule](@ref PARTONS::GPDModule)	| [GPDVGG99](@ref PARTONS::GPDVGG99)		| Vanderhaeghen-Guichon-Guidal model 1999		 		| 
| [GPDModule](@ref PARTONS::GPDModule)	| [GPDVinnikov06](@ref PARTONS::GPDVinnikov06)	| Vinnikov model 2011 							| 
|		|				|									|
| [GPDEvolutionModule](@ref PARTONS::GPDEvolutionModule)	| [GPDEvolutionVinnikov](@ref PARTONS::GPDEvolutionVinnikov)		| LO fixed NF evolution Vinnikov routines		|
|		|				|									|
| [ConvolCoeffFunctionModule](@ref PARTONS::ConvolCoeffFunctionModule)		| [DVCSCFFStandard](@ref PARTONS::DVCSCFFStandard)	| LO/NLO light quarks				|
| [ConvolCoeffFunctionModule](@ref PARTONS::ConvolCoeffFunctionModule)		| [DVCSCFFHeavyQuark](@ref PARTONS::DVCSCFFHeavyQuark)	| LO/NLO light and heavy quarks 		|
| [ConvolCoeffFunctionModule](@ref PARTONS::ConvolCoeffFunctionModule)		| [DVCSCFFConstant](@ref PARTONS::DVCSCFFConstant)	| Constant CFFs to be set by the user		|
|		|				|									|
| [ProcessModule](@ref PARTONS::ProcessModule)	| [DVCSProcessGV08](@ref PARTONS::DVCSProcessGV08)	| Guichon-Vanderhaeghen expressions 2008				|
| [ProcessModule](@ref PARTONS::ProcessModule)	| [DVCSProcessBMJ12](@ref PARTONS::DVCSProcessBMJ12)	| Belitsky-Muller-Kirchner expressions 2012				|
| [ProcessModule](@ref PARTONS::ProcessModule)	| [DVCSProcessVGG99](@ref PARTONS::DVCSProcessVGG99)	| Vanderhaeghen-Guichon-Guidal expressions 1999				|
|		|				|									|
| [ActiveFlavorsThresholdsModule](@ref PARTONS::ActiveFlavorsThresholdsModule)	| [ActiveFlavorsThresholdsQuarkMasses](@ref PARTONS::ActiveFlavorsThresholdsQuarkMasses) 	| Thresholds by quarks masses		|
| [ActiveFlavorsThresholdsModule](@ref PARTONS::ActiveFlavorsThresholdsModule)	| [ActiveFlavorsThresholdsConstant](@ref PARTONS::ActiveFlavorsThresholdsConstant)   	| Thresholds to be set by the user	|
|		|				|									|
| [RunningAlphaStrongModule](@ref PARTONS::RunningAlphaStrongModule)	| [RunningAlphaStrongStandard](@ref PARTONS::RunningAlphaStrongStandard)	| Evaluation in MSbar for 3 <= NF <= 6 		|
| [RunningAlphaStrongModule](@ref PARTONS::RunningAlphaStrongModule)	| [RunningAlphaStrongVinnikov](@ref PARTONS::RunningAlphaStrongVinnikov)	| Evaluation in MSbar for 3 <= NF <= 5 as in Vinnikov evolution	|
|		|				|									|
| [XiConverterModule](@ref PARTONS::XiConverterModule)	| [XiConverterXBToXi](@ref PARTONS::XiConverterXBToXi)	| xi = xB / (2 - xB)						|
|		|				|									|
| [ScalesModule](@ref PARTONS::ScalesModule)	| [ScalesQ2Multiplier](@ref PARTONS::ScalesQ2Multiplier)	| muF2 = muR2 = lambda * Q2						|
|		|				|									|
|[Observable](@ref PARTONS::Observable)	| [DVCSAc](@ref PARTONS::DVCSAc)		| DVCS-like beam charge asymmetry phi angle dependent				| 
|[Observable](@ref PARTONS::Observable)	| [DVCSAcCos0Phi](@ref PARTONS::DVCSAcCos0Phi)	| DVCS-like beam charge asymmetry cos(0) Fourier moment				|	
|[Observable](@ref PARTONS::Observable)	| [DVCSAcCos1Phi](@ref PARTONS::DVCSAcCos1Phi)	| DVCS-like beam charge asymmetry \f$\cos(\phi)\f$ Fourier moment				|
|[Observable](@ref PARTONS::Observable)	| [DVCSAcCos2Phi](@ref PARTONS::DVCSAcCos2Phi)	| DVCS-like beam charge asymmetry \f$\cos(2\phi)\f$ Fourier moment				|
|[Observable](@ref PARTONS::Observable)	| [DVCSAcCos3Phi](@ref PARTONS::DVCSAcCos3Phi)	| DVCS-like beam charge asymmetry \f$\cos(3\phi)\f$ Fourier moment				|
|[Observable](@ref PARTONS::Observable)	| [DVCSAllMinus](@ref PARTONS::DVCSAllMinus)		| DVCS-like beam-target LL asymmetry phi angle dependent			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAllMinusCos0Phi](@ref PARTONS::DVCSAllMinusCos0Phi)	| DVCS-like beam-target LL asymmetry cos(0) Fourier moment			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAllMinusCos1Phi](@ref PARTONS::DVCSAllMinusCos1Phi)	| DVCS-like beam-target LL asymmetry \f$\cos(\phi)\f$ Fourier moment			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAllMinusCos2Phi](@ref PARTONS::DVCSAllMinusCos2Phi)	| DVCS-like beam-target LL asymmetry \f$\cos(2\phi)\f$ Fourier moment			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAluDVCS](@ref PARTONS::DVCSAluDVCS)		| DVCS-like beam L asymmetry DVCS part phi angle dependent			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAluDVCSSin1Phi](@ref PARTONS::DVCSAluDVCSSin1Phi)	| DVCS-like beam L asymmetry DVCS part \f$\sin(\phi)\f$ Fourier moment			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAluInt](@ref PARTONS::DVCSAluInt)		| DVCS-like beam L asymmetry INT part phi angle dependent			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAluIntSin1Phi](@ref PARTONS::DVCSAluIntSin1Phi)	| DVCS-like beam L asymmetry INT part \f$\sin(\phi)\f$ Fourier moment			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAluIntSin2Phi](@ref PARTONS::DVCSAluIntSin2Phi)	| DVCS-like beam L asymmetry INT part \f$\sin(2\phi)\f$ Fourier moment			|
|[Observable](@ref PARTONS::Observable)	| [DVCSAluMinus](@ref PARTONS::DVCSAluMinus)		| DVCS-like beam L asymmetry beam charge minus phi angle dependent		|
|[Observable](@ref PARTONS::Observable)	| [DVCSAluMinusSin1Phi](@ref PARTONS::DVCSAluMinusSin1Phi)	| DVCS-like beam L asymmetry beam charge minus \f$\sin(\phi)\f$ Fourier moment		|
|[Observable](@ref PARTONS::Observable)	| [DVCSAulMinus](@ref PARTONS::DVCSAulMinus)		| DVCS-like target L asymmetry beam charge minus phi angle dependent		|
|[Observable](@ref PARTONS::Observable)	| [DVCSAulMinusSin1Phi](@ref PARTONS::DVCSAulMinusSin1Phi)	| DVCS-like target L asymmetry beam charge minus \f$\sin(\phi)\f$ Fourier moment		|
|[Observable](@ref PARTONS::Observable)	| [DVCSAulMinusSin2Phi](@ref PARTONS::DVCSAulMinusSin2Phi)	| DVCS-like target L asymmetry beam charge minus \f$\sin(2\phi)\f$ Fourier moment		|
|[Observable](@ref PARTONS::Observable)	| [DVCSAulMinusSin3Phi](@ref PARTONS::DVCSAulMinusSin3Phi)	| DVCS-like target L asymmetry beam charge minus \f$\sin(3\phi)\f$ Fourier moment		|
|[Observable](@ref PARTONS::Observable)	| [DVCSAutSinPhiMPhis](@ref PARTONS::DVCSAutSinPhiMPhis)	| DVCS-like target T asymmetry beam charge minus phi angle dependent		|
|[Observable](@ref PARTONS::Observable)	| [DVCSAutSinPhiMPhisCos0Phi](@ref PARTONS::DVCSAutSinPhiMPhisCos0Phi)	|DVCS-like target T asymmetry beam charge minus cos(0) Fourier moment	|
|[Observable](@ref PARTONS::Observable)	| [DVCSCrossSectionUUMinus](@ref PARTONS::DVCSCrossSectionUUMinus) | DVCS-like unpolarized cross section phi angle dependent			|
|[Observable](@ref PARTONS::Observable)	| [DVCSCrossSectionDifferenceLUMinus](@ref PARTONS::DVCSCrossSectionDifferenceLUMinus) | DVCS-like beam cross section difference L phi angle dependent	|

