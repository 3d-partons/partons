#include "../../../../../include/partons/database/observable/dao/DVCSObservableKinematicDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

DVCSObservableKinematicDao::DVCSObservableKinematicDao() :
        ObservableKinematicDao("DVCSObservableKinematicDao") {
}

DVCSObservableKinematicDao::~DVCSObservableKinematicDao() {
}

int DVCSObservableKinematicDao::insert(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi,
        const std::string& hashSum) const {

    //check if already in db
    int result = getKinematicIdByHashSum(hashSum);

    if (result != -1) {

        warn(__func__, "Kinematics already in database, insertion skipped");
        return result;
    }

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO dvcs_observable_kinematic (xB, xB_unit, t, t_unit, Q2, Q2_unit, E, E_unit, phi, phi_unit, hash_sum) VALUES (:xB, :xB_unit, :t, :t_unit, :Q2, :Q2_unit, :E, :E_unit, :phi, :phi_unit, :hash_sum);");

    query.bindValue(":xB", xB.getValue());
    query.bindValue(":xB_unit", xB.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2", Q2.getValue());
    query.bindValue(":Q2_unit", Q2.getUnit());
    query.bindValue(":E", E.getValue());
    query.bindValue(":E_unit", E.getUnit());
    query.bindValue(":phi", phi.getValue());
    query.bindValue(":phi_unit", phi.getUnit());
    query.bindValue(":hash_sum", hashSum.c_str());

    //execute query
    if (query.exec()) {

        //get result
        result = query.lastInsertId().toInt();
    } else {

        //thrown if error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

int DVCSObservableKinematicDao::select(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT dvcs_observable_kinematic_id FROM dvcs_observable_kinematic WHERE xB = :xB AND xB_unit = :xB_unit AND t = :t AND t_unit = :t_unit AND Q2 = :Q2 AND Q2_unit = :Q2_unit AND E = :E AND E_unit = :E_unit AND phi = :phi AND phi_unit = :phi_unit;");

    query.bindValue(":xB", xB.getValue());
    query.bindValue(":xB_unit", xB.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2", Q2.getValue());
    query.bindValue(":Q2_unit", Q2.getUnit());
    query.bindValue(":E", E.getValue());
    query.bindValue(":E_unit", E.getUnit());
    query.bindValue(":phi", phi.getValue());
    query.bindValue(":phi_unit", phi.getUnit());

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //set first
        query.first();

        //get result
        result = query.value(0).toInt();
    }

    return result;
}

DVCSObservableKinematic DVCSObservableKinematicDao::getKinematicById(
        const int id) const {

    //result
    DVCSObservableKinematic observableKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM dvcs_observable_kinematic_view WHERE dvcs_observable_kinematic_id = :kinematicId;");

    query.bindValue(":kinematicId", id);

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //set first
        query.first();

        //fill
        fillKinematicFromQuery(observableKinematic, query);
    }

    return observableKinematic;
}

List<DVCSObservableKinematic> DVCSObservableKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    //result
    List<DVCSObservableKinematic> observableKinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT k.* FROM dvcs_observable_kinematic_view k, dvcs_observable_result r WHERE r.computation_id = :computationId AND r.dvcs_observable_kinematic_id = k.dvcs_observable_kinematic_id");

    query.bindValue(":computationId", computationId);

    //execute query
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillObservableKinematicListFromQuery(observableKinematicList, query);
    }

    return observableKinematicList;
}

void DVCSObservableKinematicDao::fillObservableKinematicListFromQuery(
        List<DVCSObservableKinematic>& observableKinematicList,
        QSqlQuery& query) const {

    //loop over single queries
    while (query.next()) {

        //single result
        DVCSObservableKinematic observableKinematic;

        //get
        fillKinematicFromQuery(observableKinematic, query);

        //store
        observableKinematicList.add(observableKinematic);
    }
}

void DVCSObservableKinematicDao::fillKinematicFromQuery(
        DVCSObservableKinematic &observableKinematic, QSqlQuery &query) const {

    //get indices
    int field_id = query.record().indexOf("dvcs_observable_kinematic_id");
    int field_xB = query.record().indexOf("xB");
    int field_xB_unit = query.record().indexOf("xB_unit");
    int field_t = query.record().indexOf("t");
    int field_t_unit = query.record().indexOf("t_unit");
    int field_Q2 = query.record().indexOf("Q2");
    int field_Q2_unit = query.record().indexOf("Q2_unit");
    int field_E = query.record().indexOf("E");
    int field_E_unit = query.record().indexOf("E_unit");
    int field_phi = query.record().indexOf("phi");
    int field_phi_unit = query.record().indexOf("phi_unit");
    int field_hash_sum = query.record().indexOf("hash_sum");

    //get values
    int id = query.value(field_id).toInt();
    double xB = query.value(field_xB).toDouble();
    PhysicalUnit::Type xB_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_xB_unit).toInt());
    double t = query.value(field_t).toDouble();
    PhysicalUnit::Type t_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_t_unit).toInt());
    double Q2 = query.value(field_Q2).toDouble();
    PhysicalUnit::Type Q2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_Q2_unit).toInt());
    double E = query.value(field_E).toDouble();
    PhysicalUnit::Type E_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_E_unit).toInt());
    double phi = query.value(field_phi).toDouble();
    PhysicalUnit::Type phi_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_phi_unit).toInt());

    //set
    observableKinematic = DVCSObservableKinematic(
            PhysicalType<double>(xB, xB_unit), PhysicalType<double>(t, t_unit),
            PhysicalType<double>(Q2, Q2_unit), PhysicalType<double>(E, E_unit),
            PhysicalType<double>(phi, phi_unit));
    observableKinematic.setIndexId(id);

    //check hash sum
    if (observableKinematic.getHashSum()
            != query.value(field_hash_sum).toString().toStdString()) {
        warn(__func__,
                "Retrieved kinematics has different hash sum than original one from database");
    }
}

int DVCSObservableKinematicDao::getKinematicIdByHashSum(
        const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT dvcs_observable_kinematic_id FROM dvcs_observable_kinematic WHERE hash_sum = :hashSum;");

    query.bindValue(":hashSum", QString(hashSum.c_str()));

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //set first
        query.first();

        //get result
        result = query.value(0).toInt();
    }

    return result;
}

} /* namespace PARTONS */
