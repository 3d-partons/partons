#include "../../../../../include/partons/database/observable/dao/TCSObservableKinematicDao.h"

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

TCSObservableKinematicDao::TCSObservableKinematicDao() :
        ObservableKinematicDao("TCSObservableKinematicDao") {
}

TCSObservableKinematicDao::~TCSObservableKinematicDao() {
}

int TCSObservableKinematicDao::insert(const PhysicalType<double>& t,
        const PhysicalType<double>& Q2Prim, const PhysicalType<double>& E,
        const PhysicalType<double>& phi, const PhysicalType<double>& theta,
        const PhysicalType<double>& MLepton, const std::string& hashSum) const {

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
            "INSERT INTO tcs_observable_kinematic (t, t_unit, Q2Prim, Q2Prim_unit, E, E_unit, phi, phi_unit, theta, theta_unit, MLepton, MLepton_unit, hash_sum) VALUES (:t, :t_unit, :Q2Prim, :Q2Prim_unit, :E, :E_unit, :phi, :phi_unit, :theta, :theta_unit, :MLepton, :MLepton_unit, :hash_sum);");

    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2Prim", Q2Prim.getValue());
    query.bindValue(":Q2Prim_unit", Q2Prim.getUnit());
    query.bindValue(":E", E.getValue());
    query.bindValue(":E_unit", E.getUnit());
    query.bindValue(":phi", phi.getValue());
    query.bindValue(":phi_unit", phi.getUnit());
    query.bindValue(":theta", theta.getValue());
    query.bindValue(":theta_unit", theta.getUnit());
    query.bindValue(":MLepton", MLepton.getValue());
    query.bindValue(":MLepton_unit", MLepton.getUnit());
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

int TCSObservableKinematicDao::select(const PhysicalType<double>& t,
        const PhysicalType<double>& Q2Prim, const PhysicalType<double>& E,
        const PhysicalType<double>& phi, const PhysicalType<double>& theta,
        const PhysicalType<double>& MLepton) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT tcs_observable_kinematic_id FROM tcs_observable_kinematic WHERE  t = :t AND t_unit = :t_unit AND Q2Prim = :Q2Prim AND Q2Prim_unit = :Q2Prim_unit AND E = :E AND E_unit = :E_unit AND phi = :phi AND phi_unit = :phi_unit AND theta = :theta AND theta_unit = :theta_unit AND MLepton = :MLepton AND MLepton_unit = :MLepton_unit;");

    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2Prim", Q2Prim.getValue());
    query.bindValue(":Q2Prim_unit", Q2Prim.getUnit());
    query.bindValue(":E", E.getValue());
    query.bindValue(":E_unit", E.getUnit());
    query.bindValue(":phi", phi.getValue());
    query.bindValue(":phi_unit", phi.getUnit());
    query.bindValue(":theta", theta.getValue());
    query.bindValue(":theta_unit", theta.getUnit());
    query.bindValue(":MLepton", theta.getValue());
    query.bindValue(":MLepton_unit", theta.getUnit());

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query) != 0) {

        //get result
        result = query.value(0).toInt();
    }

    return result;
}

TCSObservableKinematic TCSObservableKinematicDao::getKinematicById(
        const int id) const {

    //result
    TCSObservableKinematic observableKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM tcs_observable_kinematic_view WHERE tcs_observable_kinematic_id = :kinematicId;");

    query.bindValue(":kinematicId", id);

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query) != 0) {

        //fill
        fillKinematicFromQuery(observableKinematic, query);
    }

    return observableKinematic;
}

List<TCSObservableKinematic> TCSObservableKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    //result
    List<TCSObservableKinematic> observableKinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT k.* FROM tcs_observable_kinematic_view k, tcs_observable_result r WHERE r.computation_id = :computationId AND r.tcs_observable_kinematic_id = k.tcs_observable_kinematic_id");

    query.bindValue(":computationId", computationId);

    //execute query
    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //fill
    fillObservableKinematicListFromQuery(observableKinematicList, query);

    return observableKinematicList;
}

void TCSObservableKinematicDao::fillObservableKinematicListFromQuery(
        List<TCSObservableKinematic>& observableKinematicList,
        QSqlQuery& query) const {

    //loop over single queries
    while (query.next()) {

        //single result
        TCSObservableKinematic observableKinematic;

        //get
        fillKinematicFromQuery(observableKinematic, query);

        //store
        observableKinematicList.add(observableKinematic);
    }
}

void TCSObservableKinematicDao::fillKinematicFromQuery(
        TCSObservableKinematic &observableKinematic, QSqlQuery &query) const {

    //get indices
    int field_id = query.record().indexOf("tcs_observable_kinematic_id");
    int field_t = query.record().indexOf("t");
    int field_t_unit = query.record().indexOf("t_unit");
    int field_Q2Prim = query.record().indexOf("Q2Prim");
    int field_Q2Prim_unit = query.record().indexOf("Q2Prim_unit");
    int field_E = query.record().indexOf("E");
    int field_E_unit = query.record().indexOf("E_unit");
    int field_phi = query.record().indexOf("phi");
    int field_phi_unit = query.record().indexOf("phi_unit");
    int field_theta = query.record().indexOf("theta");
    int field_theta_unit = query.record().indexOf("theta_unit");
    int field_MLepton = query.record().indexOf("MLepton");
    int field_MLepton_unit = query.record().indexOf("MLepton_unit");
    int field_hash_sum = query.record().indexOf("hash_sum");

    //get values
    int id = query.value(field_id).toInt();
    double t = query.value(field_t).toDouble();
    PhysicalUnit::Type t_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_t_unit).toInt());
    double Q2Prim = query.value(field_Q2Prim).toDouble();
    PhysicalUnit::Type Q2Prim_unit =
            static_cast<PhysicalUnit::Type>(query.value(field_Q2Prim_unit).toInt());
    double E = query.value(field_E).toDouble();
    PhysicalUnit::Type E_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_E_unit).toInt());
    double phi = query.value(field_phi).toDouble();
    PhysicalUnit::Type phi_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_phi_unit).toInt());
    double theta = query.value(field_theta).toDouble();
    PhysicalUnit::Type theta_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_theta_unit).toInt());
    double MLepton = query.value(field_MLepton).toDouble();
    PhysicalUnit::Type MLepton_unit =
            static_cast<PhysicalUnit::Type>(query.value(field_MLepton_unit).toInt());

    //set
    observableKinematic = TCSObservableKinematic(
            PhysicalType<double>(t, t_unit),
            PhysicalType<double>(Q2Prim, Q2Prim_unit),
            PhysicalType<double>(E, E_unit),
            PhysicalType<double>(phi, phi_unit),
            PhysicalType<double>(theta, theta_unit),
            PhysicalType<double>(MLepton, MLepton_unit));
    observableKinematic.setIndexId(id);

    //check hash sum
    if (observableKinematic.getHashSum()
            != query.value(field_hash_sum).toString().toStdString()) {
        warn(__func__,
                "Retrieved kinematics has different hash sum than original one from database");
    }
}

int TCSObservableKinematicDao::getKinematicIdByHashSum(
        const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT tcs_observable_kinematic_id FROM tcs_observable_kinematic WHERE hash_sum = :hashSum;");

    query.bindValue(":hashSum", QString(hashSum.c_str()));

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query) != 0) {
        result = query.value(0).toInt();
    }

    return result;
}

} /* namespace PARTONS */
