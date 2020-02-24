#ifndef TCS_OBSERVABLE_RESULT_DAO_SERVICE_H
#define TCS_OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file TCSObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/TCS/TCSObservableResult.h"
#include "../dao/TCSObservableResultDao.h"
#include "TCSObservableKinematicDaoService.h"
#include "ObservableResultDaoService.h"

namespace PARTONS {

/**
 * @class TCSObservableResultDaoService
 *
 * @brief TCS Observable result Data Access Object (DAO) service.
 */
class TCSObservableResultDaoService: public ObservableResultDaoService {

public:

    /**
     * Default constructor.
     */
    TCSObservableResultDaoService();

    /**
     * Destructor.
     */
    virtual ~TCSObservableResultDaoService();

    /**
     * Insert into database a new ObservableResult object with transactions mechanisms.
     *
     * @param observableResult Reference to ObservableResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const TCSObservableResult &observableResult);

    /**
     * Insert into database a list of ObservableResult objects with transactions mechanisms.
     *
     * @param observableResultList Reference to List of ObservableResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<TCSObservableResult> &resultList);

    /**
     * Try to find all entries in the database containing observable results that are associated with given unique computation id value.
     *
     * @param computationId  Computation id of database entries to be found.
     * @return List of ObservableResult objects that matches with computation id.
     */
    List<TCSObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return  List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<TCSObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * TCSObservableResultDao object to perform database queries.
     */
    TCSObservableResultDao m_observableResultDao;

    /**
     * TCSObservableKinematicDaoService object to perform database queries.
     */
    TCSObservableKinematicDaoService m_observableKinematicDaoService;
};

} /* namespace PARTONS */

#endif /* TCS_OBSERVABLE_RESULT_DAO_SERVICE_H */
