#include "../../../../include/partons/services/automation/AutomationService.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>

#include "../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../include/partons/beans/automation/Task.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/automation/DefaultXMLParser.h"
#include "../../../../include/partons/services/automation/Qt4XMLValidator.h"
#include "../../../../include/partons/ServiceObject.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

const std::string AutomationService::PROPERTY_NAME_XML_SCHEMA_FILE_PATH =
        "xml.schema.file.path";

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int AutomationService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new AutomationService("AutomationService"));

AutomationService::AutomationService(const std::string &className) :
        BaseObject(className), m_xmlSchemaFilePath(
                ElemUtils::StringUtils::EMPTY), m_pXMLValidatorI(0), m_pXMLParserI(
                0) {
    // Plug Qt4 XML validator as XML validator.
    m_pXMLValidatorI = new Qt4XMLValidator();
    // Plug our own XML parser as XML parser
    m_pXMLParserI = new DefaultXMLParser();
}

AutomationService::~AutomationService() {
    // Free XML validator object memory
    if (m_pXMLValidatorI) {
        delete m_pXMLValidatorI;
        m_pXMLValidatorI = 0;
    }

    // Free XML parser object memory
    if (m_pXMLParserI) {
        delete m_pXMLParserI;
        m_pXMLParserI = 0;
    }
}

const std::string& AutomationService::getXmlSchemaFilePath() const {
    return m_xmlSchemaFilePath;
}

Scenario AutomationService::parseXMLFile(const std::string& xmlFilePath) const {
    Scenario scenario = parseXMLDocument(
            ElemUtils::FileUtils::read(xmlFilePath));
    scenario.setFilePath(xmlFilePath);
    return scenario;
}

Scenario AutomationService::parseXMLDocument(
        const std::string& xmlDocument) const {

    if (!(m_pXMLValidatorI->isValidXMLDocument(getXmlSchemaFilePath(),
            xmlDocument))) {
        error(__func__, "invalid scenario XML");
    }

    return m_pXMLParserI->parseXMLDocument(xmlDocument);
}

//ComputationConfigurationParameters AutomationService::readComputationConfigurationXMLFile(
//        const std::string& xmlFilePath) {
//
////    if (!m_pXMLValidatorI->isValidXMLFile(xmlFilePath)) {
////        error(__func__, "Invalid XML file");
////    }
//
//    m_pXMLValidatorI->isValidXMLFile(xmlFilePath);
//
//    //TODO implement
//    // return m_pXMLParserI->parseComputationConfigurationXMLFile(xmlFilePath);
//}

void AutomationService::resolveObjectDependencies() {
    m_xmlSchemaFilePath =
            ElemUtils::PropertiesManager::getInstance()->getString(
                    AutomationService::PROPERTY_NAME_XML_SCHEMA_FILE_PATH);
}

void AutomationService::playScenario(const Scenario& scenario) const {
    // compute each tasks found
    for (size_t i = 0; i < scenario.size(); i++) {
        Task task = scenario.getTask(i);
        Partons::getInstance()->getServiceObjectRegistry()->get(
                task.getServiceName())->computeTask(task);
    }
}
