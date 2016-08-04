#ifndef RESULT_DAO_SERVICE_H
#define RESULT_DAO_SERVICE_H

/**
 * @file ResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date May 25, 2016
 * @version 1.0
 *
 * @class ResultDaoService
 *
 * @brief
 */

#include <ctime>
#include <string>
#include <utility>

#include "../beans/gpd/GPDResult.h"
#include "../beans/List.h"
#include "common/service/ComputationDaoService.h"
#include "common/service/EnvironmentConfigurationDaoService.h"
#include "common/service/ScenarioDaoService.h"

class Plot2DList;

class ResultDaoService: public BaseObject {
public:
    ResultDaoService(const std::string &className);
    virtual ~ResultDaoService();

    bool insert(const List<GPDResult> &result);

    int getLastComputationId() const;

protected:
    std::pair<time_t, int> m_previousComputationId;

    void prepareCommonTablesFromResultInfo(const ResultInfo &resultInfo);

    void insertCommonDataIntoDatabaseTables();

    void insertDataIntoDatabaseTables(const std::string &fileName,
            std::string &string, const std::string &tableName);

    void loadDataInFileIntoTable(const std::string &fileName,
            const std::string &tableName);

    QString prepareInsertQuery(const std::string &fileName,
            const std::string &tableName);

    Plot2DList getPlot2DListFromCustomQuery(const std::string &sqlQuery) const;

private:
    std::string m_temporaryFolderPath;

    int m_lastComputationId;
    int m_lastScenarioComputation;

    std::string m_computationDatabaseFile;
    std::string m_scenario_computation_table;

    //TODO remove unused member.
    ComputationDaoService m_computationDaoService; ///< reference to be able to generate computationId and store ComputationConfiguration object and EnvironmentConfiguration object related to the result.
    ScenarioDaoService m_scenarioDaoService;
    EnvironmentConfigurationDaoService m_environmentConfigurationDaoService;

    std::pair<std::string, int> m_previousScenarioId;
    std::pair<std::string, int> m_previousEnvConfId;
};

#endif /* RESULT_DAO_SERVICE_H */
