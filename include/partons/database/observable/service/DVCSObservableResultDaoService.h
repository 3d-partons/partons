#ifndef DVCS_OBSERVABLE_RESULT_DAO_SERVICE_H
#define DVCS_OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file DVCSObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DVCS/DVCSObservableResult.h"
#include "../dao/DVCSObservableResultDao.h"
#include "DVCSObservableKinematicDaoService.h"
#include "ObservableResultDaoService.h"

namespace PARTONS {

/**
 * @class DVCSObservableResultDaoService
 *
 * @brief DVCS Observable result Data Access Object (DAO) service.
 */
class DVCSObservableResultDaoService: public ObservableResultDaoService {

public:

    /**
     * Default constructor.
     */
    DVCSObservableResultDaoService();

    /**
     * Destructor.
     */
    virtual ~DVCSObservableResultDaoService();

    /**
     * Insert into database a new ObservableResult object with transactions mechanisms.
     *
     * @param observableResult Reference to ObservableResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DVCSObservableResult &observableResult);

    /**
     * Insert into database a list of ObservableResult objects with transactions mechanisms.
     *
     * @param observableResultList Reference to List of ObservableResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<DVCSObservableResult> &resultList);

    /**
     * Try to find all entries in the database containing observable results that are associated with given unique computation id value.
     *
     * @param computationId  Computation id of database entries to be found.
     * @return List of ObservableResult objects that matches with computation id.
     */
    List<DVCSObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return  List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<DVCSObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * DVCSObservableResultDao object to perform database queries.
     */
    DVCSObservableResultDao m_observableResultDao;

    /**
     * DVCSObservableKinematicDaoService object to perform database queries.
     */
    DVCSObservableKinematicDaoService m_observableKinematicDaoService;
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_RESULT_DAO_SERVICE_H */
