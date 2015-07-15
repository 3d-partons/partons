#include "GPDKinematicReport.h"

#include <sstream>
#include <string>

GPDKinematicReport::GPDKinematicReport() :
        ComparisonReport() {
}

GPDKinematicReport::~GPDKinematicReport() {
}

const DoubleComparisonReport& GPDKinematicReport::getMuFReport() const {
    return m_MuFReport;
}

void GPDKinematicReport::setMuFReport(const DoubleComparisonReport& muFReport) {
    m_MuFReport = muFReport;
}

const DoubleComparisonReport& GPDKinematicReport::getMuRReport() const {
    return m_MuRReport;
}

void GPDKinematicReport::setMuRReport(const DoubleComparisonReport& muRReport) {
    m_MuRReport = muRReport;
}

const DoubleComparisonReport& GPDKinematicReport::getTReport() const {
    return m_tReport;
}

void GPDKinematicReport::setTReport(const DoubleComparisonReport& tReport) {
    m_tReport = tReport;
}

const DoubleComparisonReport& GPDKinematicReport::getXiReport() const {
    return m_xiReport;
}

void GPDKinematicReport::setXiReport(const DoubleComparisonReport& xiReport) {
    m_xiReport = xiReport;
}

const DoubleComparisonReport& GPDKinematicReport::getXReport() const {
    return m_xReport;
}

void GPDKinematicReport::setXReport(const DoubleComparisonReport& xReport) {
    m_xReport = xReport;
}

std::string GPDKinematicReport::toString() const {

    std::ostringstream os;
    // TODO : Use Formatter?
    os << "x" << std::endl;
    os << m_xReport.toString() << std::endl;

    os << "xi" << std::endl;
    os << m_xiReport.toString() << std::endl;

    os << "t" << std::endl;
    os << m_tReport.toString() << std::endl;

    os << "MuF" << std::endl;
    os << m_MuFReport.toString() << std::endl;

    os << "MuR" << std::endl;
    os << m_MuRReport.toString() << std::endl;

    return os.str();
}
