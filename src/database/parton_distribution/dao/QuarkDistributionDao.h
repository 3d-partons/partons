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

#include "../../../BaseObject.h"

class QuarkDistributionDao: public BaseObject {
public:
    QuarkDistributionDao();
    virtual ~QuarkDistributionDao();

    int insert(double quarkDistributionPlus, double quarkDistributionMinus,
            double quarkDistribution, int quarkFlavorTypeId,
            int partonDistributionId) const;
};

#endif /* QUARK_DISTRIBUTION_DAO */
