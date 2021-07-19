#include "../../../../../include/partons/database/collinear_distribution/dao/CollinearDistributionKinematicDao.h"

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

CollinearDistributionKinematicDao::CollinearDistributionKinematicDao() :
        BaseObject("CollinearDistributionKinematicDao") {
}

CollinearDistributionKinematicDao::~CollinearDistributionKinematicDao() {
}

int CollinearDistributionKinematicDao::insert(const PhysicalType<double>& x,
        const PhysicalType<double>& MuF2, const PhysicalType<double>& MuR2,
        const std::string& hashSum) const {

    //check if already in db
    int result = getKinematicIdByHashSum(hashSum);

    if (result != -1) {

        warn(__func__, "Kinematics already in database, insertion skipped");
        return result;
    }

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //form query
    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_COLLINEAR_DISTRIBUTION_KINEMATIC
            << " (x, x_unit, MuF2, MuF2_unit, MuR2, MuR2_unit, hash_sum)"
            << " VALUES (:x, :x_unit, :MuF2, :MuF2_unit, :MuR2, :MuR2_unit, :hash_sum)";

    //prepare query
    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":x", x.getValue());
    query.bindValue(":x_unit", x.getUnit());
    query.bindValue(":MuF2", MuF2.getValue());
    query.bindValue(":MuF2_unit", MuF2.getUnit());
    query.bindValue(":MuR2", MuR2.getValue());
    query.bindValue(":MuR2_unit", MuR2.getUnit());
    query.bindValue(":hash_sum", hashSum.c_str());

    //execute query
    if (query.exec()) {

        //get result
        result = query.lastInsertId().toInt();
    } else {

        //throw if error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

int CollinearDistributionKinematicDao::select(const PhysicalType<double>& x,
        const PhysicalType<double>& MuF2,
        const PhysicalType<double>& MuR2) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //form query
    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_COLLINEAR_DISTRIBUTION_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_COLLINEAR_DISTRIBUTION_KINEMATIC << " WHERE"
            << " x = :x AND x_unit = :x_unit AND"
            << " MuF2 = :MuF2 AND MuF2_unit = :MuF2_unit AND"
            << " MuR2 = :MuR2 AND MuR2_unit = :MuR2_unit";

    //prepare query
    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":x", x.getValue());
    query.bindValue(":x_unit", x.getUnit());
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

CollinearDistributionKinematic CollinearDistributionKinematicDao::getKinematicById(const int id) const {

    //result
    CollinearDistributionKinematic colldistKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM collinear_distribution_kinematic_view WHERE collinear_distribution_kinematic_id = :id");

    query.bindValue(":id", id);

    //execute and check if unique (if false true exception)
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //set first
        query.first();

        //fill
        fillCollinearDistributionKinematicFromQuery(colldistKinematic, query);
    }

    return colldistKinematic;
}

List<CollinearDistributionKinematic> CollinearDistributionKinematicDao::getKinematicListByComputationId(
        const int computationId) const {

    //result
    List<CollinearDistributionKinematic> kinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT k.* FROM collinear_distribution_kinematic_view k, collinear_distribution_result r WHERE r.computation_id = :computationId AND r.collinear_distribution_kinematic_id = k.collinear_distribution_kinematic_id;");

    query.bindValue(":computationId", QVariant(computationId));

    //execute and check how many results retrieved (if 0 throw exception)
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillCollinearDistributionKinematicListFromQuery(kinematicList, query);
    }

    return kinematicList;
}

void CollinearDistributionKinematicDao::fillCollinearDistributionKinematicListFromQuery(
        List<CollinearDistributionKinematic>& colldistKinematicList, QSqlQuery& query) const {

    //loop over single queries
    while (query.next()) {

        //single result
        CollinearDistributionKinematic colldistKinematic;

        //get
        fillCollinearDistributionKinematicFromQuery(colldistKinematic, query);

        //store
        colldistKinematicList.add(colldistKinematic);
    }
}

void CollinearDistributionKinematicDao::fillCollinearDistributionKinematicFromQuery(CollinearDistributionKinematic &colldistKinematic,
        QSqlQuery& query) const {

    //get indices
    int field_id = query.record().indexOf("collinear_distribution_kinematic_id");
    int field_x = query.record().indexOf("x");
    int field_x_unit = query.record().indexOf("x_unit");
    int field_MuF2 = query.record().indexOf("MuF2");
    int field_MuF2_unit = query.record().indexOf("MuF2_unit");
    int field_MuR2 = query.record().indexOf("MuR2");
    int field_MuR2_unit = query.record().indexOf("MuR2_unit");
    int field_hash_sum = query.record().indexOf("hash_sum");

    //get values
    int id = query.value(field_id).toInt();
    double x = query.value(field_x).toDouble();
    PhysicalUnit::Type x_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_x_unit).toInt());
    double MuF2 = query.value(field_MuF2).toDouble();
    PhysicalUnit::Type MuF2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuF2_unit).toInt());
    double MuR2 = query.value(field_MuR2).toDouble();
    PhysicalUnit::Type MuR2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuR2_unit).toInt());

    //set
    colldistKinematic = CollinearDistributionKinematic(PhysicalType<double>(x, x_unit),
            PhysicalType<double>(MuF2, MuF2_unit),
            PhysicalType<double>(MuR2, MuR2_unit));
    colldistKinematic.setIndexId(id);

    //check hash sum
    if (colldistKinematic.getHashSum()
            != query.value(field_hash_sum).toString().toStdString()) {
        warn(__func__,
                "Retrieved kinematics has different hash sum than original one from database");
    }
}

int CollinearDistributionKinematicDao::getKinematicIdByHashSum(const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_COLLINEAR_DISTRIBUTION_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_COLLINEAR_DISTRIBUTION_KINEMATIC
            << " WHERE hash_sum = :hashSum";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":hashSum", QString(hashSum.c_str()));

    //execute and check if unique (if false true exception)
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
