#include "../../../../include/partons/beans/system/EnvironmentConfiguration.h"

#include "../../../../include/partons/database/common/service/EnvironmentConfigurationDaoService.h"

namespace PARTONS {


EnvironmentConfiguration::EnvironmentConfiguration() :
        DatabaseFileObject("EnvironmentConfiguration") {
}

EnvironmentConfiguration::EnvironmentConfiguration(int indexId,
        time_t storeDate, const std::string& configuration,
        const std::string& hashSum) :
        DatabaseFileObject("EnvironmentConfiguration") {
    setIndexId(indexId);
}

EnvironmentConfiguration::~EnvironmentConfiguration() {
}

std::string EnvironmentConfiguration::fillFile() const {
    EnvironmentConfigurationDaoService daoService;
    return daoService.getConfigurationByIndexId(getIndexId());
}

} /* namespace PARTONS */
