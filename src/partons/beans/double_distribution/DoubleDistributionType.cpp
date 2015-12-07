#include "../../../../include/partons/beans/double_distribution/DoubleDistributionType.h"

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

std::string DoubleDistributionType::toString() {

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
    default:
        return "UNDEFINED";
    }
}
