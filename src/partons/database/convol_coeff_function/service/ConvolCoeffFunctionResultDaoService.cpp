#include "../../../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionResultDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtSql/qsqlquery.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

ConvolCoeffFunctionResultDaoService::ConvolCoeffFunctionResultDaoService(
        const std::string &className) :
        ResultDaoService(className), m_lastCCFKinematicProcessId(-1), m_lastCCFResultId(
                -1), m_lastCCFResultProcessId(-1), m_ccfKinematicTableFile(
                ElemUtils::StringUtils::EMPTY), m_ccfResultTableFile(
                ElemUtils::StringUtils::EMPTY) {

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //get last id of CCF result
    if (query.exec("SELECT COUNT(ccf_result_id) FROM ccf_result;")) {
        if (query.first()) {
            m_lastCCFResultId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

}

ConvolCoeffFunctionResultDaoService::~ConvolCoeffFunctionResultDaoService() {
}

} /* namespace PARTONS */
