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
#include <vector>

namespace PARTONS {

/**
 * @class Database
 *
 * @brief Basic object storing database information.
 *
 * It stores names of tables and columns used in all kinds of database queries. In addition, it provides a set of useful tools for handling the queries.
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
    static const std::string COLUMN_NAME_OBSERVABLE_NAME;
    static const std::string COLUMN_NAME_OBSERVABLE_VALUE;

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
     * If position in given query is set to the last record, return number of records and set position to -1. Otherwise, return 0.
     * @param query Input QSqlQuery query.
     * @return Number of records or 0 if position in query is not set to the last record.
     */
    static int getNumberOfRows(QSqlQuery& query);

    /**
     * Execute SQL SELECT-like query.
     * @param query Input QSqlQuery query.
     * @return Number of selected records.
     */
    static int execSelectQuery(QSqlQuery& query);

    /**
     * Get the last executed query.
     * @param query Input QSqlQuery query.
     * @return String containing the last executed SQL query.
     */
    static std::string getLastExecutedQuery(const QSqlQuery& query);

    /**
     * Check if the result of given query contains only one record.
     * @param className Name of class to be used in eventual exception.
     * @param funcName Name of function to be used in eventual exception.
     * @param resultSize Number of records returned by given query.
     * @param query Input QSqlQuery query.
     * @return Number of queries (resultSize variable).
     */
    static unsigned int checkUniqueResult(const std::string &className,
            const std::string &funcName, const unsigned int resultSize,
            const QSqlQuery& query);

    /**
     * Check if the result of given query contains any record.
     * @param className Name of class to be used in eventual exception.
     * @param funcName Name of function to be used in eventual exception.
     * @param resultSize Number of records returned by given query.
     * @param query Input QSqlQuery query.
     */
    static void checkManyResults(const std::string &className,
            const std::string &funcName, const unsigned int resultSize,
            const QSqlQuery& query);

    /**
     * Get pre-formated string containing column names.
     * @param columnNames Vector containing column names.
     * @return Pre-formated string containing columns names as "(name1,name2,...)"
     */
    static std::string getPreFormatedColumnNamesFromVector(
            const std::vector<std::string> &columnNames);

    /**
     * Get pre-formated string containing column values.
     * @param columnValues Vector containing column values.
     * @return  Pre-formated string containing columns values as "VALUES (value1,value2,...)"
     */
    static std::string getPreFormatedColumnValuesFromVector(
            const std::vector<std::string> &columnValues);
};

} /* namespace PARTONS */

#endif /* DATABASE_H */
