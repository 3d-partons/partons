#ifndef PARTON_DISTRIBUTION_DAO_SERVICE
#define PARTON_DISTRIBUTION_DAO_SERVICE

/**
 * @file PartonDistributionDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class PartonDistributionDaoService
 *
 * @brief
 */

#include <vector>

#include "../../beans/parton_distribution/PartonDistribution.h"

class PartonDistributionDaoService {
public:
    static int insert(int gpdResultId,
            const PartonDistribution &partonDistribution);
    static int insert(int gpdResultId,
            const std::vector<PartonDistribution> &partonDistributionList);
};

#endif /* PARTON_DISTRIBUTION_DAO_SERVICE */
