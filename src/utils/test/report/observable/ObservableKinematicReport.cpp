/*
 * ObservableKinematicReport.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: debian
 */

#include "ObservableKinematicReport.h"

#include <sstream>

ObservableKinematicReport::ObservableKinematicReport() :
        ComparisonReport() {

}

ObservableKinematicReport::ObservableKinematicReport(
        const DoubleComparisonReport& xBReport,
        const DoubleComparisonReport& tReport,
        const DoubleComparisonReport& q2Report,
        const DoubleComparisonReport& phiReport) :
        ComparisonReport(), m_xBReport(xBReport), m_tReport(tReport), m_Q2Report(
                q2Report), m_phiReport(phiReport) {
    testComparison();
}

ObservableKinematicReport::~ObservableKinematicReport() {
    // TODO Auto-generated destructor stub
}

const DoubleComparisonReport& ObservableKinematicReport::getQ2Report() const {
    return m_Q2Report;
}

void ObservableKinematicReport::setQ2Report(
        const DoubleComparisonReport& q2Report) {
    m_Q2Report = q2Report;
    testComparison();
}

const DoubleComparisonReport& ObservableKinematicReport::getTReport() const {
    return m_tReport;
}

void ObservableKinematicReport::setTReport(
        const DoubleComparisonReport& tReport) {
    m_tReport = tReport;
    testComparison();
}

const DoubleComparisonReport& ObservableKinematicReport::getXBReport() const {
    return m_xBReport;
}

std::string ObservableKinematicReport::toString() const {
    std::ostringstream os;
    // TODO : Use Formatter?
    os << "xB" << std::endl;
    os << m_xBReport.toString() << std::endl;

    os << "t" << std::endl;
    os << m_tReport.toString() << std::endl;

    os << "Q2" << std::endl;
    os << m_Q2Report.toString() << std::endl;

    os << "Phi" << std::endl;
    os << m_phiReport.toString() << std::endl;

    return os.str();
}

void ObservableKinematicReport::setXBReport(
        const DoubleComparisonReport& xBReport) {
    m_xBReport = xBReport;
    testComparison();
}

void ObservableKinematicReport::testComparison() {
    m_comparisonResult = m_xBReport.isEqual() && m_tReport.isEqual()
            && m_Q2Report.isEqual() && m_phiReport.isEqual();
}

const DoubleComparisonReport& ObservableKinematicReport::getPhiReport() const {
    return m_phiReport;
}

void ObservableKinematicReport::setPhiReport(
        const DoubleComparisonReport& phiReport) {
    m_phiReport = phiReport;
    testComparison();
}
