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

class Tolerances;
class PartonDistribution;
class PartonDistributionReport;
class GPDResult;
class GPDResultReport;
class QuarkDistributionReport;

class HadronStructureUtils {
public:
    static GPDResultReport compareGPDResults(GPDResult* p_lhsGpdResult,
            GPDResult* p_rhsGpdResult, Tolerances& tolerances);

    static PartonDistributionReport comparePartonDistributions(
            const PartonDistribution &lhsPartonDistribution,
            const PartonDistribution &rhsPartonDistribution,
            Tolerances& tolerances);

    static QuarkDistributionReport compareQuarkDistribution(
            const QuarkDistribution& lhsQuarDistribution,
            const QuarkDistribution& rhsQuarkDistribution,
            Tolerances& tolerances);
};

#endif /* HADRONSTRUCTUREUTILS_H_ */
