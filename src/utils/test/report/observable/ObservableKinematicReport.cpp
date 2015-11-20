/*
 * ObservableKinematicReport.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: debian
 */

#include "ObservableKinematicReport.h"

#include <sstream>

ObservableKinematicReport::ObservableKinematicReport() :
        ComparisonReport(), m_sameSizeListOfPhi(true) {

}

ObservableKinematicReport::ObservableKinematicReport(
        const DoubleComparisonReport& xBReport,
        const DoubleComparisonReport& tReport,
        const DoubleComparisonReport& q2Report,
        const std::vector<DoubleComparisonReport>& listOfPhiReport) :
        ComparisonReport(), m_sameSizeListOfPhi(true), m_xBReport(xBReport), m_tReport(tReport), m_Q2Report(
                q2Report), m_listOfPhiReport(listOfPhiReport) {
    testComparison();
}

ObservableKinematicReport::ObservableKinematicReport(
        const DoubleComparisonReport& xBReport,
        const DoubleComparisonReport& tReport,
        const DoubleComparisonReport& q2Report,
        const std::vector<DoubleComparisonReport>& listOfPhiReport,
        bool sameSizeListOfPhi) :
        ComparisonReport(), m_xBReport(xBReport), m_tReport(tReport), m_Q2Report(
                        q2Report), m_listOfPhiReport(listOfPhiReport), m_sameSizeListOfPhi(
                sameSizeListOfPhi) {
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

    os << "List of Phi" << std::endl;
    if (!isSameSizeListOfPhi()) {
        os
                << "Lists of Phi of different sizes, only comparing up to the least size :"
                << std::endl;
    }
    for (unsigned i = 0; i < m_listOfPhiReport.size(); i++) {
        os << m_listOfPhiReport.at(i).toString() << std::endl;
    }

    return os.str();
}

void ObservableKinematicReport::setXBReport(
        const DoubleComparisonReport& xBReport) {
    m_xBReport = xBReport;
    testComparison();
}

const std::vector<DoubleComparisonReport>& ObservableKinematicReport::getListOfPhiReport() const {
    return m_listOfPhiReport;
}

void ObservableKinematicReport::setListOfPhiReport(
        const std::vector<DoubleComparisonReport>& listOfPhiReport) {
    m_listOfPhiReport = listOfPhiReport;
    testComparison();
}

void ObservableKinematicReport::testComparison() {
    m_comparisonResult = m_xBReport.isEqual() && m_tReport.isEqual()
            && m_Q2Report.isEqual() && isSameSizeListOfPhi();
    for (unsigned int i = 0; i < m_listOfPhiReport.size(); i++) {
        m_comparisonResult = m_comparisonResult
                && m_listOfPhiReport.at(i).isEqual();
    }
}

bool ObservableKinematicReport::isSameSizeListOfPhi() const {
    return m_sameSizeListOfPhi;
}

void ObservableKinematicReport::setSameSizeListOfPhi(bool sameSizeListOfPhi) {
    m_sameSizeListOfPhi = sameSizeListOfPhi;
}
