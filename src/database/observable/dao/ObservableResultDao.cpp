#include "ObservableResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>

#include "../../../beans/observable/ObservableResult.h"
#include "../../../beans/observable/ObservableResultList.h"
#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

ObservableResultDao::ObservableResultDao() :
        BaseObject("ObservableResultDao") {
}

ObservableResultDao::~ObservableResultDao() {
}

int ObservableResultDao::insert(const std::string& observableName,
        double observableValue, double statErrorLB, double statErrorUB,
        double systErrorLB, double systErrorUB, double errorTotal,
        const std::string &computationModuleName, int observableTypeId,
        int kinematicId, int computationId) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO observable_result (observable_name, observable_value, stat_error_lb, stat_error_ub, syst_error_lb, syst_error_ub, total_error, computation_module_name, observable_type_id, observable_kinematic_id, computation_id)  VALUES (:observable_name, :observable_value, :stat_error_lb, :stat_error_ub, :syst_error_lb, :syst_error_ub, :total_error, :computationModuleName, :observableTypeId, :observable_kinematic_id, :computation_id)");

    query.bindValue(":observable_name", QString(observableName.c_str()));
    query.bindValue(":observable_value", observableValue);
    query.bindValue(":stat_error_lb", statErrorLB);
    query.bindValue(":stat_error_ub", statErrorUB);
    query.bindValue(":syst_error_lb", systErrorLB);
    query.bindValue(":syst_error_ub", systErrorUB);
    query.bindValue(":total_error", errorTotal);
    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":observableTypeId", observableTypeId);
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

    ObservableResultList observableResultList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM observable_result WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        while (query.next()) {
            observableResultList.add(getObservableResultFromQuery(query));
        }
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return observableResultList;
}

ObservableResult ObservableResultDao::getObservableResultFromQuery(
        QSqlQuery& query) const {
    ObservableResult result;

    int field_id = query.record().indexOf("id");
    int field_observable_name = query.record().indexOf("observable_name");
    int field_observable_value = query.record().indexOf("observable_value");
    int field_computation_module_name = query.record().indexOf(
            "computation_module_name");

    int id = query.value(field_id).toInt();
    std::string observable_name =
            query.value(field_observable_name).toString().toStdString();
    double observable_value = query.value(field_observable_value).toDouble();
    int computation_module_name =
            query.value(field_computation_module_name).toInt();

    result = ObservableResult(observable_name, observable_value);
    // result.setId(id);

    return result;
}
