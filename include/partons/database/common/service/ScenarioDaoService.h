#ifndef SCENARIO_DAO_SERVICE_H
#define SCENARIO_DAO_SERVICE_H

/**
 * @file ScenarioDaoService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 12, 2015
 * @version 1.0
 */

#include <string>

#include "../dao/ScenarioDao.h"

class ResourceManager;
class Scenario;

/**
 * @class ScenarioDaoService
 *
 * @brief
 */
class ScenarioDaoService: public BaseObject {
public:
    ScenarioDaoService();
    virtual ~ScenarioDaoService();

    int insert(const Scenario &scenario) const;

    int getScenarioIdByHashSum(const std::string &hashSum) const;

    std::string getXMLFileByIndexId(const int indexId) const;

    int getScenarioIdByComputationId(const int computationId) const;

    Scenario* getScenarioById(const int scenarioId);

    std::string getHashSumById(const int scenarioId);

private:

    ScenarioDao m_scenarioDao; ///< reference to the right DAO object to perform database queries
    ResourceManager* m_pResourceManager;
};

#endif /* SCENARIO_DAO_SERVICE_H */
