#include "../../../../../include/partons/database/convol_coeff_function/dao/TCSConvolCoeffFunctionResultDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <QtSql/qsqlquery.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <complex>
#include <string>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

TCSConvolCoeffFunctionResultDao::TCSConvolCoeffFunctionResultDao() :
        ConvolCoeffFunctionResultDao("TCSConvolCoeffFunctionResultDao") {
}

TCSConvolCoeffFunctionResultDao::~TCSConvolCoeffFunctionResultDao() {
}

int TCSConvolCoeffFunctionResultDao::insertIntoTCSCCFResultComplex(
        const double realPart, const double imgPart,
        const GPDType::Type gpdType, const int ccfResultId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO tcs_ccf_result (real_part, img_part, gpd_type_id, ccf_result_id) VALUES (:realPart, :imgPart, :gpdTypeId, :ccfResultId)");

    query.bindValue(":realPart", realPart);
    query.bindValue(":imgPart", imgPart);
    query.bindValue(":gpdTypeId", gpdType);
    query.bindValue(":ccfResultId", ccfResultId);

    //execute query
    if (query.exec()) {

        //get result
        result = query.lastInsertId().toInt();
    } else {

        //throw if error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

List<TCSConvolCoeffFunctionResult> TCSConvolCoeffFunctionResultDao::getResultListByComputationId(
        const int computationId) const {

    //result
    List<TCSConvolCoeffFunctionResult> resultList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM tcs_ccf_result_view WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    //execute
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillConvolCoeffFunctionResultList(resultList, query);
    }

    return resultList;
}

void TCSConvolCoeffFunctionResultDao::fillConvolCoeffFunctionResultList(
        List<TCSConvolCoeffFunctionResult> &resultList,
        QSqlQuery& query) const {

    //get indices
    int ccf_result_id_field = query.record().indexOf(
            QString(Database::COLUMN_NAME_CCF_RESULT_ID.c_str()));
    int computation_module_name_field = query.record().indexOf(
            QString(Database::COLUMN_NAME_COMPUTATION_MODULE_NAME.c_str()));
    int channel_id_field = query.record().indexOf("channel_id");
    int gpd_type_id_field = query.record().indexOf(
            QString(Database::COLUMN_NAME_GPD_TYPE_ID.c_str()));
    int real_part_field = query.record().indexOf("real_part");
    int img_part_field = query.record().indexOf("img_part");
    int computation_id_field = query.record().indexOf(
            QString(Database::COLUMN_NAME_COMPUTATION_ID.c_str()));
    int kinematic_id_field = query.record().indexOf("ccf_kinematic_id");

    //results
    TCSConvolCoeffFunctionResult ccfResult;
    std::complex<double> complexValue;

    int currentCCFResultId = -1;
    int currentGPDTypeId = -1;

    //first query
    if (query.first()) {

        //retrieve gpd_type_id
        currentGPDTypeId = query.value(gpd_type_id_field).toInt();

        //retrieve ccf_result_id
        currentCCFResultId = query.value(ccf_result_id_field).toInt();

        //set index id
        ccfResult.setIndexId(currentCCFResultId);

        //set kinematics
        ccfResult.setKinematic(
                m_tcsConvolCoeffFunctionKinematicDao.getKinematicById(
                        query.value(kinematic_id_field).toInt()));

        //set value
        complexValue = std::complex<double>(
                query.value(real_part_field).toDouble(),
                query.value(img_part_field).toDouble());

        //add
        ccfResult.addResult(static_cast<GPDType::Type>(currentGPDTypeId),
                complexValue);
    }

    //loop over single queries
    while (query.next()) {

        //retrieve gpd_type_id
        currentGPDTypeId = query.value(gpd_type_id_field).toInt();

        //retrieve ccf_result_id
        currentCCFResultId = query.value(ccf_result_id_field).toInt();

        //new result
        if (currentCCFResultId != ccfResult.getIndexId()) {

            //add
            resultList.add(ccfResult);

            //make new for next one
            ccfResult = TCSConvolCoeffFunctionResult();
            ccfResult.setIndexId(currentCCFResultId);

            //set kinematics
            ccfResult.setKinematic(
                    m_tcsConvolCoeffFunctionKinematicDao.getKinematicById(
                            query.value(kinematic_id_field).toInt()));
        }

        //set value
        complexValue = std::complex<double>(
                query.value(real_part_field).toDouble(),
                query.value(img_part_field).toDouble());

        //add
        ccfResult.addResult(static_cast<GPDType::Type>(currentGPDTypeId),
                complexValue);
    }

    //store last parsed result
    resultList.add(ccfResult);
}

} /* namespace PARTONS */
