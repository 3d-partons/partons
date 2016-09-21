#include "../../../../../include/partons/database/convol_coeff_function/dao/ConvolCoeffFunctionResultDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <complex>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

ConvolCoeffFunctionResultDao::ConvolCoeffFunctionResultDao() :
        BaseObject("ConvolCoeffFunctionResultDao") {
}

ConvolCoeffFunctionResultDao::~ConvolCoeffFunctionResultDao() {
}

int ConvolCoeffFunctionResultDao::insert(
        const std::string &computationModuleName, const int kinematicId,
        const int computationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO ccf_result (computation_module_name, channel_id, ccf_kinematic_id, computation_id) VALUES (:computationModuleName, :channelId, :kinematicId, :computationId)");

    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    //TODO remove hardcoded value for DVCS ; channelId
    query.bindValue(":channelId", 0);
    query.bindValue(":kinematicId", kinematicId);
    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

int ConvolCoeffFunctionResultDao::insertIntoCCFResultComplex(const int realPart,
        const int imgPart, const int gpdTypeId, const int ccfResultId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO ccf_result_complex (real_part, img_part,gpd_type_id, ccf_result_id) VALUES (:realPart, :imgPart, :gpdTypeId, :ccfResultId)");

    query.bindValue(":realPart", realPart);
    query.bindValue(":imgPart", imgPart);
    query.bindValue(":gpdTypeId", gpdTypeId);
    query.bindValue(":ccfResultId", ccfResultId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionResultDao::getResultListByComputationId(
        const int computationId) const {
    List<DVCSConvolCoeffFunctionResult> resultList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM ccf_result_view WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    fillConvolCoeffFunctionResultList(resultList, query);

    return resultList;
}

void ConvolCoeffFunctionResultDao::fillConvolCoeffFunctionResultList(
        List<DVCSConvolCoeffFunctionResult> &resultList,
        QSqlQuery& query) const {

    info(__func__, "Preparing retrieved data ...");

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

    DVCSConvolCoeffFunctionResult ccfResult;
    std::complex<double> complexValue;

    int currentCCFResultId = -1;
    int currentGPDTypeId = -1;

    if (query.first()) {

        // retrieve gpd_type_id
        currentGPDTypeId = query.value(gpd_type_id_field).toInt();

        // retrieve ccf_result_id
        currentCCFResultId = query.value(ccf_result_id_field).toInt();

        ccfResult.setIndexId(currentCCFResultId);

        complexValue = std::complex<double>(
                query.value(real_part_field).toDouble(),
                query.value(img_part_field).toDouble());

        ccfResult.add(static_cast<GPDType::Type>(currentGPDTypeId),
                complexValue);
    }

    while (query.next()) {

        //TODO create ResultInfo, Computation, ...
        //TODO join kinematic

        // retrieve gpd_type_id
        currentGPDTypeId = query.value(gpd_type_id_field).toInt();

        // retrieve ccf_result_id
        currentCCFResultId = query.value(ccf_result_id_field).toInt();

        if (currentCCFResultId != ccfResult.getIndexId()) {
            resultList.add(ccfResult);
            ccfResult = DVCSConvolCoeffFunctionResult();
            ccfResult.setIndexId(currentCCFResultId);
        }

        complexValue = std::complex<double>(
                query.value(real_part_field).toDouble(),
                query.value(img_part_field).toDouble());

        ccfResult.add(static_cast<GPDType::Type>(currentGPDTypeId),
                complexValue);
    }

    resultList.add(ccfResult);

    info(__func__, "Done !");
}
