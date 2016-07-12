#include "../../../../../include/partons/database/common/dao/EnvironmentConfigurationDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

EnvironmentConfigurationDao::EnvironmentConfigurationDao() :
        BaseObject("EnvironmentConfigurationDao") {
}

EnvironmentConfigurationDao::~EnvironmentConfigurationDao() {
}

int EnvironmentConfigurationDao::insert(const time_t& storeDate,
        const std::string& configuration, const std::string& hashSum) const {
    int indexId = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO "
            << Database::TABLE_NAME_ENVIRONMENT_CONFIGURATION << " ("
            << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_STORE_DATE << ","
            << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_CONFIGURATION
            << ", " << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_HASH_SUM
            << ") VALUES (:storeDate, :configuration, :hashSum)";

    query.prepare(QString(formatter.str().c_str()));

    QDateTime qStoreDate;
    qStoreDate.setTime_t(storeDate);
    query.bindValue(":storeDate", qStoreDate);
    query.bindValue(":configuration", QString(configuration.c_str()));
    query.bindValue(":hashSum", QString(hashSum.c_str()));

    if (query.exec()) {
        indexId = query.lastInsertId().toInt();
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return indexId;
}

EnvironmentConfiguration* EnvironmentConfigurationDao::selectByIndexId(
        const int indexId) const {
    EnvironmentConfiguration* pResult = 0;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM "
            << Database::TABLE_NAME_ENVIRONMENT_CONFIGURATION << " WHERE "
            << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_ID
            << " = :indexId";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":indexId", indexId);

    if (query.exec()) {
        if (query.first()) {
            pResult = getEnvironmentConfigurationFromQuery(query);
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return pResult;
}

void EnvironmentConfigurationDao::deleteByIndexId(const int indexId) const {
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "DELETE FROM environment_configuration WHERE environment_configuration_id = :indexId");

    query.bindValue(":indexId", indexId);

    if (query.exec()) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}

EnvironmentConfiguration* EnvironmentConfigurationDao::getEnvironmentConfigurationFromQuery(
        QSqlQuery& query) const {

    int field_id = query.record().indexOf(
            Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_ID.c_str());
    int field_md5 = query.record().indexOf(
            Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_HASH_SUM.c_str());
    int field_store_date = query.record().indexOf("store_date");

    int indexId = query.value(field_id).toInt();
    std::string hashSum = query.value(field_md5).toString().toStdString();
    time_t storeDate = query.value(field_store_date).toDateTime().toTime_t();

    return new EnvironmentConfiguration(indexId, storeDate,
            ElemUtils::StringUtils::EMPTY, hashSum);
}

int EnvironmentConfigurationDao::getEnvironmentConfigurationIdByHashSum(
        const std::string& hashSum) const {
    int indexId = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_ID
            << " FROM " << Database::TABLE_NAME_ENVIRONMENT_CONFIGURATION
            << " WHERE "
            << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_HASH_SUM
            << " = :hashSum";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":hashSum", QString(hashSum.c_str()));

    if (query.exec()) {
        if (query.first()) {
            indexId = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return indexId;
}

std::string EnvironmentConfigurationDao::getConfigurationByIndexId(
        const int indexId) const {
    std::string configuration = ElemUtils::StringUtils::EMPTY;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());
    ElemUtils::Formatter formatter;
    formatter << "SELECT "
            << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_CONFIGURATION
            << " FROM " << Database::TABLE_NAME_ENVIRONMENT_CONFIGURATION
            << " WHERE " << Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_ID
            << " = :indexId";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":indexId", indexId);

    if (query.exec()) {
        if (query.first()) {
            configuration = query.value(0).toString().toStdString();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return configuration;
}
