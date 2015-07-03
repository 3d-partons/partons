#include "PartonDistributionReport.h"

#include <sstream>
#include <utility>
#include "../../../beans/parton_distribution/QuarkDistribution.h"

PartonDistributionReport::PartonDistributionReport() :
        ComparisonReport(), m_commonPartonType(false), m_gpdType(GPDType::UNDEFINED), m_gluonDistributionReport() {
    // TODO Something should be done to initialize m_quarkDistributionReports
}

PartonDistributionReport::PartonDistributionReport(GPDType::Type gpdType) :
        ComparisonReport(), m_commonPartonType(false), m_gpdType(gpdType), m_gluonDistributionReport() {
    // TODO Something should be done to initialize m_quarkDistributionReports
}

PartonDistributionReport::~PartonDistributionReport() {
}

std::string PartonDistributionReport::toString() const {
    std::ostringstream os;

    os << "g" << std::endl;
    os << m_gluonDistributionReport.toString();

    std::map<QuarkFlavor::Type, QuarkDistributionReport>::const_iterator it;

    for (it = m_quarkDistributionReports.begin(); it != m_quarkDistributionReports.end();
            it++) {
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
        const DoubleComparisonReport& gluonDistributionReport) {
    m_gluonDistributionReport = gluonDistributionReport;
}

void PartonDistributionReport::setCommonPartonType(bool commonPartonType) {
    m_commonPartonType = commonPartonType;
}
