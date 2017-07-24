#ifndef ENVIRONMENT_CONFIGURATION_DAO_H
#define ENVIRONMENT_CONFIGURATION_DAO_H

/**
 * @file EnvironmentConfigurationDao.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 11, 2016
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <ctime>
#include <string>

#include "../../../BaseObject.h"

namespace PARTONS {

class EnvironmentConfiguration;

/**
 * @class EnvironmentConfigurationDao
 *
 * @brief Environment configuration Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete environment configurations from the database. This class in not intended to be used by regular users who should deal with EnvironmentConfigurationDaoService instead.
 */
class EnvironmentConfigurationDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    EnvironmentConfigurationDao();

    /**
     * Destructor.
     */
    virtual ~EnvironmentConfigurationDao();

    /**
     * Insert environment configuration into the database.
     * @param storeDate Insertion time.
     * @param configuration Environment configuration to be stored.
     * @param md5 Hash sum of environment configuration.
     * @return Unique id of inserted row in the database.
     */
    int insert(const time_t &storeDate, const std::string &configuration,
            const std::string &md5) const;

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
     * Retrieve unique id of row containing environment configuration in the database by given hash sum.
     * @param md5 Hash sum of environment configuration to be selected.
     * @return Unique id of database entry that matches with hash sum value.
     */
    int getEnvironmentConfigurationIdByHashSum(const std::string& md5) const;

    /**
     * Retrieve environment configuration by given unique id.
     * @param indexId Unique id of row containing environment configuration to be selected.
     * @return String containing environment configuration.
     */
    std::string getConfigurationByIndexId(const int indexId) const;

private:

    /**
     * Get EnvironmentConfiguration object from given query.
     * @param query Input QSqlQuery query.
     * @return Pointer to extracted EnvironmentConfiguration object.
     */
    EnvironmentConfiguration* getEnvironmentConfigurationFromQuery(
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* ENVIRONMENT_CONFIGURATION_DAO_H */
