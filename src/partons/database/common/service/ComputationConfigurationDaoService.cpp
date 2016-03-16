#include "../../../../../include/partons/database/common/service/ComputationConfigurationDaoService.h"

#include "../../../../../include/partons/beans/automation/ComputationConfiguration.h"

ComputationConfigurationDaoService::ComputationConfigurationDaoService() {
}

ComputationConfigurationDaoService::~ComputationConfigurationDaoService() {
}

int ComputationConfigurationDaoService::insert(
        const ComputationConfiguration& computationConfiguration) const {
    return m_computationConfigurationDao.insert(
            computationConfiguration.getXmlFile(),
            computationConfiguration.getMd5());
}

int ComputationConfigurationDaoService::getComputationConfigurationIdByMD5(
        const std::string& md5) const {
    return m_computationConfigurationDao.getComputationConfigurationIdByMD5(md5);
}
