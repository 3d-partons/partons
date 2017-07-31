#ifndef RESULT_DAO_SERVICE_H
#define RESULT_DAO_SERVICE_H

/**
 * @file ResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date May 25, 2016
 * @version 1.0
 */

#include <ctime>
#include <string>
#include <utility>

#include "common/service/ComputationDaoService.h"
#include "common/service/EnvironmentConfigurationDaoService.h"
#include "common/service/ScenarioDaoService.h"

namespace PARTONS {

class Plot2DList;
class ResultInfo;

/**
 * @class ResultDaoService
 *
 * @brief Use temporary CSV file before insert data into database.
 *
 * If the switch `database.load.infile.use` is set to `false` in `partons.properties`, then the temporary file is never written,
 * and the `infile` mechanism is not used.
 */
class ResultDaoService: public BaseObject {
public:

    /**
     * Constructor.
     * @param className Name of class.
     */
    ResultDaoService(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ResultDaoService();

    /**
     * Get value of ResultDaoService::m_lastComputationId.
     * @return Requested value.
     */
    int getLastComputationId() const;

    /**
     * Get data for x vs. y plot from given SQL query being like "SELECT x,y FROM ...".
     * @param sqlQuery Input QSqlQuery query.
     * @return Data as Plot2DList object.
     */
    static Plot2DList getPlot2DListFromCustomQuery(const std::string &sqlQuery);

protected:

    std::pair<time_t, int> m_previousComputationId; ///< Unique id of the last processed computation information with corresponding time.

    /**
     * Prepare computation information and scenario information to be inserted into database.
     * @param resultInfo
     */
    void prepareCommonTablesFromResultInfo(const ResultInfo &resultInfo);

    /**
     * Insert computation information and scenario information via temporary files
     * (if the switch is set to `true`, otherwise no temporary file is used).
     */
    void insertCommonDataIntoDatabaseTables();

    /**
     * Insert information stored in string into given table in the database via temporary file
     * (if the switch is set to `true`, otherwise no temporary file is used).
     * @param fileName Name of temporary file.
     * @param string String containing input information.
     * @param tableName Name of target table.
     */
    void insertDataIntoDatabaseTables(const std::string &fileName,
            std::string &string, const std::string &tableName);

    /**
     * Insert information stored in file (if the switch is set to `true`,
     * otherwise no temporary file is used) into given table in the database.
     * @param inputData Input information.
     * @param tableName Name of target table.
     */
    void loadDataIntoTable(const std::string &inputData,
            const std::string &tableName);

    /**
     * Prepare INSERT-like SQL query for given table and file containing input information
     * (if the switch is set to `true`, otherwise no temporary file is used).
     * @param inputData Input information.
     * @param tableName Name of target table.
     * @return String containing INSERT-like SQL query.
     */
    QString prepareInsertQuery(const std::string &inputData,
            const std::string &tableName);

private:

    std::string m_temporaryFolderPath; ///< Path to directory storing temporary files.
    bool m_useTmpFiles; ///< Switch indicating the usage of temporary files to speed up the transaction.

    int m_lastComputationId; ///< Last unique id in table storing computation information in the database.
    int m_lastScenarioComputation; ///< Last unique id in table storing scenario information in the database.

    std::string m_computationDatabaseFile; ///< String containing computation information to be stored in the database via temporary file.
    std::string m_scenario_computation_table; ///< String containing scenario information to be stored in the database via temporary file.

    /**
     * ComputationDaoService object to perform database queries.
     */
    ComputationDaoService m_computationDaoService;

    /**
     * ScenarioDaoService object to perform database queries.
     */
    ScenarioDaoService m_scenarioDaoService;

    /**
     * EnvironmentConfigurationDaoService object to perform database queries.
     */
    EnvironmentConfigurationDaoService m_environmentConfigurationDaoService;

    std::pair<std::string, int> m_previousScenarioId; ///< Unique id of the last processed scenario with corresponding hash sum.
    std::pair<std::string, int> m_previousEnvConfId; ///<Unique id of the last processed environment configuration with corresponding hash sum.
};

} /* namespace PARTONS */

#endif /* RESULT_DAO_SERVICE_H */
