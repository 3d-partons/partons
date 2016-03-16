#include "../../../../../include/partons/database/common/service/ComputationDaoService.h"

#include "../../../../../include/partons/beans/automation/ComputationConfiguration.h"
#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/system/EnvironmentConfiguration.h"

ComputationDaoService::ComputationDaoService() :
        BaseObject("CommonDaoService") {
}

ComputationDaoService::~ComputationDaoService() {
}

int ComputationDaoService::insert(const Computation &computation) const {

    int computationId = -1;

    EnvironmentConfiguration* pEnvironmentConfiguration =
            computation.getEnvironmentConfiguration();

    if (pEnvironmentConfiguration == 0) {
        error(__func__,
                "EnvironmentConfiguration object from Computation object is NULL pointer ; missing object");
    }

    // Check if environment computation object already exists in database ; compare md5
    int environmentConfigurationId =
            m_environmentConfigurationDaoService.getEnvironmentConfigurationIdByMD5(
                    pEnvironmentConfiguration->getMd5());

    // If not, insert new entry in database and retrieve its id
    if (environmentConfigurationId == -1) {
        environmentConfigurationId =
                m_environmentConfigurationDaoService.insert(
                        *pEnvironmentConfiguration);
    }

    // Insert new computation entry in database
    computationId = m_computationDao.insert(computation.getDateTime(),
            environmentConfigurationId);

    ComputationConfiguration* pComputationConfiguration =
            computation.getComputationConfiguration();

    if (pComputationConfiguration) {
        // Check if computation configuration object already exists in database ; compare md5
        int computationConfigurationId =
                m_computationConfigurationDaoService.getComputationConfigurationIdByMD5(
                        pComputationConfiguration->getMd5());

        // If not, insert new entry in database and retrieve its id
        if (computationConfigurationId == -1) {
            computationConfigurationId =
                    m_computationConfigurationDaoService.insert(
                            *pComputationConfiguration);
        }

        // Insert new entry in the association table
        m_computationDao.insertIntoComputationConfigurationComputation(
                computationConfigurationId, computationId);
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
