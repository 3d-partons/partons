#include "ObservableKinematicDao.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

#include "../../DatabaseManager.h"

int ObservableKinematicDao::insert(double xB, double t, double Q2,
        const std::string& listOfPhi) {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO observable_kinematic (bin_id, xB, t, Q2, phi_list) VALUES (:bin_id, :xB, :t, :Q2, :phi_list )");

    //TODO remove hardcoded value 0 for bin_id
    query.bindValue(":bin_id", 0);
    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":phi_list", QString(listOfPhi.c_str()));

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    query.clear();

    return result;
}

int ObservableKinematicDao::select(double xB, double t, double Q2,
        const std::string &listOfPhi_str) {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "SELECT id FROM observable_kinematic WHERE xB = :xB AND t = :t AND Q2 = :Q2 AND phi_list = :listOfPhi_str");

    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":listOfPhi_str", QString(listOfPhi_str.c_str()));

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    }

    query.clear();

    return result;
}
