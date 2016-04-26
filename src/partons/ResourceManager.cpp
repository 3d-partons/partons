#include "../../include/partons/ResourceManager.h"

#include <utility>

ResourceManager::ResourceManager() :
        BaseObject("ResourceManager") {

}

ResourceManager::~ResourceManager() {
    std::map<std::string, Scenario*>::iterator it_scenarioResourceList;

    for (it_scenarioResourceList = m_scenarioResourceList.begin();
            it_scenarioResourceList != m_scenarioResourceList.end();
            it_scenarioResourceList++) {
        if (it_scenarioResourceList->second) {
            delete (it_scenarioResourceList->second);
            (it_scenarioResourceList->second) = 0;
        }
    }

    std::map<std::string, EnvironmentConfiguration*>::iterator it_environmentConfigurationResourceList;

    for (it_environmentConfigurationResourceList =
            m_environmentConfigurationResourceList.begin();
            it_environmentConfigurationResourceList
                    != m_environmentConfigurationResourceList.end();
            it_environmentConfigurationResourceList++) {
        if (it_environmentConfigurationResourceList->second) {
            delete (it_environmentConfigurationResourceList->second);
            (it_environmentConfigurationResourceList->second) = 0;
        }
    }
}

Scenario* ResourceManager::getScenario(const std::string& hash_sum) const {
}

EnvironmentConfiguration* ResourceManager::getEnvironmentConfiguration(
        const std::string& hash_sum) const {
}
