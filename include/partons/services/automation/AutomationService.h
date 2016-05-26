#ifndef AUTOMATION_SERVICE_H
#define AUTOMATION_SERVICE_H

/**
 * @file AutomationService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 03, 2016
 * @version 1.0
 *
 * @class AutomationService
 *
 * @brief Automation service is designed to dynamically run complex tasks (by calling service object methods) or to create some complex C++ objects, all described by an XML file.
 * It allow users to perform complex tasks in a simple way without know C++ programming language and without rebuild their project.
 * Automation service provides some methods to retrieve Scenario or ComputationConfiguration objects from a specific XML file.
 * Scenario can be used to automates the whole computation process by calling many other services to performs tasks previously defined in XML file.
 * ComputationConfiguration is only used to configure a specific module, then you can do whatever you want with this new pre-configured module.
 */

#include <string>

#include "../../BaseObject.h"

class ResourceManager;

class CryptographicHashService;

class ComputationConfigurationParameters;
class Scenario;
class XMLParserI;
class XMLValidatorI;

class AutomationService: public BaseObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor
     *
     * @param className
     */
    AutomationService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~AutomationService();

    /**
     * Call resolveObjectDependencies method of XMLValidatorI interface (after all modules have been registered and after all programm have been started) to retrieve XML schema file path from property file.
     */
    virtual void resolveObjectDependencies();

    /**
     * Open and validate XML file with XML schema file.
     * Then parse it and return string representation of this file into a Scenario object.
     *
     * @param xmlFilePath
     * @return string representation of XML file into a Scenario object
     */
    Scenario* parseXMLFile(const std::string &xmlFilePath) const;

    /**
     * Open and validate XML file with XML schema file.
     * Then parse it and return string representation of the configuration of a module from XML file into a ComputationConfigurationParameters object.
     *
     * @param xmlFilePath
     * @return string representation of XML file into a ComputationConfigurationParameters object
     */
//    ComputationConfigurationParameters readComputationConfigurationXMLFile(
//            const std::string &xmlFilePath);

    //TODO doxygen documentation
    /**
     *
     * @param scenario
     */
    void playScenario(Scenario* pScenario) const;

private:
    static const std::string PROPERTY_NAME_XML_SCHEMA_FILE_PATH;
    std::string m_xmlSchemaFile;

    XMLValidatorI* m_pXMLValidatorI; ///< Specific interface to allow the plug or unplug xml validator (see AutomationService constructor) for third party libraries. Helpful for developers to avoid code rewriting and for more flexibility.
    XMLParserI* m_pXMLParserI; ///< Specific interface to allow the plug or unplug xml parser (see AutomationService constructor) for third party libraries. Helpful for developers to avoid code rewriting and for more flexibility.

    CryptographicHashService* m_pCryptographicHashService;
    ResourceManager* m_pResourceManager;

    const std::string& getXmlSchemaFile() const;

    Scenario* parseScenario(Scenario* pScenario) const;
};

#endif /* AUTOMATION_SERVICE_H */
