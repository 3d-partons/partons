#include "../../../../../include/partons/database/common/dao/ComputationDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

ComputationDao::ComputationDao() :
        BaseObject("ComputationDao") {
}

ComputationDao::~ComputationDao() {
}

int ComputationDao::insert(const time_t &dateTime,
        const int environmentConfigurationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO computation (computation_date, environment_configuration_id) VALUES (:dateTime, :environmentConfigurationId)");

    QDateTime qDateTime;
    qDateTime.setTime_t(dateTime);
    query.bindValue(":dateTime", qDateTime);
    query.bindValue(":environmentConfigurationId", environmentConfigurationId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

int ComputationDao::getComputationIdByDateTime(const time_t &dateTime) const {
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
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

Computation ComputationDao::selectByIndexId(const int indexId) const {
    Computation computation;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT * FROM computation WHERE id = :indexId");

    query.bindValue(":indexId", indexId);

    if (query.exec()) {
        // TODO implement this test in other dao classes
        if (DatabaseManager::getNumberOfRows(query) != 0) {
            fillComputation(computation, query);
        } else {
            warn(__func__,
                    ElemUtils::Formatter() << "No entry for id = " << indexId);
        }
    }

    query.clear();

    return computation;
}

void ComputationDao::fillComputation(Computation &computation,
        QSqlQuery &query) const {
    int field_id = query.record().indexOf("id");
    int field_computation_date = query.record().indexOf("computation_date");

    while (query.next()) {
        int id = query.value(field_id).toInt();
        QDateTime qComputationDate =
                query.value(field_computation_date).toDateTime();

        computation.setIndexId(id);
        computation.setDateTime(qComputationDate.toTime_t());
        //TODO implement environment and configuration
        //TODO implement computation configuration
    }
}

int ComputationDao::insertIntoComputationConfigurationComputation(
        const int computationConfigurationId, const int computationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO computation_configuration_computation (computation_configuration_id, computation_id) VALUES (:computationConfigurationId, :computationId)");

    query.bindValue(":computationConfigurationId", computationConfigurationId);
    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}
