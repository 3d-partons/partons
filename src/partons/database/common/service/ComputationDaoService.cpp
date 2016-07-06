#include "../../../../../include/partons/database/common/service/ComputationDaoService.h"

#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../../../../include/partons/Partons.h"

ComputationDaoService::ComputationDaoService() :
        BaseObject("CommonDaoService") {
}

ComputationDaoService::~ComputationDaoService() {
}

//TODO refactoring
int ComputationDaoService::insertWithoutTransaction(
        const Computation &computation) const {

    int computationId = -1;

    EnvironmentConfiguration* pEnvironmentConfiguration =
            Partons::getInstance()->getEnvironmentConfiguration();

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

    return computationId;
}

int ComputationDaoService::getComputationIdByDateTime(
        const time_t &dateTime) const {
    return m_computationDao.getComputationIdByDateTime(dateTime);
}

Computation ComputationDaoService::getByComputationId(const int indexId) const {
    return m_computationDao.getByComputationId(indexId);
}

bool ComputationDaoService::isAvailable(const int computationId) const {
    return m_computationDao.isAvailable(computationId);
}
