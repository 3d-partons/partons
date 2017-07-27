#include "../../../../include/partons/services/automation/ScenarioManager.h"

#include "../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../include/partons/beans/automation/Task.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/ServiceObject.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {


// Global static pointer used to ensure a single instance of the class.
ScenarioManager* ScenarioManager::pInstance = 0;

ScenarioManager::ScenarioManager() :
        BaseObject("ScenarioManager") {
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

void ScenarioManager::playScenario(const Scenario &scenario) const {
    // compute each each tasks
    for (unsigned int i = 0; i < scenario.size(); i++) {
        //TODO replace by :  Partons::getInstance()->getServiceObjectRegistry()->get(task.getServiceName())->computeTask(scenario.getTask(i));
        Task task = scenario.getTask(i);
        Partons::getInstance()->getServiceObjectRegistry()->get(
                task.getServiceName())->computeTask(task);
    }
}

} /* namespace PARTONS */
