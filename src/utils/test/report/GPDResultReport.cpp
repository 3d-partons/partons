#include "GPDResultReport.h"

#include <sstream>
#include <stdexcept>
#include <utility>

//#include "../../../beans/parton_distribution/PartonDistribution.h"

GPDResultReport::GPDResultReport() :
        ComparisonReport(), m_commonGPDType(false) {
}

GPDResultReport::~GPDResultReport() {
}

bool GPDResultReport::isCommonGpdType() const {
    return m_commonGPDType;
}

void GPDResultReport::setCommonGpdType(bool commonGpdType) {
    m_commonGPDType = commonGpdType;
}

std::string GPDResultReport::toString() const {

    std::ostringstream os;
    std::map<GPDType::Type, PartonDistributionReport>::const_iterator it;

    for (it = m_partonDistributionReports.begin();
            it != m_partonDistributionReports.end(); it++) {
        os << "GPD_" << GPDType(it->first).toString() << std::endl;
        os << (it->second).toString();
        os << std::endl;
    }

    return os.str();
}

void GPDResultReport::addPartonDistributionReport(GPDType::Type gpdType,
        PartonDistributionReport partonDistributionReport) {
    m_partonDistributionReports.insert(
            std::make_pair(gpdType, partonDistributionReport));
}

const PartonDistributionReport& GPDResultReport::getPartonDistributionReport(
        GPDType::Type gpdType) const {

    std::map<GPDType::Type, PartonDistributionReport>::const_iterator it =
            m_partonDistributionReports.find(gpdType);

    if (it == m_partonDistributionReports.end()) {
        throw std::runtime_error(
                "[GPDResultReport::getPartonDistributionReport] Enable to find PartonDistributionReport object from GPDType = "
                        + GPDType(gpdType).toString());
    }

    return (it->second);
}
