#include "../../../../../include/partons/database/common/service/ScenarioDaoService.h"

#include "../../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../../include/partons/ResourceManager.h"

ScenarioDaoService::ScenarioDaoService() :
        BaseObject("ScenarioDaoService") {
    m_pResourceManager = ResourceManager::getInstance();
}

ScenarioDaoService::~ScenarioDaoService() {
}

int ScenarioDaoService::insert(const Scenario &scenario) const {
    return m_scenarioDao.insertWithoutTransaction(scenario.getDescription(),
            scenario.getFile(), scenario.getHashSum());
}

int ScenarioDaoService::getScenarioIdByHashSum(
        const std::string &hashSum) const {
    return m_scenarioDao.getScenarioIdByHashSum(hashSum);
}

std::string ScenarioDaoService::getXMLFileByIndexId(const int indexId) const {
    return m_scenarioDao.getXMLFileByIndexId(indexId);
}

int ScenarioDaoService::getScenarioIdByComputationId(
        const int computationId) const {
    debug(__func__, "Processing ...");

    return m_scenarioDao.getScenarioIdByComputationId(computationId);
}

Scenario* ScenarioDaoService::getScenarioById(const int scenarioId) {
    debug(__func__, "Processing ...");

    Scenario* pScenario = 0;

    std::string scenarioHashSum = getHashSumById(scenarioId);

    pScenario = m_pResourceManager->getScenarioByHashSum(scenarioHashSum);

    if (pScenario == 0) {
        pScenario = m_scenarioDao.getScenarioById(scenarioId);
    }

    return pScenario;
}

std::string ScenarioDaoService::getHashSumById(const int scenarioId) {
    return m_scenarioDao.getHashSumById(scenarioId);
}
