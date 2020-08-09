#include "../../../../../include/partons/database/collinear_distribution/service/CollinearDistributionResultDaoService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <map>
#include <utility>

#include "../../../../../include/partons/beans/collinear_distribution/CollinearDistributionKinematic.h"
#include "../../../../../include/partons/beans/collinear_distribution/CollinearDistributionType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/beans/system/ResultInfo.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

CollinearDistributionResultDaoService::CollinearDistributionResultDaoService() :
        ResultDaoService("CollinearDistributionResultDaoService"), m_lastCollinearDistributionKinematicId(-1), m_lastCollinearDistributionResultId(
                -1), m_lastPartonDistributionId(-1), m_lastQuarkDistributionId(
                -1), m_lastCollinearDistributionResultPartonDistributionId(-1), m_lastPartonDistributionQuarkDistributionId(
                -1) {

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //get last id of collinear distribution kinematics
    if (query.exec("SELECT COUNT(collinear_distribution_kinematic_id) FROM collinear_distribution_kinematic;")) {
        if (query.first()) {
            m_lastCollinearDistributionKinematicId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    //get last id of collinear distribution result
    if (query.exec("SELECT COUNT(collinear_distribution_result_id) FROM collinear_distribution_result;")) {
        if (query.first()) {
            m_lastCollinearDistributionResultId = query.value(0).toInt();
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

    //get last id of collinear-distribution result <-> parton distribution
    if (query.exec(
            "SELECT COUNT(collinear_distribution_result_parton_distribution_id) FROM collinear_distribution_result_parton_distribution;")) {
        if (query.first()) {
            m_lastCollinearDistributionResultPartonDistributionId = query.value(0).toInt();
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}

CollinearDistributionResultDaoService::~CollinearDistributionResultDaoService() {

}

int CollinearDistributionResultDaoService::insert(const CollinearDistributionResult &colldistResult) {

    //create list
    List<CollinearDistributionResult> results;

    //add
    results.add(colldistResult);

    //insert
    return insert(results);
}

int CollinearDistributionResultDaoService::insert(const List<CollinearDistributionResult> &resultList) {

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
            CollinearDistributionKinematic kinematic = resultList[i].getKinematic();

            //check if kinematics already in DB
            int kinematicId = m_colldistKinematicDaoService.getKinematicIdByHashSum(
                    kinematic.getHashSum());

            //if not, store it
            if (kinematicId == -1) {

                //increment counter and set last
                m_lastCollinearDistributionKinematicId++;
                kinematicId = m_lastCollinearDistributionKinematicId;

                //prepare query
                m_colldistKinematicDatabaseFile += ElemUtils::Formatter()
                        << m_lastCollinearDistributionKinematicId << ","
                        << kinematic.getX().getValue() << ","
                        << kinematic.getX().getUnit() << ","
                        << kinematic.getMuF2().getValue() << ","
                        << kinematic.getMuF2().getUnit() << ","
                        << kinematic.getMuR2().getValue() << ","
                        << kinematic.getMuR2().getUnit() << ","
                        << kinematic.getHashSum() << '\n';
            }

            //increment counter
            m_lastCollinearDistributionResultId++;

            //prepare query
            m_colldistResultDatabaseFile += ElemUtils::Formatter()
                    << m_lastCollinearDistributionResultId << ","
                    << resultList[i].getComputationModuleName() << ","
                    << kinematicId << "," << m_previousComputationId.second
                    << '\n';

            //get all PartonDistribution objects indexed by CollinearDistributionType
            std::map<CollinearDistributionType::Type, PartonDistribution> partonDistributionMap =
                    resultList[i].getPartonDistributions();

            //loop over CollinearDistributionType to store PartonDistribution objects into database
            for (std::map<CollinearDistributionType::Type, PartonDistribution>::const_iterator it =
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

                //fill collinear_distribution_result_parton_distribution association table with previous retrieved ids.

                //increment counter
                m_lastCollinearDistributionResultPartonDistributionId++;

                //prepare query
                m_collinear_distribution_result_parton_distribution_table += ElemUtils::Formatter()
                        << m_lastCollinearDistributionResultPartonDistributionId << ","
                        << (it->first) << "," << m_lastCollinearDistributionResultId << ","
                        << m_lastPartonDistributionId << '\n';
            }
        }

        //insert
        insertCommonDataIntoDatabaseTables();

        insertDataIntoDatabaseTables("colldistKinematicDatabaseFile.csv",
                m_colldistKinematicDatabaseFile, "collinear_distribution_kinematic");
        insertDataIntoDatabaseTables("colldistResultDatabaseFile.csv",
                m_colldistResultDatabaseFile, "collinear_distribution_result");
        insertDataIntoDatabaseTables("partonDistributionDatabaseFile.csv",
                m_parton_distribution_table, "parton_distribution");
        insertDataIntoDatabaseTables("quarkDistributionDatabaseFile.csv",
                m_quark_distribution_table, "quark_distribution");
        insertDataIntoDatabaseTables(
                "colldistResultPartonDistributionDatabaseFile.csv",
                m_collinear_distribution_result_parton_distribution_table,
                "collinear_distribution_result_parton_distribution");
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

List<CollinearDistributionResult> CollinearDistributionResultDaoService::getCollinearDistributionResultListByComputationId(
        const int computationId) const {

    //info
    info(__func__,
            ElemUtils::Formatter()
                    << "Searching in database for collinear distribution result(s) with computation id = "
                    << computationId << " ...");

    //get result info
    ResultInfo resultInfo = m_resultInfoDaoService.getResultInfoByComputationId(
            computationId);

    //get results
    List<CollinearDistributionResult> results = m_colldistResultDao.getCollinearDistributionResultListByComputationId(
            computationId);

    //set info
    for (unsigned int i = 0; i != results.size(); i++) {
        results[i].setResultInfo(resultInfo);
    }

    return results;
}

} /* namespace PARTONS */
