#include "QuarkDistributionDaoService.h"

#include "../dao/QuarkDistributionDao.h"

int QuarkDistributionDaoService::insert(int partonDistributionId,
        const QuarkDistribution &quarkDistribution) {
    return QuarkDistributionDao::insert(partonDistributionId,
            quarkDistribution.getQuarkFlavor(),
            quarkDistribution.getQuarkDistribution(),
            quarkDistribution.getQuarkDistributionMinus(),
            quarkDistribution.getQuarkDistributionPlus());
}

int QuarkDistributionDaoService::insert(int partonDistributionId,
        const std::vector<QuarkDistribution> &quarkDistributionList) {
    int result = -1;

    for (unsigned int i = 0; i != quarkDistributionList.size(); i++) {
        result = insert(partonDistributionId, quarkDistributionList[i]);
    }

    return result;
}
