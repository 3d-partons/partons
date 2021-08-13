#include "../../../../../include/partons/database/convol_coeff_function/dao/DVMPConvolCoeffFunctionKinematicDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

DVMPConvolCoeffFunctionKinematicDao::DVMPConvolCoeffFunctionKinematicDao() :
        ConvolCoeffFunctionKinematicDao("DVMPConvolCoeffFunctionKinematicDao") {
}

DVMPConvolCoeffFunctionKinematicDao::~DVMPConvolCoeffFunctionKinematicDao() {
}

int DVMPConvolCoeffFunctionKinematicDao::insert(const PhysicalType<double>& xi,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& MuF2, const PhysicalType<double>& MuR2,
        MesonType::Type mesonType, PolarizationType::Type mesonPolarization,
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
            "INSERT INTO dvmp_ccf_kinematic (xi, xi_unit, t, t_unit, Q2, Q2_unit, MuF2, MuF2_unit, MuR2, MuR2_unit, meson_type, meson_polarization, hash_sum) VALUES (:xi, :xi_unit, :t, :t_unit, :Q2, :Q2_unit, :MuF2, :MuF2_unit, :MuR2, :MuR2_unit, :meson_type, :meson_polarization, :hash_sum);");

    query.bindValue(":xi", xi.getValue());
    query.bindValue(":xi_unit", xi.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2", Q2.getValue());
    query.bindValue(":Q2_unit", Q2.getUnit());
    query.bindValue(":MuF2", MuF2.getValue());
    query.bindValue(":MuF2_unit", MuF2.getUnit());
    query.bindValue(":MuR2", MuR2.getValue());
    query.bindValue(":MuR2_unit", MuR2.getUnit());
    query.bindValue(":meson_type", mesonType);
    query.bindValue(":meson_polarization", mesonPolarization);
    query.bindValue(":hash_sum", hashSum.c_str());

    //execute quey
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

int DVMPConvolCoeffFunctionKinematicDao::select(const PhysicalType<double>& xi,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& MuF2, const PhysicalType<double>& MuR2,
        MesonType::Type mesonType,
        PolarizationType::Type mesonPolarization) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT dvmp_ccf_kinematic_id FROM dvmp_ccf_kinematic WHERE xi = :xi AND xi_unit = :xi_unit AND t = :t AND t_unit = :t_unit AND Q2 = :Q2 AND Q2_unit = :Q2_unit AND MuF2 = :MuF2 AND MuF2_unit = :MuF2_unit AND MuR2 = :MuR2 AND MuR2_unit = :MuR2_unit AND meson_type = :meson_type AND meson_polarization = :meson_polarization;");

    query.bindValue(":xi", xi.getValue());
    query.bindValue(":xi_unit", xi.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2", Q2.getValue());
    query.bindValue(":Q2_unit", Q2.getUnit());
    query.bindValue(":MuF2", MuF2.getValue());
    query.bindValue(":MuF2_unit", MuF2.getUnit());
    query.bindValue(":MuR2", MuR2.getValue());
    query.bindValue(":MuR2_unit", MuR2.getUnit());
    query.bindValue(":meson_type", mesonType);
    query.bindValue(":meson_polarization", mesonPolarization);

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

DVMPConvolCoeffFunctionKinematic DVMPConvolCoeffFunctionKinematicDao::getKinematicById(
        const int id) const {

    //result
    DVMPConvolCoeffFunctionKinematic convolCoeffFunctionKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM dvmp_ccf_kinematic_view WHERE dvmp_ccf_kinematic_id = :id;");

    query.bindValue(":id", id);

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //set first
        query.first();

        //fill
        fillKinematicFromQuery(convolCoeffFunctionKinematic, query);
    }

    return convolCoeffFunctionKinematic;
}

List<DVMPConvolCoeffFunctionKinematic> DVMPConvolCoeffFunctionKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    //result
    List<DVMPConvolCoeffFunctionKinematic> kinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT k.* FROM dvmp_ccf_kinematic_view k, ccf_result r WHERE r.channel_id = :channelId AND r.computation_id = :computationId AND r.ccf_kinematic_id = k.dvmp_ccf_kinematic_id;");

    query.bindValue(":computationId", computationId);
    query.bindValue(":channelId", ChannelType::DVMP);

    //execute query
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillKinematicListFromQuery(kinematicList, query);
    }

    return kinematicList;
}

