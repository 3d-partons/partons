/*
 * HadronStructureUtils.cpp
 *
 *  Created on: 30 juin 2015
 *      Author: debian
 */

#include "HadronStructureUtils.h"

#include <vector>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../MapUtils.h"
#include "GPDResultReport.h"
#include "PartonDistributionReport.h"

GPDResultReport HadronStructureUtils::compareGPDResults(
        GPDResult* p_gpdResultLeft, GPDResult* p_gpdResultRight,
        double absoluteTolerance, double relativeTolerance) {

    std::vector<GPDType::Type> gpdComputedType = MapUtils::intersectionOfKey<
            GPDType::Type, PartonDistribution>(
            p_gpdResultLeft->getPartonDistributions(),
            p_gpdResultRight->getPartonDistributions());

    for (unsigned int i = 0; i < gpdComputedType.size(); i++) {
        PartonDistributionReport partonDistributionReport =
                HadronStructureUtils::comparePartonDistributions(
                        p_gpdResultLeft->getPartonDistribution(
                                gpdComputedType[i]),
                        p_gpdResultRight->getPartonDistribution(
                                gpdComputedType[i]), 1., 1.);
    }

    return GPDResultReport();
}

PartonDistributionReport HadronStructureUtils::comparePartonDistributions(
        const PartonDistribution &partonDistributionLeft,
        const PartonDistribution &partonDistributionRight,
        double absoluteTolerance, double relativeTolerance) {
    return PartonDistributionReport();
}
