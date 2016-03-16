#include "../../../../../include/partons/database/common/dao/ComputationConfigurationDao.h"

#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>

#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

ComputationConfigurationDao::ComputationConfigurationDao() :
        BaseObject("ComputationConfigurationDao") {
}

ComputationConfigurationDao::~ComputationConfigurationDao() {
}

int ComputationConfigurationDao::insert(const std::string& xmlFile,
        const std::string& md5) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO computation_configuration (xml_file, md5) VALUES (:xmlFile, :md5)");

    query.bindValue(":xmlFile", QString(xmlFile.c_str()));
    query.bindValue(":md5", QString(md5.c_str()));

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

int ComputationConfigurationDao::getComputationConfigurationIdByMD5(
        const std::string& md5) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT id FROM computation_configuration WHERE md5 = :md5");

    query.bindValue(":md5", QString(md5.c_str()));

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
