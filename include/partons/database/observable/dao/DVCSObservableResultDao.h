#ifndef DVCS_OBSERVABLE_RESULT_DAO_H
#define DVCS_OBSERVABLE_RESULT_DAO_H

/**
 * @file DVCSObservableResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DVCS/DVCSObservableResult.h"
#include "../../../utils/type/PhysicalUnit.h"

namespace PARTONS {

/**
 * @class DVCSObservableResultDao
 *
 * @brief DVCS Observable result Data Access Object (DAO).
 */
class DVCSObservableResultDao: public BaseObject {

public:

    /**
     * Default constructor.
     */
    DVCSObservableResultDao();

    /**
     * Destructor.
     */
    virtual ~DVCSObservableResultDao();

    /**
     * Insert observable result into database.
     * @param observableName Observable name.
     * @param observableValue Observable value.
     * @param statErrorLB Statistical error (lower bound).
     * @param statErrorUB Statistical error (upper bound).
     * @param systErrorLB Systematic error (lower bound).
     * @param systErrorUB Systematic error (upper bound).
     * @param scaleErrorLB Scale error (lower bound).
     * @param scaleErrorUB Scale error (upper bound).
     * @param unit Unit.
     * @param kinematicId Unique id of row in the database containing observable kinematics associated to this result.
     * @param computationId Unique id of row in the database containing computation information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insert(const std::string& observableName, const double observableValue,
            const double statErrorLB, const double statErrorUB,
            const double systErrorLB, const double systErrorUB,
            const double scaleErrorLB, const double scaleErrorUB,
            const PhysicalUnit::Type unit, const int kinematicId,
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<DVCSObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<DVCSObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * Fill List of ObservableResult objects from given query.
     * @param observableResultList  List of ObservableResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillObservableResultList(
            List<DVCSObservableResult> &observableResultList,
            QSqlQuery& query) const;
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_RESULT_DAO_H */
