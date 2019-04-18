#include "../../../../../include/partons/database/observable/dao/DVCSObservableResultDao.h"

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

DVCSObservableResultDao::DVCSObservableResultDao() :
        BaseObject("DVCSObservableResultDao") {
}

DVCSObservableResultDao::~DVCSObservableResultDao() {
}

int DVCSObservableResultDao::insert(const std::string& observableName,
        const double observableValue, const double statErrorLB,
        const double statErrorUB, const double systErrorLB,
        const double systErrorUB, const double scaleErrorLB,
        const double scaleErrorUB, const PhysicalUnit::Type unit,
        const int kinematicId, const int computationId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    std::vector<std::string> names, values;

    names.push_back("observable_name");
    names.push_back("observable_value");
    names.push_back("stat_error_lb");
    names.push_back("stat_error_ub");
    names.push_back("syst_error_lb");
    names.push_back("syst_error_ub");
    names.push_back("scale_error_lb");
    names.push_back("scale_error_ub");
    names.push_back("unit");
    names.push_back("dvcs_observable_kinematic_id");
    names.push_back("computation_id");

    values.push_back(":observable_name");
    values.push_back(":observable_value");
    values.push_back(":stat_error_lb");
    values.push_back(":stat_error_ub");
    values.push_back(":syst_error_lb");
    values.push_back(":syst_error_ub");
    values.push_back(":scale_error_lb");
    values.push_back(":scale_error_ub");
    values.push_back(":unit");
    values.push_back(":dvcs_observable_kinematic_id");
    values.push_back(":computation_id");

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << "dvcs_observable_result"
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
    query.bindValue(QString(values[8].c_str()), unit);
    query.bindValue(QString(values[9].c_str()), kinematicId);
    query.bindValue(QString(values[10].c_str()), computationId);

    //execute query
    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    //TODO clear?

    return result;
}

List<DVCSObservableResult> DVCSObservableResultDao::getObservableResultListByComputationId(
        const int computationId) const {

    //result
    List<DVCSObservableResult> results;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM dvcs_observable_result WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    //execute
    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //fill
    fillObservableResultList(results, query);

    return results;
}

void DVCSObservableResultDao::fillObservableResultList(
        List<DVCSObservableResult> &observableResultList,
        QSqlQuery& query) const {

    //get indices
    int field_id = query.record().indexOf("dvcs_observable_result_id");
    int field_observable_name = query.record().indexOf("observable_name");
    int field_observable_value = query.record().indexOf("observable_value");
    int field_stat_error_lb = query.record().indexOf("stat_error_lb");
    int field_stat_error_ub = query.record().indexOf("stat_error_ub");
    int field_syst_error_lb = query.record().indexOf("syst_error_lb");
    int field_syst_error_ub = query.record().indexOf("syst_error_ub");
    int field_scale_error_lb = query.record().indexOf("scale_error_lb");
    int field_scale_error_ub = query.record().indexOf("scale_error_ub");
    int field_unit = query.record().indexOf("unit");
    int field_kinematic_id = query.record().indexOf(
            "dvcs_observable_kinematic_id");

    //results
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
        PhysicalUnit::Type unit = static_cast<PhysicalUnit::Type>(query.value(
                field_unit).toInt());

        DVCSObservableResult observableResult;

        observableResult.setIndexId(id);
        observableResult.setValue(PhysicalType<double>(observable_value, unit));
        observableResult.setComputationModuleName(observable_name);
//                observableResult.setStatError(statError);
//                observableResult.setSystError(systError);
//                observableResult.setScaleError(scaleError);
        observableResult.setComputationModuleName(observable_name);
//                observableResult.setKinematic(
//                        m_observableKinematicDao.getKinematicById(
//                                query.value(field_kinematic_id).toInt()));

//add
        observableResultList.add(observableResult);
    }
}

List<DVCSObservableResult> DVCSObservableResultDao::getObservableResultListFromSQLQuery(
        const std::string& sqlQuery) const {

    //result
    List<DVCSObservableResult> results;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(QString(sqlQuery.c_str()));

    //execute query
    if (query.exec()) {
        fillObservableResultList(results, query);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return results;
}

} /* namespace PARTONS */
