#include "ObservableResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

#include "../../../beans/observable/ObservableResultList.h"
#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

ObservableResultDao::ObservableResultDao() :
        BaseObject("ObservableResultDao") {
}

ObservableResultDao::~ObservableResultDao() {
}

int ObservableResultDao::insert(const std::string& observableName,
        double observableValue, double phi, double statErrorLB,
        double statErrorUB, double systErrorLB, double systErrorUB,
        double errorTotal, const std::string &computationModuleName,
        int kinematicId, int computationId) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO observable_result (observable_name, observable_value, phi, stat_error_lb, stat_error_ub, syst_error_lb, syst_error_ub, total_error, computation_module_name, observable_kinematic_id, computation_id)  VALUES (:observable_name, :observable_value, :phi, :stat_error_lb, :stat_error_ub, :syst_error_lb, :syst_error_ub, :total_error, :computationModuleName, :observable_kinematic_id, :computation_id)");

    query.bindValue(":observable_name", QString(observableName.c_str()));
    query.bindValue(":observable_value", observableValue);
    query.bindValue(":phi", phi);
    query.bindValue(":stat_error_lb", statErrorLB);
    query.bindValue(":stat_error_ub", statErrorUB);
    query.bindValue(":syst_error_lb", systErrorLB);
    query.bindValue(":syst_error_ub", systErrorUB);
    query.bindValue(":total_error", errorTotal);
    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":observable_kinematic_id", kinematicId);
    query.bindValue(":computation_id", computationId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}

ObservableResultList ObservableResultDao::getObservableResultListByComputationId(
        const int computationId) const {
//TODO
    return ObservableResultList();
}
