#ifndef COMPUTATION_CONFIGURATION_DAO_SERVICE_H
#define COMPUTATION_CONFIGURATION_DAO_SERVICE_H

/**
 * @file ComputationConfigurationDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 14 March 2016
 * @version 1.0
 *
 * @class ComputationConfigurationDaoService
 *
 * @brief
 */

#include <string>

#include "../dao/ComputationConfigurationDao.h"

class ComputationConfiguration;

class ComputationConfigurationDaoService {
public:
    ComputationConfigurationDaoService();
    virtual ~ComputationConfigurationDaoService();

    int insert(const ComputationConfiguration &computationConfiguration) const;
    int getComputationConfigurationIdByMD5(const std::string &md5) const;

private:

    ComputationConfigurationDao m_computationConfigurationDao;
};

#endif /* COMPUTATION_CONFIGURATION_DAO_SERVICE_H */
