#include "HadronStructureUtils.h"

#include <map>
#include <vector>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/gpd/GPDResultList.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/QuarkFlavor.h"
#include "../../math/MathUtils.h"
#include "../../MapUtils.h"
#include "../../test/DoubleComparisonReport.h"
#include "../../test/report/GluonDistributionReport.h"
#include "../../test/report/GPDKinematicReport.h"
#include "../../test/report/GPDResultListReport.h"
#include "../../test/report/GPDResultReport.h"
#include "../../test/report/PartonDistributionReport.h"
#include "../../test/report/QuarkDistributionReport.h"

class Partons;

GPDResultListReport HadronStructureUtils::compareGPDResultsLists(
        const GPDResultList& lhsGpdResultList,
        const GPDResultList& rhsGpdResultList, const Tolerances& tolerances) {

    bool comparableLists = false;
    bool isEqualList = true;
    std::vector<unsigned int> differentGpdResultsIndex;

    GPDResultListReport gpdResultListReport;
    GPDResultReport gpdResultReport;

    unsigned int lhsGpdResultListSize = lhsGpdResultList.getSize();
    unsigned int rhsGpdResultListSize = rhsGpdResultList.getSize();
    gpdResultListReport.setLhsGpdResultListSize(lhsGpdResultListSize);
    gpdResultListReport.setRhsGpdResultListSize(rhsGpdResultListSize);

    if (lhsGpdResultListSize == rhsGpdResultListSize) {
        comparableLists = true;
        for (unsigned int i = 0; i < lhsGpdResultListSize; i++) {
            gpdResultReport = HadronStructureUtils::compareGPDResults(
                    lhsGpdResultList.get(i), rhsGpdResultList.get(i),
                    tolerances);
            if (!gpdResultReport.isEqual())
                differentGpdResultsIndex.push_back(i);
            gpdResultListReport.add(gpdResultReport);
            isEqualList = isEqualList && gpdResultReport.isEqual();
        }
        gpdResultListReport.setDifferentResultIndex(differentGpdResultsIndex);
        gpdResultListReport.setSameSize(comparableLists);
        gpdResultListReport.setComparisonResult(isEqualList);
    }

    return gpdResultListReport;
}

GPDResultReport HadronStructureUtils::compareGPDResults(
        const GPDResult& lhsGpdResult, const GPDResult& rhsGpdResult,
        const Tolerances& tolerances) {

    bool isEqualGpdResult = true;
    bool missingGpdTypes = false;
    PartonDistributionReport partonDistributionReport;
    GPDResultReport gpdResultReport;

    // Compare corresponding GPD kinematics
    GPDKinematic lhsGpdKinematic = lhsGpdResult.getGpdKinematic();
    GPDKinematic rhsGpdKinemaitc = rhsGpdResult.getGpdKinematic();
    GPDKinematicReport gpdKinematicReport =
            HadronStructureUtils::compareGPDKinematics(lhsGpdKinematic,
                    rhsGpdKinemaitc, tolerances);
    gpdResultReport.setGpdKinematicReport(gpdKinematicReport);

    // Retrieve vector of computed GPDs and identify common GPDs
    std::map<GPDType::Type, PartonDistribution> lhsPartonDistributions =
            lhsGpdResult.getPartonDistributions();
    std::map<GPDType::Type, PartonDistribution> rhsPartonDistributions =
            rhsGpdResult.getPartonDistributions();
    gpdResultReport.setLhsGpdTypes(
            MapUtils::mapToVectorOfKey(lhsPartonDistributions));
    gpdResultReport.setRhsGpdTypes(
            MapUtils::mapToVectorOfKey(rhsPartonDistributions));
    std::vector<GPDType::Type> commonGpdTypes = MapUtils::intersectionOfKey<
            GPDType::Type, PartonDistribution>(
            lhsGpdResult.getPartonDistributions(),
            rhsGpdResult.getPartonDistributions());

    // Compare corresponding parton distributions (if there are any in common)
    if (commonGpdTypes.size() > 0) {
        gpdResultReport.setCommonGpdType(true);
        for (unsigned int i = 0; i < commonGpdTypes.size(); i++) {
            partonDistributionReport =
                    HadronStructureUtils::comparePartonDistributions(
                            lhsGpdResult.getPartonDistribution(
                                    commonGpdTypes.at(i)),
                            rhsGpdResult.getPartonDistribution(
                                    commonGpdTypes.at(i)), tolerances);
            isEqualGpdResult = isEqualGpdResult
                    && partonDistributionReport.isEqual();
            gpdResultReport.addPartonDistributionReport(commonGpdTypes.at(i),
                    partonDistributionReport);
        }
    }

    // Test different sets of quark flavors in rhs and lhs
    if (commonGpdTypes.size() < lhsPartonDistributions.size()
            || commonGpdTypes.size() < rhsPartonDistributions.size())
        missingGpdTypes = true;

    if (gpdKinematicReport.isEqual() && isEqualGpdResult && !missingGpdTypes)
        gpdResultReport.setComparisonResult(true);

    return gpdResultReport;
}

