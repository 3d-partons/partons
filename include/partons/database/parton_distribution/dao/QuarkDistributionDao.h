#ifndef QUARK_DISTRIBUTION_DAO
#define QUARK_DISTRIBUTION_DAO

/**
 * @file QuarkDistributionDao
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class QuarkDistributionDao
 *
 * @brief
 */

#include <QSqlQuery>

#include "../../../beans/parton_distribution/QuarkDistribution.h"
#include "../../../BaseObject.h"

class QuarkDistributionDao: public BaseObject {
public:
    QuarkDistributionDao();
    virtual ~QuarkDistributionDao();

    int insert(double quarkDistributionPlus, double quarkDistributionMinus,
            double quarkDistribution, int quarkFlavorTypeId,
            int partonDistributionId) const;

    QuarkDistribution getQuarkDistributionById(
            const int quarkDistributionId) const;

private:

    void fillQuarkDistributionFromQuery(QuarkDistribution &quarkDistribution,
            QSqlQuery &query) const;
};

#endif /* QUARK_DISTRIBUTION_DAO */
