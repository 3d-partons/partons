#ifndef DVMP_OBSERVABLE_RESULT_DAO_SERVICE_H
#define DVMP_OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file DVMPObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DVMP/DVMPObservableResult.h"
#include "../dao/DVMPObservableResultDao.h"
#include "DVMPObservableKinematicDaoService.h"
#include "ObservableResultDaoService.h"

namespace PARTONS {

/**
 * @class DVMPObservableResultDaoService
 *
 * @brief DVMP Observable result Data Access Object (DAO) service.
 */
class DVMPObservableResultDaoService: public ObservableResultDaoService {

public:

    /**
     * Default constructor.
     */
    DVMPObservableResultDaoService();

    /**
     * Destructor.
     */
    virtual ~DVMPObservableResultDaoService();

    /**
     * Insert into database a new ObservableResult object with transactions mechanisms.
     *
     * @param observableResult Reference to ObservableResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DVMPObservableResult &observableResult);

    /**
     * Insert into database a list of ObservableResult objects with transactions mechanisms.
     *
     * @param observableResultList Reference to List of ObservableResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<DVMPObservableResult> &resultList);

    /**
     * Try to find all entries in the database containing observable results that are associated with given unique computation id value.
     *
     * @param computationId  Computation id of database entries to be found.
     * @return List of ObservableResult objects that matches with computation id.
     */
    List<DVMPObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return  List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<DVMPObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * DVMPObservableResultDao object to perform database queries.
     */
    DVMPObservableResultDao m_observableResultDao;

    /**
     * DVMPObservableKinematicDaoService object to perform database queries.
     */
    DVMPObservableKinematicDaoService m_observableKinematicDaoService;
};

} /* namespace PARTONS */

#endif /* DVMP_OBSERVABLE_RESULT_DAO_SERVICE_H */
