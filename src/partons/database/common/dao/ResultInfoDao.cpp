#include "../../../../../include/partons/database/common/dao/ResultInfoDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/system/ResultInfo.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

ResultInfoDao::ResultInfoDao() :
        BaseObject("ResultInfoDao") {
}

ResultInfoDao::~ResultInfoDao() {
}

ResultInfo ResultInfoDao::getResultInfoByComputationId(
        const int computationId) const {
    ResultInfo result;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM result_info_view WHERE "
            << Database::COLUMN_NAME_COMPUTATION_ID << " = :computationId;";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        if (DatabaseManager::getNumberOfRows(query) != 0) {
            fillResultInfo(result, query);
        } else {
            warn(__func__,
                    ElemUtils::Formatter()
                            << "No entries found for computationId = "
                            << computationId);
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

void ResultInfoDao::fillResultInfo(ResultInfo &resultInfo,
        QSqlQuery& query) const {

    int f_computationId = query.record().indexOf(
            QString(Database::COLUMN_NAME_COMPUTATION_ID.c_str()));
    int f_computationDate = query.record().indexOf("computationDate");
    int f_scenarioTaskIndexNumber = query.record().indexOf(
            "scenario_task_index_number");
    int f_scenarioHashSum = query.record().indexOf("scenario_hash_sum");
    int f_envConfHashSum = query.record().indexOf("env_conf_hash_sum");

    resultInfo.setComputation(
            Computation(query.value(f_computationId).toInt(),
                    query.value(f_computationDate).toDateTime().toTime_t()));
    resultInfo.setScenarioTaskIndexNumber(
            query.value(f_scenarioTaskIndexNumber).toInt());
    resultInfo.setEnvironmentConfigurationHashSum(
            query.value(f_envConfHashSum).toString().toStdString());
    resultInfo.setScenarioHashSum(
            query.value(f_scenarioHashSum).toString().toStdString());
}
