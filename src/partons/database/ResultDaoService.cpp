#include "../../../include/partons/database/ResultDaoService.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>

#include "../../../include/partons/beans/automation/Scenario.h"
#include "../../../include/partons/beans/Computation.h"
#include "../../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/database/DatabaseManager.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/ResourceManager.h"

ResultDaoService::ResultDaoService(const std::string &className) :
        BaseObject(className), m_lastComputationId(-1), m_lastScenarioComputation(
                -1), m_scenario_computation_table(
                ElemUtils::StringUtils::EMPTY), m_computationDatabaseFile(
                ElemUtils::StringUtils::EMPTY), m_previousComputationId(
                std::make_pair<time_t, int>(0, -1)), m_previousScenarioId(
                std::make_pair<std::string, int>(ElemUtils::StringUtils::EMPTY,
                        -1)), m_previousEnvConfId(
                std::make_pair<std::string, int>(ElemUtils::StringUtils::EMPTY,
                        -1)) {

    m_temporaryFolderPath =
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "temporary.working.directory.path");

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());
    if (query.exec("SELECT COUNT(computation_id) FROM computation;")) {
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

    if (query.exec(
            "SELECT COUNT(scenario_computation_id) FROM scenario_computation;")) {
        if (query.first()) {
            m_lastScenarioComputation = query.value(0).toInt();
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

void ResultDaoService::prepareCommonTablesFromResultInfo(
        const ResultInfo& resultInfo) {
    int environmentConfigurationId = -1;
    std::string envConfHashSum =
            Partons::getInstance()->getEnvironmentConfiguration()->getHashSum();

    // Check if previousEnvConfId stored is not the same as current
    if (envConfHashSum == m_previousEnvConfId.first) {
        environmentConfigurationId = m_previousEnvConfId.second;
    } else {
        environmentConfigurationId =
                m_environmentConfigurationDaoService.getEnvironmentConfigurationIdByHashSum(
                        envConfHashSum);

        // If not, insert new entry in database and retrieve its id
        if (environmentConfigurationId == -1) {
            environmentConfigurationId =
                    m_environmentConfigurationDaoService.insert(
                            *(Partons::getInstance()->getEnvironmentConfiguration()));
        }

        m_previousEnvConfId = std::make_pair<std::string, int>(envConfHashSum,
                environmentConfigurationId);
    }

    int scenarioId = -1;
    std::string scenarioHashSum = resultInfo.getScenarioHashSum();

    // Check if previousComputationId stored is not the same as current
    if (scenarioHashSum == m_previousScenarioId.first) {
        scenarioId = m_previousComputationId.second;
    } else {
        scenarioId = m_scenarioDaoService.getScenarioIdByHashSum(
                scenarioHashSum);

        // If not, insert new entry in database and retrieve its id
        if (scenarioId == -1) {
            scenarioId = m_scenarioDaoService.insert(
                    *(ResourceManager::getInstance()->getScenarioByHashSum(
                            scenarioHashSum)));
        }

        m_previousScenarioId = std::make_pair<std::string, int>(scenarioHashSum,
                scenarioId);
    }

    int computationId = -1;
    time_t computationDateTime = resultInfo.getComputation().getDateTime();

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

            m_computationDatabaseFile += ElemUtils::Formatter() << computationId
                    << "," << qDateTime.toString(Qt::ISODate).toStdString()
                    << "," << m_previousEnvConfId.second << '\n';

            // Fill scenario_computation association table.
            m_lastScenarioComputation++;
            m_scenario_computation_table += ElemUtils::Formatter()
                    << m_lastScenarioComputation << ","
                    << resultInfo.getScenarioTaskIndexNumber() << ","
                    << scenarioId << "," << computationId << '\n';

        }

        m_previousComputationId = std::make_pair<time_t, int>(
                computationDateTime, computationId);
    }
}

void ResultDaoService::insertDataIntoDatabaseTables(const std::string& fileName,
        std::string &string, const std::string &tableName) {

    info(__func__,
            ElemUtils::Formatter() << "Filling database table [" << tableName
                    << "]");

    // keep file path
    std::string filePath = ElemUtils::Formatter() << m_temporaryFolderPath
            << "/" << fileName;

    // write string into temporary file
    ElemUtils::FileUtils::write(filePath, string);

    // free string memory
    string = ElemUtils::StringUtils::EMPTY;

    // inject temporary file into right database table
    loadDataInFileIntoTable(fileName, tableName);

    // remove temporary file
    ElemUtils::FileUtils::remove(filePath);
}

void ResultDaoService::loadDataInFileIntoTable(const std::string& fileName,
        const std::string& tableName) {
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    if (query.exec(prepareInsertQuery(fileName, tableName))) {
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query m_scenario_computation_table = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}

void ResultDaoService::insertCommonDataIntoDatabaseTables() {
    insertDataIntoDatabaseTables("computationDatabaseFile.csv",
            m_computationDatabaseFile, "computation");
    insertDataIntoDatabaseTables("scenarioComputationDatabaseFile.csv",
            m_scenario_computation_table, "scenario_computation");
}

QString ResultDaoService::prepareInsertQuery(const std::string &fileName,
        const std::string &tableName) {
    ElemUtils::Formatter formatter;
    formatter << "LOAD DATA INFILE '" << m_temporaryFolderPath << "/"
            << fileName << "' INTO TABLE " << tableName
            << " FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';";

    return QString::fromUtf8(formatter.str().c_str());
}

