#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

/**
 * @file ResourceManager.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 22, 2016
 * @version 1.0
 */

#include <map>
#include <string>

#include "beans/automation/Scenario.h"
#include "beans/system/EnvironmentConfiguration.h"

/**
 * @class ResourceManager
 *
 * @brief
 */
class ResourceManager {
public:
    ResourceManager();
    virtual ~ResourceManager();

    Scenario* getScenario(const std::string &hash_sum) const;
    EnvironmentConfiguration* getEnvironmentConfiguration(
            const std::string &hash_sum) const;

private:
    // key = hash sum file
    std::map<std::string, Scenario*> m_scenarioResourceList;
    // key = hash sum file
    std::map<std::string, EnvironmentConfiguration*> m_environmentConfigurationResourceList;
};

#endif /* RESOURCE_MANAGER_H */
