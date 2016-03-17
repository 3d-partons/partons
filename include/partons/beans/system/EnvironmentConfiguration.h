#ifndef ENVIRONMENT_CONFIGURATION_H
#define ENVIRONMENT_CONFIGURATION_H

/**
 * @file EnvironmentConfiguration.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 09 March 2016
 * @version 1.0
 *
 * @class EnvironmentConfiguration
 *
 * @brief
 */

#include <ctime>
#include <string>

#include "../../database/DatabaseObject.h"

class EnvironmentConfiguration: public DatabaseObject {
public:
    EnvironmentConfiguration();
    EnvironmentConfiguration(const std::string &configuration,
            const std::string &md5);
    EnvironmentConfiguration(int indexId, time_t storeDate,
            const std::string &configuration, const std::string &md5);
    virtual ~EnvironmentConfiguration();

    // ##### GETTERS & SETTERS #####

    const std::string& getMd5() const;
    void setMd5(const std::string& md5);
    time_t getStoreDate() const;
    void setStoreDate(time_t storeDate);
    std::string getConfiguration() const;
    void setConfiguration(const std::string& configuration);

private:
    time_t m_storeDate;
    // configuration member is empty when object created from database to save possible large memory space
    // user need to call EnvironmentConfigurationDaoService manually to get configuration by this object indexId
    std::string m_configuration;
    std::string m_md5;
};

#endif /* ENVIRONMENT_CONFIGURATION_H */
