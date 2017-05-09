# Services {#services}

[TOC]

Services are developed to make your life easier with PARTONS. You do not need to worry about how to use low level objects or functions, services configure and use them for you. It's a sort of toolbox, which allows you to write simple tasks and get back powerful C++ objects.
Services are instanciated once and for all at the start of the program and all of these instances are stored into the Registry to be able to call them later.

There are many types of services:
* [System](@ref services_system) services.
* [Automation](@ref services_automation) service: to compute automatically things from an XML file that describes a computation scenario. Or to pre-configure modules from an XML file that describes a computation configuration.
* [Computation](@ref services_computation) services: to perform computation of a specific physics's model at a specific computation level with specific kinematics (e.g. compute a GPD model).
* [Database](@ref services_database) services: to store complex C++ objects from PARTONS into the database and get back complex C++ objects from database tables.

# System # {#services_system}

* Partons
* ServiceObjectRegistry
* ModuleObjectFactory

### How to retrieve a service ? ##

~~~~~~~~~~~~~{.cpp}
// Create an empty pointer to store GPD service
GPDService* pGPDService = 0;

// Retrieve existing ServiceObjectRegistry
ServiceObjectRegistry* pServiceObjectRegistry = Partons::getInstance()->getServiceObjectRegistry();

// Ask ServiceObjectRegistry to give us existing GPDService 
pGPDService = pServiceObjectRegistry->getGPDService();


// Or in one line
GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();
~~~~~~~~~~~~~

# Computation # {#services_computation}

Once you have a Service (e.g. GPDService as in the example above), you can use it to compute things.

### Example: GPDService

~~~~~~~~~~~~~{.cpp}
// Get service
GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

// Create GPD module with the ModuleObjectFactory
GPDModule* pGPDModel =Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MMS13Model::classId);

// Create a GPDKinematic(x, xi, t, MuF, MuR) object
GPDKinematic gpdKinematic(-0.99, 0.99, 0., 1., 1.);

GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel);
~~~~~~~~~~~~~

GPDService has many other useful methods (for instance, if you're considering several models, or a list of kinematics). See its documentation for more details.

The same can be said for the other Services, such as ConvolCoeffFunctionService (used for perturbative coefficient functions) or ObservableService.

# Automation # {#services_automation}

The automation service is designed to dynamically run specific tasks (call service object methods) or to create some complex C++ objects, all described by an XML file.
It allows users to perform complex tasks in a simple way without C++ programming and without rebuilding their project.
The automation service provides some methods to retrieve Scenario or ComputationConfiguration objects from a specific XML file.
Scenario can be used to automate the whole computation process by calling many other services to perform tasks previously defined in a XML file.
Computation Configuration is only used to configure a specific module, then you can do whatever you want with this new pre-configured module.

### Examples ##

XML file (named : scenario_example.xml) that describes a scenario to compute a GPD model (GK11Model) with one kinematic.
~~~~~~~~~~~~~{.xml}
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<scenario date="2016-03-25"
    description="This is a test scenario to use the GPD service">

    <task service="GPDService" method="computeGPDModel">
        <kinematic type=GPDKinematic"">
            <param name="x" value="0.1" />
            <param name="xi" value="0.00050025" />
            <param name="t" value="-0.3" />
            <param name="MuF2" value="8" />
            <param name="MuR2" value="8" />
        </kinematic>

        <computation_configuration>
            <module type="GPDModule">
                <param name="className" value="GK11Model" />
            </module>
        </computation_configuration>
    </task>

</scenario>
~~~~~~~~~~~~~

Retrieve scenario from XML file and compute it :

~~~~~~~~~~~~~{.cpp}
// TODO
~~~~~~~~~~~~~

Retrieve computation configuration from XML file and use it :

~~~~~~~~~~~~~{.cpp}
// Retrieve Automation service to read XML file
AutomationService* pAutomationService = Partons::getInstance()->getServiceObjectRegistry()->getAutomationService();

// Retrieve computation configuration string parameters from XML file
ComputationConfigurationParameters computationConfiguration = pAutomationService->readComputationConfigurationXMLFile("computation_configuration_example.xml");

// Retrieve GPD service to perform the configuration of the new GPD module
GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

// Create a new pre-configured GPD module
GPDModule* pGPDModule = pGPDService->newGPDModule(computationConfiguration);
~~~~~~~~~~~~~

# Database # {#services_database}

* Database services are developed to store complex C++ objects into database and get back those objects from database tables in a simple way for the user, without knowing about SQL query language or database schema.
* They ensure the integrity of the database by using transaction and rollback mechanisms if something wrong happens : the database will always stay in a stable state.
* They improve querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.

There are many DAO services for each computation level and related classes are suffixed by « DaoService ».

TODO: mettre le schéma sur la structure du calcul, GPD, CFF, Observable.

* GPDKinematicDaoService
* GPDResultDaoService
* ConvolCoeffFunctionKinematicDaoService
* ConvolCoeffFunctionResultDaoService
* ObservableKinematicDaoService : Its role is to deal with ObservableKinematic C++ objects and related tables from the database.
* ObservableResultDaoService : Its role is to deal with ObervableResult C++ objects and related tables from the database.

(see doxygen documentation for more information)

### How to use database services ? ##

You just need to create a new instance of the desired Dao Service and use it :

~~~~~~~~~~~~~{.cpp}
// Create a new ObservableKinematicDaoService
ObservableKinematicDaoService observableKinematicDaoService;

// Retrieve a list of  ObservableKinematic identified by a computation id from previous inserted results.
List<ObservableKinematic> observableKinematicList = observableKinematicDaoService.getKinematicListByComputationId(computationId);
~~~~~~~~~~~~~

### Some tricks ##

How to insert large number of new experimental data points into database ?

~~~~~~~~~~~~~{.cpp}
// Create a new CSV file with your experimental data points (check documentation)

// Use ObservableUtils class to read the previous created file and get back a list of ObservableKinematic
List<ObservableKinematic> observableKinematicList = ObservableUtils::readKinematicsFromFile("csvFilePath.txt") ;

// Create a new ObservableKinematicDaoService
ObservableKinematicDaoService observableKinematicDaoService;

// Store Observable kinematics to database
observableKinematicDaoService.insert(observableKinematicList) ;
~~~~~~~~~~~~~