#include "../../../../../include/partons/database/convol_coeff_function/dao/ConvolCoeffFunctionKinematicDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/database/DatabaseManager.h"

ConvolCoeffFunctionKinematicDao::ConvolCoeffFunctionKinematicDao() :
        BaseObject("ConvolCoeffFunctionKinematicDao") {
}

ConvolCoeffFunctionKinematicDao::~ConvolCoeffFunctionKinematicDao() {
}

int ConvolCoeffFunctionKinematicDao::insert(double xi, double t, double Q2,
        double MuF2, double MuR2) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO convol_coeff_function_kinematic (xi, t, Q2, MuF2, MuR2) VALUES (:xi, :t, :Q2, :MuF2, :MuR2)");

    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

int ConvolCoeffFunctionKinematicDao::select(double xi, double t, double Q2,
        double MuF2, double MuR2) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT id FROM convol_coeff_function_kinematic WHERE xi = :xi AND t = :t AND Q2 = :Q2 AND MuF2 = :MuF2 AND MuR2 = :MuR2");

    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

DVCSConvolCoeffFunctionKinematic ConvolCoeffFunctionKinematicDao::getKinematicById(
        const int id) const {
    DVCSConvolCoeffFunctionKinematic convolCoeffFunctionKinematic;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM convol_coeff_function_kinematic WHERE id = :id;");

    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.first()) {
            fillKinematicFromQuery(convolCoeffFunctionKinematic, query);
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return convolCoeffFunctionKinematic;
}

List<DVCSConvolCoeffFunctionKinematic> ConvolCoeffFunctionKinematicDao::getKinematicListByComputationId(
        int computationId) const {
    List<DVCSConvolCoeffFunctionKinematic> kinematicList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT k.id, k.xi, k.t, k.Q2, k.MuF2, k.MuR2 FROM convol_coeff_function_kinematic k, convol_coeff_function_result r WHERE r.computation_id = :computationId AND r.ccf_kinematic_id = k.id");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        if (DatabaseManager::getNumberOfRows(query) != 0) {
            fillKinematicListFromQuery(kinematicList, query);
        } else {
            warn(__func__,
                    ElemUtils::Formatter() << "No entry for computationId = "
                            << computationId);
        }
    }

    query.clear();

    return kinematicList;
}

//TODO test implementation
void ConvolCoeffFunctionKinematicDao::fillKinematicListFromQuery(
        List<DVCSConvolCoeffFunctionKinematic>& kinematicList,
        QSqlQuery& query) const {
    while (query.next()) {
        DVCSConvolCoeffFunctionKinematic kinematic;
        fillKinematicFromQuery(kinematic, query);
        kinematicList.add(kinematic);
    }
}

//TODO test implementation
void ConvolCoeffFunctionKinematicDao::fillKinematicFromQuery(
        DVCSConvolCoeffFunctionKinematic &kinematic, QSqlQuery &query) const {

    int field_id = query.record().indexOf("id");
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

int ConvolCoeffFunctionKinematicDao::getKinematicIdByHashSum(
        const std::string& hashSum) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT ccf_kinematic_id FROM ccf_kinematic WHERE hash_sum = :hashSum");

    query.bindValue(":hashSum", QString(hashSum.c_str()));

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}
