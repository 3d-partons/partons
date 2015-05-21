#include "GPDKinematicDao.h"

#include <Qt/qvariant.h> // Needed by bindValue
#include <QtSql/qsqlquery.h>

#include "../DatabaseManager.h"

//TODO comment gere-t-on les problème d'insertion : exception, test de la valeur result = -1 ????
int GPDKinematicDao::insert(int scenarioId, int kinematicType, double x,
        double xi, double t, double MuF, double MuR) {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO gpd_kinematic (scenario_id, kinematic_type_id, x, xi, t, MuF, MuR) VALUES (:scenarioId, :kinematicTypeId, :x, :xi, :t, :MuF, :MuR )");

    query.bindValue(":scenarioId", scenarioId);
    query.bindValue(":kinematicTypeId", kinematicType);
    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF", MuF);
    query.bindValue(":MuR", MuR);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    return result;
}
