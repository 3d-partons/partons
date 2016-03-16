#ifndef ENVIRONMENT_CONFIGURATION_DAO_SERVICE_H
#define ENVIRONMENT_CONFIGURATION_DAO_SERVICE_H

/**
 * @file EnvironmentConfigurationDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 11 March 2016
 * @version 1.0
 *
 * @class EnvironmentConfigurationDaoService
 *
 * @brief
 */

#include <string>

#include "../dao/EnvironmentConfigurationDao.h"

class EnvironmentConfiguration;

class EnvironmentConfigurationDaoService: public BaseObject {
public:
    EnvironmentConfigurationDaoService();
    virtual ~EnvironmentConfigurationDaoService();

    int insert(const EnvironmentConfiguration &environmentConfiguration) const;
    EnvironmentConfiguration* selectByIndexId(const int indexId) const;
    void deleteByIndexId(const int indexId) const;
    std::string getConfigurationByIndexId(const int indexId) const;
    int getEnvironmentConfigurationIdByMD5(const std::string &md5) const;

private:
    EnvironmentConfigurationDao m_environmentConfigurationDao;
};

#endif /* ENVIRONMENT_CONFIGURATION_DAO_SERVICE_H */
