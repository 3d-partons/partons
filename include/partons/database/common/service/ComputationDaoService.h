#ifndef COMMON_DAO_SERVICE_H
#define COMMON_DAO_SERVICE_H

/**
 * @file ComputationDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <ctime>

#include "../dao/ComputationDao.h"
#include "EnvironmentConfigurationDaoService.h"

class Computation;

/** @class ComputationDaoService
 *
 * @brief
 */
class ComputationDaoService: public BaseObject {
public:
    /**
     * Default constructor
     */
    ComputationDaoService();

    /**
     * Default destructor
     */
    virtual ~ComputationDaoService();

    int insertWithoutTransaction(const Computation &computation) const;

    Computation getByComputationId(const int indexId) const;

    int getComputationIdByDateTime(const time_t &dateTime) const;

    bool isAvailable(const int computationId) const;

private:
    ComputationDao m_computationDao; ///< reference to the right DAO object to perform database queries

    EnvironmentConfigurationDaoService m_environmentConfigurationDaoService;
};

#endif /* COMMON_DAO_SERVICE_H */
