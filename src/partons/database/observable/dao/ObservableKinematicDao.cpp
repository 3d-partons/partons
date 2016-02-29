#include "../../../../../include/partons/database/observable/dao/ObservableKinematicDao.h"

#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <QtCore/qvariant.h>
#include <QtCore/qstring.h>
#include <string>

#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

ObservableKinematicDao::ObservableKinematicDao() :
        BaseObject("ObservableKinematicDao") {
}

ObservableKinematicDao::~ObservableKinematicDao() {
}

int ObservableKinematicDao::insert(double xB, double t, double Q2,
        double phi) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO observable_kinematic (bin_id, xB, t, Q2, phi) VALUES (:bin_id, :xB, :t, :Q2, :phi )");

    //TODO remove hardcoded value 0 for bin_id
    query.bindValue(":bin_id", 0);
    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":phi", phi);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

int ObservableKinematicDao::select(double xB, double t, double Q2,
        double phi) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT id FROM observable_kinematic WHERE xB = :xB AND t = :t AND Q2 = :Q2 AND phi = :phi");

    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":phi", phi);

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

List<ObservableKinematic> ObservableKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    List<ObservableKinematic> observableKinematicList;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            " SELECT DISTINCT obk.id, obk.xB, obk.t, obk.Q2, obk.phi FROM observable_kinematic obk, observable_result obr WHERE obr.observable_kinematic_id = obk.id AND obr.computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        fillObservableKinematicList(observableKinematicList, query);
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return observableKinematicList;

}

ObservableKinematic ObservableKinematicDao::getKinematicById(
        const int kinematicId) const {

    ObservableKinematic observableKinematic;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM observable_kinematic WHERE id = :kinematicId;");

    query.bindValue(":kinematicId", kinematicId);

    if (query.exec()) {
        fillObservableKinematic(observableKinematic, query);
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return observableKinematic;
}

void ObservableKinematicDao::fillObservableKinematic(
        ObservableKinematic &observableKinematic, QSqlQuery &query) const {

    int field_id = query.record().indexOf("id");
    int field_xB = query.record().indexOf("xB");
    int field_t = query.record().indexOf("t");
    int field_Q2 = query.record().indexOf("Q2");
    int field_phi = query.record().indexOf("phi");

    if (query.first()) {
        int id = query.value(field_id).toInt();
        double xB = query.value(field_xB).toDouble();
        double t = query.value(field_t).toDouble();
        double Q2 = query.value(field_Q2).toDouble();
        double phi = query.value(field_phi).toDouble();

        observableKinematic = ObservableKinematic(xB, t, Q2, phi);
        observableKinematic.setId(id);
    }
}

void ObservableKinematicDao::fillObservableKinematicList(
        List<ObservableKinematic>& observableKinematicList,
        QSqlQuery& query) const {
    while (query.next()) {
        ObservableKinematic observableKinematic;
        fillObservableKinematic(observableKinematic, query);
        observableKinematicList.add(observableKinematic);
    }
}
