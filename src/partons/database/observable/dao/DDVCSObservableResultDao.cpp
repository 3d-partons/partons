#include "../../../../../include/partons/database/observable/dao/DDVCSObservableResultDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <vector>

#include "../../../../../include/partons/beans/observable/ObservableResult.h"
#include "../../../../../include/partons/beans/Result.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/math/ErrorBar.h"
#include "../../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

DDVCSObservableResultDao::DDVCSObservableResultDao() :
        BaseObject("DDVCSObservableResultDao") {
}

DDVCSObservableResultDao::~DDVCSObservableResultDao() {
}

int DDVCSObservableResultDao::insert(const std::string& observableName,
        const double value, const double statErrorLB, const double statErrorUB,
        const double systErrorLB, const double systErrorUB,
        const double scaleErrorLB, const double scaleErrorUB,
        const PhysicalUnit::Type valueUnit, const int kinematicId,
        const int computationId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    std::vector<std::string> names, values;

    names.push_back("observable_name");
    names.push_back("value");
    names.push_back("stat_error_lb");
    names.push_back("stat_error_ub");
    names.push_back("syst_error_lb");
    names.push_back("syst_error_ub");
    names.push_back("scale_error_lb");
    names.push_back("scale_error_ub");
    names.push_back("value_unit");
    names.push_back("ddvcs_observable_kinematic_id");
    names.push_back("computation_id");

    values.push_back(":observable_name");
    values.push_back(":value");
    values.push_back(":stat_error_lb");
    values.push_back(":stat_error_ub");
    values.push_back(":syst_error_lb");
    values.push_back(":syst_error_ub");
    values.push_back(":scale_error_lb");
    values.push_back(":scale_error_ub");
    values.push_back(":value_unit");
    values.push_back(":ddvcs_observable_kinematic_id");
    values.push_back(":computation_id");

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << "ddvcs_observable_result"
            << Database::getPreFormatedColumnNamesFromVector(names)
            << Database::getPreFormatedColumnValuesFromVector(values);

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(QString(values[0].c_str()),
            QString(observableName.c_str()));
    query.bindValue(QString(values[1].c_str()), value);
    query.bindValue(QString(values[2].c_str()), statErrorLB);
    query.bindValue(QString(values[3].c_str()), statErrorUB);
    query.bindValue(QString(values[4].c_str()), systErrorLB);
    query.bindValue(QString(values[5].c_str()), systErrorUB);
    query.bindValue(QString(values[6].c_str()), scaleErrorLB);
    query.bindValue(QString(values[7].c_str()), scaleErrorUB);
    query.bindValue(QString(values[8].c_str()), valueUnit);
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

    return result;
}

List<DDVCSObservableResult> DDVCSObservableResultDao::getObservableResultListByComputationId(
        const int computationId) const {

    //result
    List<DDVCSObservableResult> results;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM ddvcs_observable_result WHERE computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    //execute
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillObservableResultList(results, query);
    }

    return results;
}

void DDVCSObservableResultDao::fillObservableResultList(
        List<DDVCSObservableResult> &observableResultList,
        QSqlQuery& query) const {

    //get indices
    int field_id = query.record().indexOf("ddvcs_observable_result_id");
    int field_observable_name = query.record().indexOf("observable_name");
    int field_value = query.record().indexOf("value");
    int field_stat_error_lb = query.record().indexOf("stat_error_lb");
    int field_stat_error_ub = query.record().indexOf("stat_error_ub");
    int field_syst_error_lb = query.record().indexOf("syst_error_lb");
    int field_syst_error_ub = query.record().indexOf("syst_error_ub");
    int field_scale_error_lb = query.record().indexOf("scale_error_lb");
    int field_scale_error_ub = query.record().indexOf("scale_error_ub");
    int field_value_unit = query.record().indexOf("value_unit");
    int field_kinematic_id = query.record().indexOf(
            "ddvcs_observable_kinematic_id");

    //results
    while (query.next()) {

        int id = query.value(field_id).toInt();
        std::string observable_name =
                query.value(field_observable_name).toString().toStdString();
        double value = query.value(field_value).toDouble();
        ErrorBar<double> statError(query.value(field_stat_error_lb).toDouble(),
                query.value(field_stat_error_ub).toDouble());
        ErrorBar<double> systError(query.value(field_syst_error_lb).toDouble(),
                query.value(field_syst_error_ub).toDouble());
        ErrorBar<double> scaleError(
                query.value(field_scale_error_lb).toDouble(),
                query.value(field_scale_error_ub).toDouble());
        PhysicalUnit::Type value_unit =
                static_cast<PhysicalUnit::Type>(query.value(field_value_unit).toInt());

        DDVCSObservableResult observableResult;

        observableResult.setIndexId(id);
        observableResult.setValue(PhysicalType<double>(value, value_unit));
        observableResult.setErrStat(statError);
        observableResult.setErrSys(systError);
        observableResult.setErrScale(scaleError);
        observableResult.setComputationModuleName(observable_name);
        observableResult.setKinematic(
                m_ddvcsObservableKinematicDao.getKinematicById(
                        query.value(field_kinematic_id).toInt()));

        //add
        observableResultList.add(observableResult);
    }
}

List<DDVCSObservableResult> DDVCSObservableResultDao::getObservableResultListFromSQLQuery(
        const std::string& sqlQuery) const {

    //result
    List<DDVCSObservableResult> results;

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
