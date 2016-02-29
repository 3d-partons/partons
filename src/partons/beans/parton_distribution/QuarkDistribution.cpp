#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"

#include <sstream>

#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"
#include "../../../../include/partons/utils/compare/ComparisonReport.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION =
        "quark_distribution";
const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS =
        "quark_distribution_plus";
const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS =
        "quark_distribution_minus";

QuarkDistribution::QuarkDistribution() :
        BaseObject("QuarkDistribution"), m_quarkFlavor(QuarkFlavor::UNDEFINED), m_quarkDistribution(
                0.), m_quarkDistributionPlus(0.), m_quarkDistributionMinus(0.) {
}

QuarkDistribution::QuarkDistribution(QuarkFlavor::Type quarkFlavor) :
        BaseObject("QuarkDistribution"), m_quarkFlavor(quarkFlavor), m_quarkDistribution(
                0.), m_quarkDistributionPlus(0.), m_quarkDistributionMinus(0.) {
}

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
    std::string quarkFlavorShortName =
            QuarkFlavor(m_quarkFlavor).getShortName();

    std::ostringstream os;
    // TODO : Use Formatter?
    os << quarkFlavorShortName << " = " << m_quarkDistribution << std::endl;
    os << quarkFlavorShortName << "(+) = " << m_quarkDistributionPlus
            << std::endl;
    os << quarkFlavorShortName << "(-) = " << m_quarkDistributionMinus
            << std::endl;

    return os.str();
}

double QuarkDistribution::getAntiQuark() const {
    return antiQuark;
}

void QuarkDistribution::setAntiQuark(double antiQuark) {
    this->antiQuark = antiQuark;
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

QuarkFlavor::Type QuarkDistribution::getQuarkFlavor() const {
    return m_quarkFlavor;
}

void QuarkDistribution::setQuarkFlavor(QuarkFlavor::Type quarkFlavorType) {
    m_quarkFlavor = quarkFlavorType;
}

double QuarkDistribution::getQuark() const {
    return quark;
}

void QuarkDistribution::setQuark(double quark) {
    this->quark = quark;
}

void QuarkDistribution::compare(ComparisonReport &rootComparisonReport,
        const QuarkDistribution &referenceObject,
        const NumA::Tolerances &tolerances,
        std::string parentObjectInfo) const {
    ComparisonData quark_distribution_value_comparisonData =
            CompareUtils::compareDouble(
                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION,
                    m_quarkDistribution, referenceObject.getQuarkDistribution(),
                    tolerances,
                    Formatter() << parentObjectInfo << " " << getClassName());

    ComparisonData quark_distribution_plus_value_comparisonData =
            CompareUtils::compareDouble(
                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS,
                    m_quarkDistributionPlus,
                    referenceObject.getQuarkDistributionPlus(), tolerances,
                    Formatter() << parentObjectInfo << " " << getClassName());

    ComparisonData quark_distribution_minus_value_comparisonData =
            CompareUtils::compareDouble(
                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS,
                    m_quarkDistributionMinus,
                    referenceObject.getQuarkDistributionMinus(), tolerances,
                    Formatter() << parentObjectInfo << " " << getClassName());

    rootComparisonReport.addComparisonData(
            quark_distribution_value_comparisonData);
    rootComparisonReport.addComparisonData(
            quark_distribution_plus_value_comparisonData);
    rootComparisonReport.addComparisonData(
            quark_distribution_minus_value_comparisonData);
}

//ComparisonReport QuarkDistribution::compare(
//        const QuarkDistribution& referenceObject,
//        const NumA::Tolerances& tolerances) const {
//    ComparisonReport comparisonReport(getClassName());
//
//    ComparisonData quark_distribution_plus_comparisonData =
//            CompareUtils::compareDouble(
//                    QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS,
//                    getQuarkDistributionPlus(),
//                    referenceObject.getQuarkDistributionPlus(), tolerances);
//    comparisonReport.addComparisonData(quark_distribution_plus_comparisonData);
//
//    return comparisonReport;
//}
