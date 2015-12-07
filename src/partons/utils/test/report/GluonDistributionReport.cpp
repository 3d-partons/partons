#include "../../../../../include/partons/utils/test/report/GluonDistributionReport.h"

//#include <iostream>
#include <sstream>

//#include <string>

GluonDistributionReport::GluonDistributionReport() :
        ComparisonReport(), m_absoluteDifference(0.), m_relativeDifference(0.), m_absoluteTolerance(
                0.), m_relativeTolerance(0.) {
}

GluonDistributionReport::GluonDistributionReport(
        DoubleComparisonReport doubleComparisonReport) :
        ComparisonReport(doubleComparisonReport.isEqual()), m_absoluteDifference(
                doubleComparisonReport.getAbsoluteDifference()), m_relativeDifference(
                doubleComparisonReport.getRelativeDifference()), m_absoluteTolerance(
                doubleComparisonReport.getRelativeDifference()), m_relativeTolerance(
                doubleComparisonReport.getRelativeTolerance()) {
}

GluonDistributionReport::~GluonDistributionReport() {
}

std::string GluonDistributionReport::toString() const {

    std::ostringstream os;
    // TODO : Use Formatter?
    os.setf(std::ios::scientific, std::ios::floatfield);
    os << "Absolute Difference = " << m_absoluteDifference << std::endl;
    os << "Relative Difference = " << m_relativeDifference << std::endl;
    os << "Absolute Tolerance = " << m_absoluteTolerance << std::endl;
    os << "Relative Tolerance = " << m_relativeTolerance << std::endl;

    return os.str();
}

void GluonDistributionReport::setGluonComparisonReport(
        const DoubleComparisonReport& gluonComparisonReport) {
    m_comparisonResult = gluonComparisonReport.isEqual();
    m_absoluteDifference = gluonComparisonReport.getAbsoluteDifference();
    m_relativeDifference = gluonComparisonReport.getRelativeDifference();
    m_absoluteTolerance = gluonComparisonReport.getAbsoluteTolerance();
    m_relativeTolerance = gluonComparisonReport.getRelativeTolerance();
}
