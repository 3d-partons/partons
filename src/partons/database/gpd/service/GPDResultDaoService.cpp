#include "../../../../../include/partons/database/gpd/service/GPDResultDaoService.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <map>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

GPDResultDaoService::GPDResultDaoService() :
        ResultDaoService("GPDResultDaoService"), m_lastGPDKinematicId(-1), m_lastGPDResultId(
                -1), m_lastPartonDistributionId(-1), m_lastQuarkDistributionId(
                -1), m_lastGPDResultPartonDistributionId(-1), m_lastPartonDistributionQuarkDistributionId(
                -1) {

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());
    if (query.exec("SELECT COUNT(gpd_kinematic_id) FROM gpd_kinematic;")) {
        if (query.first()) {
            m_lastGPDKinematicId = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec("SELECT COUNT(gpd_result_id) FROM gpd_result;")) {
        if (query.first()) {
            m_lastGPDResultId = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "SELECT COUNT(parton_distribution_id) FROM parton_distribution;")) {
        if (query.first()) {
            m_lastPartonDistributionId = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "SELECT COUNT(quark_distribution_id) FROM quark_distribution;")) {
        if (query.first()) {
            m_lastQuarkDistributionId = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "SELECT COUNT(parton_distribution_quark_distribution_id) FROM parton_distribution_quark_distribution;")) {
        if (query.first()) {
            m_lastPartonDistributionQuarkDistributionId =
                    query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "SELECT COUNT(gpd_result_parton_distribution_id) FROM gpd_result_parton_distribution;")) {
        if (query.first()) {
            m_lastGPDResultPartonDistributionId = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();
}

GPDResultDaoService::~GPDResultDaoService() {

}

int GPDResultDaoService::insert(const GPDResult &gpdResult) const {

    int computationId = -1;

//    // For multiple query it's better to use transaction to guarantee database's integrity and performance
//    QSqlDatabase::database().transaction();
//
//    try {
//        computationId = insertWithoutTransaction(gpdResult);
//
//        // If there is no exception we can commit all query
//        QSqlDatabase::database().commit();
//
//    } catch (std::exception &e) {
//        error(__func__, e.what());
//
//        // Else return database in a stable state : n-1
//        QSqlDatabase::database().rollback();
//    }

//TODO refactoring

    return computationId;
}

//int GPDResultDaoService::insertWithoutTransactionResultList(
//        const ResultList<GPDResult> &gpdResultList) const {
//
//
//
//}
//
//int GPDResultDaoService::insertWithoutTransaction(
//        const GPDResult &gpdResult) const {
//
//    int computationId = -1;
//    time_t computationDateTime =
//            gpdResult.getResultInfo().getComputation().getDateTime();
//
//    // Check if previousComputationId stored is not the same as current
//    if (computationDateTime == m_previousComputationId.first) {
//        computationId = m_previousComputationId.second;
//    } else {
//        // Check if this computation date already exists and retrieve Id
//        computationId = m_computationDaoService.getComputationIdByDateTime(
//                computationDateTime);
//
//        // If not, insert new entry in database and retrieve its id
//        if (computationId == -1) {
//            computationId = m_resultInfoDaoService.insertWithoutTransaction(
//                    gpdResult.getResultInfo());
//        }
//
//        m_previousComputationId = std::make_pair<time_t, int>(
//                computationDateTime, computationId);
//    }
//
//    // Check if this gpd_kinematic already exists
//    int gpdKinematicId = m_gpdKinematicDaoService.getIdByKinematicObject(
//            gpdResult.getKinematic());
//
//    // If not, insert new entry in database and retrieve its id
//    if (gpdKinematicId == -1) {
//        gpdKinematicId = m_gpdKinematicDaoService.insertWithoutTransaction(
//                gpdResult.getKinematic());
//    }
//
//    // Insert new gpd_result entry in database
//    int gpdResultId = m_gpdResultDao.insertResult(
//            gpdResult.getComputationModuleName(), gpdKinematicId,
//            computationId);
//
//    // Get all PartonDistribution objects indexed by GPDType
//    std::map<GPDType::Type, PartonDistribution> partonDistributionMap =
//            gpdResult.getPartonDistributions();
//
//    // Defined a parton_distribution_id
//    int partonDistributionId = -1;
//
//    // Then loop over GPDType to store PartonDistribution objets into database.
//    for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
//            partonDistributionMap.begin(); it != partonDistributionMap.end();
//            it++) {
//        // Insert new PartonDistribution object into database ; retrieve its id
//        partonDistributionId = m_partonDistributionDaoService.insert(
//                (it->second));
//
//        // Fill gpd_result_parton_distribution association table with previous retrieved ids.
//        m_gpdResultDao.insertIntoGPDResultPartonDistributionTable((it->first),
//                gpdResultId, partonDistributionId);
//    }
//
//    return computationId;
//}

//int GPDResultDaoService::insert(const List<GPDResult> &gpdResultList) const {
//
//    info(__func__,
//            ElemUtils::Formatter() << "Inserting object size = "
//                    << gpdResultList.size());
//
//    int computationId = -1;
//
////    unsigned int batchSize = 250;
////    unsigned int startListIndex = 0;
////    unsigned int numberOfIteration = gpdResultList.size() / batchSize();
////
////    for()
////    {
////
////    }
////
//    // For multiple query it's better to use transaction to guarantee database's integrity and performance
//    QSqlDatabase::database().transaction();
//
//    try {
//
//        for (unsigned int i = 0; i != gpdResultList.size(); i++) {
//            computationId = insertWithoutTransaction(gpdResultList.get(i));
//        }
//
//        // If there is no exception we can commit all query
//        QSqlDatabase::database().commit();
//
//    } catch (std::exception &e) {
//
//        error(__func__, e.what());
//
//        // Else return database in a stable state : n-1
//        QSqlDatabase::database().rollback();
//    }
//
//    return computationId;
//}

bool GPDResultDaoService::insert(const List<GPDResult> &resultList) {
    bool inserted = false;

    info(__func__, "Prepare data before inserting them into database ...");

    for (unsigned int i = 0; i != resultList.size(); i++) {

        prepareCommonTablesFromResultInfo(resultList[i].getResultInfo());

        GPDKinematic kinematic = resultList[i].getKinematic();
//        int kinematicId = m_gpdKinematicDaoService.getIdByKinematicObject(
//                kinematic);

        int kinematicId = m_gpdKinematicDaoService.getKinematicIdByHashSum(
                kinematic.getHashSum());

        if (kinematicId == -1) {
            m_lastGPDKinematicId++;
            kinematicId = m_lastGPDKinematicId;

            m_gpdKinematicDatabaseFile += ElemUtils::Formatter()
                    << m_lastGPDKinematicId << "," << kinematic.getX() << ","
                    << kinematic.getXi() << "," << kinematic.getT() << ","
                    << kinematic.getMuF2() << "," << kinematic.getMuR2() << ","
                    << kinematic.getHashSum() << '\n';
        }

        m_lastGPDResultId++;
        m_gpdResultDatabaseFile += ElemUtils::Formatter() << m_lastGPDResultId
                << "," << resultList[i].getComputationModuleName() << ","
                << kinematicId << "," << m_previousComputationId.second << '\n';

        // Get all PartonDistribution objects indexed by GPDType
        std::map<GPDType::Type, PartonDistribution> partonDistributionMap =
                resultList[i].getPartonDistributions();

        // Then loop over GPDType to store PartonDistribution objets into database.
        for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
                partonDistributionMap.begin();
                it != partonDistributionMap.end(); it++) {

            m_lastPartonDistributionId++;

            m_parton_distribution_table +=
                    ElemUtils::Formatter() << m_lastPartonDistributionId << ","
                            << (it->second).getGluonDistribution().getGluonDistribution()
                            << '\n';

            std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributionList =
                    (it->second).getQuarkDistributions();

            for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_qd =
                    quarkDistributionList.begin();
                    it_qd != quarkDistributionList.end(); it_qd++) {

                // insert QuarkDistribution object
                m_lastQuarkDistributionId++;

                m_quark_distribution_table += ElemUtils::Formatter()
                        << m_lastQuarkDistributionId << ","
                        << (it_qd->second).getQuarkDistributionPlus() << ","
                        << (it_qd->second).getQuarkDistributionMinus() << ","
                        << (it_qd->second).getQuarkDistribution() << ","
                        << (it_qd->first) << '\n';

                // fill association table "parton_distribution_quark_distribution"
                m_lastPartonDistributionQuarkDistributionId++;

                m_parton_distribution_quark_distribution_table +=
                        ElemUtils::Formatter()
                                << m_lastPartonDistributionQuarkDistributionId
                                << "," << m_lastPartonDistributionId << ","
                                << m_lastQuarkDistributionId << '\n';
            }

            // Fill gpd_result_parton_distribution association table with previous retrieved ids.
            m_lastGPDResultPartonDistributionId++;

            m_gpd_result_parton_distribution_table += ElemUtils::Formatter()
                    << m_lastGPDResultPartonDistributionId << "," << (it->first)
                    << "," << m_lastGPDResultId << ","
                    << m_lastPartonDistributionId << '\n';
        }
    }

    insertCommonDataIntoDatabaseTables();

    insertDataIntoDatabaseTables("gpdKinematicDatabaseFile.csv",
            m_gpdKinematicDatabaseFile, "gpd_kinematic");
    insertDataIntoDatabaseTables("gpdResultDatabaseFile.csv",
            m_gpdResultDatabaseFile, "gpd_result");
    insertDataIntoDatabaseTables("partonDistributionDatabaseFile.csv",
            m_parton_distribution_table, "parton_distribution");
    insertDataIntoDatabaseTables("quarkDistributionDatabaseFile.csv",
            m_quark_distribution_table, "quark_distribution");
    insertDataIntoDatabaseTables("gpdResultPartonDistributionDatabaseFile.csv",
            m_gpd_result_parton_distribution_table,
            "gpd_result_parton_distribution");
    insertDataIntoDatabaseTables(
            "partonDistributionQuarkDistributionDatabaseFile.csv",
            m_parton_distribution_quark_distribution_table,
            "parton_distribution_quark_distribution");

    info(__func__, "Done !");

    return inserted;
}

List<GPDResult> GPDResultDaoService::getGPDResultListByComputationId(
        const int computationId) const {
    return m_gpdResultDao.getGPDResultListByComputationId(computationId);
}
