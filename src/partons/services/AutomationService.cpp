#include "../../../include/partons/services/AutomationService.h"

#include <ElementaryUtils/parser/XMLAttributs.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/ScenarioManager.h"

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int AutomationService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new AutomationService("AutomationService"));

AutomationService::AutomationService(const std::string &className) :
        BaseObject(className), ElemUtils::XMLParser() {
    // TODO Auto-generated constructor stub

}

AutomationService::~AutomationService() {
    // TODO Auto-generated destructor stub
}

Scenario AutomationService::parseScenarioFile(
        const std::string& scenarioFilePath) {

    // clear previous scenario description
    m_scenario = Scenario();

    // parse XML file
    analyse(scenarioFilePath);

    return m_scenario;
}

void AutomationService::startElement(const std::string &elementName,
        ElemUtils::XMLAttributs attributes, const std::string &elementData) {

    debug(__func__,
            ElemUtils::Formatter() << "StartElementName = " << elementName);
    debug(__func__,
            ElemUtils::Formatter() << "Attributs : \n"
                    << attributes.toString());

    if (ElemUtils::StringUtils::equals(elementName,
            ScenarioManager::SCENARIO_NODE_NAME)) {
        std::string scenarioId = attributes.getStringValueOf("id");
        std::string scenarioDescription = attributes.getStringValueOf(
                "description");

        m_scenario.setId(scenarioId);
        m_scenario.setDescription(scenarioDescription);
    }

    if (ElemUtils::StringUtils::equals(elementName,
            ScenarioManager::TASK_NODE_NAME)) {
        m_task = Task();

        m_task.setServiceName(attributes.getStringValueOf("service"));
        m_task.setFunctionName(attributes.getStringValueOf("method"));

        if (attributes.isAvailable("storeInDB")) {
            m_task.setStoreInDB(attributes.getBooleanValueOf("storeInDB"));
        }
    }

    //TODO replace hardcoded parameter name lot of error in parsing parameters later !!!!
    if (ElemUtils::StringUtils::equals(elementName, "param")) {
        m_parameterList.add(attributes.getStringValueOf("name"),
                attributes.getStringValueOf("value"));
    }
}

void AutomationService::endElement(const std::string& elementName) {
    debug(__func__,
            ElemUtils::Formatter() << "EndElementName = " << elementName);

    // If the end node task is reached is that the task can be stored in the list of tasks scenario.
    if (ElemUtils::StringUtils::equals(elementName,
            ScenarioManager::TASK_NODE_NAME)) {
        m_scenario.add(m_task);
    }
    // else is that an object parameterization is over and it can be stored
    else {
        m_task.addParameters(elementName, m_parameterList);

        // temporary parameterList object need to be cleared for the next object parameterization
        m_parameterList.clear();
    }
}
