#include "../../../include/partons/database/ResultDaoService.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <map>

#include "../../../include/partons/beans/automation/Scenario.h"
#include "../../../include/partons/beans/Computation.h"
#include "../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../include/partons/beans/gpd/GPDType.h"
#include "../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../include/partons/beans/QuarkFlavor.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/database/DatabaseManager.h"
#include "../../../include/partons/ResourceManager.h"

ResultDaoService::ResultDaoService() :
        BaseObject("ResultDaoService"), m_lastComputationId(-1), m_lastGPDKinematicId(
                -1), m_lastGPDResultId(-1), m_lastPartonDistributionId(-1), m_lastQuarkDistributionId(
                -1), m_lastGPDResultPartonDistributionId(-1), m_lastPartonDistributionQuarkDistributionId(
                -1), m_previousComputationId(
                std::make_pair<time_t, int>(0, -1)), m_previousScenarioId(
                std::make_pair<std::string, int>(ElemUtils::StringUtils::EMPTY,
                        -1)) {

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

    if (query.exec("SELECT COUNT(id) FROM gpd_kinematic;")) {
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

    if (query.exec(
            "SELECT COUNT(id) FROM parton_distribution_quark_distribution;")) {
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

    if (query.exec("SELECT COUNT(id) FROM gpd_result_parton_distribution;")) {
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

ResultDaoService::~ResultDaoService() {
    // TODO Auto-generated destructor stub
}

bool ResultDaoService::insert(const List<GPDResult>& result) {
    bool isInserted = false;

    for (unsigned int i = 0; i != result.size(); i++) {

        int scenarioId = -1;
        std::string scenarioHashSum =
                result[i].getResultInfo().getScenarioHashSum();

        // Check if previousComputationId stored is not the same as current
        if (scenarioHashSum == m_previousScenarioId.first) {
            scenarioId = m_previousComputationId.second;
        } else {
            // Check if this computation date already exists and retrieve Id
            scenarioId = m_scenarioDaoService.getScenarioIdByHashSum(
                    scenarioHashSum);

            // If not, insert new entry in database and retrieve its id
            if (scenarioId == -1) {
                scenarioId = m_scenarioDaoService.insert(
                        *(ResourceManager::getInstance()->getScenarioByHashSum(
                                scenarioHashSum)));
            }

            m_previousScenarioId = std::make_pair<std::string, int>(
                    scenarioHashSum, scenarioId);
        }

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

                QDateTime qDateTime;
                qDateTime.setTime_t(computationDateTime);

                m_computationDatabaseFile += ElemUtils::Formatter()
                        << computationId << ","
                        << qDateTime.toString(Qt::ISODate).toStdString() << ","
                        << scenarioId << '\n';
            }

            m_previousComputationId = std::make_pair<time_t, int>(
                    computationDateTime, computationId);
        }

        GPDKinematic kinematic = result[i].getKinematic();
        int kinematicId = m_gpdKinematicDaoService.getIdByKinematicObject(
                kinematic);

        if (kinematicId == -1) {
            m_lastGPDKinematicId++;
            kinematicId = m_lastGPDKinematicId;

            m_gpdKinematicDatabaseFile += ElemUtils::Formatter()
                    << m_lastGPDKinematicId << "," << kinematic.getX() << ","
                    << kinematic.getXi() << "," << kinematic.getT() << ","
                    << kinematic.getMuF2() << "," << kinematic.getMuR2()
                    << '\n';
        }

        m_lastGPDResultId++;
        m_gpdResultDatabaseFile += ElemUtils::Formatter() << m_lastGPDResultId
                << "," << result[i].getComputationModuleName() << ","
                << kinematicId << "," << computationId << '\n';

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

            std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributionList =
                    (it->second).getQuarkDistributions();

            for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it_qd =
                    quarkDistributionList.begin();
                    it_qd != quarkDistributionList.end(); it_qd++) {

                // insert QuarkDistribution object
                m_lastQuarkDistributionId++;

                m_quark_distribution_table += ElemUtils::Formatter()
                        << m_lastQuarkDistributionId << "," << (it_qd->first)
                        << "," << (it_qd->second).getQuarkDistribution() << ","
                        << (it_qd->second).getQuarkDistributionMinus() << ","
                        << (it_qd->second).getQuarkDistributionPlus() << '\n';

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

//    warn(__func__, ElemUtils::Formatter() << m_gpdResultDatabaseFile << '\n');
//    warn(__func__, ElemUtils::Formatter() << m_computationDatabaseFile << '\n');

    ElemUtils::FileUtils::write(
            "/home/debian/temp/gpdKinematicDatabaseFile.csv",
            m_gpdKinematicDatabaseFile);
    ElemUtils::FileUtils::write("/home/debian/temp/gpdResultDatabaseFile.csv",
            m_gpdResultDatabaseFile);
    ElemUtils::FileUtils::write("/home/debian/temp/computationDatabaseFile.csv",
            m_computationDatabaseFile);
    ElemUtils::FileUtils::write(
            "/home/debian/temp/parton_distribution_table.csv",
            m_parton_distribution_table);
    ElemUtils::FileUtils::write(
            "/home/debian/temp/quark_distribution_table.csv",
            m_quark_distribution_table);
    ElemUtils::FileUtils::write(
            "/home/debian/temp/parton_distribution_quark_distribution_table.csv",
            m_parton_distribution_quark_distribution_table);
    ElemUtils::FileUtils::write(
            "/home/debian/temp/gpd_result_parton_distribution_table.csv",
            m_gpd_result_parton_distribution_table);

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
            "LOAD DATA INFILE '/home/debian/temp/gpdKinematicDatabaseFile.csv' INTO TABLE gpd_kinematic FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query gpdResultDatabaseFile = "
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
            "LOAD DATA INFILE '/home/debian/temp/parton_distribution_table.csv' INTO TABLE parton_distribution FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query m_parton_distribution_table = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "LOAD DATA INFILE '/home/debian/temp/quark_distribution_table.csv' INTO TABLE quark_distribution FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query m_quark_distribution_table = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "LOAD DATA INFILE '/home/debian/temp/parton_distribution_quark_distribution_table.csv' INTO TABLE parton_distribution_quark_distribution FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query m_parton_distribution_quark_distribution_table = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    if (query.exec(
            "LOAD DATA INFILE '/home/debian/temp/gpd_result_parton_distribution_table.csv' INTO TABLE gpd_result_parton_distribution FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';")) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query m_gpd_result_parton_distribution_table = "
                        << query.executedQuery().toStdString());
    }
    query.clear();

    info(__func__, "Insertion done !");

    if (ElemUtils::FileUtils::remove(
            "/home/debian/temp/computationDatabaseFile.csv")) {
        info(__func__,
                ElemUtils::Formatter()
                        << "File successfully deleted : computationDatabaseFile.csv");
    } else {
        //TODO implementing
    }

    ElemUtils::FileUtils::remove(
            "/home/debian/temp/gpdKinematicDatabaseFile.csv");
    ElemUtils::FileUtils::remove("/home/debian/temp/gpdResultDatabaseFile.csv");
    ElemUtils::FileUtils::remove(
            "/home/debian/temp/parton_distribution_table.csv");
    ElemUtils::FileUtils::remove(
            "/home/debian/temp/quark_distribution_table.csv");
    ElemUtils::FileUtils::remove(
            "/home/debian/temp/parton_distribution_quark_distribution_table.csv");
    ElemUtils::FileUtils::remove(
            "/home/debian/temp/gpd_result_parton_distribution_table.csv");

    return isInserted;
}
