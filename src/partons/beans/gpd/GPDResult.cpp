#include "../../../../include/partons/beans/gpd/GPDResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

GPDResult::GPDResult() :
        Result<GPDKinematic>("GPDResult", ChannelType::UNDEFINED) {
}

GPDResult::GPDResult(const GPDKinematic& kinematic) :
        Result<GPDKinematic>("GPDResult", ChannelType::UNDEFINED, kinematic) {
}

GPDResult::GPDResult(const GPDResult &other) :
        Result<GPDKinematic>(other) {
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

void GPDResult::addPartonDistribution(GPDType::Type gpdType,
        const PartonDistribution& partonDistribution) {

    //get iterator
    std::map<GPDType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.find(gpdType);

    //throw exception
    if (it != m_partonDistributions.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Result for GPDType = "
                        << GPDType(gpdType).toString() << " exists");
    }

    //insert
    m_partonDistributions.insert(
            std::pair<GPDType::Type, PartonDistribution>(gpdType,
                    partonDistribution));
}

const PartonDistribution& GPDResult::getPartonDistribution(
        GPDType::Type gpdType) const {

    //get iterator
    std::map<GPDType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.find(gpdType);

    //throw exception
    if (it == m_partonDistributions.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot to find PartonDistribution object from GPDType = "
                        << GPDType(gpdType).toString());
    }

    //return
    return (it->second);
}

bool GPDResult::isAvailable(const GPDType::Type& gpdType) const {

    //get iterator
    m_it = m_partonDistributions.find(gpdType);

    //return
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

std::vector<GPDType> GPDResult::listGPDTypeComputed() const {

    //result
    std::vector<GPDType> list;

    //iterator
    std::map<GPDType::Type, PartonDistribution>::const_iterator it;

    //fill vector
    if (m_partonDistributions.size() != 0) {

        for (it = m_partonDistributions.begin();
                it != m_partonDistributions.end(); ++it) {
            list.push_back(it->first);
        }
    }

    //return
    return list;

}

const std::map<GPDType::Type, PartonDistribution>& GPDResult::getPartonDistributions() const {
    return m_partonDistributions;
}

void GPDResult::setPartonDistributions(
        const std::map<GPDType::Type, PartonDistribution>& partonDistributions) {
    m_partonDistributions = partonDistributions;
}

} /* namespace PARTONS */
