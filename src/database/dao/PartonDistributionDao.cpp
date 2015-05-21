#include "PartonDistributionDao.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>

#include "../DatabaseManager.h"

int PartonDistributionDao::insert(int gpdResultId, int gpdTypeId,
        double gluonDistribution) {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO parton_distribution (gpd_result_id, gpd_type_id, gluon_distribution) VALUES (:gpdResultId, :gpdTypeId, :gluonDistribution )");

    query.bindValue(":gpdResultId", gpdResultId);
    query.bindValue(":gpdTypeId", gpdTypeId);
    query.bindValue(":gluonDistribution", gluonDistribution);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    return result;
}
