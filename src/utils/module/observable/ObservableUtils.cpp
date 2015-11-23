#include "ObservableUtils.h"

#include <algorithm>
#include <string>
#include <vector>

#include "../../../beans/observable/ObservableResult.h"
#include "../../../beans/observable/ObservableResultList.h"
#include "../../../beans/observable/ObservableType.h"
#include "../../math/MathUtils.h"
#include "../../test/DoubleComparisonReport.h"
#include "../../test/report/observable/ObservableKinematicReport.h"
#include "../../test/report/observable/ObservableResultListReport.h"
#include "../../test/report/observable/ObservableResultReport.h"

ObservableResultListReport ObservableUtils::compareObservableResultsLists(
        const ObservableResultList& lhsObservableResultList,
        const ObservableResultList& rhsObservableResultList,
        const Tolerances& tolerances) {

    ObservableResultListReport observableResultListReport(
            lhsObservableResultList.size(), rhsObservableResultList.size());
    ObservableResultReport observableResultReport;

    unsigned int sizeList = std::min(lhsObservableResultList.size(),
            rhsObservableResultList.size());
    for (unsigned int i = 0; i < sizeList; i++) {
        observableResultReport = compareObservableResults(
                lhsObservableResultList[i], rhsObservableResultList[i],
                tolerances);
        observableResultListReport.addObservableResultReport(
                observableResultReport);
    }

    return observableResultListReport;
}

ObservableResultReport ObservableUtils::compareObservableResults(
        const ObservableResult& lhsObservableResult,
        const ObservableResult& rhsObservableResult,
        const Tolerances& tolerances) {

    // Compare corresponding Observable kinematics
    ObservableKinematic lhsObservableKinematic =
            lhsObservableResult.getKinematic();
    ObservableKinematic rhsObservableKinematic =
            rhsObservableResult.getKinematic();
    ObservableKinematicReport observableKinematicReport =
            ObservableUtils::compareObservableKinematics(lhsObservableKinematic,
                    rhsObservableKinematic, tolerances);
    //observableResultReport.setObservableKinematicReport(observableKinematicReport);

    // Compare Observable values
    DoubleComparisonReport observableReport = MathUtils::compare(
            lhsObservableResult.getValue(), rhsObservableResult.getValue(),
            tolerances);
    std::string observableName = lhsObservableResult.getObservableName();
    ObservableType::Type observableType =
            lhsObservableResult.getObservableType();
    bool sameObservable = (observableName
            == rhsObservableResult.getObservableName()
            && observableType == rhsObservableResult.getObservableType());

    ObservableResultReport observableResultReport = ObservableResultReport(
            observableName, observableType, observableReport,
            observableKinematicReport, sameObservable);

    return observableResultReport;
}

ObservableKinematicReport ObservableUtils::compareObservableKinematics(
        const ObservableKinematic& lhsObservableKinematic,
        const ObservableKinematic& rhsObservableKinematic,
        const Tolerances& tolerances) {

    DoubleComparisonReport xBReport = MathUtils::compare(
            lhsObservableKinematic.getXB(), rhsObservableKinematic.getXB(),
            tolerances);
    DoubleComparisonReport tReport = MathUtils::compare(
            lhsObservableKinematic.getT(), rhsObservableKinematic.getT(),
            tolerances);
    DoubleComparisonReport Q2Report = MathUtils::compare(
            lhsObservableKinematic.getQ2(), rhsObservableKinematic.getQ2(),
            tolerances);
    std::vector<DoubleComparisonReport> listOfPhiReport;
    bool sameSizeListOfPhi = lhsObservableKinematic.getListOfPhi().size()
            == rhsObservableKinematic.getListOfPhi().size();
    unsigned int sizeListOfPhi = std::min(
            lhsObservableKinematic.getListOfPhi().size(),
            rhsObservableKinematic.getListOfPhi().size());
    for (unsigned int i = 0; i < sizeListOfPhi; i++) {
        listOfPhiReport.push_back(
                MathUtils::compare(lhsObservableKinematic.getListOfPhi().at(i),
                        rhsObservableKinematic.getListOfPhi().at(i),
                        tolerances));
    }

    ObservableKinematicReport observableKinematicReport =
            ObservableKinematicReport(xBReport, tReport, Q2Report,
                    listOfPhiReport, sameSizeListOfPhi);

    return observableKinematicReport;
}
