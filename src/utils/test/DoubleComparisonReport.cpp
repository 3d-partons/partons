#include "DoubleComparisonReport.h"

#include <sstream>

#include "../math/Tolerances.h"

DoubleComparisonReport::DoubleComparisonReport() :
        ComparisonReport(), m_absoluteDifference(0.), m_relativeDifference(0.), m_absoluteTolerance(
                0.), m_relativeTolerance(0.) {
}

DoubleComparisonReport::DoubleComparisonReport(bool comparisonResult,
        double absoluteDifference, double relativeDifference,
        double absoluteTolerance, double relativeTolerance) :
        ComparisonReport(comparisonResult), m_absoluteDifference(
                absoluteDifference), m_relativeDifference(relativeDifference), m_absoluteTolerance(
                absoluteTolerance), m_relativeTolerance(relativeTolerance) {
}

DoubleComparisonReport::DoubleComparisonReport(bool comparisonResult,
        double absoluteDifference, double relativeDifference,
        Tolerances& tolerances) :
        ComparisonReport(comparisonResult), m_absoluteDifference(
                absoluteDifference), m_relativeDifference(relativeDifference), m_absoluteTolerance(
                tolerances.getAbsoluteTolerance()), m_relativeTolerance(
                tolerances.getRelativeTolerance()) {
}

DoubleComparisonReport::~DoubleComparisonReport() {
}

std::string DoubleComparisonReport::toString() const {

    std::ostringstream os;
    // TODO : Use Formatter?
    os.setf(std::ios::scientific, std::ios::floatfield);
    os << "Absolute Difference = " << m_absoluteDifference << std::endl;
    os << "Relative Difference = " << m_relativeDifference << std::endl;
    os << "Absolute Tolerance = " << m_absoluteTolerance << std::endl;
    os << "Relative Tolerance = " << m_relativeTolerance << std::endl;

    return os.str();
}

double DoubleComparisonReport::getAbsoluteDifference() const {
    return m_absoluteDifference;
}

double DoubleComparisonReport::getAbsoluteTolerance() const {
    return m_absoluteTolerance;
}

double DoubleComparisonReport::getRelativeDifference() const {
    return m_relativeDifference;
}

double DoubleComparisonReport::getRelativeTolerance() const {
    return m_relativeTolerance;
}
