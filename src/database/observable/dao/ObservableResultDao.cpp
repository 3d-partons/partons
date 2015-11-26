#include "ObservableResultDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>

#include "../../../beans/observable/ObservableKinematic.h"
#include "../../../beans/observable/ObservableResult.h"
#include "../../../beans/observable/ObservableResultList.h"
#include "../../../beans/observable/ObservableType.h"
#include "../../../utils/math/ErrorBar.h"
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
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

ObservableResultList ObservableResultDao::getObservableResultListByComputationId(
        const int computationId) const {

    ObservableResultList results;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM observable_result WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        fillObservableResultList(results, query);
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return results;
}

void ObservableResultDao::fillObservableResultList(
        ObservableResultList &observableResultList, QSqlQuery& query) const {

    int field_id = query.record().indexOf("id");
    int field_observable_name = query.record().indexOf("observable_name");
    int field_observable_value = query.record().indexOf("observable_value");
    int field_stat_error_lb = query.record().indexOf("stat_error_lb");
    int field_stat_error_ub = query.record().indexOf("stat_error_ub");
    int field_syst_error_lb = query.record().indexOf("syst_error_lb");
    int field_syst_error_ub = query.record().indexOf("syst_error_ub");
    int field_total_error = query.record().indexOf("total_error");
    int field_computation_module_name = query.record().indexOf(
            "computation_module_name");
    int field_observable_type_id = query.record().indexOf("observable_type_id");
    int field_kinematic_id = query.record().indexOf("observable_kinematic_id");

    while (query.next()) {
        int id = query.value(field_id).toInt();
        std::string observable_name =
                query.value(field_observable_name).toString().toStdString();
        double observable_value =
                query.value(field_observable_value).toDouble();
        int computation_module_name =
                query.value(field_computation_module_name).toInt();

        ErrorBar statError(query.value(field_stat_error_ub).toDouble(),
                query.value(field_stat_error_lb).toDouble());
        ErrorBar systError(query.value(field_syst_error_ub).toDouble(),
                query.value(field_syst_error_lb).toDouble());

        ObservableResult observableResult;

        observableResult = ObservableResult(observable_name, observable_value);
        observableResult.setStatError(statError);
        observableResult.setSystError(systError);
        observableResult.setTotalError(
                query.value(field_total_error).toDouble());
        observableResult.setObservableType(
                static_cast<ObservableType::Type>(query.value(
                        field_observable_type_id).toInt()));
        observableResult.setKinematic(
                m_observableKinematicDao.getKinematicById(
                        query.value(field_kinematic_id).toInt()));
        observableResult.setId(id);

        observableResultList.add(observableResult);
    }
}
