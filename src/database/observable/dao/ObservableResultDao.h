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

#include <Qt/qsqlquery.h>
#include <string>

#include "../../../BaseObject.h"

class ObservableResult;

class ObservableResultList;

class ObservableResultDao: public BaseObject {
public:
    ObservableResultDao();
    virtual ~ObservableResultDao();

    int insert(const std::string &observableName, double observableValue,
            double statErrorLB, double statErrorUB, double systErrorLB,
            double systErrorUB, double errorTotal,
            const std::string &computationModuleName, int kinematicId,
            int computationId) const;

    ObservableResultList getObservableResultListByComputationId(
            const int computationId) const;

private:
    ObservableResult getObservableResultFromQuery(QSqlQuery &query) const;
};

#endif /* OBSERVABLE_RESULT_DAO_H */
