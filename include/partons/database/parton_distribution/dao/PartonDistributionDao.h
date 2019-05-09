#ifndef PARTON_DISTRIBUTION_DAO
#define PARTON_DISTRIBUTION_DAO

/**
 * @file PartonDistributionDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>

#include "QuarkDistributionDao.h"

namespace PARTONS {
class PartonDistribution;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class PartonDistributionDao
 *
 * @brief Parton distribution Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete parton distributions from the database. This class in not intended to be used by regular users who should deal with PartonDistributionDaoService instead.
 */
class PartonDistributionDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    PartonDistributionDao();

    /**
     * Destructor.
     */
    virtual ~PartonDistributionDao();

    /**
     * Insert gluon distribution into the database.
     * @param gluonDistributionValue Gluon distribution value.
     * @return Unique id of inserted row in the database.
     */
    int insert(double gluonDistributionValue) const;

    /**
     * Associate row in the database representing QuarkDistribution to that representing PartonDistribution.
     * @param partonDistributionId Unique id of row in the database representing PartonDistribution object.
     * @param quarkDistributionId Unique id of row in the database representing QuarkDistribution object.
     * @return Unique id of inserted row in the database.
     */
    int insertIntoPartonDistributionQuarkDistributionTable(
            const int partonDistributionId,
            const int quarkDistributionId) const;

    /**
     * Retrieve parton distribution from the database by given unique id of row.
     * @param partonDistributionId Unique id of row in the database to be selected.
     * @return PartonDistribution object containing parton distribution retrieved from the database.
     */
    PartonDistribution getPartonDistributionById(
            const int partonDistributionId) const;

private:

    /**
     * QuarkDistributionDao object to perform database queries.
     */
    QuarkDistributionDao m_quarkDistributionDao;

    /**
     * Fill PartonDistribution object from given query.
     * @param partonDistribution PartonDistribution object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillPartonDistributionFromQuery(PartonDistribution &partonDistribution,
            QSqlQuery &query) const;

    /**
     * Fill PartonDistribution object from the database by given unique id.
     * @param partonDistribution PartonDistribution object to be filled.
     * @param partonDistributionId Unique id to be selected.
     */
    void fillPartonDistribution(PartonDistribution &partonDistribution,
            const int partonDistributionId) const;
};

} /* namespace PARTONS */

#endif /* PARTON_DISTRIBUTION_DAO */
