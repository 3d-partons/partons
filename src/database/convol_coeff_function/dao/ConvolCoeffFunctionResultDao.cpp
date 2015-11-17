#include "ConvolCoeffFunctionResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

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

