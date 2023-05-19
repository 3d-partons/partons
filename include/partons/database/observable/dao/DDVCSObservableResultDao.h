#ifndef DDVCS_OBSERVABLE_RESULT_DAO_H
#define DDVCS_OBSERVABLE_RESULT_DAO_H

/**
 * @file DDVCSObservableResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableResult.h"
#include "../../../utils/type/PhysicalUnit.h"
#include "DDVCSObservableKinematicDao.h"

namespace PARTONS {

/**
 * @class DDVCSObservableResultDao
 *
 * @brief DDVCS Observable result Data Access Object (DAO).
 */
class DDVCSObservableResultDao: public BaseObject {

public:

    /**
     * Default constructor.
     */
    DDVCSObservableResultDao();

    /**
     * Destructor.
     */
    virtual ~DDVCSObservableResultDao();

    /**
     * Insert observable result into database.
     * @param observableName Observable name.
     * @param value Observable value.
     * @param statErrorLB Statistical error (lower bound).
     * @param statErrorUB Statistical error (upper bound).
     * @param systErrorLB Systematic error (lower bound).
     * @param systErrorUB Systematic error (upper bound).
     * @param scaleErrorLB Scale error (lower bound).
     * @param scaleErrorUB Scale error (upper bound).
     * @param valueUnit Unit.
     * @param kinematicId Unique id of row in the database containing observable kinematics associated to this result.
     * @param computationId Unique id of row in the database containing computation information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insert(const std::string& observableName, const double value,
            const double statErrorLB, const double statErrorUB,
            const double systErrorLB, const double systErrorUB,
            const double scaleErrorLB, const double scaleErrorUB,
            const PhysicalUnit::Type valueUnit, const int kinematicId,
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<DDVCSObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<DDVCSObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * Fill List of ObservableResult objects from given query.
     * @param observableResultList  List of ObservableResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillObservableResultList(
            List<DDVCSObservableResult> &observableResultList,
            QSqlQuery& query) const;

    /**
     * Kinematic DAO.
     */
    DDVCSObservableKinematicDao m_ddvcsObservableKinematicDao;
};

} /* namespace PARTONS */

#endif /* DDVCS_OBSERVABLE_RESULT_DAO_H */
