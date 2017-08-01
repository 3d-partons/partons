#include "../../../include/partons/database/ResultDaoService.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <fstream>

#include "../../../include/partons/beans/automation/Scenario.h"
#include "../../../include/partons/beans/Computation.h"
#include "../../../include/partons/beans/List.h"
#include "../../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/database/Database.h"
#include "../../../include/partons/database/DatabaseManager.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/ResourceManager.h"
#include "../../../include/partons/utils/plot2D/Plot2D.h"
#include "../../../include/partons/utils/plot2D/Plot2DList.h"

namespace PARTONS {

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

    m_useTmpFiles = ElemUtils::StringUtils::equals(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "database.load.infile.use"), "true");

    if (m_useTmpFiles) {
        m_temporaryFolderPath =
                ElemUtils::PropertiesManager::getInstance()->getString(
                        "database.load.infile.directory");
    } else {
        m_temporaryFolderPath = ElemUtils::StringUtils::EMPTY;
    }

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare("SELECT COUNT(computation_id) FROM computation");

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    m_lastComputationId = query.value(0).toInt();

    query.clear();

    query.prepare(
            "SELECT COUNT(scenario_computation_id) FROM scenario_computation");

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    m_lastScenarioComputation = query.value(0).toInt();

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

    //check if not empty
    if (string.empty()) {

        //nothing to insert
        return;
    }

    //use temporary files to speed up the transaction
    if (m_useTmpFiles) {

        // keep file path
        std::string filePath = ElemUtils::Formatter() << m_temporaryFolderPath
                << "/" << fileName;

        try {

            // open file
            std::ofstream fileOutputStream;

            if (!ElemUtils::FileUtils::open(fileOutputStream, filePath)) {
                throw ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter() << "Cannot open \"" << filePath
                                << "\"");
            }

            // write and flush
            ElemUtils::FileUtils::writeAndFlush(fileOutputStream, string);

            // close
            ElemUtils::FileUtils::close(fileOutputStream);

            // free string memory
            string = ElemUtils::StringUtils::EMPTY;

            // status
            info(__func__,
                    ElemUtils::Formatter() << "Filling database table ["
                            << tableName << "]");

            // inject temporary file into right database table
            loadDataIntoTable(fileName, tableName);

            // remove temporary file
            ElemUtils::FileUtils::remove(filePath);
        }

        // if something wrong happened
        catch (const ElemUtils::CustomException &e) {

            // remove temporary file
            if (ElemUtils::FileUtils::isReadable(filePath)) {
                ElemUtils::FileUtils::remove(filePath);
            }

            // throw again the same exception to propagate the error and allow other method to perform their clean (ex : transaction/rollback/...)
            throw ElemUtils::CustomException(e);
        }

    } else {

        // status
        info(__func__,
                ElemUtils::Formatter() << "Filling database table ["
                        << tableName << "]");

        // inject query into right database table
        loadDataIntoTable(string, tableName);

        // free string memory
        string = ElemUtils::StringUtils::EMPTY;
    }
}

void ResultDaoService::loadDataIntoTable(const std::string& inputData,
        const std::string& tableName) {
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    if (query.exec(prepareInsertQuery(inputData, tableName))) {
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
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

QString ResultDaoService::prepareInsertQuery(const std::string &inputData,
        const std::string &tableName) {

    ElemUtils::Formatter formatter;

    //use temporary files to speed up the transaction
    if (m_useTmpFiles) {

        //set
        formatter << "LOAD DATA LOCAL INFILE '" << m_temporaryFolderPath << "/"
                << inputData << "' INTO TABLE " << tableName
                << " FIELDS TERMINATED BY ',' LINES TERMINATED BY '\n';";

    } else {

        //set
        formatter << "INSERT INTO " << tableName << " VALUES";

        //loop over lines
        std::istringstream input(inputData);
        std::string line;

        bool first1 = true;

        while (std::getline(input, line)) {

            if (first1) {
                formatter << " (";
                first1 = false;
            } else {
                formatter << " ,(";
            }

            //get words
            std::istringstream ss(line);
            std::string token;

            bool first2 = true;

            while (std::getline(ss, token, ',')) {

                if (first2) {
                    formatter << "'" << token << "'";
                    first2 = false;
                } else {
                    formatter << ",'" << token << "'";
                }
            }

            formatter << ")";
        }

        formatter << ";";
    }

    return QString::fromUtf8(formatter.str().c_str());
}

int ResultDaoService::getLastComputationId() const {
    return m_lastComputationId;
}

Plot2DList ResultDaoService::getPlot2DListFromCustomQuery(
        const std::string& sqlQuery) {
    Plot2DList plot2DList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(QString(sqlQuery.c_str()));

    Database::checkManyResults("ResultDaoService", __func__,
            Database::execSelectQuery(query), query);

    if (query.first()) {
        do {
            plot2DList.add(
                    Plot2D(query.value(0).toDouble(),
                            query.value(1).toDouble()));
        } while (query.next());
    } else {
        //TODO print warning
    }

    query.clear();

    return plot2DList;
}

} /* namespace PARTONS */
