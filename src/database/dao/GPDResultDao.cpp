#include "GPDResultDao.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>

#include "../DatabaseManager.h"

//#include <QtCore/qstring.h>
//#include <QtSql/qsqlrecord.h>
//#include <iostream>

//#include "../../beans/gpd/GPDResult.h"
//#include "../../beans/gpd/GPDType.h"
//#include "../../beans/kinematic/GPDKinematic.h"
//#include "../../beans/parton_distribution/GluonDistribution.h"
//#include "../../beans/parton_distribution/QuarkDistribution.h"
//#include "../../beans/QuarkFlavor.h"

//
//GPDResultDao::GPDResultDao() {
//    m_QSqlTableModel.setTable(
//            QString::fromStdString(GPDResult::GPD_RESULT_DB_TABLE_NAME));
//}
//
//GPDResultDao::~GPDResultDao() {
//}
//
//int GPDResultDao::insert(double x, double xi, double t, double MuF, double MuR,
//        int gpdType, double gluonDistribution, int quarkFlavorType,
//        double quarkDistribution, double quarkDistributionPlus,
//        double quarkDistributionMinus) {
//    QSqlRecord record = m_QSqlTableModel.record();
//    record.setValue(
//            QString::fromStdString(
//                    GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_X), x);
//
//    record.setValue(
//            QString::fromStdString(
//                    GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_XI), xi);
//
//    record.setValue(
//            QString::fromStdString(
//                    GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_T), t);
//
//    record.setValue(
//            QString::fromStdString(
//                    GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_MUF), MuF);
//
//    record.setValue(
//            QString::fromStdString(
//                    GPDKinematic::GPD_KINEMATIC_DB_COLUMN_NAME_MUR), MuR);
//
//    record.setValue(QString::fromStdString(GPDType::GPD_TYPE_DB_COLUMN_NAME),
//            gpdType);
//
//    record.setValue(
//            QString::fromStdString(
//                    GluonDistribution::GLUON_DISTRIBUTION_DB_COLUMN_NAME),
//            gluonDistribution);
//
//    record.setValue(
//            QString::fromStdString(
//                    QuarkFlavor::QUARK_FLAVOR_TYPE_DB_COLUMN_NAME),
//            quarkFlavorType);
//
//    record.setValue(
//            QString::fromStdString(
//                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION),
//            quarkDistribution);
//
//    record.setValue(
//            QString::fromStdString(
//                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS),
//            quarkDistributionPlus);
//
//    record.setValue(
//            QString::fromStdString(
//                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS),
//            quarkDistributionMinus);
//
//    // insert record at the end of the table
//    bool inserted = m_QSqlTableModel.insertRecord(-1, record);
//
//    if (!inserted) {
//        std::cerr << "[GPDResultDao::save] Can't insert data" << std::endl;
//    }
//}
//
//QSqlRecord GPDResultDao::getPlot2D(const std::string &abscissaName,
//        const std::string &ordinateName) {
//    m_QSqlTableModel.clear();
//    m_QSqlTableModel.select();
//
//    return m_QSqlTableModel.record();
//}

int GPDResultDao::insert(int moduleId, int gpdKinematicId) {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO gpd_result (module_id, gpd_kinematic_id) VALUES (:moduleId, :gpdKinematicId)");

    query.bindValue(":moduleId", moduleId);
    query.bindValue(":gpdKinematicId", gpdKinematicId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    return result;
}
