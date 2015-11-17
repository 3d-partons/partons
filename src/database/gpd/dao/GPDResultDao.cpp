#include "GPDResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

GPDResultDao::GPDResultDao() :
        BaseObject("GPDResultDao") {
}

GPDResultDao::~GPDResultDao() {
}

int GPDResultDao::insert(const std::string &computationModuleName,
        int gpdKinematicId, int computationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO gpd_result (computation_module_name, gpd_kinematic_id, computation_id) VALUES (:computationModuleName, :gpdKinematicId, :computationId)");

    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":gpdKinematicId", gpdKinematicId);
    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}
