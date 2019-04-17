#include "../../../../../include/partons/database/gpd/dao/GPDKinematicDao.h"

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

GPDKinematicDao::GPDKinematicDao() :
        BaseObject("GPDKinematicDao") {
}

GPDKinematicDao::~GPDKinematicDao() {
}

int GPDKinematicDao::insert(const PhysicalType<double>& x,
        const PhysicalType<double>& xi, const PhysicalType<double>& t,
        const PhysicalType<double>& MuF2,
        const PhysicalType<double>& MuR2) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //form query
    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_GPD_KINEMATIC
            << " (x, x_unit, xi, xi_unit, t, t_unit, MuF2, MuF2_unit, MuR2, MuR2_unit)"
            << " VALUES (:x, :x_unit, :xi, :xi_unit, :t, :t_unit, :MuF2, :MuF2_unit, :MuR2, :MuR2_unit)";

    //prepare query
    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":x", x.getValue());
    query.bindValue(":x_unit", x.getUnit());
    query.bindValue(":xi", xi.getValue());
    query.bindValue(":xi_unit", xi.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":MuF2", MuF2.getValue());
    query.bindValue(":MuF2_unit", MuF2.getUnit());
    query.bindValue(":MuR2", MuR2.getValue());
    query.bindValue(":MuR2_unit", MuR2.getUnit());

    //execute
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

int GPDKinematicDao::select(const PhysicalType<double>& x,
        const PhysicalType<double>& xi, const PhysicalType<double>& t,
        const PhysicalType<double>& MuF2,
        const PhysicalType<double>& MuR2) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //form query
    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_GPD_KINEMATIC << " WHERE"
            << " x = :x AND x_unit = :x_unit AND"
            << " xi = :xi AND xi_unit = :xi_unit AND"
            << " t = :t AND t_unit = :t_unit AND"
            << " MuF2 = :MuF2 AND MuF2_unit = :MuF2_unit AND"
            << " MuR2 = :MuR2 AND MuR2_unit = :MuR2_unit";

    //prepare query
    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":x", x.getValue());
    query.bindValue(":x_unit", x.getUnit());
    query.bindValue(":xi", xi.getValue());
    query.bindValue(":xi_unit", xi.getUnit());
    query.bindValue(":t", t.getValue());
    query.bindValue(":t_unit", t.getUnit());
    query.bindValue(":MuF2", MuF2.getValue());
    query.bindValue(":MuF2_unit", MuF2.getUnit());
    query.bindValue(":MuR2", MuR2.getValue());
    query.bindValue(":MuR2_unit", MuR2.getUnit());

    //execute
    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //get result
    result = query.value(0).toInt();

    return result;
}

GPDKinematic GPDKinematicDao::getKinematicById(const int id) const {

    //result
    GPDKinematic gpdKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //form query
    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM " << Database::TABLE_NAME_GPD_KINEMATIC
            << " WHERE " << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " = :id";

    //prepare query
    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":id", id);

    //execute and check if unique (if false true exception)
    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //fill
    fillGPDKinematicFromQuery(gpdKinematic, query);

    return gpdKinematic;
}

List<GPDKinematic> GPDKinematicDao::getKinematicListByComputationId(
        const int computationId) const {

    //result
    List<GPDKinematic> kinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //form query
    ElemUtils::Formatter formatter;
    formatter << "SELECT k." << Database::COLUMN_NAME_GPD_KINEMATIC_ID
            << ", k.x, k.x_unit, k.xi, k.xi_unit, k.t, k.t_unit, k.MuF2, k.MuF2_unit, k.MuR2, k.MuR2_unit FROM "
            << Database::TABLE_NAME_GPD_KINEMATIC << " k, "
            << Database::TABLE_NAME_GPD_RESULT
            << " r WHERE r.computation_id = :computationId AND r."
            << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " = k."
            << Database::COLUMN_NAME_GPD_KINEMATIC_ID;

    //prepare query
    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":computationId", QVariant(computationId));

    //execute and check how many results retrieved (if 0 throw exception)
    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //fill
    fillGPDKinematicListFromQuery(kinematicList, query);

    return kinematicList;
}

void GPDKinematicDao::fillGPDKinematicFromQuery(GPDKinematic &gpdKinematic,
        QSqlQuery& query) const {

    //get indices
    int field_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_GPD_KINEMATIC_ID.c_str()));
    int field_x = query.record().indexOf("x");
    int field_x_unit = query.record().indexOf("x_unit");
    int field_xi = query.record().indexOf("xi");
    int field_xi_unit = query.record().indexOf("xi_unit");
    int field_t = query.record().indexOf("t");
    int field_t_unit = query.record().indexOf("t_unit");
    int field_MuF2 = query.record().indexOf("MuF2");
    int field_MuF2_unit = query.record().indexOf("MuF2_unit");
    int field_MuR2 = query.record().indexOf("MuR2");
    int field_MuR2_unit = query.record().indexOf("MuR2_unit");

    //get values
    int id = query.value(field_id).toInt();
    double x = query.value(field_x).toDouble();
    PhysicalUnit::Type x_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_x_unit).toInt());
    double xi = query.value(field_xi).toDouble();
    PhysicalUnit::Type xi_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_xi_unit).toInt());
    double t = query.value(field_t).toDouble();
    PhysicalUnit::Type t_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_t_unit).toInt());
    double MuF2 = query.value(field_MuF2).toDouble();
    PhysicalUnit::Type MuF2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuF2_unit).toInt());
    double MuR2 = query.value(field_MuR2).toDouble();
    PhysicalUnit::Type MuR2_unit = static_cast<PhysicalUnit::Type>(query.value(
            field_MuR2_unit).toInt());

    //set
    gpdKinematic = GPDKinematic(PhysicalType<double>(x, x_unit),
            PhysicalType<double>(xi, xi_unit), PhysicalType<double>(t, t_unit),
            PhysicalType<double>(MuF2, MuF2_unit),
            PhysicalType<double>(MuR2, MuR2_unit));
    gpdKinematic.setIndexId(id);
}

void GPDKinematicDao::fillGPDKinematicListFromQuery(
        List<GPDKinematic>& gpdKinematicList, QSqlQuery& query) const {

    //loop over single queries
    while (query.next()) {

        //single result
        GPDKinematic gpdKinematic;

        //get
        fillGPDKinematicFromQuery(gpdKinematic, query);

        //store
        gpdKinematicList.add(gpdKinematic);
    }
}

int GPDKinematicDao::getKinematicIdByHashSum(const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_GPD_KINEMATIC
            << " WHERE hash_sum = :hashSum";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":hashSum", QString(hashSum.c_str()));

    //execute and check if unique (if false true exception)
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query) != 0) {
        result = query.value(0).toInt();
    }

    return result;
}

} /* namespace PARTONS */
