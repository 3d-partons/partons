#include "ConvolCoeffFunctionKinematicDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
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
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

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
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "SELECT id FROM convol_coeff_function_kinematic WHERE xi = :xi AND t = :t AND Q2 = :MuF2 AND MuR2 = :MuR2 AND MuR2 = :Q2");

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

