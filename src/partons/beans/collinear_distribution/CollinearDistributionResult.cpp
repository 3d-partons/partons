#include "../../../../include/partons/beans/collinear_distribution/CollinearDistributionResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/utils/type/PhysicalType.h"

namespace PARTONS {

CollinearDistributionResult::CollinearDistributionResult() :
        Result<CollinearDistributionKinematic>("CollinearDistributionResult", ChannelType::UNDEFINED) {
}

CollinearDistributionResult::CollinearDistributionResult(const CollinearDistributionKinematic& kinematic) :
        Result<CollinearDistributionKinematic>("CollinearDistributionResult", ChannelType::UNDEFINED, kinematic) {
}

CollinearDistributionResult::CollinearDistributionResult(const CollinearDistributionResult &other) :
        Result<CollinearDistributionKinematic>(other) {
    m_partonDistributions = other.m_partonDistributions;
}

CollinearDistributionResult::~CollinearDistributionResult() {
}

std::string CollinearDistributionResult::toString() const {

    ElemUtils::Formatter formatter;

    formatter << '\n';
    formatter << Result::toString();
    formatter << '\n';

    std::map<CollinearDistributionType::Type, PartonDistribution>::const_iterator it;

    for (it = m_partonDistributions.begin(); it != m_partonDistributions.end();
            it++) {
        formatter << '\n';
        formatter << "Result: " << "collinear distribution " << CollinearDistributionType(it->first).toString();
        formatter << '\n';
        formatter << (it->second).toString();
    }

    return formatter.str();
}

void CollinearDistributionResult::addPartonDistribution(CollinearDistributionType::Type colldistType,
        const PartonDistribution& partonDistribution) {

    //get iterator
    std::map<CollinearDistributionType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.find(colldistType);

    //throw exception
    if (it != m_partonDistributions.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Result for CollinearDistributionType = "
                        << CollinearDistributionType(colldistType).toString() << " exists");
    }

    //insert
    m_partonDistributions.insert(
            std::pair<CollinearDistributionType::Type, PartonDistribution>(colldistType,
                    partonDistribution));
}

const PartonDistribution& CollinearDistributionResult::getPartonDistribution(
        CollinearDistributionType::Type colldistType) const {

    //get iterator
    std::map<CollinearDistributionType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.find(colldistType);

    //throw exception
    if (it == m_partonDistributions.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot to find PartonDistribution object from CollinearDistributionType = "
                        << CollinearDistributionType(colldistType).toString());
    }

    //return
    return (it->second);
}

bool CollinearDistributionResult::isAvailable(const CollinearDistributionType::Type& colldistType) const {

    //get iterator
    m_it = m_partonDistributions.find(colldistType);

    //return
    return (m_it != m_partonDistributions.end());
}

PartonDistribution& CollinearDistributionResult::getLastAvailable() const {
    return const_cast<PartonDistribution&>(m_it->second);
}

void CollinearDistributionResult::compare(ComparisonReport &rootComparisonReport,
        const CollinearDistributionResult &referenceObject, std::string parentObjectInfo) const {

    if (m_kinematic != referenceObject.getKinematic()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot perform comparison because kinematics is diferent ; With CollinearDistributionResult index id = "
                        << referenceObject.getIndexId() << '\n' << toString()
                        << '\n' << referenceObject.toString());
    }

    if (m_partonDistributions.size()
            != referenceObject.getPartonDistributions().size()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot perform comparison between parton distribution map because they are not equal in size ; With CollinearDistributionResult index id = "
                        << referenceObject.getIndexId() << '\n' << toString()
                        << '\n' << referenceObject.toString());
    }

    for (std::map<CollinearDistributionType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.begin(); it != m_partonDistributions.end();
            it++) {
        (it->second).compare(rootComparisonReport,
                referenceObject.getPartonDistribution((it->first)),
                ElemUtils::Formatter() << parentObjectInfo << " collinear-distribution("
                        << m_kinematic.toString() << ") "
                        << CollinearDistributionType(it->first).toString());
    }
}

std::vector<CollinearDistributionType> CollinearDistributionResult::listCollinearDistributionTypeComputed() const {

    //result
    std::vector<CollinearDistributionType> list;

    //iterator
    std::map<CollinearDistributionType::Type, PartonDistribution>::const_iterator it;

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

const std::map<CollinearDistributionType::Type, PartonDistribution>& CollinearDistributionResult::getPartonDistributions() const {
    return m_partonDistributions;
}

void CollinearDistributionResult::setPartonDistributions(
        const std::map<CollinearDistributionType::Type, PartonDistribution>& partonDistributions) {
    m_partonDistributions = partonDistributions;
}

} /* namespace PARTONS */
