#include "../../../../../include/partons/database/common/service/ComputationDaoService.h"

#include <stddef.h>
#include <vector>

#include "../../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../../include/partons/beans/automation/Task.h"
#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/system/EnvironmentConfiguration.h"

ComputationDaoService::ComputationDaoService() :
        BaseObject("CommonDaoService") {
}

ComputationDaoService::~ComputationDaoService() {
}

int ComputationDaoService::insertWithoutTransaction(
        const Computation &computation) const {

    int computationId = -1;

    EnvironmentConfiguration* pEnvironmentConfiguration =
            computation.getEnvironmentConfiguration();

    if (pEnvironmentConfiguration == 0) {
        error(__func__,
                "EnvironmentConfiguration object from Computation object is NULL pointer ; missing object");
    }

    // Check if environment computation object already exists in database ; compare hash sum
    int environmentConfigurationId =
            m_environmentConfigurationDaoService.getEnvironmentConfigurationIdByHashSum(
                    pEnvironmentConfiguration->getHashSum());

    // If not, insert new entry in database and retrieve its id
    if (environmentConfigurationId == -1) {
        environmentConfigurationId =
                m_environmentConfigurationDaoService.insert(
                        *pEnvironmentConfiguration);
    }

    // Insert new computation entry in database
    computationId = m_computationDao.insert(computation.getDateTime(),
            environmentConfigurationId);

    Scenario* pScenario = computation.getScenario();

    if (pScenario) {
        // Check if computation configuration object already exists in database ; compare hash sum
        int scenarioId = m_scenarioDaoService.getScenarioIdByHashSum(
                pScenario->getHashSum());

        // If not, insert new entry in database and retrieve its id
        if (scenarioId == -1) {
            scenarioId = m_scenarioDaoService.insert(*pScenario);
        }

        std::vector<Task> tasks = pScenario->getTasks();

        for (size_t i = 0; i != tasks.size(); i++) {
            // Insert new entry in the association table
            m_computationDao.insertIntoScenarioComputation(i, scenarioId,
                    computationId);
        }
    } else {
        error(__func__,
                "pScenario is NULL pointer ; This result has not been produced by a scenario ; It cannot be stored into the database.");
    }

    return computationId;
}

int ComputationDaoService::getComputationIdByDateTime(
        const time_t &dateTime) const {
    return m_computationDao.getComputationIdByDateTime(dateTime);
}

Computation ComputationDaoService::selectByIndexId(const int indexId) const {
    return m_computationDao.selectByIndexId(indexId);
}

bool ComputationDaoService::isAvailable(const int computationId) const {
    return m_computationDao.isAvailable(computationId);
}
