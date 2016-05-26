#ifndef OBSERVABLE_RESULT_DAO_H
#define OBSERVABLE_RESULT_DAO_H

/**
 * @file ObservableResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ObservableResultDao
 *
 * @brief
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/ObservableResult.h"
#include "ObservableKinematicDao.h"

class ObservableResultDao: public BaseObject {
public:
    ObservableResultDao();
    virtual ~ObservableResultDao();

    int insert(const std::string &observableName, double observableValue,
            double statErrorLB, double statErrorUB, double systErrorLB,
            double systErrorUB, double errorTotal,
            const std::string &computationModuleName, int observableTypeId,
            int kinematicId, int computationId) const;

    List<ObservableResult> getObservableResultListByComputationId(
            const int computationId) const;

    List<ObservableResult> getObservableResultListFromSQLQuery(
            const std::string &sqlQuery) const;

private:
    ObservableKinematicDao m_observableKinematicDao;

    void fillObservableResultList(List<ObservableResult> &observableResultList,
            QSqlQuery& query) const;
};

#endif /* OBSERVABLE_RESULT_DAO_H */
