/*
 * ResultDaoService.h
 *
 *  Created on: May 25, 2016
 *      Author: debian
 */

#ifndef RESULTDAOSERVICE_H_
#define RESULTDAOSERVICE_H_

#include <ctime>
#include <string>
#include <utility>

#include "../beans/gpd/GPDResult.h"
#include "../beans/List.h"
#include "common/service/ComputationDaoService.h"
#include "common/service/ResultInfoDaoService.h"
#include "common/service/ScenarioDaoService.h"
#include "gpd/dao/GPDResultDao.h"
#include "gpd/service/GPDKinematicDaoService.h"
#include "parton_distribution/service/PartonDistributionDaoService.h"

class ResultDaoService: public BaseObject {
public:
    ResultDaoService();
    virtual ~ResultDaoService();

    bool insert(const List<GPDResult> &result);

private:
    int m_lastComputationId;
    int m_lastGPDKinematicId;
    int m_lastGPDResultId;
    int m_lastPartonDistributionId;
    int m_lastQuarkDistributionId;
    int m_lastPartonDistributionQuarkDistributionId;
    int m_lastGPDResultPartonDistributionId;

    std::string m_gpdResultDatabaseFile;
    std::string m_gpdKinematicDatabaseFile;
    std::string m_computationDatabaseFile;
    std::string m_parton_distribution_table;
    std::string m_gpd_result_parton_distribution_table;
    std::string m_parton_distribution_quark_distribution_table;
    std::string m_quark_distribution_table;

    GPDResultDao m_gpdResultDao; ///< reference to the right DAO object to perform database queries

    GPDKinematicDaoService m_gpdKinematicDaoService; ///< reference to be able to store kinematic object related to the result.
    PartonDistributionDaoService m_partonDistributionDaoService; ///< reference to be able to store PartonDistribution object and link it to the result.
    ResultInfoDaoService m_resultInfoDaoService;

    //TODO remove unused member.
    ComputationDaoService m_computationDaoService; ///< reference to be able to generate computationId and store ComputationConfiguration object and EnvironmentConfiguration object related to the result.
    ScenarioDaoService m_scenarioDaoService;

    /**
     * Insert into the database a new GPDResult object without using transactions mechanisms.
     * Helpful when dealing with a ResultList<GPDResult> object, because transactions are already performed earlier.
     *
     * @param gpdResult
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(const GPDResult &gpdResult) const;

    mutable std::pair<time_t, int> m_previousComputationId;
    mutable std::pair<std::string, int> m_previousScenarioId;
};

#endif /* RESULTDAOSERVICE_H_ */
