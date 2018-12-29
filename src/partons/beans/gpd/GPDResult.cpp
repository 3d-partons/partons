#include "../../../../include/partons/beans/gpd/GPDResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

const std::string GPDResult::GPD_RESULT_DB_TABLE_NAME = "gpd_result";

GPDResult::GPDResult() :
        Result("GPDResult", ChannelType::UNDEFINED) {
}

GPDResult::GPDResult(const GPDResult &other) :
        Result(other) {
    m_kinematic = other.m_kinematic;
    m_partonDistributions = other.m_partonDistributions;
}

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
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot to find PartonDistribution object from GPDType = "
                        << GPDType(gpdType).toString());
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

//TODO improve memory usage ; don't copy object parton distribution ; use reference or pointer
List<PartonDistribution> GPDResult::getPartonDistributionList() const {
    List<PartonDistribution> partonDistributionList;

    for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.begin(); it != m_partonDistributions.end();
            ++it) {
        partonDistributionList.add(it->second);
    }

    return partonDistributionList;
}

std::string GPDResult::toString() const {
    ElemUtils::Formatter formatter;
    std::map<GPDType::Type, PartonDistribution>::const_iterator it;

    formatter << BaseObject::toString() << '\n';

    formatter << m_kinematic.toString() << '\n';

    formatter << "[PartonDistributionList]" << '\n';

    for (it = m_partonDistributions.begin(); it != m_partonDistributions.end();
            it++) {
        //os << "ComputedBy: " << m_computedByGPDModuleId << std::endl;
        formatter << "GPD_" << GPDType(it->first).toString() << '\n';
        formatter << (it->second).toString();
        formatter << '\n';
    }

    return formatter.str();
}

const std::map<GPDType::Type, PartonDistribution>& GPDResult::getPartonDistributions() const {
    return m_partonDistributions;
}

void GPDResult::setPartonDistributions(
        const std::map<GPDType::Type, PartonDistribution>& partonDistributions) {
    m_partonDistributions = partonDistributions;
}

const GPDKinematic& GPDResult::getKinematic() const {
    return m_kinematic;
}

void GPDResult::setKinematic(const GPDKinematic& kinematic) {
    m_kinematic = kinematic;
}

void GPDResult::compare(ComparisonReport &rootComparisonReport,
        const GPDResult &referenceObject, std::string parentObjectInfo) const {

    //TODO faire un test pour valider la cinématique associée

    if (size() != referenceObject.size()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot perform comparison between parton distribution map because they are not equal in size ; With GPDResult index id = "
                        << referenceObject.getIndexId() << '\n' << toString()
                        << '\n' << referenceObject.toString());
    }

    for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.begin(); it != m_partonDistributions.end();
            it++) {
        (it->second).compare(rootComparisonReport,
                referenceObject.getPartonDistribution((it->first)),
                ElemUtils::Formatter() << parentObjectInfo << " "
                        << this->getObjectInfo() << " "
                        << GPDType(it->first).toString());
    }
}

std::string GPDResult::getObjectInfo() const {
    return ElemUtils::Formatter() << "GPD( " << m_kinematic.toString() << ")";
}

bool GPDResult::isAvailable(const GPDType::Type& gpdType) const {
    bool result = false;

    m_it = m_partonDistributions.find(gpdType);

    if (m_it != m_partonDistributions.end()) {
        result = true;
    }

    return result;
}

PartonDistribution& GPDResult::getLastAvailable() const {
    return const_cast<PartonDistribution&>(m_it->second);
}

size_t GPDResult::size() const {
    return m_partonDistributions.size();
}

bool GPDResult::operator <(const GPDResult& other) const {
    return (m_kinematic < other.m_kinematic);
}

} /* namespace PARTONS */
