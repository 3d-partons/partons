#include "../../../../../include/partons/database/gpd/dao/GPDKinematicDao.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/database/Database.h"
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

    ElemUtils::Formatter formatter;
    formatter << "INSERT INTO " << Database::TABLE_NAME_GPD_KINEMATIC
            << " (x, xi, t, MuF2, MuR2) VALUES (:x, :xi, :t, :MuF2, :MuR2)";

    query.prepare(QString(formatter.str().c_str()));

    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    if (query.exec()) {
        debug(__func__, Database::getLastExecutedQuery(query));

        result = query.lastInsertId().toInt();
    } else {
        //TODO move implementation in mother classe for avoid code redondance
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

int GPDKinematicDao::select(double x, double xi, double t, double MuF2,
        double MuR2) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_GPD_KINEMATIC
            << " WHERE x = :x AND xi = :xi AND t = :t AND MuF2 = :MuF2 AND MuR2 = :MuR2";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":x", x);
    query.bindValue(":xi", xi);
    query.bindValue(":t", t);
    query.bindValue(":MuF2", MuF2);
    query.bindValue(":MuR2", MuR2);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.value(0).toInt();

    return result;
}

GPDKinematic GPDKinematicDao::getKinematicById(const int id) const {
    GPDKinematic gpdKinematic;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT * FROM " << Database::TABLE_NAME_GPD_KINEMATIC
            << " WHERE " << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " = :id";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":id", id);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    fillGPDKinematicFromQuery(gpdKinematic, query);

    return gpdKinematic;
}

List<GPDKinematic> GPDKinematicDao::getKinematicListByComputationId(
        const int computationId) const {
    debug(__func__, "Processing ...");

    List<GPDKinematic> kinematicList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT k." << Database::COLUMN_NAME_GPD_KINEMATIC_ID
            << ", k.x, k.xi, k.t, k.MuF2, k.MuR2 FROM "
            << Database::TABLE_NAME_GPD_KINEMATIC << " k, "
            << Database::TABLE_NAME_GPD_RESULT
            << " r WHERE r.computation_id = :computationId AND r."
            << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " = k."
            << Database::COLUMN_NAME_GPD_KINEMATIC_ID;

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":computationId", QVariant(computationId));

    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    fillGPDKinematicListFromQuery(kinematicList, query);

    return kinematicList;
}

//TODO test implementation
void GPDKinematicDao::fillGPDKinematicFromQuery(GPDKinematic &gpdKinematic,
        QSqlQuery& query) const {
    int field_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_GPD_KINEMATIC_ID.c_str()));
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
    gpdKinematic.setIndexId(id);
}

//TODO test implementation
void GPDKinematicDao::fillGPDKinematicListFromQuery(
        List<GPDKinematic>& gpdKinematicList, QSqlQuery& query) const {
    debug(__func__, "Processing ...");

    while (query.next()) {
        GPDKinematic gpdKinematic;
        fillGPDKinematicFromQuery(gpdKinematic, query);
        gpdKinematicList.add(gpdKinematic);
    }
}

int GPDKinematicDao::getKinematicIdByHashSum(const std::string& hashSum) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    ElemUtils::Formatter formatter;
    formatter << "SELECT " << Database::COLUMN_NAME_GPD_KINEMATIC_ID << " FROM "
            << Database::TABLE_NAME_GPD_KINEMATIC
            << " WHERE hash_sum = :hashSum";

    query.prepare(QString(formatter.str().c_str()));
    query.bindValue(":hashSum", QString(hashSum.c_str()));

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.value(0).toInt();

    return result;
}
