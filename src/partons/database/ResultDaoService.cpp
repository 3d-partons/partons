#include "../../../include/partons/database/ResultDaoService.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <map>

#include "../../../include/partons/beans/Computation.h"
#include "../../../include/partons/beans/gpd/GPDType.h"
#include "../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/database/DatabaseManager.h"

ResultDaoService::ResultDaoService() :
        BaseObject("ResultDaoService"), m_previousComputationId(
                std::make_pair<time_t, int>(0, -1)), m_lastComputationId(-1), m_lastGPDResultId(
                -1), m_lastPartonDistributionId(-1), m_lastQuarkDistributionId(
                -1) {

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());
    if (query.exec("SELECT COUNT(id) FROM computation;")) {
        if (query.first()) {
            m_lastComputationId = query.value(0).toInt();
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec("SELECT COUNT(id) FROM gpd_result;")) {
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

    if (query.exec("SELECT COUNT(id) FROM parton_distribution;")) {
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

    if (query.exec("SELECT COUNT(id) FROM quark_distribution;")) {
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

    warn(__func__,
            ElemUtils::Formatter() << "m_lastComputationId = "
                    << m_lastComputationId);
    warn(__func__,
            ElemUtils::Formatter() << "m_lastGPDResultId = "
                    << m_lastGPDResultId);
}

ResultDaoService::~ResultDaoService() {
    // TODO Auto-generated destructor stub
}

bool ResultDaoService::insert(const List<GPDResult>& result) {
    bool isInserted = false;

    for (unsigned int i = 0; i != result.size(); i++) {

        int computationId = -1;
        time_t computationDateTime =
                result[i].getResultInfo().getComputation().getDateTime();

        // Check if previousComputationId stored is not the same as current
        if (computationDateTime == m_previousComputationId.first) {
            computationId = m_previousComputationId.second;
        } else {
            // Check if this computation date already exists and retrieve Id
            computationId = m_computationDaoService.getComputationIdByDateTime(
                    computationDateTime);

            // If not, insert new entry in database and retrieve its id
            if (computationId == -1) {
                m_lastComputationId++;
                computationId = m_lastComputationId;
                m_computationDatabaseFile += ElemUtils::Formatter()
                        << computationId << "," << computationDateTime << ","
                        << -1 << '\n';
            }

            m_previousComputationId = std::make_pair<time_t, int>(
                    computationDateTime, computationId);
        }

        m_lastGPDResultId++;
        m_gpdResultDatabaseFile += ElemUtils::Formatter() << m_lastGPDResultId
                << "," << result[i].getComputationModuleName() << "," << -1
                << "," << computationId << '\n';

        // Get all PartonDistribution objects indexed by GPDType
        std::map<GPDType::Type, PartonDistribution> partonDistributionMap =
                result[i].getPartonDistributions();

        // Then loop over GPDType to store PartonDistribution objets into database.
        for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
                partonDistributionMap.begin();
                it != partonDistributionMap.end(); it++) {

            m_lastPartonDistributionId++;

            m_parton_distribution_table +=
                    ElemUtils::Formatter() << m_lastPartonDistributionId << ","
                            << (it->second).getGluonDistribution().getGluonDistribution()
                            << '\n';

            // Fill gpd_result_parton_distribution association table with previous retrieved ids.
//                m_gpdResultDao.insertIntoGPDResultPartonDistributionTable((it->first),
//                        gpdResultId, partonDistributionId);
        }
    }

//    warn(__func__, ElemUtils::Formatter() << m_gpdResultDatabaseFile << '\n');
//    warn(__func__, ElemUtils::Formatter() << m_computationDatabaseFile << '\n');

    ElemUtils::FileUtils::write("/home/debian/temp/gpdResultDatabaseFile.csv",
            m_gpdResultDatabaseFile);
    ElemUtils::FileUtils::write("/home/debian/temp/computationDatabaseFile.csv",
            m_computationDatabaseFile);
    ElemUtils::FileUtils::write(
            "/home/debian/temp/m_parton_distribution_table.csv",
            m_parton_distribution_table);

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    if (query.exec(
            "LOAD DATA INFILE '/home/debian/temp/computationDatabaseFile.csv' INTO TABLE computation FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query computationDatabaseFile = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "LOAD DATA INFILE '/home/debian/temp/gpdResultDatabaseFile.csv' INTO TABLE gpd_result FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query gpdResultDatabaseFile = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "LOAD DATA INFILE '/home/debian/temp/m_parton_distribution_table.csv' INTO TABLE parton_distribution FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query m_parton_distribution_table = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    return isInserted;
}
