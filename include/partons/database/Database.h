#ifndef DATABASE_H
#define DATABASE_H

/**
 * @file Database.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 25, 2016
 * @version 1.0
 */

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

    static const std::string TABLE_NAME_OBSERVABLE_KINEMATIC;
    static const std::string COLUMN_NAME_OBSERVABLE_KINEMATIC_ID;

    static const std::string TABLE_NAME_OBSERVABLE_RESULT;
    static const std::string COLUMN_NAME_OBSERVABLE_RESULT_ID;
};

#endif /* DATABASE_H */
