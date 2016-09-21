#include "../../../../../include/partons/database/observable/dao/ObservableKinematicDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

ObservableKinematicDao::ObservableKinematicDao() :
        BaseObject("ObservableKinematicDao") {
}

ObservableKinematicDao::~ObservableKinematicDao() {
}

int ObservableKinematicDao::insert(double xB, double t, double Q2,
        double phi) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_OBSERVABLE_KINEMATIC
            << " (bin_id, xB, t, Q2, phi) VALUES (:bin_id, :xB, :t, :Q2, :phi)";

    query.prepare(QString(formatter.str().c_str()));

    //TODO remove hardcoded value 0 for bin_id
    query.bindValue(":bin_id", 0);
    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":phi", phi);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
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

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID
            << " FROM " << Database::TABLE_NAME_OBSERVABLE_KINEMATIC
            << " WHERE xB = :xB AND t = :t AND Q2 = :Q2 AND phi = :phi";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":xB", xB);
    query.bindValue(":t", t);
    query.bindValue(":Q2", Q2);
    query.bindValue(":phi", phi);

    if (query.exec()) {
        if (query.first()) {
            result = query.value(0).toInt();
        }
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
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

    ElemUtils::Formatter formatter;
    formatter << "SELECT observable_kinematic_view WHERE "
            << Database::COLUMN_NAME_COMPUTATION_ID << " = :computationId";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":computationId", computationId);

    if (query.exec()) {
        fillObservableKinematicList(observableKinematicList, query);
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
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

    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM " << Database::TABLE_NAME_OBSERVABLE_KINEMATIC
            << " WHERE " << Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID
            << " = :kinematicId";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":kinematicId", kinematicId);

    if (query.exec()) {
        fillObservableKinematic(observableKinematic, query);
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return observableKinematic;
}

void ObservableKinematicDao::fillObservableKinematic(
        ObservableKinematic &observableKinematic, QSqlQuery &query) const {

    int field_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID.c_str()));
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
        observableKinematic.setIndexId(id);
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
