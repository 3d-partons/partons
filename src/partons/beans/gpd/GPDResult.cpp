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

GPDResult::GPDResult(const GPDKinematic& kinematic) :
        Result("GPDResult", ChannelType::UNDEFINED) {
    m_kinematic = kinematic;
}

GPDResult::GPDResult(const GPDResult &other) :
        Result(other) {

    m_kinematic = other.m_kinematic;
    m_partonDistributions = other.m_partonDistributions;
}

GPDResult::~GPDResult() {
}

std::string GPDResult::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Result::toString();

    std::map<GPDType::Type, PartonDistribution>::const_iterator it;

    for (it = m_partonDistributions.begin(); it != m_partonDistributions.end();
            it++) {
        formatter << "GPD_" << GPDType(it->first).toString() << '\n';
        formatter << (it->second).toString();
        formatter << '\n';
    }
    formatter << "Kinematics: " << m_kinematic.toString();

    return formatter.str();
}

bool GPDResult::operator <(const GPDResult& other) const {
    return (m_kinematic < other.m_kinematic);
}

void GPDResult::addPartonDistribution(GPDType::Type gpdType,
        const PartonDistribution& partonDistribution) {
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

bool GPDResult::isAvailable(const GPDType::Type& gpdType) const {

    m_it = m_partonDistributions.find(gpdType);

    return (m_it != m_partonDistributions.end());
}

PartonDistribution& GPDResult::getLastAvailable() const {
    return const_cast<PartonDistribution&>(m_it->second);
}

void GPDResult::compare(ComparisonReport &rootComparisonReport,
        const GPDResult &referenceObject, std::string parentObjectInfo) const {

    //TODO faire un test pour valider la cinématique associée

    if (m_partonDistributions.size()
            != referenceObject.getPartonDistributions().size()) {
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
                ElemUtils::Formatter() << parentObjectInfo << " GPD("
                        << m_kinematic.toString() << ") "
                        << GPDType(it->first).toString());
    }
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

} /* namespace PARTONS */
