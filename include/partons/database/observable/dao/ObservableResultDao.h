#ifndef OBSERVABLE_RESULT_DAO_H
#define OBSERVABLE_RESULT_DAO_H

/**
 * @file ObservableResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 *
 * @class ObservableResultDao
 *
 * @brief Observable result Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete observable results from the database. This class in not intended to be used by regular users who should deal with ObservableResultDaoService instead.
 */

#include <include/partons/beans/List.h>
#include <include/partons/beans/observable/ObservableResult.h>
#include <include/partons/database/observable/dao/ObservableKinematicDao.h>
#include <QtSql/qsqlquery.h>
#include <string>

class ObservableResultDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ObservableResultDao();

    /**
     * Destructor.
     */
    virtual ~ObservableResultDao();

    /**
     * Insert observable result into database.
     * @param observableName Observable name.
     * @param observableValue Observabke value.
     * @param statErrorLB Statistical error (lower bound).
     * @param statErrorUB Statistical error (upper bound).
     * @param systErrorLB Systematic error (lower bound).
     * @param systErrorUB Systematic error (upper bound).
     * @param errorTotal Total error.
     * @param computationModuleName Name of module used to evaluate this result.
     * @param observableTypeId Observable typ, see ObservableType::Type.
     * @param kinematicId Unique id of row in the database containing observable kinematics associated to this result.
     * @param computationId Unique id of row in the database containing computation information associated to this result.
     * @return Unique id of inserted row in the database.
     */
    int insert(const std::string &observableName, double observableValue,
            double statErrorLB, double statErrorUB, double systErrorLB,
            double systErrorUB, double errorTotal,
            const std::string &computationModuleName, int observableTypeId,
            int kinematicId, int computationId) const;

    /**
     * Retrieve list of observable results from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<ObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    /**
     * Retrieve list of observable results from the database by user-defined SQL query.
     * @param sqlQuery User-defined SQL query.
     * @return List of ObservableResult objects containing observable results retrieved from the database.
     */
    List<ObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:

    /**
     * ObservableKinematicDao object to perform database queries.
     */
    ObservableKinematicDao m_observableKinematicDao;

    /**
     * Fill List of ObservableResult objects from given query.
     * @param observableResultList  List of ObservableResult objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillObservableResultList(List<ObservableResult> &observableResultList,
            QSqlQuery& query) const;
};

#endif /* OBSERVABLE_RESULT_DAO_H */
