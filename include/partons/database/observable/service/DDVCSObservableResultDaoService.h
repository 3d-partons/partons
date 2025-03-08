#ifndef DDVCS_OBSERVABLE_RESULT_DAO_SERVICE_H
#define DDVCS_OBSERVABLE_RESULT_DAO_SERVICE_H

/**
 * @file DDVCSObservableResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableResult.h"
#include "../dao/DDVCSObservableResultDao.h"
#include "DDVCSObservableKinematicDaoService.h"
#include "ObservableResultDaoService.h"

namespace PARTONS {

/**
 * @class DDVCSObservableResultDaoService
 *
 * @brief DDVCS Observable result Data Access Object (DAO) service.
 */
class DDVCSObservableResultDaoService: public ObservableResultDaoService {

public:

    /**
     * Default constructor.
     */
    DDVCSObservableResultDaoService();

    /**
     * Destructor.
     */
    virtual ~DDVCSObservableResultDaoService();

    /**
     * Insert into database a new ObservableResult object with transactions mechanisms.
     *
     * @param observableResult Reference to ObservableResult object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DDVCSObservableResult &observableResult);

    /**
     * Insert into database a list of ObservableResult objects with transactions mechanisms.
     *
     * @param observableResultList Reference to List of ObservableResult objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<DDVCSObservableResult> &resultList);

    /**
     * Try to find all entries in the database containing observable results that are associated with given unique computation id value.
     *
     * @param computationId  Computation id of database entries to be found.
     * @return List of ObservableResult objects that matches with computation id.
     */
    List<DDVCSObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return  List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<DDVCSObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * DDVCSObservableResultDao object to perform database queries.
     */
    DDVCSObservableResultDao m_observableResultDao;

    /**
     * DDVCSObservableKinematicDaoService object to perform database queries.
     */
    DDVCSObservableKinematicDaoService m_observableKinematicDaoService;
};

} /* namespace PARTONS */

#endif /* DDVCS_OBSERVABLE_RESULT_DAO_SERVICE_H */
