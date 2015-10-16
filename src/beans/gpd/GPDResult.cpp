#include "GPDResult.h"

#include <sstream>
#include <stdexcept>
#include <utility>

const std::string GPDResult::GPD_RESULT_DB_TABLE_NAME = "gpd_result";

GPDResult::GPDResult() :
        m_computedByGPDModuleId("UNDEFINED") {
}

//GPDResult::GPDResult(GPDKinematic &gpdKinematic)
//        : m_gpdKinematic(gpdKinematic) {
//}

GPDResult::~GPDResult() {

}

void GPDResult::addPartonDistribution(GPDType::Type gpdType,
        PartonDistribution partonDistribution) {
    // TODO: The object partonDistribution already has a GPDType member, so the arguments of the function are redundant (without check...).
    m_partonDistributions.insert(
            std::pair<GPDType::Type, PartonDistribution>(gpdType,
                    partonDistribution));
}

const PartonDistribution& GPDResult::getPartonDistribution(
        GPDType::Type gpdType) const {

    std::map<GPDType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.find(gpdType);

    if (it == m_partonDistributions.end()) {
        throw std::runtime_error(
                "[GPDResult::getPartonDistribution] Enable to find PartonDistribution object from GPDType = "
                        + GPDType(gpdType).toString());
    }

    return (it->second);
}

//TODO tester cette méthode
std::vector<GPDType> GPDResult::listGPDTypeComputed() {
    std::vector<GPDType> list;

    std::map<GPDType::Type, PartonDistribution>::const_iterator it;

    if (m_partonDistributions.size() != 0) {

        for (it = m_partonDistributions.begin();
                it != m_partonDistributions.end(); ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

const std::vector<PartonDistribution> GPDResult::getPartonDistributionList() const {
    std::vector<PartonDistribution> partonDistributionList;
    std::map<GPDType::Type, PartonDistribution>::const_iterator it;

    for (it = m_partonDistributions.begin(); it != m_partonDistributions.end();
            ++it) {
        partonDistributionList.push_back(it->second);
    }

    return partonDistributionList;
}

std::string GPDResult::toString() {
    std::ostringstream os;
    std::map<GPDType::Type, PartonDistribution>::const_iterator it;

    for (it = m_partonDistributions.begin(); it != m_partonDistributions.end();
            it++) {
        //os << "ComputedBy: " << m_computedByGPDModuleId << std::endl;
        os << "GPD_" << GPDType(it->first).toString() << std::endl;
        os << (it->second).toString();
        os << std::endl;
    }

    return os.str();
}

const std::map<GPDType::Type, PartonDistribution>& GPDResult::getPartonDistributions() const {
    return m_partonDistributions;
}

void GPDResult::setPartonDistributions(
        const std::map<GPDType::Type, PartonDistribution>& partonDistributions) {
    m_partonDistributions = partonDistributions;
}

const std::string& GPDResult::getComputedByGpdModuleId() const {
    return m_computedByGPDModuleId;
}

void GPDResult::setComputedByGpdModuleId(
        const std::string& computedByGpdModuleId) {
    m_computedByGPDModuleId = computedByGpdModuleId;
}

const GPDKinematic& GPDResult::getKinematic() const {
    return m_kinematic;
}

void GPDResult::setKinematic(const GPDKinematic& kinematic) {
    m_kinematic = kinematic;
}
