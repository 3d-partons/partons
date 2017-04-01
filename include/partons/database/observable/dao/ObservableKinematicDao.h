#ifndef OBSERVABLE_KINEMATIC_DAO_H
#define OBSERVABLE_KINEMATIC_DAO_H

/**
 * @file ObservableKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/ObservableKinematic.h"

/**
 * @class ObservableKinematicDao
 *
 * @brief
 */
class ObservableKinematicDao: public BaseObject {
public:
    ObservableKinematicDao();
    virtual ~ObservableKinematicDao();

    int insert(double xB, double t, double Q2, double E, double phi) const;

    int select(double xB, double t, double Q2, double E, double phi) const;

    List<ObservableKinematic> getKinematicListByComputationId(
            int computationId) const;

    ObservableKinematic getKinematicById(const int kinematicId) const;

    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:
    void fillKinematicFromQuery(ObservableKinematic &observableKinematic,
            QSqlQuery &query) const;

    void fillObservableKinematicListFromQuery(
            List<ObservableKinematic> &observableKinematicList,
            QSqlQuery &query) const;
};

#endif /* OBSERVABLE_KINEMATIC_DAO_H */
