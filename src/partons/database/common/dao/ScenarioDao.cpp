#include "../../../../../include/partons/database/common/dao/ScenarioDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>

#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/ResourceManager.h"

ScenarioDao::ScenarioDao() :
        BaseObject("ScenarioDao") {
}

ScenarioDao::~ScenarioDao() {
}

int ScenarioDao::insertWithoutTransaction(const std::string &description,
        const std::string& xmlFile, const std::string& hashSum) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_SCENARIO << " ("
            << Database::COLUMN_NAME_SCENARIO_DESCRIPTION << ","
            << Database::COLUMN_NAME_SCENARIO_XML_FILE << ", "
            << Database::COLUMN_NAME_SCENARIO_HASH_SUM
            << ") VALUES (:description, :xmlFile, :hashSum)";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue("description", QString(description.c_str()));
    query.bindValue(":xmlFile", QString(xmlFile.c_str()));
    query.bindValue(":hashSum", QString(hashSum.c_str()));

    if (query.exec()) {
        debug(__func__, Database::getLastExecutedQuery(query));

        result = query.lastInsertId().toInt();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

int ScenarioDao::getScenarioIdByHashSum(const std::string& hashSum) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_SCENARIO_ID << " FROM "
            << Database::TABLE_NAME_SCENARIO << " WHERE "
            << Database::COLUMN_NAME_SCENARIO_HASH_SUM << " = :hashSum";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":hashSum", QString(hashSum.c_str()));

    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query) != 0) {
        result = query.value(0).toInt();
    }

    return result;
}

std::string ScenarioDao::getXMLFileByIndexId(const int indexId) const {
    std::string xmlFile = ElemUtils::StringUtils::EMPTY;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_SCENARIO_XML_FILE
            << " FROM " << Database::TABLE_NAME_SCENARIO << " WHERE "
            << Database::COLUMN_NAME_SCENARIO_ID << " = :indexId";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":indexId", indexId);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    xmlFile = query.value(0).toString().toStdString();

    return xmlFile;
}

int ScenarioDao::getScenarioIdByComputationId(const int computationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_SCENARIO_ID
            << " FROM scenario_computation WHERE "
            << Database::COLUMN_NAME_COMPUTATION_ID << " = :computationId";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":computationId", computationId);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.value(0).toInt();

    return result;
}

std::string ScenarioDao::getHashSumById(const int scenarioId) {
    std::string result = ElemUtils::StringUtils::EMPTY;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_SCENARIO_HASH_SUM
            << " FROM " << Database::TABLE_NAME_SCENARIO << " WHERE "
            << Database::COLUMN_NAME_SCENARIO_ID << " = :scenarioId";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":scenarioId", scenarioId);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.value(0).toString().toStdString();

    return result;
}

Scenario* ScenarioDao::getScenarioById(const int scenarioId) {
    Scenario* pScenario = 0;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM " << Database::TABLE_NAME_SCENARIO << " WHERE "
            << Database::COLUMN_NAME_SCENARIO_ID << " = :scenarioId";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":scenarioId", scenarioId);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    pScenario = ResourceManager::getInstance()->registerScenario(
            query.value(0).toInt(), query.value(2).toString().toStdString(),
            query.value(1).toDateTime().toTime_t(),
            ElemUtils::StringUtils::EMPTY,
            query.value(4).toString().toStdString(),
            query.value(3).toString().toStdString());

    return pScenario;
}
