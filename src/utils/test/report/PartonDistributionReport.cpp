#include "PartonDistributionReport.h"

#include <sstream>
#include <stdexcept>
#include <utility>

PartonDistributionReport::PartonDistributionReport() :
        ComparisonReport(), m_commonPartonType(false), m_gpdType(
                GPDType::UNDEFINED), m_gluonDistributionReport() {
}

PartonDistributionReport::PartonDistributionReport(GPDType::Type gpdType) :
        ComparisonReport(), m_commonPartonType(false), m_gpdType(gpdType), m_gluonDistributionReport() {
}

PartonDistributionReport::~PartonDistributionReport() {
}

std::string PartonDistributionReport::toString() const {

    std::ostringstream os;

    os << "g" << std::endl;
    os << m_gluonDistributionReport.toString();

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
