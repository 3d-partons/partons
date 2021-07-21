#include "../../../../../include/partons/database/observable/dao/DVMPObservableKinematicDao.h"

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

DVMPObservableKinematicDao::DVMPObservableKinematicDao() :
        ObservableKinematicDao("DVMPObservableKinematicDao") {
}

DVMPObservableKinematicDao::~DVMPObservableKinematicDao() {
}

int DVMPObservableKinematicDao::insert(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi, MesonType::Type mesonType,
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
            "INSERT INTO dvmp_observable_kinematic (xB, xB_unit, t, t_unit, Q2, Q2_unit, E, E_unit, phi, phi_unit, meson_type_id, hash_sum) VALUES (:xB, :xB_unit, :t, :t_unit, :Q2, :Q2_unit, :E, :E_unit, :phi, :phi_unit, :meson_type_id, :hash_sum);");

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
    query.bindValue(":meson_type_id", mesonType);
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

int DVMPObservableKinematicDao::select(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi,MesonType::Type mesonType) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT dvmp_observable_kinematic_id FROM dvmp_observable_kinematic WHERE xB = :xB AND xB_unit = :xB_unit AND t = :t AND t_unit = :t_unit AND Q2 = :Q2 AND Q2_unit = :Q2_unit AND E = :E AND E_unit = :E_unit AND phi = :phi AND phi_unit = :phi_unit AND meson_type_id = :meson_type_id;");

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
    query.bindValue(":meson_type_id", mesonType);

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

DVMPObservableKinematic DVMPObservableKinematicDao::getKinematicById(
        const int id) const {

    //result
    DVMPObservableKinematic observableKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM dvmp_observable_kinematic_view WHERE dvmp_observable_kinematic_id = :kinematicId;");

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

List<DVMPObservableKinematic> DVMPObservableKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    //result
    List<DVMPObservableKinematic> observableKinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT k.* FROM dvmp_observable_kinematic_view k, dvmp_observable_result r WHERE r.computation_id = :computationId AND r.dvmp_observable_kinematic_id = k.dvmp_observable_kinematic_id");

    query.bindValue(":computationId", computationId);

    //execute query
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillObservableKinematicListFromQuery(observableKinematicList, query);
    }

    return observableKinematicList;
}

void DVMPObservableKinematicDao::fillObservableKinematicListFromQuery(
        List<DVMPObservableKinematic>& observableKinematicList,
        QSqlQuery& query) const {

    //loop over single queries
    while (query.next()) {

        //single result
        DVMPObservableKinematic observableKinematic;

        //get
        fillKinematicFromQuery(observableKinematic, query);

        //store
        observableKinematicList.add(observableKinematic);
    }
}

void DVMPObservableKinematicDao::fillKinematicFromQuery(
        DVMPObservableKinematic &observableKinematic, QSqlQuery &query) const {

    //get indices
    int field_id = query.record().indexOf("dvmp_observable_kinematic_id");
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
    int field_meson_type_id = query.record().indexOf("meson_type_id");
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
    MesonType::Type meson_type = static_cast<MesonType::Type>(query.value(
            field_meson_type_id).toInt());

    //set
    observableKinematic = DVMPObservableKinematic(
            PhysicalType<double>(xB, xB_unit), PhysicalType<double>(t, t_unit),
            PhysicalType<double>(Q2, Q2_unit), PhysicalType<double>(E, E_unit),
            PhysicalType<double>(phi, phi_unit), meson_type);
    observableKinematic.setIndexId(id);

    //check hash sum
    if (observableKinematic.getHashSum()
            != query.value(field_hash_sum).toString().toStdString()) {
        warn(__func__,
                "Retrieved kinematics has different hash sum than original one from database");
    }
}

int DVMPObservableKinematicDao::getKinematicIdByHashSum(
        const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT dvmp_observable_kinematic_id FROM dvmp_observable_kinematic WHERE hash_sum = :hashSum;");

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
