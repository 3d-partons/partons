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

    static const std::string TABLE_NAME_GPD_KINEMATIC;
    static const std::string COLUMN_NAME_GPD_KINEMATIC_ID;

    static const std::string TABLE_NAME_GPD_RESULT;
};

#endif /* DATABASE_H */
