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

class QuarkDistributionDao {
public:
    static int insert(int partonDistributionId, int quarkFlavorType,
            double quarkDistribution, double quarkDistributionMinus,
            double quarkDistributionPlus);

private:

};

#endif /* QUARK_DISTRIBUTION_DAO */
