#ifndef ENVIRONMENT_CONFIGURATION_DAO_H
#define ENVIRONMENT_CONFIGURATION_DAO_H

/**
 * @file EnvironmentConfigurationDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 11 March 2016
 * @version 1.0
 *
 * @class EnvironmentConfigurationDao
 *
 * @brief
 */

#include <QtSql/qsqlquery.h>
#include <ctime>
#include <string>

#include "../../../BaseObject.h"

class EnvironmentConfiguration;

class EnvironmentConfigurationDao: public BaseObject {
public:
    EnvironmentConfigurationDao();
    virtual ~EnvironmentConfigurationDao();

    int insert(const time_t &storeDate, const std::string &configuration,
            const std::string &md5) const;
    EnvironmentConfiguration* selectByIndexId(const int indexId) const;
    void deleteByIndexId(const int indexId) const;

    int getEnvironmentConfigurationIdByHashSum(const std::string& md5) const;
    std::string getConfigurationByIndexId(const int indexId) const;

private:
    EnvironmentConfiguration* getEnvironmentConfigurationFromQuery(
            QSqlQuery &query) const;
};

#endif /* ENVIRONMENT_CONFIGURATION_DAO_H */
