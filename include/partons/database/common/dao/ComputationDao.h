#ifndef COMMON_DAO_H
#define COMMON_DAO_H

/**
 * @file ComputationDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <ctime>

#include "../../../BaseObject.h"

namespace PARTONS {

class Computation;

/**
 * @class ComputationDao
 *
 * @brief Computation information Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete computation information from the database. This class in not intended to be used by regular users who should deal with ComputationDaoService instead.
 */
class ComputationDao: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ComputationDao();

    /**
     * Destructor.
     */
    virtual ~ComputationDao();

    /**
     * Insert computation information into the database.
     * @param dateTime Computation time.
     * @param environmentConfigurationId Unique id of row in the database containing environment configuration information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insert(const time_t &dateTime,
            const int environmentConfigurationId) const;

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
     * Associate row in the database representing Task in Scenario object to that representing Computation.
     * @param scenarioTaskIndexNumber Task object index in given Scenario.
     * @param scenarioId Unique id of row in the database representing Scenario object.
     * @param computationId Unique id of row in the database representing Computation object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoScenarioComputation(
            const unsigned int scenarioTaskIndexNumber, const int scenarioId,
            const int computationId) const;

    /**
     * Check if row containing computation information of given id exists in the database.
     * @param computationId Unique id to be checked.
     * @return True if requested information is available, false otherwise.
     */
    bool isAvailable(const int computationId) const;

private:

    /**
     * Fill Computation object from given query.
     * @param computation Computation object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillComputation(Computation &computation, QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* COMMON_DAO_H */
