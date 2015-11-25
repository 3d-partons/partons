#include "ConvolCoeffFunctionResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>
#include <complex>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResultList.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

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
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}

DVCSConvolCoeffFunctionResultList ConvolCoeffFunctionResultDao::getResultListByComputationId(
        const int computationId) const {
    DVCSConvolCoeffFunctionResultList resultList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM convol_coeff_function_result WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        while (query.next()) {
            DVCSConvolCoeffFunctionResult result;
            getResultFromQuery(query, result);
            resultList.add(result);
        }
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    for (unsigned int i = 0; i != resultList.size(); i++) {
        fillResult(resultList[i]);
    }

    return resultList;
}

void ConvolCoeffFunctionResultDao::getResultFromQuery(QSqlQuery& query,
        DVCSConvolCoeffFunctionResult &result) const {

    int field_id = query.record().indexOf("id");
    int field_kinematic_id = query.record().indexOf(
            "convol_coeff_function_kinematic_id");
    int field_computation_module_name = query.record().indexOf(
            "computation_module_name");

    int id = query.value(field_id).toInt();
    int kinematicId = query.value(field_kinematic_id).toInt();
    std::string computationModuleName = query.value(
            field_computation_module_name).toString().toStdString();

    result.setKinematic(
            m_convolCoeffFunctionKinematicDao.getKinematicById(kinematicId));
    result.setComputationModuleName(computationModuleName);

    result.setId(id);
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
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}

void ConvolCoeffFunctionResultDao::fillResult(
        DVCSConvolCoeffFunctionResult &result) const {

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM ccf_result_complex WHERE ccf_result_id = :resultId");

    query.bindValue(":resultId", result.getId());

    if (query.exec()) {
        while (query.next()) {

            int field_gpd_type_id = query.record().indexOf("gpd_type_id");
            int field_complex_id = query.record().indexOf("complex_id");

            int gpd_type_id = query.value(field_gpd_type_id).toInt();
            int complex_id = query.value(field_complex_id).toInt();

            std::complex<double> complex = m_complexDao.getComplexById(
                    complex_id);

            result.add(static_cast<GPDType::Type>(gpd_type_id), complex);
        }
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();
}
