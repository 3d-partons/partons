#include "../../../../../include/partons/database/common/dao/ResultInfoDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/system/ResultInfo.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {


ResultInfoDao::ResultInfoDao() :
        BaseObject("ResultInfoDao") {
}

ResultInfoDao::~ResultInfoDao() {
}

ResultInfo ResultInfoDao::getResultInfoByComputationId(
        const int computationId) const {
    debug(__func__, "Processing ...");

    ResultInfo result;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM result_info_view WHERE "
            << Database::COLUMN_NAME_COMPUTATION_ID << " = :computationId;";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":computationId", computationId);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    fillResultInfo(result, query);

    return result;
}

void ResultInfoDao::fillResultInfo(ResultInfo &resultInfo,
        QSqlQuery& query) const {
    debug(__func__, "Processing ...");

    int f_computationId = query.record().indexOf(
            QString(Database::COLUMN_NAME_COMPUTATION_ID.c_str()));
    int f_computationDate = query.record().indexOf("computation_date");
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

} /* namespace PARTONS */
