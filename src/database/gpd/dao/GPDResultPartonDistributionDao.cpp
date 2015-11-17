#include "GPDResultPartonDistributionDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <string>

#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

GPDResultPartonDistributionDao::GPDResultPartonDistributionDao() :
        BaseObject("GPDResultPartonDistributionDao") {
}

GPDResultPartonDistributionDao::~GPDResultPartonDistributionDao() {
}

int GPDResultPartonDistributionDao::insert(int gpdTypeId, int gpdResultId,
        int partonDistributionId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO gpd_result_parton_distribution (gpd_type_id, gpd_result_id, parton_distribution_id) VALUES (:gpdTypeId, :gpdResultId, :partonDistributionId)");

    query.bindValue(":gpdTypeId", gpdTypeId);
    query.bindValue(":gpdResultId", gpdResultId);
    query.bindValue(":partonDistributionId", partonDistributionId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}
