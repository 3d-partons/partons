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

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/ResultList.h"
#include "../../common/service/ComputationDaoService.h"
#include "../../parton_distribution/service/PartonDistributionDaoService.h"
#include "../dao/GPDResultDao.h"
#include "GPDKinematicDaoService.h"

class GPDResultDaoService: public BaseObject {
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
     * @return unique id related to the last entry inserted into the database
     */
    int insert(const ResultList<GPDResult> &gpdResultList) const;

    /**
     * Return a list of GPDResult objects from the database identified by a specific computation identifier.
     *
     * @param computationId
     * @return list of GPDResult objects.
     */
    ResultList<GPDResult> getGPDResultListByComputationId(
            const int computationId) const;

private:
    GPDResultDao m_gpdResultDao; ///< reference to the right DAO object to perform database queries

    GPDKinematicDaoService m_gpdKinematicDaoService; ///< reference to be able to store kinematic object related to the result.
    PartonDistributionDaoService m_partonDistributionDaoService; ///< reference to be able to store PartonDistribution object and link it to the result.
    ComputationDaoService m_computationDaoService; ///< reference to be able to generate computationId and store ComputationConfiguration object and EnvironmentConfiguration object related to the result.

    /**
     * Insert into the database a new GPDResult object without using transactions mechanisms.
     * Helpful when dealing with a ResultList<GPDResult> object, because transactions are already performed earlier.
     *
     * @param gpdResult
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(const GPDResult &gpdResult) const;
};

#endif /* GPD_RESULT_DAO_SERVICE */
