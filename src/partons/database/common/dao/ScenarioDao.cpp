#include "../../../../../include/partons/database/common/dao/ScenarioDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>

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

    query.prepare(
            "INSERT INTO scenario (description, xml_file, hash_sum) VALUES (:description, :xmlFile, :hashSum)");

    query.bindValue("description", QString(description.c_str()));
    query.bindValue(":xmlFile", QString(xmlFile.c_str()));
    query.bindValue(":hashSum", QString(hashSum.c_str()));

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

int ScenarioDao::getScenarioIdByHashSum(const std::string& hashSum) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT id FROM scenario WHERE hash_sum = :hashSum");

    query.bindValue(":hashSum", QString(hashSum.c_str()));

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

std::string ScenarioDao::getXMLFileByIndexId(const int indexId) const {
    std::string xmlFile = ElemUtils::StringUtils::EMPTY;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT xml_file FROM scenario WHERE id = :indexId");

    query.bindValue(":indexId", indexId);

    if (query.exec()) {
        if (query.first()) {
            xmlFile = query.value(0).toString().toStdString();
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Cannot found result for scenarioId = "
                            << indexId);
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return xmlFile;
}

int ScenarioDao::getScenarioIdByComputationId(const int computationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT scenario_id FROM scenario_computation WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

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

std::string ScenarioDao::getHashSumById(const int scenarioId) {
    std::string result = ElemUtils::StringUtils::EMPTY;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT hash_sum FROM scenario WHERE id = :scenarioId;");

    query.bindValue(":scenarioId", scenarioId);

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toString().toStdString();
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Cannot found scenario into database with id = "
                            << scenarioId);
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

Scenario* ScenarioDao::getScenarioById(const int scenarioId) {
    Scenario* pScenario = 0;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT * FROM scenario WHERE id = :scenarioId");

    query.bindValue(":scenarioId", scenarioId);

    if (query.exec()) {
        if (query.first()) {
            pScenario = ResourceManager::getInstance()->registerScenario(
                    query.value(0).toInt(),
                    query.value(2).toString().toStdString(),
                    query.value(1).toDateTime().toTime_t(),
                    ElemUtils::StringUtils::EMPTY,
                    query.value(4).toString().toStdString(),
                    query.value(3).toString().toStdString());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Cannot found scenario into database with id = "
                            << scenarioId);
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return pScenario;
}
