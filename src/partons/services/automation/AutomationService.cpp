#include "../../../../include/partons/services/automation/AutomationService.h"

#include <stddef.h>

#include "../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../include/partons/beans/automation/Task.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/automation/DefaultXMLParser.h"
#include "../../../../include/partons/services/automation/Qt4XMLValidator.h"
#include "../../../../include/partons/ServiceObject.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int AutomationService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new AutomationService("AutomationService"));

AutomationService::AutomationService(const std::string &className) :
        BaseObject(className), m_pXMLValidatorI(0), m_pXMLParserI(0) {
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

//TODO refactoring
Scenario AutomationService::parseScenarioXMLFile(
        const std::string& scenarioFilePath) const {

    if(scenarioFilePath.empty())
    {
        error(__func__, "Cannot read XML file : file path is empty");
    }

    if (!(m_pXMLValidatorI->isValidXMLFile(scenarioFilePath))) {
        error(__func__, "invalid scenario XML file");
    }

    //m_pXMLValidatorI->isValidXMLFile(scenarioFilePath);

    return m_pXMLParserI->parseScenarioXMLFile(scenarioFilePath);
}

bool AutomationService::isValidComputationConfigurationXMLFile(
        const std::string& xmlFilePath) const {
    return m_pXMLValidatorI->isValidXMLFile(xmlFilePath);
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
    m_pXMLValidatorI->init();
}

void AutomationService::playScenario(const Scenario& scenario) const {
    // compute each tasks found
    for (size_t i = 0; i < scenario.size(); i++) {
        Task task = scenario.getTask(i);
        Partons::getInstance()->getServiceObjectRegistry()->get(
                task.getServiceName())->computeTask(task);
    }
}
