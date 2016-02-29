#ifndef OBSERVABLE_KINEMATIC_DAO_H
#define OBSERVABLE_KINEMATIC_DAO_H

/**
 * @file ObservableKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ObservableKinematicDao
 *
 * @brief
 */

#include <QtSql/qsqlquery.h>

#include "../../../beans/List.h"
#include "../../../beans/observable/ObservableKinematic.h"

class ObservableKinematicDao: public BaseObject {
public:
    ObservableKinematicDao();
    virtual ~ObservableKinematicDao();

    int insert(double xB, double t, double Q2, double phi) const;

    int select(double xB, double t, double Q2, double phi) const;

    List<ObservableKinematic> getKinematicListByComputationId(
            int computationId) const;

    ObservableKinematic getKinematicById(const int kinematicId) const;

private:
    void fillObservableKinematic(ObservableKinematic &observableKinematic,
            QSqlQuery &query) const;

    void fillObservableKinematicList(
            List<ObservableKinematic> &observableKinematicList,
            QSqlQuery &query) const;
};

#endif /* OBSERVABLE_KINEMATIC_DAO_H */
