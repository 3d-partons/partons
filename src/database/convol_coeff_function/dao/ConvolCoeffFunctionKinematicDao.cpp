#include "ConvolCoeffFunctionKinematicDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

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
        error(__func__, Formatter() << query.lastError().text().toStdString());
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
        error(__func__, Formatter() << query.lastError().text().toStdString());
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
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return convolCoeffFunctionKinematic;
}

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
    kinematic.setId(id);
}
