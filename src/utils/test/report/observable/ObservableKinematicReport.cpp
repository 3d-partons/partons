/*
 * ObservableKinematicReport.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: debian
 */

#include "ObservableKinematicReport.h"

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

//TODO implement
std::string ObservableKinematicReport::toString() const {
    return "";
}

void ObservableKinematicReport::setXBReport(
        const DoubleComparisonReport& xBReport) {
    m_xBReport = xBReport;
}
