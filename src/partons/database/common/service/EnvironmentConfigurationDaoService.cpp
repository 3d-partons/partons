#include "../../../../../include/partons/database/common/service/EnvironmentConfigurationDaoService.h"

#include "../../../../../include/partons/beans/system/EnvironmentConfiguration.h"

EnvironmentConfigurationDaoService::EnvironmentConfigurationDaoService() :
        BaseObject("EnvironmentConfigurationDaoService") {
}

EnvironmentConfigurationDaoService::~EnvironmentConfigurationDaoService() {
}

int EnvironmentConfigurationDaoService::insert(
        const EnvironmentConfiguration& environmentConfiguration) const {
    return m_environmentConfigurationDao.insert(
            environmentConfiguration.getStoreDate(),
            environmentConfiguration.getFile(),
            environmentConfiguration.getHashSum());
}

EnvironmentConfiguration* EnvironmentConfigurationDaoService::selectByIndexId(
        const int indexId) const {
    return m_environmentConfigurationDao.selectByIndexId(indexId);
}

std::string EnvironmentConfigurationDaoService::getConfigurationByIndexId(
        const int indexId) const {
    return m_environmentConfigurationDao.getConfigurationByIndexId(indexId);
}

void EnvironmentConfigurationDaoService::deleteByIndexId(
        const int indexId) const {
    m_environmentConfigurationDao.deleteByIndexId(indexId);
}

int EnvironmentConfigurationDaoService::getEnvironmentConfigurationIdByHashSum(
        const std::string& hashSum) const {
    return m_environmentConfigurationDao.getEnvironmentConfigurationIdByHashSum(
            hashSum);
}
