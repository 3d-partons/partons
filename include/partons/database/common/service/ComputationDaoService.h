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

namespace PARTONS {

class Computation;

/**
 * @class ComputationDaoService
 *
 * @brief Computation information Data Access Object (DAO) service.
 *
 * It deals with Computation C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove computation information from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */
class ComputationDaoService: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ComputationDaoService();

    /**
     * Destructor.
     */
    virtual ~ComputationDaoService();

    /**
     * Insert into database a new Computation object without transactions mechanisms.
     * @param computation Reference to Computation object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(const Computation &computation) const;

    /**
     * Retrieve computation information from the database by given unique id of row.
     * @param indexId Unique id of row in the database to be selected.
     * @return Computation object containing computation information retrieved from the database.
     */
    Computation getByComputationId(const int indexId) const;

    /**
     * Retrieve computation information from the database by given computation time.
     * @param dateTime Computation time to be selected.
     * @return Unique id of selected row in the database.
     */
    int getComputationIdByDateTime(const time_t &dateTime) const;

    /**
     * Check if row containing computation information of given id exists in the database.
     * @param computationId Unique id to be checked.
     * @return True if requested information is available, false otherwise.
     */
    bool isAvailable(const int computationId) const;

private:

    /**
     * ComputationDao object to perform database queries.
     */
    ComputationDao m_computationDao;

    /**
     * EnvironmentConfigurationDaoService object to perform database queries.
     */
    EnvironmentConfigurationDaoService m_environmentConfigurationDaoService;
};

} /* namespace PARTONS */

#endif /* COMMON_DAO_SERVICE_H */
