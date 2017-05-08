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
 * @brief Environment configuration Data Access Object (DAO) service.
 *
 * It deals with EnvironmentConfiguration C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove environment configurations from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#include <include/partons/database/common/dao/EnvironmentConfigurationDao.h>
#include <string>

class EnvironmentConfiguration;

//TODO implement transactions and rollback mechanisms
class EnvironmentConfigurationDaoService: public BaseObject {
public:

    /**
     * Default constructor.
     */
    EnvironmentConfigurationDaoService();

    /**
     * Destructor.
     */
    virtual ~EnvironmentConfigurationDaoService();

    /**
     * Insert into database a new EnvironmentConfiguration object with transactions mechanisms.
     * @param environmentConfiguration Reference to EnvironmentConfiguration object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const EnvironmentConfiguration &environmentConfiguration) const;

    /**
     * Try to find an entry in the database containing environment configuration that matches with given unique id value.
     * @param indexId Unique id of database entry to be found.
     * @return Pointer to EnvironmentConfiguration object filled with the retrieved data or null pointer if couldn't find it.
     */
    EnvironmentConfiguration* selectByIndexId(const int indexId) const;

    /**
     * Delete an entry in the database containing environment configuration that matches with given unique id value.
     * @param indexId Unique id of database entry to be deleted.
     */
    void deleteByIndexId(const int indexId) const;

    /**
     * Retrieve environment configuration by given unique id.
     * @param indexId Unique id of row containing environment configuration to be selected.
     * @return String containing  environment configuration.
     */
    std::string getConfigurationByIndexId(const int indexId) const;

    /**
     * Retrieve unique id of row containing environment configuration in the database by given hash sum.
     * @param hashSum Hash sum of environment configuration to be selected.
     * @return Unique id of database entry that matches with hash sum value.
     */
    int getEnvironmentConfigurationIdByHashSum(
            const std::string &hashSum) const;

private:

    /**
     * EnvironmentConfigurationDao object to perform database queries.
     */
    EnvironmentConfigurationDao m_environmentConfigurationDao;
};

#endif /* ENVIRONMENT_CONFIGURATION_DAO_SERVICE_H */
