#include "QuarkDistributionReport.h"

#include <sstream>

QuarkDistributionReport::QuarkDistributionReport() :
        ComparisonReport() {
}

QuarkDistributionReport::QuarkDistributionReport(QuarkFlavor::Type quarkFlavor,
        DoubleComparisonReport quarkDistributionReport,
        DoubleComparisonReport quarkDistributionPlusReport,
        DoubleComparisonReport quarkDistributionMinusReport) :
        ComparisonReport(
                quarkDistributionReport.isEqual()
                        && quarkDistributionPlusReport.isEqual()
                        && quarkDistributionMinusReport.isEqual()), m_quarkFlavor(
                quarkFlavor), m_quarkDistributionReport(
                quarkDistributionReport), m_quarkDistributionPlusReport(
                quarkDistributionPlusReport), m_quarkDistributionMinusReport(
                quarkDistributionMinusReport) {
}

QuarkDistributionReport::~QuarkDistributionReport() {
}

std::string QuarkDistributionReport::toString() const {
    std::string quarkFlavorShortName =
            QuarkFlavor(m_quarkFlavor).getShortName();

    std::ostringstream os;
    // TODO : Use Formatter?
    os << quarkFlavorShortName << std::endl;
    os << m_quarkDistributionReport.toString() << std::endl;
    os << quarkFlavorShortName << "(+)" << std::endl;
    os << m_quarkDistributionPlusReport.toString() << std::endl;
    os << quarkFlavorShortName << "(-)" << std::endl;
    os << m_quarkDistributionMinusReport.toString() << std::endl;

    return os.str();
}

QuarkFlavor QuarkDistributionReport::getQuarkFlavor() const {
    return m_quarkFlavor;
}
