#ifndef ENVIRONMENT_CONFIGURATION_H
#define ENVIRONMENT_CONFIGURATION_H

/**
 * @file EnvironmentConfiguration.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 09, 2016
 * @version 1.0
 */

#include <ctime>
#include <string>

#include "../../database/DatabaseFileObject.h"

/**
 * @class EnvironmentConfiguration
 *
 * @brief
 */
class EnvironmentConfiguration: public DatabaseFileObject {
public:
    EnvironmentConfiguration();
    EnvironmentConfiguration(int indexId, time_t storeDate,
            const std::string &configuration, const std::string &hashSum);
    virtual ~EnvironmentConfiguration();
};

#endif /* ENVIRONMENT_CONFIGURATION_H */
