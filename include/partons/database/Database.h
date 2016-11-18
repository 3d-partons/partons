#ifndef DATABASE_H
#define DATABASE_H

/**
 * @file Database.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 25, 2016
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

/**
 * @class Database
 *
 * @brief
 */
class Database {
public:
    static const std::string TABLE_NAME_COMPUTATION;
    static const std::string COLUMN_NAME_COMPUTATION_ID;
    static const std::string COLUMN_NAME_COMPUTATION_MODULE_NAME;

    static const std::string TABLE_NAME_SCENARIO;
    static const std::string COLUMN_NAME_SCENARIO_ID;
    static const std::string COLUMN_NAME_SCENARIO_STORE_DATE;
    static const std::string COLUMN_NAME_SCENARIO_DESCRIPTION;
    static const std::string COLUMN_NAME_SCENARIO_XML_FILE;
    static const std::string COLUMN_NAME_SCENARIO_HASH_SUM;

    static const std::string TABLE_NAME_ENVIRONMENT_CONFIGURATION;
    static const std::string COLUMN_NAME_ENVIRONMENT_CONFIGURATION_ID;
    static const std::string COLUMN_NAME_ENVIRONMENT_CONFIGURATION_STORE_DATE;
    static const std::string COLUMN_NAME_ENVIRONMENT_CONFIGURATION_CONFIGURATION;
    static const std::string COLUMN_NAME_ENVIRONMENT_CONFIGURATION_HASH_SUM;

    static const std::string TABLE_NAME_GPD_KINEMATIC;
    static const std::string COLUMN_NAME_GPD_KINEMATIC_ID;

    static const std::string TABLE_NAME_GPD_RESULT;
    static const std::string COLUMN_NAME_GPD_RESULT_ID;

    static const std::string COLUMN_NAME_PARTON_DISTRIBUTION_ID;

    static const std::string TABLE_NAME_OBSERVABLE_KINEMATIC;
    static const std::string COLUMN_NAME_OBSERVABLE_KINEMATIC_ID;

    static const std::string TABLE_NAME_OBSERVABLE_RESULT;
    static const std::string COLUMN_NAME_OBSERVABLE_RESULT_ID;

    static const std::string COLUMN_NAME_GPD_TYPE_ID;

    static const std::string COLUMN_NAME_GLUON_DISTRIBUTION;

    static const std::string COLUMN_NAME_QUARK_FLAVOR_ID;

    static const std::string COLUMN_NAME_QUARK_DISTRIBUTION;
    static const std::string COLUMN_NAME_QUARK_DISTRIBUTION_PLUS;
    static const std::string COLUMN_NAME_QUARK_DISTRIBUTION_MINUS;

    static const std::string TABLE_NAME_CCF_KINEMATIC;
    static const std::string COLUMN_NAME_CCF_KINEMATIC_ID;

    static const std::string TABLE_NAME_CCF_RESULT;
    static const std::string COLUMN_NAME_CCF_RESULT_ID;
    /**
     * http://stackoverflow.com/a/26500811
     *
     * query.size() is not supported with SQLite. But you can get the number of rows with a workaround.
     *
     * @param query
     * @return
     */
    static int getNumberOfRows(QSqlQuery &query);

    static int execSelectQuery(QSqlQuery& query);
    static std::string getLastExecutedQuery(const QSqlQuery& query);
    static unsigned int checkUniqueResult(const std::string &className,
            const std::string &funcName, const unsigned int resultSize,
            const QSqlQuery& query);
    static void checkManyResults(const std::string &className,
            const std::string &funcName, const unsigned int resultSize,
            const QSqlQuery& query);
};

#endif /* DATABASE_H */
