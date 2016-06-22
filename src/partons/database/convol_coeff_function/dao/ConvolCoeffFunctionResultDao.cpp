#include "../../../../../include/partons/database/convol_coeff_function/dao/ConvolCoeffFunctionResultDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <complex>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
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

    query.clear();

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

    query.clear();

    return result;
}

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionResultDao::getResultListByComputationId(
        const int computationId) const {
    List<DVCSConvolCoeffFunctionResult> resultList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM ccf_result_view WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        if (DatabaseManager::getNumberOfRows(query) != 0) {
            fillConvolCoeffFunctionResultList(resultList, query);
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

    return resultList;
}

void ConvolCoeffFunctionResultDao::fillConvolCoeffFunctionResultList(
        List<DVCSConvolCoeffFunctionResult> &resultList,
        QSqlQuery& query) const {

    info(__func__, "Preparing retrieved data ...");

    int ccf_result_id_field = query.record().indexOf("ccf_result_id");
    int computation_module_name_field = query.record().indexOf(
            "computation_module_name");
    int channel_id_field = query.record().indexOf("channel_id");
    int gpd_type_id_field = query.record().indexOf(" gpd_type_id");
    int real_part_field = query.record().indexOf("real_part");
    int img_part_field = query.record().indexOf("img_part");
    int computation_id_field = query.record().indexOf("computation_id");

    DVCSConvolCoeffFunctionResult previousResult;
    GPDType tempGPDType;
    std::complex<double> tempComplex;

    int tempGPDResultId = -1;

    while (query.next()) {

        //TODO create ResultInfo, Computation, ...
        //TODO join kinematic

        tempGPDType =
                GPDType(
                        static_cast<GPDType::Type>(query.value(
                                gpd_type_id_field).toInt()));

        tempComplex.real(query.value(real_part_field).toDouble());
        tempComplex.imag(query.value(img_part_field).toDouble());

        if (tempGPDResultId != previousResult.getIndexId()) {

            if (previousResult.getIndexId() != -1) {
                resultList.add(previousResult);
            }

            tempGPDResultId = query.value(ccf_result_id_field).toInt();
            previousResult = DVCSConvolCoeffFunctionResult();
            previousResult.setIndexId(tempGPDResultId);
            previousResult.setComputationModuleName(
                    query.value(computation_module_name_field).toString().toStdString());
        }

        previousResult.add(tempGPDType.getType(), tempComplex);
    }

    resultList.add(previousResult);
}
