/**
 * @file HadronStructureUtils.h
 * @author: Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 30 juin 2015
 * @version 1.0
 *
 * @class HadronStructureUtils
 *
 * @brief Compare various parton distributions within given tolerances.
 */

#ifndef HADRON_STRUCTURE_UTILS_H
#define HADRON_STRUCTURE_UTILS_H

#include "../../../beans/gpd/GPDKinematic.h"
#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/parton_distribution/GluonDistribution.h"
#include "../../../beans/parton_distribution/QuarkDistribution.h"
#include "../../../beans/ResultList.h"

class GPDResultListReport;

class GluonDistributionReport;
class GPDKinematicReport;
class GPDResultReport;
class PartonDistribution;
class PartonDistributionReport;
class QuarkDistributionReport;
class Tolerances;

class HadronStructureUtils {
public:
    static GPDResultListReport compareGPDResultsLists(
            const ResultList<GPDResult>& lhsGpdResultList,
            const ResultList<GPDResult>& rhsGpdResultList,
            const Tolerances& tolerances);

    static GPDResultReport compareGPDResults(const GPDResult& lhsGpdResult,
            const GPDResult& rhsGpdResult, const Tolerances& tolerances);

    static PartonDistributionReport comparePartonDistributions(
            const PartonDistribution& lhsPartonDistribution,
            const PartonDistribution& rhsPartonDistribution,
            const Tolerances& tolerances);

    static GluonDistributionReport compareGluonDistributions(
            const GluonDistribution& lhsGluonDistribution,
            const GluonDistribution& rhsGluonDistribution,
            const Tolerances& tolerances);

    static QuarkDistributionReport compareQuarkDistributions(
            const QuarkDistribution& lhsQuarDistribution,
            const QuarkDistribution& rhsQuarkDistribution,
            const Tolerances& tolerances);

    static GPDKinematicReport compareGPDKinematics(
            const GPDKinematic& lhsGpdKinematic,
            const GPDKinematic& rhsGpdKinematic, const Tolerances& tolerances);
};

#endif /* HADRON_STRUCTURE_UTILS_H */