PartonDistributionReport HadronStructureUtils::comparePartonDistributions(
        const PartonDistribution &lhsPartonDistribution,
        const PartonDistribution &rhsPartonDistribution,
        const Tolerances& tolerance) {

    bool comparableGluons = false;
    bool comparableQuarkFlavors = false;
    bool missingQuarkFlavor = false;
    bool isEqualQuarkDistributions = true;
    GluonDistributionReport gluonDistributionReport;
    PartonDistributionReport partonDistributionReport;

    // Retrieve and test definition of gluon distributions
    GluonDistribution lhsGluonDistribution =
            lhsPartonDistribution.getGluonDistribution();
    if (lhsGluonDistribution.isNullObject())
        partonDistributionReport.setLhsUndefinedGluons(true);
    GluonDistribution rhsGluonDistribution =
            rhsPartonDistribution.getGluonDistribution();
    if (rhsGluonDistribution.isNullObject())
        partonDistributionReport.setRhsUndefinedGluons(true);

    if ((lhsGluonDistribution.isNullObject()
            && rhsGluonDistribution.isNullObject())
            || (!lhsGluonDistribution.isNullObject()
                    && !rhsGluonDistribution.isNullObject()))
        comparableGluons = true;

    gluonDistributionReport = HadronStructureUtils::compareGluonDistributions(
            lhsGluonDistribution.getGluonDistribution(),
            rhsGluonDistribution.getGluonDistribution(), tolerance);
    partonDistributionReport.setGluonDistributionReport(
            gluonDistributionReport);

    // Retrieve the list of computed quark flavors and identify common elements
    std::map<QuarkFlavor::Type, QuarkDistribution> lhsQuarkDistributions =
            lhsPartonDistribution.getQuarkDistributions();
    std::map<QuarkFlavor::Type, QuarkDistribution> rhsQuarkDistributions =
            rhsPartonDistribution.getQuarkDistributions();
    partonDistributionReport.setLhsQuarkFlavors(
            MapUtils::mapToVectorOfKey(lhsQuarkDistributions));
    partonDistributionReport.setRhsQuarkFlavors(
            MapUtils::mapToVectorOfKey(rhsQuarkDistributions));
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

    // Test different sets of quark flavors in rhs and lhs
    if (commonQuarkFlavors.size() < lhsQuarkDistributions.size()
            || commonQuarkFlavors.size() < rhsQuarkDistributions.size())
        missingQuarkFlavor = true;

    if (comparableGluons || comparableQuarkFlavors) {
        partonDistributionReport.setCommonPartonType(true);
        if (gluonDistributionReport.isEqual() && isEqualQuarkDistributions
                && !missingQuarkFlavor)
            partonDistributionReport.setComparisonResult(true);
    }

    return partonDistributionReport;
}

GluonDistributionReport HadronStructureUtils::compareGluonDistributions(
        const GluonDistribution& lhsGluonDistribution,
        const GluonDistribution& rhsGluonDistribution,
        const Tolerances& tolerances) {

    GluonDistributionReport gluoncomparisonReport;

    if (!lhsGluonDistribution.isNullObject()
            && !rhsGluonDistribution.isNullObject()) {

        DoubleComparisonReport doubleComparisonReport = MathUtils::compare(
                lhsGluonDistribution.getGluonDistribution(),
                rhsGluonDistribution.getGluonDistribution(), tolerances);
        gluoncomparisonReport.setGluonComparisonReport(doubleComparisonReport);
    }

    if (lhsGluonDistribution.isNullObject()
            && rhsGluonDistribution.isNullObject()) {

        gluoncomparisonReport.setComparisonResult(true);
    }

    return gluoncomparisonReport;
}

QuarkDistributionReport HadronStructureUtils::compareQuarkDistributions(
        const QuarkDistribution& lhsQuarkDistribution,
        const QuarkDistribution& rhsQuarkDistribution,
        const Tolerances& tolerance) {

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
    }

    return QuarkDistributionReport(lhsQuarkDistribution.getQuarkFlavor(),
            quarkDistributionReport, quarkDistributionPlusReport,
            quarkDistributionMinusReport);
}

GPDKinematicReport HadronStructureUtils::compareGPDKinematics(
        const GPDKinematic& lhsGpdKinematic,
        const GPDKinematic& rhsGpdKinematic, const Tolerances& tolerances) {

    GPDKinematicReport gpdKinematicReport;

    DoubleComparisonReport xReport = MathUtils::compare(lhsGpdKinematic.getX(),
            rhsGpdKinematic.getX(), tolerances);
    DoubleComparisonReport xiReport = MathUtils::compare(
            lhsGpdKinematic.getXi(), rhsGpdKinematic.getXi(), tolerances);
    DoubleComparisonReport tReport = MathUtils::compare(lhsGpdKinematic.getT(),
            rhsGpdKinematic.getT(), tolerances);
    DoubleComparisonReport muFReport = MathUtils::compare(
            lhsGpdKinematic.getMuF2(), rhsGpdKinematic.getMuF2(), tolerances);
    DoubleComparisonReport muRReport = MathUtils::compare(
            lhsGpdKinematic.getMuR2(), rhsGpdKinematic.getMuR2(), tolerances);

    gpdKinematicReport.setXReport(xReport);
    gpdKinematicReport.setXiReport(xiReport);
    gpdKinematicReport.setTReport(tReport);
    gpdKinematicReport.setMuFReport(muFReport);
    gpdKinematicReport.setMuRReport(muRReport);

    if (xReport.isEqual() && xiReport.isEqual() && tReport.isEqual()
            && muFReport.isEqual() && muRReport.isEqual())
        gpdKinematicReport.setComparisonResult(true);

    return gpdKinematicReport;
}
