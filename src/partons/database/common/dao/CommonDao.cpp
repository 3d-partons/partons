#include "../../../../../include/partons/database/common/dao/CommonDao.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QString>
#include <string>

#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

CommonDao::CommonDao() :
        BaseObject("CommonDao") {
}

CommonDao::~CommonDao() {
}

int CommonDao::insertComputation(const time_t &dateTime) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO computation (computation_date) VALUES (:dateTime)");

    QDateTime qDateTime;
    qDateTime.setTime_t(dateTime);
    query.bindValue(":dateTime", qDateTime);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

int CommonDao::getComputationIdByDateTime(const time_t &dateTime) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

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
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}
