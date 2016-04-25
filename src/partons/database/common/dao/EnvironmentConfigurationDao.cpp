#include "../../../../../include/partons/database/common/dao/EnvironmentConfigurationDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/beans/system/EnvironmentConfiguration.h"
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

    query.prepare(
            "INSERT INTO environment_configuration (store_date, configuration, hash_sum) VALUES (:storeDate, :configuration, :hashSum)");

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

    query.prepare(
            "SELECT id, store_date, hash_sum FROM environment_configuration WHERE id = :indexId");

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

    query.prepare("DELETE FROM environment_configuration WHERE id = :indexId");

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

    int field_id = query.record().indexOf("id");
    int field_md5 = query.record().indexOf("hash_sum");
    int field_store_date = query.record().indexOf("store_date");

    int indexId = query.value(field_id).toInt();
    std::string hashSum = query.value(field_md5).toString().toStdString();
    time_t storeDate = query.value(field_store_date).toDateTime().toTime_t();

    return new EnvironmentConfiguration(indexId, storeDate,
            ElemUtils::StringUtils::EMPTY, hashSum);
}

int EnvironmentConfigurationDao::getEnvironmentConfigurationIdByMD5(
        const std::string& hashSum) const {
    int indexId = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT id FROM environment_configuration WHERE hash_sum = :hashSum");

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

    query.prepare(
            "SELECT configuration FROM environment_configuration WHERE id = :indexId");

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
