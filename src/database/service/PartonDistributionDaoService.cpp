#include "PartonDistributionDaoService.h"

//#include "../../beans/parton_distribution/GluonDistribution.h"
//#include "../dao/PartonDistributionDao.h"

int PartonDistributionDaoService::insert(int gpdResultId,
        const PartonDistribution &partonDistribution) {
//    return PartonDistributionDao::insert(gpdResultId,
//            partonDistribution.getGpdType(),
//            partonDistribution.getGluonDistribution().getGluonDistribution());

//TODO re-implement

    return 0;
}

int PartonDistributionDaoService::insert(int gpdResultId,
        const std::vector<PartonDistribution> &partonDistributionList) {
    int result = -1;

    for (unsigned int i = 0; i != partonDistributionList.size(); i++) {
        result = insert(gpdResultId, partonDistributionList[i]);
    }

    return result;
}
