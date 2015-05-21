#ifndef QUARK_DISTRIBUTION_DAO_SERVICE
#define QUARK_DISTRIBUTION_DAO_SERVICE

/**
 * @file QuarkDistributionDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class QuarkDistributionDaoService
 *
 * @brief
 */

#include <vector>

#include "../../beans/parton_distribution/QuarkDistribution.h"

class QuarkDistributionDaoService {
public:
    static int insert(int partonDistributionId,
            const QuarkDistribution &quarkDistribution);

    static int insert(int partonDistributionId,
            const std::vector<QuarkDistribution> &quarkDistributionList);
};

#endif /* QUARK_DISTRIBUTION_DAO_SERVICE */
