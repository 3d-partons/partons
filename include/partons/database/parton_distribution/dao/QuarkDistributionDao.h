#ifndef QUARK_DISTRIBUTION_DAO
#define QUARK_DISTRIBUTION_DAO

/**
 * @file QuarkDistributionDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>

#include "../../../beans/parton_distribution/QuarkDistribution.h"

namespace PARTONS {

/**
 * @class QuarkDistributionDao
 *
 * @brief Quark distribution Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete quark distributions from the database. This class in not intended to be used by regular users who should deal with QuarkDistributionDaoService instead.
 */
class QuarkDistributionDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    QuarkDistributionDao();

    /**
     * Destructor.
     */
    virtual ~QuarkDistributionDao();

    /**
     * Insert quark distribution into the database.
     * @param quarkDistributionPlus Quark distribution singlet value.
     * @param quarkDistributionMinus Quark distribution non-singlet value.
     * @param quarkDistribution Quark distribution value.
     * @param quarkFlavorTypeId Quark flavor, see QuarkFlavorType::Type.
     * @param partonDistributionId Unique id of row in the database representing PartonDistribution object associated to this quark distribution.
     * @return Unique id of inserted row in the database.
     */
    int insert(double quarkDistributionPlus, double quarkDistributionMinus,
            double quarkDistribution, int quarkFlavorTypeId,
            int partonDistributionId) const;

    /**
     * Retrieve quark distribution from the database by given unique id of row.
     * @param quarkDistributionId Unique id of row in the database to be selected.
     * @return QuarkDistribution object containing quark distribution retrieved from the database.
     */
    QuarkDistribution getQuarkDistributionById(
            const int quarkDistributionId) const;

private:

    /**
     * Fill QuarkDistribution object from given query.
     * @param quarkDistribution QuarkDistribution object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillQuarkDistributionFromQuery(QuarkDistribution &quarkDistribution,
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* QUARK_DISTRIBUTION_DAO */
