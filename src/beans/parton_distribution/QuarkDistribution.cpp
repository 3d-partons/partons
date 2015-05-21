#include "QuarkDistribution.h"

#include <sstream>

//#include <sstream>

const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION =
        "quark_distribution";
const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_PLUS =
        "quark_distribution_plus";
const std::string QuarkDistribution::QUARK_DISTRIBUTION_DB_COLUMN_NAME_QUARK_DISTRIBUTION_MINUS =
        "quark_distribution_minus";

QuarkDistribution::QuarkDistribution(QuarkFlavor::Type quarkFlavor)
        : m_quarkFlavor(quarkFlavor), m_quarkDistribution(0.), m_quarkDistributionPlus(
                0.), m_quarkDistributionMinus(0.) {
}

QuarkDistribution::QuarkDistribution(QuarkFlavor::Type quarkFlavor,
        double quarkDistribution, double quarkDistributionPlus,
        double quarkDistributionMinus)
        : m_quarkFlavor(quarkFlavor), m_quarkDistribution(quarkDistribution), m_quarkDistributionPlus(
                quarkDistributionPlus), m_quarkDistributionMinus(
                quarkDistributionMinus) {
}

QuarkDistribution::~QuarkDistribution() {
}

std::string QuarkDistribution::toString() const {
    std::ostringstream os;

    os << "H+(" << QuarkFlavor(m_quarkFlavor).getShortName() << ") = "
            << m_quarkDistributionPlus << std::endl;
    os << "H-(" << QuarkFlavor(m_quarkFlavor).getShortName() << ") = "
            << m_quarkDistributionMinus << std::endl;

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
