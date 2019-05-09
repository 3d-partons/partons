#ifndef PARTON_DISTRIBUTION_DAO_SERVICE
#define PARTON_DISTRIBUTION_DAO_SERVICE

/**
 * @file PartonDistributionDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 */

#include "../dao/PartonDistributionDao.h"
#include "../dao/QuarkDistributionDao.h"

namespace PARTONS {

class PartonDistribution;

/**
 * @class PartonDistributionDaoService
 *
 * @brief Parton distribution Data Access Object (DAO) service.
 *
 * It deals with PartonDistribution C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove parton distribution from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */
class PartonDistributionDaoService: public BaseObject {
public:

    /**
     * Default constructor.
     */
    PartonDistributionDaoService();

    /**
     * Destructor.
     */
    virtual ~PartonDistributionDaoService();

    /**
     * Insert into database a new PartonDistribution object with transactions mechanisms.
     * @param partonDistribution Reference to PartonDistribution object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const PartonDistribution &partonDistribution) const;

    /**
     * Insert into database a new PartonDistribution object without transactions mechanisms.
     * @param partonDistribution Reference to PartonDistribution object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(
            const PartonDistribution &partonDistribution) const;

private:

    /**
     * PartonDistributionDao object to perform database queries.
     */
    PartonDistributionDao m_partonDistributionDao;

    /**
     * QuarkDistributionDao object to perform database queries.
     */
    QuarkDistributionDao m_quarkDistributionDao;
};

} /* namespace PARTONS */

#endif /* PARTON_DISTRIBUTION_DAO_SERVICE */
