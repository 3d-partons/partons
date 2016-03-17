#include "../../../../include/partons/beans/double_distribution/DoubleDistributionResult.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <sstream>
#include <utility>

DoubleDistributionResult::DoubleDistributionResult() :
        BaseObject("DualDistributionResult") {
}

DoubleDistributionResult::~DoubleDistributionResult() {
}

void DoubleDistributionResult::addPartonDistribution(
        DoubleDistributionType::Type doubleDistributionType,
        PartonDistribution partonDistribution) {
    // TODO: The object partonDistribution already has a GPDType member, so the arguments of the function are redundant (without check...).
    m_partonDistributions.insert(
            std::pair<DoubleDistributionType::Type, PartonDistribution>(
                    doubleDistributionType, partonDistribution));
}

const PartonDistribution& DoubleDistributionResult::getPartonDistribution(
        DoubleDistributionType::Type doubleDistributionType) const {

    std::map<DoubleDistributionType::Type, PartonDistribution>::const_iterator it =
            m_partonDistributions.find(doubleDistributionType);

    if (it == m_partonDistributions.end()) {
        error(__func__,
                ElemUtils::Formatter()
                        << "Enable to find PartonDistribution object from type = "
                        << DoubleDistributionType(doubleDistributionType).toString());

    }

    return (it->second);
}

std::string DoubleDistributionResult::toString() {
    std::ostringstream os;
    std::map<DoubleDistributionType::Type, PartonDistribution>::const_iterator it;

    for (it = m_partonDistributions.begin(); it != m_partonDistributions.end();
            it++) {
        os << "DD_" << DoubleDistributionType(it->first).toString()
                << std::endl;
        os << (it->second).toString();
        os << std::endl;
    }

    return os.str();
}
