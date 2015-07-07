#include "HadronStructureUtils.h"

#include <map>
#include <vector>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/gpd/GPDResultList.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/parton_distribution/GluonDistribution.h"
#include "../../../beans/QuarkFlavor.h"
#include "../../math/MathUtils.h"
#include "../../math/Tolerances.h"
#include "../../MapUtils.h"
#include "../../test/DoubleComparisonReport.h"
#include "../../test/report/GluonDistributionReport.h"
#include "../../test/report/GPDResultListReport.h"
#include "../../test/report/GPDResultReport.h"
#include "../../test/report/PartonDistributionReport.h"
#include "../../test/report/QuarkDistributionReport.h"

class Partons;

GPDResultListReport HadronStructureUtils::compareGPDResultsLists(
        const GPDResultList& lhsGpdResultList, const GPDResultList& rhsGpdResultList,
        const Tolerances& tolerances) {

    bool comparableLists = false;
    GPDResultListReport gpdResultListReport;
    GPDResultReport gpdResultReport;
    if (lhsGpdResultList.getSize() == rhsGpdResultList.getSize()) {
        comparableLists = true;
        for (unsigned int i = 0; i < lhsGpdResultList.getSize(); i++) {
            gpdResultReport = HadronStructureUtils::compareGPDResults(
                    lhsGpdResultList.get(i), rhsGpdResultList.get(i),
                    tolerances);
            gpdResultListReport.add(gpdResultReport);
        }
        gpdResultListReport.setSameSize(comparableLists);
    }

    return gpdResultListReport;
}

GPDResultReport HadronStructureUtils::compareGPDResults(const GPDResult& lhsGpdResult,
        const GPDResult& rhsGpdResult, const Tolerances& tolerances) {

    bool comparableGPD = false;
    bool isEqualPartonDistributions = false;
    PartonDistributionReport partonDistributionReport(GPDType::UNDEFINED);
    GPDResultReport gpdResultReport;

    // Retrieve vector of common GPDs
    std::vector<GPDType::Type> gpdType = MapUtils::intersectionOfKey<
            GPDType::Type, PartonDistribution>(
            lhsGpdResult.getPartonDistributions(),
            rhsGpdResult.getPartonDistributions());

    // Compare corresponding parton distributions (if there are any in common)
    if (gpdType.size() > 0) {
        comparableGPD = true;
        for (unsigned int i = 0; i < gpdType.size(); i++) {
            partonDistributionReport =
                    HadronStructureUtils::comparePartonDistributions(
                            lhsGpdResult.getPartonDistribution(gpdType.at(i)),
                            rhsGpdResult.getPartonDistribution(gpdType.at(i)),
                            tolerances);
            isEqualPartonDistributions = isEqualPartonDistributions
                    && partonDistributionReport.isEqual();
            gpdResultReport.addPartonDistributionReport(gpdType.at(i),
                    partonDistributionReport);
        }
    }
    gpdResultReport.setComparisonResult(isEqualPartonDistributions);

    return gpdResultReport;
}

