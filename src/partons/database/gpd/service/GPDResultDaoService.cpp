#include "../../../../../include/partons/database/gpd/service/GPDResultDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqldatabase.h>
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
#include "../../../../../include/partons/beans/system/ResultInfo.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

GPDResultDaoService::GPDResultDaoService() :
        ResultDaoService("GPDResultDaoService"), m_lastGPDKinematicId(-1), m_lastGPDResultId(
                -1), m_lastPartonDistributionId(-1), m_lastQuarkDistributionId(
                -1), m_lastGPDResultPartonDistributionId(-1), m_lastPartonDistributionQuarkDistributionId(
                -1) {

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //get last id of GPD kinematics
    if (query.exec("SELECT COUNT(gpd_kinematic_id) FROM gpd_kinematic;")) {
        if (query.first()) {
            m_lastGPDKinematicId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last id of GPD result
    if (query.exec("SELECT COUNT(gpd_result_id) FROM gpd_result;")) {
        if (query.first()) {
            m_lastGPDResultId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last id of parton distribution
    if (query.exec(
            "SELECT COUNT(parton_distribution_id) FROM parton_distribution;")) {
        if (query.first()) {
            m_lastPartonDistributionId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last id of quark distribution
    if (query.exec(
            "SELECT COUNT(quark_distribution_id) FROM quark_distribution;")) {
        if (query.first()) {
            m_lastQuarkDistributionId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last id of parton distribution <-> quark distribution
    if (query.exec(
            "SELECT COUNT(parton_distribution_quark_distribution_id) FROM parton_distribution_quark_distribution;")) {
        if (query.first()) {
            m_lastPartonDistributionQuarkDistributionId =
                    query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last id of gpd result <-> parton distribution
    if (query.exec(
            "SELECT COUNT(gpd_result_parton_distribution_id) FROM gpd_result_parton_distribution;")) {
        if (query.first()) {
            m_lastGPDResultPartonDistributionId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}

GPDResultDaoService::~GPDResultDaoService() {

}

int GPDResultDaoService::insert(const GPDResult &gpdResult) {

    //create list
    List<GPDResult> results;

    //add
    results.add(gpdResult);

    //insert
    return insert(results);
}

int GPDResultDaoService::insert(const List<GPDResult> &resultList) {

    //for multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        //info
        info(__func__,
                ElemUtils::Formatter() << resultList.size()
                        << " results will be inserted into database; Prepare data before inserting them ...");

        //loop over results
        for (unsigned int i = 0; i != resultList.size(); i++) {

            //prepare for result info
            prepareCommonTablesFromResultInfo(resultList[i].getResultInfo());

            //get kinematics
            GPDKinematic kinematic = resultList[i].getKinematic();

            //check if kinematics already in DB
            int kinematicId = m_gpdKinematicDaoService.getKinematicIdByHashSum(
                    kinematic.getHashSum());

            //if not, store it
            if (kinematicId == -1) {

                //increment counter and set last
                m_lastGPDKinematicId++;
                kinematicId = m_lastGPDKinematicId;

                //prepare query
                m_gpdKinematicDatabaseFile += ElemUtils::Formatter()
                        << m_lastGPDKinematicId << ","
                        << kinematic.getX().getValue() << ","
                        << kinematic.getX().getUnit() << ","
                        << kinematic.getXi().getValue() << ","
                        << kinematic.getXi().getUnit() << ","
                        << kinematic.getT().getValue() << ","
                        << kinematic.getT().getUnit() << ","
                        << kinematic.getMuF2().getValue() << ","
                        << kinematic.getMuF2().getUnit() << ","
                        << kinematic.getMuR2().getValue() << ","
                        << kinematic.getMuR2().getUnit() << ","
                        << kinematic.getHashSum() << '\n';
            }

            //increment counter
            m_lastGPDResultId++;

            //prepare query
            m_gpdResultDatabaseFile += ElemUtils::Formatter()
                    << m_lastGPDResultId << ","
                    << resultList[i].getComputationModuleName() << ","
                    << kinematicId << "," << m_previousComputationId.second
                    << '\n';

            //get all PartonDistribution objects indexed by GPDType
            std::map<GPDType::Type, PartonDistribution> partonDistributionMap =
                    resultList[i].getPartonDistributions();

            //loop over GPDType to store PartonDistribution objects into database
            for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
                    partonDistributionMap.begin();
                    it != partonDistributionMap.end(); it++) {

                //increment counter
                m_lastPartonDistributionId++;

                //prepare query
                m_parton_distribution_table +=
                        ElemUtils::Formatter() << m_lastPartonDistributionId
                                << ","
                                << (it->second).getGluonDistribution().getGluonDistribution()
                                << '\n';

                //get map stroring quark distributions
                std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributionList =
                        (it->second).getQuarkDistributions();

                //loop over quark distribution to store QuarkDistribution objects into database
                for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_qd =
                        quarkDistributionList.begin();
                        it_qd != quarkDistributionList.end(); it_qd++) {

                    //increment counter
                    m_lastQuarkDistributionId++;

                    //prepare query
                    m_quark_distribution_table += ElemUtils::Formatter()
                            << m_lastQuarkDistributionId << ","
                            << (it_qd->second).getQuarkDistributionPlus() << ","
                            << (it_qd->second).getQuarkDistributionMinus()
                            << "," << (it_qd->second).getQuarkDistribution()
                            << "," << (it_qd->first) << '\n';

                    //fill association table "parton_distribution_quark_distribution"

                    //increment counter
                    m_lastPartonDistributionQuarkDistributionId++;

                    //prepare query
                    m_parton_distribution_quark_distribution_table +=
                            ElemUtils::Formatter()
                                    << m_lastPartonDistributionQuarkDistributionId
                                    << "," << m_lastPartonDistributionId << ","
                                    << m_lastQuarkDistributionId << '\n';
                }

                //fill gpd_result_parton_distribution association table with previous retrieved ids.

                //increment counter
                m_lastGPDResultPartonDistributionId++;

                //prepare query
                m_gpd_result_parton_distribution_table += ElemUtils::Formatter()
                        << m_lastGPDResultPartonDistributionId << ","
                        << (it->first) << "," << m_lastGPDResultId << ","
                        << m_lastPartonDistributionId << '\n';
            }
        }

        //insert
        insertCommonDataIntoDatabaseTables();

        insertDataIntoDatabaseTables("gpdKinematicDatabaseFile.csv",
                m_gpdKinematicDatabaseFile, "gpd_kinematic");
        insertDataIntoDatabaseTables("gpdResultDatabaseFile.csv",
                m_gpdResultDatabaseFile, "gpd_result");
        insertDataIntoDatabaseTables("partonDistributionDatabaseFile.csv",
                m_parton_distribution_table, "parton_distribution");
        insertDataIntoDatabaseTables("quarkDistributionDatabaseFile.csv",
                m_quark_distribution_table, "quark_distribution");
        insertDataIntoDatabaseTables(
                "gpdResultPartonDistributionDatabaseFile.csv",
                m_gpd_result_parton_distribution_table,
                "gpd_result_parton_distribution");
        insertDataIntoDatabaseTables(
                "partonDistributionQuarkDistributionDatabaseFile.csv",
                m_parton_distribution_quark_distribution_table,
                "parton_distribution_quark_distribution");

        //if there is no exception we can commit all query
        QSqlDatabase::database().commit();

        //info
        info(__func__, "Done !");

    } catch (const std::exception &e) {

        //else return database in a stable state : n-1
        QSqlDatabase::database().rollback();

        //and throw exception
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }

    return getLastComputationId();
}

List<GPDResult> GPDResultDaoService::getGPDResultListByComputationId(
        const int computationId) const {

    //info
    info(__func__,
            ElemUtils::Formatter()
                    << "Searching in database for GPD result(s) with computation id = "
                    << computationId << " ...");

    //get result info
    ResultInfo resultInfo = m_resultInfoDaoService.getResultInfoByComputationId(
            computationId);

    //get results
    List<GPDResult> results = m_gpdResultDao.getGPDResultListByComputationId(
            computationId);

    //set info
    for (unsigned int i = 0; i != results.size(); i++) {
        results[i].setResultInfo(resultInfo);
    }

    return results;
}

} /* namespace PARTONS */
