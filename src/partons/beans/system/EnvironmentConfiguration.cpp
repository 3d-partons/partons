#include "../../../../include/partons/beans/system/EnvironmentConfiguration.h"

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
