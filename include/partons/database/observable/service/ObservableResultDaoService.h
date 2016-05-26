#ifndef OBSERVABLE_RESULT_DAO_SERVICE_H
#define OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file ObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/ObservableResult.h"
#include "../../common/service/ComputationDaoService.h"
#include "../dao/ObservableResultDao.h"
#include "ObservableKinematicDaoService.h"

/** @class ObservableResultDaoService
 *
 * @brief Its role is to deal with ObervableResult C++ object and related tables from the database.
 * You can insert, select or remove results from database by many kind of stuff.
 * Therefore database services ensure the integrity of the database by using transaction and rollback mechanisms if something wrong append : the database will stay always in a stable state.
 * And they improve querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */
class ObservableResultDaoService: public BaseObject {
public:
    /**
     * Default constructor
     */
    ObservableResultDaoService();

    /**
     * Default destructor
     */
    virtual ~ObservableResultDaoService();

    /**
     * Insert into database a new ObservableResult object with transactions mechanisms.
     *
     * @param observableResult
     * @return unique id related to the new entry inserted into the database
     */
    int insert(const ObservableResult &observableResult) const;

    /**
     * Insert into database a list of ObservableResult objects with transactions mechanisms.
     *
     * @param observableResultList
     * @return unique id related to the last entry inserted into the database
     */
    int insert(const List<ObservableResult> &observableResultList);

    /**
     * Return a list of ObservableResult objects from the database identified by a specific computation identifier.
     *
     * @param computationId
     * @return list of ObservableResult objects.
     */
    List<ObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     *
     *
     * @param sqlQuery
     * @return
     */
    List<ObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:
    ObservableResultDao m_observableResultDao; ///< reference to the right DAO object to perform database queries

    ObservableKinematicDaoService m_observableKinematicDaoService; ///< reference to be able to store kinematic object related to the result.
    ComputationDaoService m_computationDaoService; ///< reference to be able to generate computationId and store ComputationConfiguration object and EnvironmentConfiguration object related to the result.

    /**
     * Insert into the database a new ObservableResult object without using transactions mechanisms.
     * Helpful when dealing with a ResultList<ObservableResult> object, because transactions are already performed earlier.
     *
     * @param observableResult
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(
            const ObservableResult& observableResult) const;
};

#endif /* OBSERVABLE_RESULT_DAO_SERVICE_H */
