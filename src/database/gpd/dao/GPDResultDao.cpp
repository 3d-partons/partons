#include "GPDResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <stdexcept>
#include <string>

#include "../DatabaseManager.h"

int GPDResultDao::insert(int moduleConfigurationId, int gpdKinematicId) {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO gpd_result (module_id, gpd_kinematic_id) VALUES (:moduleId, :gpdKinematicId)");

    query.bindValue(":moduleId", moduleConfigurationId);
    query.bindValue(":gpdKinematicId", gpdKinematicId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        throw std::runtime_error(
                "[GPDResultDao::insert] Error when inserting new gpd_result entry in database : "
                        + query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}
