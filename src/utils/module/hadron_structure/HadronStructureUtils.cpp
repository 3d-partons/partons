#include "HadronStructureUtils.h"

#include <vector>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../math/MathUtils.h"
#include "../../MapUtils.h"
#include "../../test/DoubleComparisonReport.h"
#include "../../test/report/GPDResultReport.h"
#include "../../test/report/PartonDistributionReport.h"
#include "../../test/report/QuarkDistributionReport.h"

GPDResultReport HadronStructureUtils::compareGPDResults(
        GPDResult* p_lhsGpdResult, GPDResult* p_rhsGpdResult,
        double absoluteTolerance, double relativeTolerance) {

    // Retrieve vector of common GPDs
    std::vector<GPDType::Type> gpdType = MapUtils::intersectionOfKey<
            GPDType::Type, PartonDistribution>(
            p_lhsGpdResult->getPartonDistributions(),
            p_rhsGpdResult->getPartonDistributions());

//    for (unsigned int i = 0; i < gpdType.size(); i++) {
//        PartonDistributionReport partonDistributionReport =
//                HadronStructureUtils::comparePartonDistributions(
//                        p_lhsGpdResult->getPartonDistribution(
//                                gpdComputedType[i]),
//                        p_rhsGpdResult->getPartonDistribution(
//                                gpdComputedType[i]), 1., 1.);
//    }

    return GPDResultReport();
}

PartonDistributionReport HadronStructureUtils::comparePartonDistributions(
        const PartonDistribution &lhsPartonDistribution,
        const PartonDistribution &rhsPartonDistribution,
        double absoluteTolerance, double relativeTolerance) {

//    bool comparisonResult = false;
//    bool gluons = false;
//    bool commonQuarkFlavors = false;
//    bool commonPartonType = false;
//
    PartonDistributionReport partonDistributionReport;
//
//    std::map<QuarkFlavor::Type, QuarkDistribution> lhsQuarkDistributions,
//            rhsQuarkDistributions;
//    if (lhsPartonDistribution.getGpdType()
//            == rhsPartonDistribution.getGpdType()) {
//
//        // Retrieve and compare gluon distributions
//        GluonDistribution lhsGluonDistribution =
//                lhsPartonDistribution.getGluonDistribution();
//        GluonDistribution rhsGluonDistribution =
//                rhsPartonDistribution.getGluonDistribution();
//        if ()
//        DoubleComparisonReport gluonDistributionReport = MathUtils::compare(
//                lhsGluonDistribution.getGluonDistribution(),
//                rhsGluonDistribution.getGluonDistribution(), absoluteTolerance,
//                relativeTolerance);
//        if (gluonDistributionReport.isEqual() == true) {
//            partonDistributionReport.setGluonDistributionReport(
//                    gluonDistributionReport);
//        }
//
//        // Retrieve the list of computed quark flavors and identify common elements
//        lhsQuarkDistributions = lhsPartonDistribution.getQuarkDistributions();
//        rhsQuarkDistributions = rhsPartonDistribution.getQuarkDistributions();
//        std::vector<QuarkFlavor::Type> commonQuarkFlavors =
//                MapUtils::intersectionOfKey(lhsQuarkDistributions,
//                        rhsQuarkDistributions);
//
//        // Compare quark distributions for common flavors
//        QuarkDistribution lhsQuarkDistribution, rhsQuarkDistribution;
//        GluonDistribution lhsGluonDistribution, rhsGluonDistribution;
//        QuarkDistributionReport quarkDistributionReport;
//        for (unsigned int i = 0; i < commonQuarkFlavors.size(); i++) {
//            lhsQuarkDistribution = lhsPartonDistribution.getQuarkDistribution(
//                    commonQuarkFlavors.at(i));
//            rhsQuarkDistribution = rhsPartonDistribution.getQuarkDistribution(
//                    commonQuarkFlavors.at(i));
//            quarkDistributionReport = compareQuarkDistribution(
//                    lhsQuarkDistribution, rhsQuarkDistribution,
//                    absoluteTolerance, relativeTolerance);
//            partonDistributionReport.addQuarkDistributionReport(
//                    quarkDistributionReport);
//        }
//    }

    return partonDistributionReport;
}

QuarkDistributionReport HadronStructureUtils::compareQuarkDistribution(
        const QuarkDistribution& lhsQuarkDistribution,
        const QuarkDistribution& rhsQuarkDistribution, double absoluteTolerance,
        double relativeTolerance) {

    bool comparisonResult = false;
    DoubleComparisonReport quarkDistributionReport, quarkDistributionPlusReport,
            quarkDistributionMinusReport;

    if (lhsQuarkDistribution.getQuarkFlavor()
            == rhsQuarkDistribution.getQuarkFlavor()) {
        quarkDistributionReport = MathUtils::compare(
                lhsQuarkDistribution.getQuarkDistribution(),
                rhsQuarkDistribution.getQuarkDistribution(), absoluteTolerance,
                relativeTolerance);
        quarkDistributionPlusReport = MathUtils::compare(
                lhsQuarkDistribution.getQuarkDistributionPlus(),
                rhsQuarkDistribution.getQuarkDistributionPlus(),
                absoluteTolerance, relativeTolerance);
        quarkDistributionMinusReport = MathUtils::compare(
                lhsQuarkDistribution.getQuarkDistributionMinus(),
                rhsQuarkDistribution.getQuarkDistributionMinus(),
                absoluteTolerance, relativeTolerance);

        if (quarkDistributionReport.isEqual() == true
                && quarkDistributionPlusReport.isEqual() == true
                && quarkDistributionMinusReport.isEqual() == true)
            comparisonResult = true;
    }

    return QuarkDistributionReport(lhsQuarkDistribution.getQuarkFlavor(),
            comparisonResult, quarkDistributionReport,
            quarkDistributionPlusReport, quarkDistributionMinusReport);
}
