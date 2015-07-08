#include "GluonDistributionReport.h"

#include <iostream>
#include <sstream>

//#include <string>

GluonDistributionReport::GluonDistributionReport() :
        ComparisonReport() {
}

GluonDistributionReport::GluonDistributionReport(
        DoubleComparisonReport gluonComparisonReport) :
        ComparisonReport(), m_gluonComparisonReport(gluonComparisonReport) {
}

GluonDistributionReport::~GluonDistributionReport() {
}

std::string GluonDistributionReport::toString() const {

    std::ostringstream os;
    // TODO : Use Formatter?

    os << "g" << std::endl;
    os << m_gluonComparisonReport.toString() << std::endl;

    return os.str();
}

const DoubleComparisonReport& GluonDistributionReport::getGluonComparisonReport() const {
    return m_gluonComparisonReport;
}

void GluonDistributionReport::setGluonComparisonReport(
        const DoubleComparisonReport& gluonComparisonReport) {
    m_gluonComparisonReport = gluonComparisonReport;
}
