#ifndef GPD_RESULT_DAO_SERVICE
#define GPD_RESULT_DAO_SERVICE

/**
 * @file GPDResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 13, 2015
 * @version 1.0
 *
 * @class GPDResultDaoService
 *
 * @brief Its role is to deal with GPDResult C++ object and related tables from the database.
 * You can insert, select or remove results from database by many kind of stuff.
 * Therefore database services ensure the integrity of the database by using transaction and rollback mechanisms if something wrong append : the database will stay always in a stable state.
 * And they improve querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#include <string>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/List.h"
#include "../../common/service/ComputationDaoService.h"
#include "../../common/service/ResultInfoDaoService.h"
#include "../../parton_distribution/service/PartonDistributionDaoService.h"
#include "../../ResultDaoService.h"
#include "../dao/GPDResultDao.h"
#include "GPDKinematicDaoService.h"

class GPDResultDaoService: public ResultDaoService {
public:
    /**
     * Default constructor
     */
    GPDResultDaoService();

    /**
     * Default destructor
     */
    virtual ~ GPDResultDaoService();

    /**
     * Insert into database a new GPDResult object with transactions mechanisms.
     *
     * @param gpdResult
     * @return unique id related to the new entry inserted into the database
     */
    int insert(const GPDResult &gpdResult) const;

    /**
     * Insert into database a list of GPDResult objects with transactions mechanisms.
     *
     * @param gpdResultList
     * @return
     */
    bool insert(const List<GPDResult> &gpdResultList);

    /**
     * Return a list of GPDResult objects from the database identified by a specific computation identifier.
     *
     * @param computationId
     * @return list of GPDResult objects.
     */
    List<GPDResult> getGPDResultListByComputationId(
            const int computationId) const;

private:
    int m_lastGPDKinematicId;
    int m_lastGPDResultId;
    int m_lastPartonDistributionId;
    int m_lastQuarkDistributionId;
    int m_lastPartonDistributionQuarkDistributionId;
    int m_lastGPDResultPartonDistributionId;

    std::string m_gpdResultDatabaseFile;
    std::string m_gpdKinematicDatabaseFile;
    std::string m_parton_distribution_table;
    std::string m_gpd_result_parton_distribution_table;
    std::string m_parton_distribution_quark_distribution_table;
    std::string m_quark_distribution_table;

    GPDResultDao m_gpdResultDao; ///< reference to the right DAO object to perform database queries

    GPDKinematicDaoService m_gpdKinematicDaoService; ///< reference to be able to store kinematic object related to the result.
    PartonDistributionDaoService m_partonDistributionDaoService; ///< reference to be able to store PartonDistribution object and link it to the result.

//    /**
//     * Insert into the database a new GPDResult object without using transactions mechanisms.
//     * Helpful when dealing with a ResultList<GPDResult> object, because transactions are already performed earlier.
//     *
//     * @param gpdResult
//     * @return unique id related to the new entry inserted into the database
//     */
//    int insertWithoutTransaction(const GPDResult &gpdResult) const;
};

#endif /* GPD_RESULT_DAO_SERVICE */
