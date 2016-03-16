#ifndef COMPUTATION_CONFIGURATION_DAO_H
#define COMPUTATION_CONFIGURATION_DAO_H

/**
 * @file ComputationConfigurationDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 14 March 2016
 * @version 1.0
 *
 * @class ComputationConfigurationDao
 *
 * @brief
 */

#include <string>

#include "../../../BaseObject.h"

class ComputationConfigurationDao : public BaseObject{
public:
    ComputationConfigurationDao();
    virtual ~ComputationConfigurationDao();

    int insert(const std::string &xmlFile, const std::string &md5) const;
    int getComputationConfigurationIdByMD5(const std::string &md5) const;
};

#endif /* COMPUTATION_CONFIGURATION_DAO_H */
