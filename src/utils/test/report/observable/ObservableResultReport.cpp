#include "ObservableResultReport.h"

#include <sstream>

ObservableResultReport::ObservableResultReport() :
        ComparisonReport(), m_observableName("Observable"), m_observableType(
                ObservableType::UNDEFINED), m_sameObservable(true) {

}

ObservableResultReport::ObservableResultReport(
        const std::string& observableName, ObservableType::Type observableType,
        const DoubleComparisonReport& observableReport,
        const ObservableKinematicReport& observableKinematicReport) :
        ComparisonReport(), m_observableName(observableName), m_observableType(
                observableType), m_observableReport(observableReport), m_observableKinematicReport(
                observableKinematicReport), m_sameObservable(true) {
    testComparison();
}

ObservableResultReport::ObservableResultReport(
        const std::string& observableName, ObservableType::Type observableType,
        const DoubleComparisonReport& observableReport,
        const ObservableKinematicReport& observableKinematicReport,
        bool sameObservable) :
        ComparisonReport(), m_observableName(observableName), m_observableType(
                observableType), m_observableReport(observableReport), m_observableKinematicReport(
                observableKinematicReport), m_sameObservable(sameObservable) {
    testComparison();
}

ObservableResultReport::~ObservableResultReport() {
    // TODO Auto-generated destructor stub
}

std::string ObservableResultReport::toString() const {
    std::ostringstream os;
    // TODO : Use Formatter?

    os << "Observable kinematics" << std::endl;
    os << m_observableKinematicReport.toString() << std::endl;

    if (isSameObservable()) {
        os << m_observableName << std::endl;
    } else {
        os << "This comparison doesn't make sense since the two observables are different!" << std::endl;
    }
    os << m_observableReport.toString() << std::endl;

    return os.str();
}

const DoubleComparisonReport& ObservableResultReport::getObservableReport() const {
    return m_observableReport;
}

void ObservableResultReport::setObservableReport(
        const DoubleComparisonReport& observableReport) {
    m_observableReport = observableReport;
    testComparison();
}

const std::string& ObservableResultReport::getObservableName() const {
    return m_observableName;
}

void ObservableResultReport::setObservableName(
        const std::string& observableName) {
    m_observableName = observableName;
}

const ObservableKinematicReport& ObservableResultReport::getObservableKinematicReport() const {
    return m_observableKinematicReport;
}

void ObservableResultReport::setObservableKinematicReport(
        const ObservableKinematicReport& observableKinematicReport) {
    m_observableKinematicReport = observableKinematicReport;
    testComparison();
}

ObservableType::Type ObservableResultReport::getObservableType() const {
    return m_observableType;
}

void ObservableResultReport::setObservableType(
        ObservableType::Type observableType) {
    m_observableType = observableType;
}

bool ObservableResultReport::isSameObservable() const {
    return m_sameObservable;
}

void ObservableResultReport::setSameObservable(bool sameObservable) {
    m_sameObservable = sameObservable;
    testComparison();
}

void ObservableResultReport::testComparison() {
    m_comparisonResult = m_observableReport.isEqual()
            && m_observableKinematicReport.isEqual() && m_sameObservable;
}
