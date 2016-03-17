#include "../../../../../include/partons/database/common/dao/EnvironmentConfigurationDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
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
        const std::string& configuration, const std::string& md5) const {
    int indexId = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO environment_configuration (store_date, configuration, md5) VALUES (:storeDate, :configuration, :md5)");

    QDateTime qStoreDate;
    qStoreDate.setTime_t(storeDate);
    query.bindValue(":storeDate", qStoreDate);
    query.bindValue(":configuration", QString(configuration.c_str()));
    query.bindValue(":md5", QString(md5.c_str()));

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
            "SELECT id, store_date, md5 FROM environment_configuration WHERE id = :indexId");

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
    int field_md5 = query.record().indexOf("md5");

    int indexId = query.value(field_id).toInt();
    std::string md5 = query.value(field_md5).toString().toStdString();

    //TODO replace hardcoded value
    time_t storeDate = time(0);

    return new EnvironmentConfiguration(indexId, storeDate, "", md5);
}

int EnvironmentConfigurationDao::getEnvironmentConfigurationIdByMD5(
        const std::string& md5) const {
    int indexId = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT id FROM environment_configuration WHERE md5 = :md5");

    query.bindValue(":md5", QString(md5.c_str()));

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
