#include "../../../../../include/partons/database/convol_coeff_function/dao/DVCSConvolCoeffFunctionKinematicDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

DVCSConvolCoeffFunctionKinematicDao::DVCSConvolCoeffFunctionKinematicDao() :
        ConvolCoeffFunctionKinematicDao("DVCSConvolCoeffFunctionKinematicDao") {
}

DVCSConvolCoeffFunctionKinematicDao::~DVCSConvolCoeffFunctionKinematicDao() {
}

int DVCSConvolCoeffFunctionKinematicDao::insert(double xi, double t, double Q2,
        double MuF2, double MuR2) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_CCF_KINEMATIC
            << " (xi, t, Q2, MuF2, MuR2) VALUES (:xi, :t, :Q2, :MuF2, :MuR2);";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

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

int DVCSConvolCoeffFunctionKinematicDao::select(double xi, double t, double Q2,
        double MuF2, double MuR2) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_CCF_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_CCF_KINEMATIC
            << " WHERE xi = :xi AND t = :t AND Q2 = :Q2 AND MuF2 = :MuF2 AND MuR2 = :MuR2;";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    //execute query
    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.value(0).toInt();

    return result;
}

DVCSConvolCoeffFunctionKinematic DVCSConvolCoeffFunctionKinematicDao::getKinematicById(
        const int id) const {

    //result
    DVCSConvolCoeffFunctionKinematic convolCoeffFunctionKinematic;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM " << Database::TABLE_NAME_CCF_KINEMATIC
            << " WHERE " << Database::COLUMN_NAME_CCF_KINEMATIC_ID << " = :id;";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":id", id);

    //execute query
    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //fill
    fillKinematicFromQuery(convolCoeffFunctionKinematic, query);

    return convolCoeffFunctionKinematic;
}

List<DVCSConvolCoeffFunctionKinematic> DVCSConvolCoeffFunctionKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    //result
    List<DVCSConvolCoeffFunctionKinematic> kinematicList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    ElemUtils::Formatter formatter;
    formatter << "SELECT ccfk." << Database::COLUMN_NAME_CCF_KINEMATIC_ID
            << ", ccfk.xi, ccfk.t, ccfk.Q2, ccfk.MuF2, ccfk.MuR2 FROM "
            << Database::TABLE_NAME_CCF_KINEMATIC << " ccfk, "
            << Database::TABLE_NAME_CCF_RESULT << " ccfr WHERE ccfr."
            << Database::COLUMN_NAME_COMPUTATION_ID
            << " = :computationId AND ccfr."
            << Database::COLUMN_NAME_CCF_RESULT_ID << " = ccfk."
            << Database::COLUMN_NAME_CCF_KINEMATIC_ID << ";";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":computationId", computationId);

    //execute query
    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //fill
    fillKinematicListFromQuery(kinematicList, query);

    return kinematicList;
}

void DVCSConvolCoeffFunctionKinematicDao::fillKinematicListFromQuery(
        List<DVCSConvolCoeffFunctionKinematic>& kinematicList,
        QSqlQuery& query) const {
    while (query.next()) {
        DVCSConvolCoeffFunctionKinematic kinematic;
        fillKinematicFromQuery(kinematic, query);
        kinematicList.add(kinematic);
    }
}

void DVCSConvolCoeffFunctionKinematicDao::fillKinematicFromQuery(
        DVCSConvolCoeffFunctionKinematic &kinematic, QSqlQuery &query) const {

    int field_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_CCF_KINEMATIC_ID.c_str()));
    int field_xi = query.record().indexOf("xi");
    int field_t = query.record().indexOf("t");
    int field_Q2 = query.record().indexOf("Q2");
    int field_MuF2 = query.record().indexOf("MuF2");
    int field_MuR2 = query.record().indexOf("MuR2");

    int id = query.value(field_id).toInt();
    double xi = query.value(field_xi).toDouble();
    double t = query.value(field_t).toDouble();
    double Q2 = query.value(field_Q2).toDouble();
    double MuF2 = query.value(field_MuF2).toDouble();
    double MuR2 = query.value(field_MuR2).toDouble();

    kinematic = DVCSConvolCoeffFunctionKinematic(xi, t, Q2, MuF2, MuR2);
    kinematic.setIndexId(id);

}

int DVCSConvolCoeffFunctionKinematicDao::getKinematicIdByHashSum(
        const std::string& hashSum) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_CCF_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_CCF_KINEMATIC
            << " WHERE hash_sum = :hashSum;";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":hashSum", QString(hashSum.c_str()));

    //execute query
    if (Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query) != 0) {
        result = query.value(0).toInt();
    }

    return result;
}

} /* namespace PARTONS */
