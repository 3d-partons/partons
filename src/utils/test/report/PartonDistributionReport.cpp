#include "PartonDistributionReport.h"

#include <sstream>
#include <stdexcept>
#include <utility>

PartonDistributionReport::PartonDistributionReport() :
        ComparisonReport(), m_commonPartonType(false), m_lhsUndefinedGluons(
                false), m_rhsUndefinedGluons(false), m_gluonDistributionReport() {
}

PartonDistributionReport::~PartonDistributionReport() {
}

std::string PartonDistributionReport::toString() const {

    std::ostringstream os;
    QuarkFlavor quarkFlavorPrint;

    os << "lhs parton distributions" << std::endl;
    if (m_lhsQuarkFlavors.empty())
        os << "No quark distributions on lhs of comparison" << std::endl;
    else {
        for (unsigned int i = 0; i < m_lhsQuarkFlavors.size(); i++) {
            quarkFlavorPrint.setType(m_lhsQuarkFlavors.at(i));
            os << quarkFlavorPrint.toString() << "    ";
        }
    }
    if (m_lhsUndefinedGluons) {
        os << std::endl;
        os << "No gluon distributions on lhs of comparison" << std::endl;
    } else
        os << "GLUON" << std::endl;
    os << std::endl;

    os << "rhs parton distributions" << std::endl;
    if (m_rhsQuarkFlavors.empty())
        os << "No quark distributions on rhs of comparison" << std::endl;
    else {
        for (unsigned int i = 0; i < m_rhsQuarkFlavors.size(); i++) {
            quarkFlavorPrint.setType(m_rhsQuarkFlavors.at(i));
            os << quarkFlavorPrint.toString() << "    ";
        }
    }
    if (m_rhsUndefinedGluons) {
        os << std::endl;
        os << "No gluon distributions on rhs of comparison" << std::endl;
    } else
        os << "GLUON" << std::endl;
    os << std::endl;

    if (m_lhsUndefinedGluons && m_rhsUndefinedGluons) {
        os << "g" << std::endl;
        os << m_gluonDistributionReport.toString();
    }

    std::map<QuarkFlavor::Type, QuarkDistributionReport>::const_iterator it;

    for (it = m_quarkDistributionReports.begin();
            it != m_quarkDistributionReports.end(); it++) {
        os << (it->second).toString();
    }

    return os.str();
}

void PartonDistributionReport::addQuarkDistributionReport(
        const QuarkDistributionReport& quarkDistributionReport) {
    m_quarkDistributionReports.insert(
            std::make_pair(quarkDistributionReport.getQuarkFlavor(),
                    quarkDistributionReport));
}

void PartonDistributionReport::setGluonDistributionReport(
        const GluonDistributionReport &gluonDistributionReport) {
    m_gluonDistributionReport = gluonDistributionReport;
}

void PartonDistributionReport::setCommonPartonType(bool commonPartonType) {
    m_commonPartonType = commonPartonType;
}

const GluonDistributionReport& PartonDistributionReport::getGluonDistributionReport() const {
    return m_gluonDistributionReport;
}

const QuarkDistributionReport& PartonDistributionReport::getQuarkDistributionReport(
        QuarkFlavor::Type quarkFlavorType) const {

    std::map<QuarkFlavor::Type, QuarkDistributionReport>::const_iterator it =
            m_quarkDistributionReports.find(quarkFlavorType);

    if (it == m_quarkDistributionReports.end()) {
        throw std::runtime_error(
                "[PartonDistributionReport::getQuarkDistributionReport] Enable to find QuardDistributionReport object from QuarFlavorType = "
                        + QuarkFlavor(quarkFlavorType).getShortName());
    }

    return (it->second);
}

const std::vector<QuarkFlavor::Type>& PartonDistributionReport::getLhsQuarkFlavors() const {
    return m_lhsQuarkFlavors;
}

void PartonDistributionReport::setLhsQuarkFlavors(
        const std::vector<QuarkFlavor::Type>& lhsQuarkFlavors) {
    m_lhsQuarkFlavors = lhsQuarkFlavors;
}

bool PartonDistributionReport::isLhsUndefinedGluons() const {
    return m_lhsUndefinedGluons;
}

void PartonDistributionReport::setLhsUndefinedGluons(bool lhsUndefinedGluons) {
    m_lhsUndefinedGluons = lhsUndefinedGluons;
}

const std::vector<QuarkFlavor::Type>& PartonDistributionReport::getRhsQuarkFlavors() const {
    return m_rhsQuarkFlavors;
}

void PartonDistributionReport::setRhsQuarkFlavors(
        const std::vector<QuarkFlavor::Type>& rhsQuarkFlavors) {
    m_rhsQuarkFlavors = rhsQuarkFlavors;
}

bool PartonDistributionReport::isRhsUndefinedGluons() const {
    return m_rhsUndefinedGluons;
}

void PartonDistributionReport::setRhsUndefinedGluons(bool rhsUndefinedGluons) {
    m_rhsUndefinedGluons = rhsUndefinedGluons;
}