void DVMPConvolCoeffFunctionKinematicDao::fillKinematicListFromQuery(
        List<DVMPConvolCoeffFunctionKinematic>& kinematicList,
        QSqlQuery& query) const {

    //loop over single queries
    while (query.next()) {

        //single result
        DVMPConvolCoeffFunctionKinematic kinematic;

        //get
        fillKinematicFromQuery(kinematic, query);

        //store
        kinematicList.add(kinematic);
    }
}

void DVMPConvolCoeffFunctionKinematicDao::fillKinematicFromQuery(
        DVMPConvolCoeffFunctionKinematic &kinematic, QSqlQuery &query) const {

    //get indices
    int field_id = query.record().indexOf("dvmp_ccf_kinematic_id");
    int field_xi = query.record().indexOf("xi");
    int field_xi_unit = query.record().indexOf("xi_unit");
    int field_t = query.record().indexOf("t");
    int field_t_unit = query.record().indexOf("t_unit");
    int field_Q2 = query.record().indexOf("Q2");
    int field_Q2_unit = query.record().indexOf("Q2_unit");
    int field_MuF2 = query.record().indexOf("MuF2");
    int field_MuF2_unit = query.record().indexOf("MuF2_unit");
    int field_MuR2 = query.record().indexOf("MuR2");
    int field_MuR2_unit = query.record().indexOf("MuR2_unit");
    int field_meson_type = query.record().indexOf("meson_type");
    int field_meson_polarization = query.record().indexOf("meson_polarization");
    int field_hash_sum = query.record().indexOf("hash_sum");

    //get values
    int id = query.value(field_id).toInt();
    double xi = query.value(field_xi).toDouble();
    PhysicalUnit::Type xi_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_xi_unit).toInt());
    double t = query.value(field_t).toDouble();
    PhysicalUnit::Type t_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_t_unit).toInt());
    double Q2 = query.value(field_Q2).toDouble();
    PhysicalUnit::Type Q2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_Q2_unit).toInt());
    double MuF2 = query.value(field_MuF2).toDouble();
    PhysicalUnit::Type MuF2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuF2_unit).toInt());
    double MuR2 = query.value(field_MuR2).toDouble();
    PhysicalUnit::Type MuR2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuR2_unit).toInt());
    MesonType::Type meson_type = static_cast<MesonType::Type>(query.value(
            field_meson_type).toInt());
    PolarizationType::Type meson_polarization =
            static_cast<PolarizationType::Type>(query.value(
                    field_meson_polarization).toInt());

    kinematic = DVMPConvolCoeffFunctionKinematic(
            PhysicalType<double>(xi, xi_unit), PhysicalType<double>(t, t_unit),
            PhysicalType<double>(Q2, Q2_unit),
            PhysicalType<double>(MuF2, MuF2_unit),
            PhysicalType<double>(MuR2, MuR2_unit), meson_type,
            meson_polarization);
    kinematic.setIndexId(id);

    //check hash sum
    if (kinematic.getHashSum()
            != query.value(field_hash_sum).toString().toStdString()) {
        warn(__func__,
                "Retrieved kinematics has different hash sum than original one from database");
    }
}

int DVMPConvolCoeffFunctionKinematicDao::getKinematicIdByHashSum(
        const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT dvmp_ccf_kinematic_id FROM dvmp_ccf_kinematic WHERE hash_sum = :hashSum;");

    query.bindValue(":hashSum", QString(hashSum.c_str()));

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //set first
        query.first();

        //get
        result = query.value(0).toInt();
    }

    return result;
}

} /* namespace PARTONS */
