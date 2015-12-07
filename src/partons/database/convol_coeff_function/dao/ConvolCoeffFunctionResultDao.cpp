#include "../../../../../include/partons/database/convol_coeff_function/dao/ConvolCoeffFunctionResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>
#include <complex>

#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

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
            "INSERT INTO convol_coeff_function_result (computation_module_name, convol_coeff_function_kinematic_id, computation_id) VALUES (:computationModuleName, :kinematicId, :computationId)");

    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":kinematicId", kinematicId);
    query.bindValue(":computationId", computationId);

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

int ConvolCoeffFunctionResultDao::insertIntoCCFResultComplex(
        const int gpdTypeId, const int ccfResultId, const int complexId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO ccf_result_complex (gpd_type_id, ccf_result_id, complex_id) VALUES (:gpdTypeId, :ccfResultId, :complexId)");

    query.bindValue(":gpdTypeId", gpdTypeId);
    query.bindValue(":ccfResultId", ccfResultId);
    query.bindValue(":complexId", complexId);

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

ResultList<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionResultDao::getResultListByComputationId(
        const int computationId) const {
    ResultList<DVCSConvolCoeffFunctionResult> resultList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM convol_coeff_function_result WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        fillConvolCoeffFunctionResultList(resultList, query);
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return resultList;
}

void ConvolCoeffFunctionResultDao::fillConvolCoeffFunctionResultList(
        ResultList<DVCSConvolCoeffFunctionResult> &resultList,
        QSqlQuery& query) const {

    int field_id = query.record().indexOf("id");
    int field_kinematic_id = query.record().indexOf(
            "convol_coeff_function_kinematic_id");
    int field_computation_module_name = query.record().indexOf(
            "computation_module_name");

    while (query.next()) {
        int id = query.value(field_id).toInt();
        int kinematicId = query.value(field_kinematic_id).toInt();
        std::string computationModuleName = query.value(
                field_computation_module_name).toString().toStdString();

        DVCSConvolCoeffFunctionResult convolCoeffFunctionResult;

        convolCoeffFunctionResult.setKinematic(
                m_convolCoeffFunctionKinematicDao.getKinematicById(
                        kinematicId));
        convolCoeffFunctionResult.setComputationModuleName(
                computationModuleName);
        convolCoeffFunctionResult.setId(id);

        fillConvolCoeffFunctionResult(convolCoeffFunctionResult);

        resultList.add(convolCoeffFunctionResult);
    }
}

void ConvolCoeffFunctionResultDao::fillConvolCoeffFunctionResult(
        DVCSConvolCoeffFunctionResult &convolCoeffFunctionResult) const {
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM ccf_result_complex WHERE ccf_result_id = :resultId");

    query.bindValue(":resultId", convolCoeffFunctionResult.getId());

    if (query.exec()) {

        int field_gpd_type_id = query.record().indexOf("gpd_type_id");
        int field_complex_id = query.record().indexOf("complex_id");

        while (query.next()) {
            int gpd_type_id = query.value(field_gpd_type_id).toInt();
            int complex_id = query.value(field_complex_id).toInt();

            std::complex<double> complex = m_complexDao.getComplexById(
                    complex_id);

            convolCoeffFunctionResult.add(
                    static_cast<GPDType::Type>(gpd_type_id), complex);
        }
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}
