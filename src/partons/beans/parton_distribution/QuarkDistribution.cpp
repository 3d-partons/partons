#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"

namespace PARTONS {

const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION =
        "quark_distribution";
const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS =
        "quark_distribution_plus";
const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS =
        "quark_distribution_minus";

QuarkDistribution::QuarkDistribution(const QuarkDistribution &other) :
        BaseObject(other) {
    m_quarkFlavor = other.m_quarkFlavor;
    m_quarkDistribution = other.m_quarkDistribution;
    m_quarkDistributionPlus = other.m_quarkDistributionPlus;
    m_quarkDistributionMinus = other.m_quarkDistributionMinus;

    //TODO add missing stuff
}

////TODO initialise missing member
//QuarkDistribution::QuarkDistribution() :
//        BaseObject("QuarkDistribution"), m_quarkFlavor(QuarkFlavor::UNDEFINED), m_quarkDistribution(
//                0.), m_quarkDistributionPlus(0.), m_quarkDistributionMinus(0.) {
//}

//QuarkDistribution::QuarkDistribution(QuarkFlavor::Type quarkFlavor) :
//        BaseObject("QuarkDistribution"), m_quarkFlavor(quarkFlavor), m_quarkDistribution(
//                0.), m_quarkDistributionPlus(0.), m_quarkDistributionMinus(0.) {
//}

QuarkDistribution::QuarkDistribution(QuarkFlavor::Type quarkFlavor,
        double quarkDistribution, double quarkDistributionPlus,
        double quarkDistributionMinus) :
        BaseObject("QuarkDistribution"), m_quarkFlavor(quarkFlavor), m_quarkDistribution(
                quarkDistribution), m_quarkDistributionPlus(
                quarkDistributionPlus), m_quarkDistributionMinus(
                quarkDistributionMinus) {
}

QuarkDistribution::~QuarkDistribution() {
}

std::string QuarkDistribution::toString() const {

    ElemUtils::Formatter formatter;

    std::string quarkFlavorShortName =
            QuarkFlavor(m_quarkFlavor).getShortName();

    formatter << quarkFlavorShortName << ": " << m_quarkDistribution << ' ';
    formatter << quarkFlavorShortName << "(+): " << m_quarkDistributionPlus
            << ' ';
    formatter << quarkFlavorShortName << "(-): " << m_quarkDistributionMinus;

    return formatter.str();
}

double QuarkDistribution::getQuarkDistribution() const {
    return m_quarkDistribution;
}

void QuarkDistribution::setQuarkDistribution(double quarkDistribution) {
    m_quarkDistribution = quarkDistribution;
}

double QuarkDistribution::getQuarkDistributionPlus() const {
    return m_quarkDistributionPlus;
}

void QuarkDistribution::setQuarkDistributionPlus(double quarkDistributionPlus) {
    m_quarkDistributionPlus = quarkDistributionPlus;
}

double QuarkDistribution::getQuarkDistributionMinus() const {
    return m_quarkDistributionMinus;
}

void QuarkDistribution::setQuarkDistributionMinus(
        double quarkDistributionMinus) {
    m_quarkDistributionMinus = quarkDistributionMinus;
}

QuarkFlavor QuarkDistribution::getQuarkFlavor() const {
    return m_quarkFlavor;
}

void QuarkDistribution::setQuarkFlavor(QuarkFlavor quarkFlavorType) {
    m_quarkFlavor = quarkFlavorType;
}

void QuarkDistribution::compare(ComparisonReport &rootComparisonReport,
        const QuarkDistribution &referenceObject,
        std::string parentObjectInfo) const {
    ComparisonData quark_distribution_value_comparisonData =
            CompareUtils::compareDouble(
                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION,
                    m_quarkDistribution, referenceObject.getQuarkDistribution(),
                    rootComparisonReport.getTolerances(),
                    ElemUtils::Formatter() << parentObjectInfo << " "
                            << getClassName());

    ComparisonData quark_distribution_plus_value_comparisonData =
            CompareUtils::compareDouble(
                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS,
                    m_quarkDistributionPlus,
                    referenceObject.getQuarkDistributionPlus(),
                    rootComparisonReport.getTolerances(),
                    ElemUtils::Formatter() << parentObjectInfo << " "
                            << getClassName());

    ComparisonData quark_distribution_minus_value_comparisonData =
            CompareUtils::compareDouble(
                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS,
                    m_quarkDistributionMinus,
                    referenceObject.getQuarkDistributionMinus(),
                    rootComparisonReport.getTolerances(),
                    ElemUtils::Formatter() << parentObjectInfo << " "
                            << getClassName());

    rootComparisonReport.addComparisonData(
            quark_distribution_value_comparisonData);
    rootComparisonReport.addComparisonData(
            quark_distribution_plus_value_comparisonData);
    rootComparisonReport.addComparisonData(
            quark_distribution_minus_value_comparisonData);
}

} /* namespace PARTONS */
