#include "../../../../../include/partons/database/common/service/ResultInfoDaoService.h"

#include "../../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/system/ResultInfo.h"
#include "../../../../../include/partons/ResourceManager.h"

ResultInfoDaoService::ResultInfoDaoService() :
        BaseObject("ResultInfoDaoService") {
    // TODO Auto-generated constructor stub

}

ResultInfoDaoService::~ResultInfoDaoService() {
    // TODO Auto-generated destructor stub
}

int ResultInfoDaoService::insertWithoutTransaction(
        const ResultInfo& resultInfo) const {

    Scenario* pScenario = ResourceManager::getInstance()->getScenarioByHashSum(
            resultInfo.getScenarioHashSum());
    int scenarioId = -1;

    if (pScenario) {
        // Check if scenario object already exists in database ; compare hash sum
        scenarioId = m_scenarioDaoService.getScenarioIdByHashSum(
                resultInfo.getScenarioHashSum());

        // If not, insert new entry in database and retrieve its id
        if (scenarioId == -1) {
            scenarioId = m_scenarioDaoService.insert(
                    *(ResourceManager::getInstance()->getScenarioByHashSum(
                            resultInfo.getScenarioHashSum())));
        }
    } else {
        error(__func__,
                "pScenario is NULL pointer ; This result has not been produced by a scenario ; It cannot be stored into the database.");
    }

    // Check if this computation date already exists and retrieve its id
    int computationId = m_computationDaoService.getComputationIdByDateTime(
            resultInfo.getComputation().getDateTime());
    // If not, insert new entry in database and retrieve its id
    if (computationId == -1) {
        computationId = m_computationDaoService.insertWithoutTransaction(
                resultInfo.getComputation());
    }

    m_computationDao.insertIntoScenarioComputation(
            resultInfo.getScenarioTaskIndexNumber(), scenarioId, computationId);

    return computationId;
}

ResultInfo ResultInfoDaoService::getResultInfoByComputationId(
        const int computationId) const {
    debug(__func__, "Processing ...");

    return m_resultInfoDao.getResultInfoByComputationId(computationId);
}