PartonDistributionReport HadronStructureUtils::comparePartonDistributions(
        const PartonDistribution &lhsPartonDistribution,
        const PartonDistribution &rhsPartonDistribution,
        const Tolerances& tolerance) {

    bool comparableGluons = false;
    bool comparableQuarkFlavors = false;
    bool isEqualQuarkDistributions = false;
    GluonDistributionReport gluonDistributionReport;
    PartonDistributionReport partonDistributionReport;

    // Check that the comparison involves the same GPD in lhs and rhs
    if (lhsPartonDistribution.getGpdType()
            == rhsPartonDistribution.getGpdType()) {

        // Retrieve and test definition of gluon distributions
        GluonDistribution lhsGluonDistribution =
                lhsPartonDistribution.getGluonDistribution();
        GluonDistribution rhsGluonDistribution =
                rhsPartonDistribution.getGluonDistribution();
        if ((lhsGluonDistribution.isNullObject() == true
                && rhsGluonDistribution.isNullObject() == true)
                || (lhsGluonDistribution.isNullObject() == false
                        && rhsGluonDistribution.isNullObject() == false)) {

            comparableGluons = true;
            gluonDistributionReport = MathUtils::compare(
                    lhsGluonDistribution.getGluonDistribution(),
                    rhsGluonDistribution.getGluonDistribution(), tolerance);
            partonDistributionReport.setGluonDistributionReport(
                    gluonDistributionReport);
        }
    }

    // Retrieve the list of computed quark flavors and identify common elements
    std::map<QuarkFlavor::Type, QuarkDistribution> lhsQuarkDistributions =
            lhsPartonDistribution.getQuarkDistributions();
    std::map<QuarkFlavor::Type, QuarkDistribution> rhsQuarkDistributions =
            rhsPartonDistribution.getQuarkDistributions();
    std::vector<QuarkFlavor::Type> commonQuarkFlavors =
            MapUtils::intersectionOfKey(lhsQuarkDistributions,
                    rhsQuarkDistributions);

    // Compare quark distributions for common flavors (if existing)
    if (commonQuarkFlavors.size() > 0) {
        comparableQuarkFlavors = true;
        QuarkDistribution lhsQuarkDistribution(QuarkFlavor::UNDEFINED),
                rhsQuarkDistribution(QuarkFlavor::UNDEFINED);
        GluonDistribution lhsGluonDistribution, rhsGluonDistribution;
        QuarkDistributionReport quarkDistributionReport;
        for (unsigned int i = 0; i < commonQuarkFlavors.size(); i++) {
            lhsQuarkDistribution = lhsPartonDistribution.getQuarkDistribution(
                    commonQuarkFlavors.at(i));
            rhsQuarkDistribution = rhsPartonDistribution.getQuarkDistribution(
                    commonQuarkFlavors.at(i));
            quarkDistributionReport = compareQuarkDistributions(
                    lhsQuarkDistribution, rhsQuarkDistribution, tolerance);
            partonDistributionReport.addQuarkDistributionReport(
                    quarkDistributionReport);
            isEqualQuarkDistributions = isEqualQuarkDistributions
                    && quarkDistributionReport.isEqual();
        }
    }

    if (comparableGluons == true || comparableQuarkFlavors == true) {
        partonDistributionReport.setCommonPartonType(true);
        if (gluonDistributionReport.isEqual()
                && isEqualQuarkDistributions == true)
            partonDistributionReport.setComparisonResult(true);
    }

    return partonDistributionReport;
}

QuarkDistributionReport HadronStructureUtils::compareQuarkDistributions(
        const QuarkDistribution& lhsQuarkDistribution,
        const QuarkDistribution& rhsQuarkDistribution, const Tolerances& tolerance) {

    bool comparisonResult = false;
    DoubleComparisonReport quarkDistributionReport, quarkDistributionPlusReport,
            quarkDistributionMinusReport;

    if (lhsQuarkDistribution.getQuarkFlavor()
            == rhsQuarkDistribution.getQuarkFlavor()) {
        quarkDistributionReport = MathUtils::compare(
                lhsQuarkDistribution.getQuarkDistribution(),
                rhsQuarkDistribution.getQuarkDistribution(), tolerance);
        quarkDistributionPlusReport = MathUtils::compare(
                lhsQuarkDistribution.getQuarkDistributionPlus(),
                rhsQuarkDistribution.getQuarkDistributionPlus(), tolerance);
        quarkDistributionMinusReport = MathUtils::compare(
                lhsQuarkDistribution.getQuarkDistributionMinus(),
                rhsQuarkDistribution.getQuarkDistributionMinus(), tolerance);

        if (quarkDistributionReport.isEqual() == true
                && quarkDistributionPlusReport.isEqual() == true
                && quarkDistributionMinusReport.isEqual() == true)
            comparisonResult = true;
    }

    return QuarkDistributionReport(lhsQuarkDistribution.getQuarkFlavor(),
            comparisonResult, quarkDistributionReport,
            quarkDistributionPlusReport, quarkDistributionMinusReport);
}
