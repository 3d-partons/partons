#include "GPDResultReport.h"

#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

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
    GPDType gpdTypePrint;

    os << "GPD kinematics" << std::endl;
    os << m_gpdKinematicReport.toString() << std::endl;

    os << "lhs GPD types" << std::endl;
    if (m_lhsGpdTypes.empty())
        os << "No GPD types on lhs of comparison" << std::endl;
    else {
        for (unsigned int i = 0; i < m_lhsGpdTypes.size(); i++) {
            gpdTypePrint.setType(m_lhsGpdTypes.at(i));
            os << gpdTypePrint.toString() << "    ";
        }
    }
    os << std::endl;

    os << "rhs GPD types" << std::endl;
    if (m_rhsGpdTypes.empty())
        os << "No GPD types on rhs of comparison" << std::endl;
    else {
        for (unsigned int i = 0; i < m_rhsGpdTypes.size(); i++) {
            gpdTypePrint.setType(m_rhsGpdTypes.at(i));
            os << gpdTypePrint.toString() << "    ";
        }
    }
    os << std::endl;

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

const std::vector<GPDType::Type>& GPDResultReport::getLhsGpdTypes() const {
    return m_lhsGpdTypes;
}

void GPDResultReport::setLhsGpdTypes(
        const std::vector<GPDType::Type>& lhsGpdTypes) {
    m_lhsGpdTypes = lhsGpdTypes;
}

const std::vector<GPDType::Type>& GPDResultReport::getRhsGpdTypes() const {
    return m_rhsGpdTypes;
}

void GPDResultReport::setRhsGpdTypes(
        const std::vector<GPDType::Type>& rhsGpdTypes) {
    m_rhsGpdTypes = rhsGpdTypes;
}
