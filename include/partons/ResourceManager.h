#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

/**
 * @file ResourceManager.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 22, 2016
 * @version 1.0
 */

#include <ctime>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "beans/automation/Scenario.h"
#include "beans/Computation.h"

namespace PARTONS {

/**
 * @class ResourceManager
 *
 * @brief
 */
class ResourceManager: public BaseObject {
public:
    /**
     * Static function to be able to retrieve a unique instance pointer of this class anywhere in the code.
     *
     * @return a unique instance of this class
     */
    static ResourceManager* getInstance();

    /**
     * Default destructor
     */
    virtual ~ResourceManager();

    Scenario* getScenarioByHashSum(const std::string &hashSum) const;

    Scenario* registerScenario(Scenario* pScenario);
    Scenario* registerScenario(
            const std::string &description,
            const std::string &filePath, const std::string &hashSum,
            const std::string &file);

    Computation* getComputationPointer(const time_t &datetime);
    Computation* newComputationObject();

private:
    /**
     * Private pointer of this class for a unique instance
     */
    static ResourceManager* m_pInstance;

    /**
     * Private default constructor for a unique instance
     */
    ResourceManager();

    // key = hashSum of the XML file
    // 1st value = scenario instantiated object
    // 2nd value = counter for know how many system objects refer to it. When counter == 0 mean that Scenario object must be free.
    std::map<std::string, std::pair<Scenario*, unsigned int> > m_scenarioResourceList;

    // key = computation datetime
    // 1st value = computation instantiated object
    // 2nd value = counter for know how many system objects refer to it. When counter == 0 mean that Computation object must be free.
    std::map<time_t, std::pair<std::unique_ptr<Computation>, unsigned int> > computationList;
};

} /* namespace PARTONS */

#endif /* RESOURCE_MANAGER_H */
