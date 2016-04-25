#ifndef ENVIRONMENT_CONFIGURATION_DAO_SERVICE_H
#define ENVIRONMENT_CONFIGURATION_DAO_SERVICE_H

/**
 * @file EnvironmentConfigurationDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 11, 2016
 * @version 1.0
 *
 * @class EnvironmentConfigurationDaoService
 *
 * @brief
 */

#include <string>

#include "../dao/EnvironmentConfigurationDao.h"

class EnvironmentConfiguration;

//TODO implement transactions and rollback mechanisms
class EnvironmentConfigurationDaoService: public BaseObject {
public:
    /**
     * Default constructor
     */
    EnvironmentConfigurationDaoService();

    /**
     * Default destructor
     */
    virtual ~EnvironmentConfigurationDaoService();

    int insert(const EnvironmentConfiguration &environmentConfiguration) const;

    EnvironmentConfiguration* selectByIndexId(const int indexId) const;

    void deleteByIndexId(const int indexId) const;

    std::string getConfigurationByIndexId(const int indexId) const;

    int getEnvironmentConfigurationIdByHashSum(
            const std::string &hashSum) const;

private:
    EnvironmentConfigurationDao m_environmentConfigurationDao; ///< reference to the right DAO object to perform database queries
};

#endif /* ENVIRONMENT_CONFIGURATION_DAO_SERVICE_H */
