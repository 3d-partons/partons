#include "../../../../../include/partons/database/convol_coeff_function/dao/ConvolCoeffFunctionResultDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqlquery.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

ConvolCoeffFunctionResultDao::ConvolCoeffFunctionResultDao(
        const std::string &className) :
        BaseObject(className) {
}

ConvolCoeffFunctionResultDao::~ConvolCoeffFunctionResultDao() {
}

int ConvolCoeffFunctionResultDao::insertIntoCCFResult(
        const std::string &computationModuleName, const ChannelType::Type channelType,
        const int kinematicId, const int computationId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO ccf_result (computation_module_name, channel_id, ccf_kinematic_id, computation_id) VALUES (:computationModuleName, :channelId, :kinematicId, :computationId)");

    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":channelId", channelType);
    query.bindValue(":kinematicId", kinematicId);
    query.bindValue(":computationId", computationId);

    //execute
    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

} /* namespace PARTONS */
