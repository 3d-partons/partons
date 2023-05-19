#include "../../../../../include/partons/database/convol_coeff_function/dao/DDVCSConvolCoeffFunctionKinematicDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

DDVCSConvolCoeffFunctionKinematicDao::DDVCSConvolCoeffFunctionKinematicDao() :
        ConvolCoeffFunctionKinematicDao("DDVCSConvolCoeffFunctionKinematicDao") {
}

DDVCSConvolCoeffFunctionKinematicDao::~DDVCSConvolCoeffFunctionKinematicDao() {
}

int DDVCSConvolCoeffFunctionKinematicDao::insert(const PhysicalType<double> &xi,
        const PhysicalType<double> &t, const PhysicalType<double> &Q2,
        const PhysicalType<double> &Q2Prim,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2,
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
            "INSERT INTO ddvcs_ccf_kinematic (xi, xi_unit, t, t_unit, Q2, Q2_unit, Q2Prim, Q2Prim_unit, MuF2, MuF2_unit, MuR2, MuR2_unit, hash_sum) VALUES (:xi, :xi_unit, :t, :t_unit, :Q2, :Q2_unit, :Q2Prim, :Q2Prim_unit, :MuF2, :MuF2_unit, :MuR2, :MuR2_unit, :hash_sum);");

    query.bindValue(":xi", xi.getValue());
    query.bindValue(":xi_unit", xi.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2", Q2.getValue());
    query.bindValue(":Q2_unit", Q2.getUnit());
    query.bindValue(":Q2Prim", Q2Prim.getValue());
    query.bindValue(":Q2Prim_unit", Q2Prim.getUnit());
    query.bindValue(":MuF2", MuF2.getValue());
    query.bindValue(":MuF2_unit", MuF2.getUnit());
    query.bindValue(":MuR2", MuR2.getValue());
    query.bindValue(":MuR2_unit", MuR2.getUnit());
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

int DDVCSConvolCoeffFunctionKinematicDao::select(const PhysicalType<double> &xi,
        const PhysicalType<double> &t, const PhysicalType<double> &Q2,
        const PhysicalType<double> &Q2Prim,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT ddvcs_ccf_kinematic_id FROM ddvcs_ccf_kinematic WHERE xi = :xi AND xi_unit = :xi_unit AND t = :t AND t_unit = :t_unit AND Q2 = :Q2 AND Q2_unit = :Q2_unit AND Q2Prim = :Q2Prim AND Q2Prim_unit = :Q2Prim_unit AND MuF2 = :MuF2 AND MuF2_unit = :MuF2_unit AND MuR2 = :MuR2 AND MuR2_unit = :MuR2_unit;");

    query.bindValue(":xi", xi.getValue());
    query.bindValue(":xi_unit", xi.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":Q2", Q2.getValue());
    query.bindValue(":Q2_unit", Q2.getUnit());
    query.bindValue(":Q2Prim", Q2Prim.getValue());
    query.bindValue(":Q2Prim_unit", Q2Prim.getUnit());
    query.bindValue(":MuF2", MuF2.getValue());
    query.bindValue(":MuF2_unit", MuF2.getUnit());
    query.bindValue(":MuR2", MuR2.getValue());
    query.bindValue(":MuR2_unit", MuR2.getUnit());

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

DDVCSConvolCoeffFunctionKinematic DDVCSConvolCoeffFunctionKinematicDao::getKinematicById(
        const int id) const {

    //result
    DDVCSConvolCoeffFunctionKinematic convolCoeffFunctionKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM ddvcs_ccf_kinematic_view WHERE ddvcs_ccf_kinematic_id = :id;");

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

List<DDVCSConvolCoeffFunctionKinematic> DDVCSConvolCoeffFunctionKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    //result
    List<DDVCSConvolCoeffFunctionKinematic> kinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT k.* FROM ddvcs_ccf_kinematic_view k, ccf_result r WHERE r.channel_id = :channelId AND r.computation_id = :computationId AND r.ccf_kinematic_id = k.ddvcs_ccf_kinematic_id;");

    query.bindValue(":computationId", computationId);
    query.bindValue(":channelId", ChannelType::DDVCS);

    //execute query
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillKinematicListFromQuery(kinematicList, query);
    }

    return kinematicList;
}

void DDVCSConvolCoeffFunctionKinematicDao::fillKinematicListFromQuery(
        List<DDVCSConvolCoeffFunctionKinematic>& kinematicList,
        QSqlQuery& query) const {

    //loop over single queries
    while (query.next()) {

        //single result
        DDVCSConvolCoeffFunctionKinematic kinematic;

        //get
        fillKinematicFromQuery(kinematic, query);

        //store
        kinematicList.add(kinematic);
    }
}

void DDVCSConvolCoeffFunctionKinematicDao::fillKinematicFromQuery(
        DDVCSConvolCoeffFunctionKinematic &kinematic, QSqlQuery &query) const {

    //get indices
    int field_id = query.record().indexOf("ddvcs_ccf_kinematic_id");
    int field_xi = query.record().indexOf("xi");
    int field_xi_unit = query.record().indexOf("xi_unit");
    int field_t = query.record().indexOf("t");
    int field_t_unit = query.record().indexOf("t_unit");
    int field_Q2 = query.record().indexOf("Q2");
    int field_Q2_unit = query.record().indexOf("Q2_unit");
    int field_Q2Prim = query.record().indexOf("Q2Prim");
    int field_Q2Prim_unit = query.record().indexOf("Q2Prim_unit");
    int field_MuF2 = query.record().indexOf("MuF2");
    int field_MuF2_unit = query.record().indexOf("MuF2_unit");
    int field_MuR2 = query.record().indexOf("MuR2");
    int field_MuR2_unit = query.record().indexOf("MuR2_unit");
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
    double Q2Prim = query.value(field_Q2Prim).toDouble();
    PhysicalUnit::Type Q2Prim_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_Q2Prim_unit).toInt());
    double MuF2 = query.value(field_MuF2).toDouble();
    PhysicalUnit::Type MuF2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuF2_unit).toInt());
    double MuR2 = query.value(field_MuR2).toDouble();
    PhysicalUnit::Type MuR2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuR2_unit).toInt());

    kinematic = DDVCSConvolCoeffFunctionKinematic(
            PhysicalType<double>(xi, xi_unit), PhysicalType<double>(t, t_unit),
            PhysicalType<double>(Q2, Q2_unit), PhysicalType<double>(Q2Prim, Q2Prim_unit),
            PhysicalType<double>(MuF2, MuF2_unit),
            PhysicalType<double>(MuR2, MuR2_unit));
    kinematic.setIndexId(id);

    //check hash sum
    if (kinematic.getHashSum()
            != query.value(field_hash_sum).toString().toStdString()) {
        warn(__func__,
                "Retrieved kinematics has different hash sum than original one from database");
    }
}

int DDVCSConvolCoeffFunctionKinematicDao::getKinematicIdByHashSum(
        const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT ddvcs_ccf_kinematic_id FROM ddvcs_ccf_kinematic WHERE hash_sum = :hashSum;");

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
