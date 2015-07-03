/*
 * HadronStructureUtils.h
 *
 *  Created on: 30 juin 2015
 *      Author: debian
 */

#ifndef HADRONSTRUCTUREUTILS_H_
#define HADRONSTRUCTUREUTILS_H_

#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../../beans/parton_distribution/QuarkDistribution.h"

class PartonDistribution;
class PartonDistributionReport;
class GPDResult;
class GPDResultReport;
class QuarkDistributionReport;

class HadronStructureUtils {
public:
    static GPDResultReport compareGPDResults(GPDResult* p_lhsGpdResult,
            GPDResult* p_rhsGpdResult, double absoluteTolerance,
            double relativeTolerance);

    static PartonDistributionReport comparePartonDistributions(
            const PartonDistribution &lhsPartonDistribution,
            const PartonDistribution &rhsPartonDistribution,
            double absoluteTolerance, double relativeTolerance);

    static QuarkDistributionReport compareQuarkDistribution(
            const QuarkDistribution& lhsQuarDistribution,
            const QuarkDistribution& rhsQuarkDistribution,
            double absoluteTolerance, double relativeTolerance);
};

#endif /* HADRONSTRUCTUREUTILS_H_ */
