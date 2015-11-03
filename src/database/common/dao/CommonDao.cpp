#include "CommonDao.h"

#include <Qt/qdatetime.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>

#include "../../DatabaseManager.h"

int CommonDao::insertComputation(const time_t &dateTime) {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO computation (computation_date) VALUES (:dateTime)");

    QDateTime qDateTime;
    qDateTime.setTime_t(dateTime);
    query.bindValue(":dateTime", qDateTime);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    query.clear();

    return result;
}

int CommonDao::getComputationIdByDateTime(const time_t &dateTime) {
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
    }

    query.clear();

    return result;
}
