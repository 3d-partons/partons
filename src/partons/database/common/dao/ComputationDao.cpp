#include "../../../../../include/partons/database/common/dao/ComputationDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/database/Database.h"
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

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_COMPUTATION
            << " (computation_date, environment_configuration_id) VALUES (:dateTime, :environmentConfigurationId)";

    query.prepare(QString(formatter.str().c_str()));

    QDateTime qDateTime;
    qDateTime.setTime_t(dateTime);
    query.bindValue(":dateTime", qDateTime);
    query.bindValue(":environmentConfigurationId", environmentConfigurationId);

    if (query.exec()) {
        debug(__func__, Database::getLastExecutedQuery(query));

        result = query.lastInsertId().toInt();
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
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

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_COMPUTATION_ID << " FROM "
            << Database::TABLE_NAME_COMPUTATION
            << " WHERE computation_date = :dateTime";

    query.prepare(QString(formatter.str().c_str()));

    QDateTime qDateTime;
    qDateTime.setTime_t(dateTime);
    query.bindValue(":dateTime", qDateTime);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.value(0).toInt();

    return result;
}

Computation ComputationDao::getByComputationId(const int indexId) const {
    Computation computation;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM " << Database::TABLE_NAME_COMPUTATION
            << " WHERE " << Database::COLUMN_NAME_COMPUTATION_ID
            << " = :indexId";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":indexId", indexId);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    fillComputation(computation, query);

    return computation;
}

void ComputationDao::fillComputation(Computation &computation,
        QSqlQuery &query) const {
    int field_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_COMPUTATION_ID.c_str()));
    int field_computation_date = query.record().indexOf("computation_date");

    int id = query.value(field_id).toInt();
    QDateTime qComputationDate =
            query.value(field_computation_date).toDateTime();

    computation.setIndexId(id);
    computation.setDateTime(qComputationDate.toTime_t());
    //TODO implement environment and configuration
    //TODO implement computation configuration
}

int ComputationDao::insertIntoScenarioComputation(
        const unsigned int scenarioTaskIndexNumber, const int scenarioId,
        const int computationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO scenario_computation (scenario_task_index_number, scenario_id, computation_id) VALUES (:scenarioTaskIndexNumber, :scenarioId, :computationId)");

    query.bindValue(":scenarioTaskIndexNumber", scenarioTaskIndexNumber);
    query.bindValue(":scenarioId", scenarioId);
    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        debug(__func__, Database::getLastExecutedQuery(query));

        result = query.lastInsertId().toInt();
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

bool ComputationDao::isAvailable(const int computationId) const {
    bool result = false;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_COMPUTATION_ID << " FROM "
            << Database::TABLE_NAME_COMPUTATION << " WHERE "
            << Database::COLUMN_NAME_COMPUTATION_ID << " = :computationId";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":computationId", computationId);

    if (Database::execSelectQuery(query) == 1) {
        result = true;
    }

    return result;
}
