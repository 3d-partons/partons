#include "CommonDao.h"

#include <Qt/qdatetime.h>
#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <string>

#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

CommonDao::CommonDao() :
        BaseObject("CommonDao") {
}

CommonDao::~CommonDao() {
}

int CommonDao::insertComputation(const time_t &dateTime) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO computation (computation_date) VALUES (:dateTime)");

    QDateTime qDateTime;
    qDateTime.setTime_t(dateTime);
    query.bindValue(":dateTime", qDateTime);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}

int CommonDao::getComputationIdByDateTime(const time_t &dateTime) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "SELECT id FROM computation WHERE computation_date = :dateTime");

    QDateTime qDateTime;
    qDateTime.setTime_t(dateTime);
    query.bindValue(":dateTime", qDateTime);

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
