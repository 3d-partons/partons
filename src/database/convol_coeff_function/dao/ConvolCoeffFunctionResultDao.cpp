#include "ConvolCoeffFunctionResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>
#include <complex>
//#include <iostream>

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

int ConvolCoeffFunctionResultDao::insert(double realPart, double imgPart,
        int gpdType, const std::string &computationModuleName, int kinematicId,
        int computationId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO convol_coeff_function_result (real_part, img_part, gpd_type_id, computation_module_name, convol_coeff_function_kinematic_id, computation_id) VALUES (:realPart, :imgPart, :gpdType, :computationModuleName, :kinematicId, :computationId)");

    query.bindValue(":realPart", realPart);
    query.bindValue(":imgPart", imgPart);
    query.bindValue(":gpdType", gpdType);
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

    return resultList;
}

void ConvolCoeffFunctionResultDao::getResultFromQuery(QSqlQuery& query,
        DVCSConvolCoeffFunctionResult &result) const {

    int field_id = query.record().indexOf("id");
    int field_real_part = query.record().indexOf("real_part");
    int field_img_part = query.record().indexOf("img_part");
    int field_gpd_type = query.record().indexOf("gpd_type_id");
    int field_kinematic_id = query.record().indexOf(
            "convol_coeff_function_kinematic_id");
    int field_computation_module_name = query.record().indexOf(
            "computation_module_name");

    int id = query.value(field_id).toInt();
    double realPart = query.value(field_real_part).toDouble();
    double imgPart = query.value(field_img_part).toDouble();
    int kinematicId = query.value(field_kinematic_id).toInt();
    std::string computationModuleName = query.value(
            field_computation_module_name).toString().toStdString();

    GPDType::Type gpdType =
            static_cast<GPDType::Type>(query.value(field_id).toInt());

    std::complex<double> complexValue(realPart, imgPart);

    result.add(gpdType, complexValue);
    result.setKinematic(
            m_convolCoeffFunctionKinematicDao.getKinematicById(kinematicId));
    result.setComputationModuleName(computationModuleName);

    //TODO implement ; storing id
    //result.setId(id);

    //std::cout << result.toString() << std::endl;
}

