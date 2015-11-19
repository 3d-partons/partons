#include "ObservableResultReport.h"

#include <sstream>

ObservableResultReport::ObservableResultReport() :
        ComparisonReport(), m_observableName("Observable") {

}

ObservableResultReport::ObservableResultReport(std::string observableName,
        DoubleComparisonReport observableReport) :
        ComparisonReport(observableReport.isEqual()), m_observableName(
                observableName), m_observableReport(observableReport) {
}

ObservableResultReport::~ObservableResultReport() {
    // TODO Auto-generated destructor stub
}

std::string ObservableResultReport::toString() const {
    std::ostringstream os;
    // TODO : Use Formatter?
    os << m_observableName << std::endl;
    os << m_observableReport.toString() << std::endl;

    return os.str();
}

const DoubleComparisonReport& ObservableResultReport::getObservableReport() const {
    return m_observableReport;
}

void ObservableResultReport::setObservableReport(
        const DoubleComparisonReport& observableReport) {
    m_observableReport = observableReport;
    m_comparisonResult = observableReport.isEqual();
}

const std::string& ObservableResultReport::getObservableName() const {
    return m_observableName;
}

void ObservableResultReport::setObservableName(
        const std::string& observableName) {
    m_observableName = observableName;
}
