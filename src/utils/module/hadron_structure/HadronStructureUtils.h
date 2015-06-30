/*
 * HadronStructureUtils.h
 *
 *  Created on: 30 juin 2015
 *      Author: debian
 */

#ifndef HADRONSTRUCTUREUTILS_H_
#define HADRONSTRUCTUREUTILS_H_
class PartonDistribution;
class PartonDistributionReport;

class GPDResult;
class GPDResultReport;

class HadronStructureUtils {
public:
    static GPDResultReport compareGPDResults(GPDResult* p_gpdResultLeft,
            GPDResult* p_gpdResultRight, double absoluteTolerance,
            double relativeTolerance);

    static PartonDistributionReport comparePartonDistributions(const PartonDistribution &partonDistributionLeft,
            const PartonDistribution &partonDistributionRight, double absoluteTolerance,
            double relativeTolerance);
};

#endif /* HADRONSTRUCTUREUTILS_H_ */
