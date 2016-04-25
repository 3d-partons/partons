#include "../../../../../include/partons/database/gpd/dao/GPDKinematicDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/database/DatabaseManager.h"

GPDKinematicDao::GPDKinematicDao() :
        BaseObject("GPDKinematicDao") {
}

GPDKinematicDao::~GPDKinematicDao() {
}

int GPDKinematicDao::insert(double x, double xi, double t, double MuF2,
        double MuR2) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO gpd_kinematic (x, xi, t, MuF2, MuR2) VALUES (:x, :xi, :t, :MuF2, :MuR2)");

    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        //TODO move implementation in mother classe for avoid code redondance
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

int GPDKinematicDao::select(double x, double xi, double t, double MuF2,
        double MuR2) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT id FROM gpd_kinematic WHERE x = :x AND xi = :xi AND t = :t AND MuF2 = :MuF2 AND MuR2 = :MuR2");

    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
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

GPDKinematic GPDKinematicDao::getKinematicById(const int id) const {
    GPDKinematic gpdKinematic;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT * FROM gpd_kinematic WHERE id = :id");

    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.first()) {
            fillGPDKinematicFromQuery(gpdKinematic, query);
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return gpdKinematic;
}

List<GPDKinematic> GPDKinematicDao::getKinematicListByComputationId(
        const int computationId) const {
    List<GPDKinematic> kinematicList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT k.id, k.x, k.xi, k.t, k.MuF2, k.MuR2 FROM gpd_kinematic k, gpd_result r WHERE r.computation_id = :computationId AND r.gpd_kinematic_id = k.id");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        if (DatabaseManager::getNumberOfRows(query) != 0) {
            fillGPDKinematicListFromQuery(kinematicList, query);
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
void GPDKinematicDao::fillGPDKinematicFromQuery(GPDKinematic &gpdKinematic,
        QSqlQuery& query) const {
    int field_id = query.record().indexOf("id");
    int field_x = query.record().indexOf("x");
    int field_xi = query.record().indexOf("xi");
    int field_t = query.record().indexOf("t");
    int field_MuF2 = query.record().indexOf("MuF2");
    int field_MuR2 = query.record().indexOf("MuR2");

    int id = query.value(field_id).toInt();
    double x = query.value(field_x).toDouble();
    double xi = query.value(field_xi).toDouble();
    double t = query.value(field_t).toDouble();
    double MuF2 = query.value(field_MuF2).toDouble();
    double MuR2 = query.value(field_MuR2).toDouble();

    gpdKinematic = GPDKinematic(x, xi, t, MuF2, MuR2);
    gpdKinematic.setId(id);
}

//TODO test implementation
void GPDKinematicDao::fillGPDKinematicListFromQuery(
        List<GPDKinematic>& gpdKinematicList, QSqlQuery& query) const {
    while (query.next()) {
        GPDKinematic gpdKinematic;
        fillGPDKinematicFromQuery(gpdKinematic, query);
        gpdKinematicList.add(gpdKinematic);
    }
}
