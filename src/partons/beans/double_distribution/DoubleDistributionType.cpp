#include "../../../../include/partons/beans/double_distribution/DoubleDistributionType.h"

namespace PARTONS {


DoubleDistributionType::DoubleDistributionType() :
        m_type(DoubleDistributionType::UNDEFINED) {
}

DoubleDistributionType::DoubleDistributionType(
        DoubleDistributionType::Type type) {
    m_type = type;
}

DoubleDistributionType::~DoubleDistributionType() {
}

DoubleDistributionType::operator DoubleDistributionType::Type() const {
    return m_type;
}

std::string DoubleDistributionType::toString() const {

    switch (m_type) {
    case ALL:
        return "ALL";
        break;
    case F:
        return "F";
        break;
    case G:
        return "G";
        break;
    case K:
        return "K";
        break;
    case fM:
        return "fM";
        break;
    case fP:
        return "fP";
        break;
    default:
        return "UNDEFINED";
    }
}

} /* namespace PARTONS */
