#include "ObservableKinematicDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>
#include <vector>

#include "../../../beans/observable/ObservableKinematic.h"
#include "../../../beans/observable/ObservableKinematicList.h"
#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

ObservableKinematicDao::ObservableKinematicDao() :
        BaseObject("ObservableKinematicDao") {
}

ObservableKinematicDao::~ObservableKinematicDao() {
}

int ObservableKinematicDao::insert(double xB, double t, double Q2,
        const std::string& listOfPhi) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO observable_kinematic (bin_id, xB, t, Q2, phi_list) VALUES (:bin_id, :xB, :t, :Q2, :phi_list )");

    //TODO remove hardcoded value 0 for bin_id
    query.bindValue(":bin_id", 0);
    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":phi_list", QString(listOfPhi.c_str()));

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}

int ObservableKinematicDao::select(double xB, double t, double Q2,
        const std::string &listOfPhi_str) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT id FROM observable_kinematic WHERE xB = :xB AND t = :t AND Q2 = :Q2 AND phi_list = :listOfPhi_str");

    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":listOfPhi_str", QString(listOfPhi_str.c_str()));

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

ObservableKinematicList ObservableKinematicDao::getKinematicListByComputationId(
        int computationId) const {

    ObservableKinematicList result;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            " SELECT DISTINCT obk.id, obk.xB, obk.t, obk.Q2, obk.phi_list FROM observable_kinematic obk, observable_result obr WHERE obr.observable_kinematic_id = obk.id AND obr.computation_id = :computationId;");

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        int field_id = query.record().indexOf("id");
        int field_xB = query.record().indexOf("xB");
        int field_t = query.record().indexOf("t");
        int field_Q2 = query.record().indexOf("Q2");

        while (query.next()) {
            int id = query.value(field_id).toInt();
            double xB = query.value(field_xB).toDouble();
            double t = query.value(field_t).toDouble();
            double Q2 = query.value(field_Q2).toDouble();

            //TODO remove hardcoded
            std::vector<double> listOfPhi;

            ObservableKinematic observableKinematic(xB, t, Q2, listOfPhi);
            observableKinematic.setId(id);

            result.add(observableKinematic);
        }
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;

}
