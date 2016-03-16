#ifndef COMMON_DAO_SERVICE_H
#define COMMON_DAO_SERVICE_H

/**
 * @file ComputationDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ComputationDaoService
 *
 * @brief
 */

#include <ctime>

#include "../dao/ComputationDao.h"
#include "ComputationConfigurationDaoService.h"
#include "EnvironmentConfigurationDaoService.h"

class Computation;
class ComputationConfiguration;

class ComputationDaoService: public BaseObject {
public:
    ComputationDaoService();
    virtual ~ComputationDaoService();

    int insert(const Computation &computation) const;
    Computation selectByIndexId(const int indexId) const;
    int getComputationIdByDateTime(const time_t &dateTime) const;

private:
    ComputationDao m_computationDao;

    EnvironmentConfigurationDaoService m_environmentConfigurationDaoService;
    ComputationConfigurationDaoService m_computationConfigurationDaoService;
};

#endif /* COMMON_DAO_SERVICE_H */
