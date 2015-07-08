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

#ifndef HADRONSTRUCTUREUTILS_H_
#define HADRONSTRUCTUREUTILS_H_

class GPDResultList;
class GPDResultListReport;
class Tolerances;
class PartonDistribution;
class PartonDistributionReport;
class GPDResult;
class GPDResultReport;
class QuarkDistribution;
class QuarkDistributionReport;
class GluonDistribution;
class GluonDistributionReport;

class HadronStructureUtils {
public:
    static GPDResultListReport compareGPDResultsLists(
            const GPDResultList& lhsGpdResultList,
            const GPDResultList& rhsGpdResultList,
            const Tolerances& tolerances);

    static GPDResultReport compareGPDResults(const GPDResult& lhsGpdResult,
            const GPDResult& rhsGpdResult, const Tolerances& tolerances);

    static PartonDistributionReport comparePartonDistributions(
            const PartonDistribution& lhsPartonDistribution,
            const PartonDistribution& rhsPartonDistribution,
            const Tolerances& tolerances);

    static GluonDistributionReport compareGluonDistributions(
            const GluonDistribution& lhsGluonDistribution,
            const GluonDistribution& rhsGluonDistribution, const Tolerances& tolerances);

    static QuarkDistributionReport compareQuarkDistributions(
            const QuarkDistribution& lhsQuarDistribution,
            const QuarkDistribution& rhsQuarkDistribution,
            const Tolerances& tolerances);
};

#endif /* HADRONSTRUCTUREUTILS_H_ */
