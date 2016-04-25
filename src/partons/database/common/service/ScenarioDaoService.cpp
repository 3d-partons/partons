#include "../../../../../include/partons/database/common/service/ScenarioDaoService.h"

#include "../../../../../include/partons/beans/automation/Scenario.h"

ScenarioDaoService::ScenarioDaoService() :
        BaseObject("ScenarioDaoService") {
}

ScenarioDaoService::~ScenarioDaoService() {
}

int ScenarioDaoService::insert(const Scenario &scenario) const {
    return m_scenarioDao.insertWithoutTransaction(scenario.getDescription(), scenario.getFile(),
            scenario.getHashSum());
}

int ScenarioDaoService::getScenarioIdByHashSum(
        const std::string &hashSum) const {
    return m_scenarioDao.getScnearioIdByHashSum(hashSum);
}

std::string ScenarioDaoService::getXMLFileByIndexId(const int indexId) const {
    return m_scenarioDao.getXMLFileByIndexId(indexId);
}
