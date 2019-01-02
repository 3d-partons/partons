#include "../../../../../include/partons/database/observable/dao/ObservableResultDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <vector>

#include "../../../../../include/partons/beans/observable/ObservableKinematic.h"
#include "../../../../../include/partons/beans/observable/ObservableType.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/math/ErrorBar.h"

namespace PARTONS {

ObservableResultDao::ObservableResultDao() :
        BaseObject("ObservableResultDao") {
}

ObservableResultDao::~ObservableResultDao() {
}

int ObservableResultDao::insert(const std::string& observableName,
        double observableValue, double statErrorLB, double statErrorUB,
        double systErrorLB, double systErrorUB, double scaleErrorLB,
        double scaleErrorUB, const std::string &computationModuleName,
        int observableTypeId, int kinematicId, int computationId) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    std::vector<std::string> names, values;

    names.push_back(Database::COLUMN_NAME_OBSERVABLE_NAME);
    names.push_back(Database::COLUMN_NAME_OBSERVABLE_VALUE);
    names.push_back("stat_error_lb");
    names.push_back("stat_error_ub");
    names.push_back("syst_error_lb");
    names.push_back("syst_error_ub");
    names.push_back("scale_error_lb");
    names.push_back("scale_error_ub");
    names.push_back(Database::COLUMN_NAME_COMPUTATION_MODULE_NAME);
    names.push_back("observable_type_id");
    names.push_back(Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID);
    names.push_back(Database::COLUMN_NAME_COMPUTATION_ID);

    values.push_back(":observable_name");
    values.push_back(":observable_value");
    values.push_back(":stat_error_lb");
    values.push_back(":stat_error_ub");
    values.push_back(":syst_error_lb");
    values.push_back(":syst_error_ub");
    values.push_back(":scale_error_lb");
    values.push_back(":scale_error_ub");
    values.push_back(":computationModuleName");
    values.push_back(":observableTypeId");
    values.push_back(":observable_kinematic_id");
    values.push_back(":computation_id");

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_OBSERVABLE_RESULT
            << Database::getPreFormatedColumnNamesFromVector(names)
            << Database::getPreFormatedColumnValuesFromVector(values);

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(QString(values[0].c_str()),
            QString(observableName.c_str()));
    query.bindValue(QString(values[1].c_str()), observableValue);
    query.bindValue(QString(values[2].c_str()), statErrorLB);
    query.bindValue(QString(values[3].c_str()), statErrorUB);
    query.bindValue(QString(values[4].c_str()), systErrorLB);
    query.bindValue(QString(values[5].c_str()), systErrorUB);
    query.bindValue(QString(values[6].c_str()), scaleErrorLB);
    query.bindValue(QString(values[7].c_str()), scaleErrorUB);
    query.bindValue(QString(values[8].c_str()),
            QString(computationModuleName.c_str()));
    query.bindValue(QString(values[9].c_str()), observableTypeId);
    query.bindValue(QString(values[10].c_str()), kinematicId);
    query.bindValue(QString(values[11].c_str()), computationId);

    names.clear();
    values.clear();

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

List<DVCSObservableResult> ObservableResultDao::getObservableResultListByComputationId(
        const int computationId) const {

    List<DVCSObservableResult> results;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM observable_result WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    fillObservableResultList(results, query);

    query.clear();

    return results;
}

List<DVCSObservableResult> ObservableResultDao::getObservableResultListFromSQLQuery(
        const std::string& sqlQuery) const {
    List<DVCSObservableResult> results;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(QString(sqlQuery.c_str()));

    if (query.exec()) {
        fillObservableResultList(results, query);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return results;
}

//TODO retrieve Computation object
void ObservableResultDao::fillObservableResultList(
        List<DVCSObservableResult> &observableResultList, QSqlQuery& query) const {

    int field_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_OBSERVABLE_RESULT_ID.c_str()));
    int field_observable_name = query.record().indexOf("observable_name");
    int field_observable_value = query.record().indexOf("observable_value");
    int field_stat_error_lb = query.record().indexOf("stat_error_lb");
    int field_stat_error_ub = query.record().indexOf("stat_error_ub");
    int field_syst_error_lb = query.record().indexOf("syst_error_lb");
    int field_syst_error_ub = query.record().indexOf("syst_error_ub");
    int field_scale_error_lb = query.record().indexOf("scale_error_lb");
    int field_scale_error_ub = query.record().indexOf("scale_error_ub");
    int field_computation_module_name = query.record().indexOf(
            "computation_module_name");
    int field_observable_type_id = query.record().indexOf("observable_type_id");
    int field_kinematic_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID.c_str()));

    while (query.next()) {

        int id = query.value(field_id).toInt();
        std::string observable_name =
                query.value(field_observable_name).toString().toStdString();
        double observable_value =
                query.value(field_observable_value).toDouble();
        ErrorBar statError(query.value(field_stat_error_lb).toDouble(),
                query.value(field_stat_error_ub).toDouble());
        ErrorBar systError(query.value(field_syst_error_lb).toDouble(),
                query.value(field_syst_error_ub).toDouble());
        ErrorBar scaleError(query.value(field_scale_error_lb).toDouble(),
                query.value(field_scale_error_ub).toDouble());
        std::string computation_module_name = query.value(
                field_computation_module_name).toString().toStdString();
        ObservableType::Type observable_type =
                static_cast<ObservableType::Type>(query.value(
                        field_observable_type_id).toInt());

        //TODO create ResultInfo, Computation, ...

        DVCSObservableResult observableResult;

//        observableResult.setIndexId(id);
//        observableResult = DVCSObservableResult(observable_name, observable_value);
//        observableResult.setStatError(statError);
//        observableResult.setSystError(systError);
//        observableResult.setScaleError(scaleError);
//        observableResult.setComputationModuleName(computation_module_name);
//        observableResult.setKinematic(
//                m_observableKinematicDao.getKinematicById(
//                        query.value(field_kinematic_id).toInt()));
//        observableResult.setObservableType(observable_type);

        observableResultList.add(observableResult);
    }
}

} /* namespace PARTONS */
