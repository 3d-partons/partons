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
            environmentConfiguration.getConfiguration(),
            environmentConfiguration.getMd5());
}

EnvironmentConfiguration* EnvironmentConfigurationDaoService::selectByIndexId(
        const int indexId) const {
    return m_environmentConfigurationDao.selectByIndexId(indexId);
}

std::string EnvironmentConfigurationDaoService::getConfigurationByIndexId(
        const int indexId) const {
    //TODO implement
    return "";
}

void EnvironmentConfigurationDaoService::deleteByIndexId(
        const int indexId) const {
    m_environmentConfigurationDao.deleteByIndexId(indexId);
}

int EnvironmentConfigurationDaoService::getEnvironmentConfigurationIdByMD5(
        const std::string& md5) const {
    return m_environmentConfigurationDao.getEnvironmentConfigurationIdByMD5(md5);
}
