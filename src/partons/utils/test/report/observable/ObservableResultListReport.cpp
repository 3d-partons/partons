#include "../../../../../../include/partons/utils/test/report/observable/ObservableResultListReport.h"

#include <sstream>

ObservableResultListReport::ObservableResultListReport() :
        ComparisonReport(), m_sameSize(false), m_lhsObservableResultListSize(0), m_rhsObservableResultListSize(
                0) {
}

ObservableResultListReport::ObservableResultListReport(
        unsigned int lhsObservableResultListSize,
        unsigned int rhsObservableResultListSize) :
        ComparisonReport(), m_sameSize(
                lhsObservableResultListSize == rhsObservableResultListSize), m_lhsObservableResultListSize(
                lhsObservableResultListSize), m_rhsObservableResultListSize(
                rhsObservableResultListSize) {
}

ObservableResultListReport::ObservableResultListReport(
        unsigned int lhsObservableResultListSize,
        unsigned int rhsObservableResultListSize,
        const std::vector<ObservableResultReport>& observableResultReports) :
        ComparisonReport(), m_sameSize(
                lhsObservableResultListSize == rhsObservableResultListSize), m_lhsObservableResultListSize(
                lhsObservableResultListSize), m_rhsObservableResultListSize(
                rhsObservableResultListSize) {
    setObservableResultReports(observableResultReports);
}

ObservableResultListReport::~ObservableResultListReport() {
    // TODO Auto-generated destructor stub
}

const std::vector<unsigned int>& ObservableResultListReport::getDifferentResultIndex() const {
    return m_differentResultIndex;
}

void ObservableResultListReport::setDifferentResultIndex(
        const std::vector<unsigned int>& differentResultIndex) {
    m_differentResultIndex = differentResultIndex;
}

const unsigned int& ObservableResultListReport::getDifferentResultIndex(
        unsigned int index) const {
    return m_differentResultIndex.at(index);
}

void ObservableResultListReport::addDifferentResultIndex(
        unsigned int differentResultIndex) {
    m_differentResultIndex.push_back(differentResultIndex);
}

unsigned int ObservableResultListReport::getLhsObservableResultListSize() const {
    return m_lhsObservableResultListSize;
}

void ObservableResultListReport::setLhsObservableResultListSize(
        unsigned int lhsObservableResultListSize) {
    m_lhsObservableResultListSize = lhsObservableResultListSize;
    m_sameSize = lhsObservableResultListSize == m_rhsObservableResultListSize;
    testComparison();
}

unsigned int ObservableResultListReport::getRhsObservableResultListSize() const {
    return m_rhsObservableResultListSize;
}

void ObservableResultListReport::setRhsObservableResultListSize(
        unsigned int rhsObservableResultListSize) {
    m_rhsObservableResultListSize = rhsObservableResultListSize;
    m_sameSize = rhsObservableResultListSize == m_lhsObservableResultListSize;
    testComparison();
}

bool ObservableResultListReport::isSameSize() const {
    return m_sameSize;
}

const std::vector<ObservableResultReport>& ObservableResultListReport::getObservableResultReports() const {
    return m_observableResultReports;
}

void ObservableResultListReport::addObservableResultReport(
        const ObservableResultReport& observableResultReport) {
    if (!observableResultReport.isEqual()) {
        addDifferentResultIndex(getSize());
    }
    m_observableResultReports.push_back(observableResultReport);
    testComparison();
}

const ObservableResultReport& ObservableResultListReport::getObservableResultReport(
        unsigned int index) const {
    return m_observableResultReports.at(index);
}

unsigned int ObservableResultListReport::getSize() const {
    return m_observableResultReports.size();
}

void ObservableResultListReport::setObservableResultReports(
        const std::vector<ObservableResultReport>& observableResultReports) {
    for (unsigned int i = 0; i < observableResultReports.size(); i++) {
        addObservableResultReport(observableResultReports.at(i));
    }
}

std::string ObservableResultListReport::toString() const {

    std::ostringstream os;

    if (isSameSize()) {
        if (!isEqual()) {
            os << "List of Observable results indices where comparison failed:"
                    << std::endl;
            for (unsigned int j = 0; j < m_differentResultIndex.size(); j++) {
                os << m_differentResultIndex.at(j) << "    ";
            }
            os << std::endl;
        }
        for (unsigned int i = 0; i != getSize(); i++) {
            os << getObservableResultReport(i).toString() << std::endl;
        }
    } else {
        os << "The compared lists do not have the same size." << std::endl;
        os << "Size of lhs list:" << m_lhsObservableResultListSize
                << " elements" << std::endl;
        os << "Size of rhs list:" << m_rhsObservableResultListSize
                << " elements" << std::endl;
    }

    return os.str();
}

void ObservableResultListReport::testComparison() {
    m_comparisonResult = m_sameSize;
    for (unsigned int i = 0; i < getSize(); i++) {
        m_comparisonResult = m_comparisonResult
                && getObservableResultReport(i).isEqual();
    }
}
