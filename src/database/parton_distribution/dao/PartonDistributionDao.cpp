#include "PartonDistributionDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <string>

#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

PartonDistributionDao::PartonDistributionDao() :
        BaseObject("PartonDistributionDao") {
}

PartonDistributionDao::~PartonDistributionDao() {
}

int PartonDistributionDao::insert(double gluonDistributionValue) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO parton_distribution (gluon_distribution_value) VALUES (:gluonDistributionValue )");

    query.bindValue(":gluonDistributionValue", gluonDistributionValue);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}
