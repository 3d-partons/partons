#include "GPDKinematicDao.h"

#include <Qt/qvariant.h> // Needed by bindValue
#include <QtSql/qsqlquery.h>

#include "../DatabaseManager.h"

//TODO comment gere-t-on les problème d'insertion : exception, test de la valeur result = -1 ????
int GPDKinematicDao::insert(int kinematicType, double x, double xi, double t,
        double MuF, double MuR) {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO gpd_kinematic (kinematic_type_id, x, xi, t, MuF, MuR) VALUES (:kinematicTypeId, :x, :xi, :t, :MuF, :MuR )");

    query.bindValue(":kinematicTypeId", kinematicType);
    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF", MuF);
    query.bindValue(":MuR", MuR);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    query.clear();

    return result;
}

//TODO select without kinematicType
int GPDKinematicDao::select(int kinematicType, double x, double xi, double t,
        double MuF, double MuR) {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "SELECT id FROM gpd_kinematic WHERE kinematic_type_id = :kinematicType AND x = :x AND xi = :xi AND t = :t AND MuF = :MuF AND MuR = :MuR");

    query.bindValue(":kinematicType", kinematicType);
    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF", MuF);
    query.bindValue(":MuR", MuR);

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    }

    query.clear();

    return result;
}
