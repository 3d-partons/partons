/*
 * ObservableKinematicReport.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: debian
 */

#include "ObservableKinematicReport.h"

#include <sstream>
#include <vector>

ObservableKinematicReport::ObservableKinematicReport() {
    // TODO Auto-generated constructor stub

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
}

const DoubleComparisonReport& ObservableKinematicReport::getTReport() const {
    return m_tReport;
}

void ObservableKinematicReport::setTReport(
        const DoubleComparisonReport& tReport) {
    m_tReport = tReport;
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
    for (unsigned i = 0; i < m_listOfPhiReport.size(); i++) {
        os << m_listOfPhiReport.at(i).toString() << std::endl;
    }

    return os.str();
}

void ObservableKinematicReport::setXBReport(
        const DoubleComparisonReport& xBReport) {
    m_xBReport = xBReport;
}

const std::vector<DoubleComparisonReport>& ObservableKinematicReport::getListOfPhiReport() const {
    return m_listOfPhiReport;
}

void ObservableKinematicReport::setListOfPhiReport(
        const std::vector<DoubleComparisonReport>& listOfPhiReport) {
    m_listOfPhiReport = listOfPhiReport;
}
