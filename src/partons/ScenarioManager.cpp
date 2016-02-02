#include "../../include/partons/ScenarioManager.h"

#include "../../include/partons/Partons.h"
#include "../../include/partons/ServiceObject.h"
#include "../../include/partons/ServiceObjectRegistry.h"
#include "../../include/partons/utils/parser/xml/Attributs.h"
#include "../../include/partons/utils/stringUtils/Formatter.h"
#include "../../include/partons/utils/stringUtils/StringUtils.h"

// Global static pointer used to ensure a single instance of the class.
ScenarioManager* ScenarioManager::pInstance = 0;

const std::string ScenarioManager::SCENARIO_NODE_NAME = "scenario";
const std::string ScenarioManager::TASK_NODE_NAME = "task";

ScenarioManager::ScenarioManager()
        : BaseObject("ScenarioManager"), XMLParser() {
}

ScenarioManager* ScenarioManager::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new ScenarioManager();
    }

    return pInstance;
}

ScenarioManager::~ScenarioManager() {
}

void ScenarioManager::delete_() {
    if (pInstance) {
        delete pInstance;
        pInstance = 0;
    }
}

void ScenarioManager::playScenario(const std::string &scenarioFilePath) {
    // parse XML file
    analyse(scenarioFilePath);

    // compute each found tasks
    for (unsigned int i = 0; i < m_scenario.size(); i++) {
        Task task = m_scenario.getTask(i);
        Partons::getInstance()->getServiceObjectRegistry()->get(
                task.getServiceName())->computeTask(task);
    }
}

void ScenarioManager::startElement(const std::string &elementName,
        Attributs attributes, const std::string &elementData) {

    debug(__func__, Formatter() << "StartElementName = " << elementName);
    debug(__func__, Formatter() << "Attributs : \n" << attributes.toString());

    if (StringUtils::equals(elementName, ScenarioManager::SCENARIO_NODE_NAME)) {
        std::string scenarioId = attributes.getStringValueOf("id");
        std::string scenarioDescription = attributes.getStringValueOf(
                "description");

        m_scenario.setId(scenarioId);
        m_scenario.setDescription(scenarioDescription);
    }

    if (StringUtils::equals(elementName, ScenarioManager::TASK_NODE_NAME)) {
        m_task = Task();

        m_task.setServiceName(attributes.getStringValueOf("service"));
        m_task.setFunctionName(attributes.getStringValueOf("method"));

        if (attributes.isAvailable("storeInDB")) {
            m_task.setStoreInDB(attributes.getBooleanValueOf("storeInDB"));
        }
    }

    //TODO replace hardcoded parameter name lot of error in parsing parameters later !!!!
    if (StringUtils::equals(elementName, "param")) {
        m_parameterList.add(attributes.getStringValueOf("name"),
                attributes.getStringValueOf("value"));
    }
}

void ScenarioManager::endElement(const std::string& elementName) {
    debug(__func__, Formatter() << "EndElementName = " << elementName);

    // If the end node task is reached is that the task can be stored in the list of tasks scenario.
    if (StringUtils::equals(elementName, ScenarioManager::TASK_NODE_NAME)) {
        m_scenario.add(m_task);
    }
    // else is that an object parameterization is over and it can be stored
    else {
        m_task.addParameterList(elementName, m_parameterList);

        // temporary parameterList object need to be cleared for the next object parameterization
        m_parameterList.clear();
    }
}
