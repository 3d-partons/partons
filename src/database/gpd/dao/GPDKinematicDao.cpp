#include "GPDKinematicDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h> // Needed by bindValue
#include <QtCore/qstring.h>
#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

GPDKinematicDao::GPDKinematicDao() :
        BaseObject("GPDKinematicDao") {
}

GPDKinematicDao::~GPDKinematicDao() {
}

int GPDKinematicDao::insert(double x, double xi, double t, double MuF2,
        double MuR2) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO gpd_kinematic (x, xi, t, MuF2, MuR2) VALUES (:x, :xi, :t, :MuF2, :MuR2)");

    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}

int GPDKinematicDao::select(double x, double xi, double t, double MuF2,
        double MuR2) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT id FROM gpd_kinematic WHERE x = :x AND xi = :xi AND t = :t AND MuF2 = :MuF2 AND MuR2 = :MuR2");

    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}
