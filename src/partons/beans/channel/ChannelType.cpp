#include "../../../../include/partons/beans/channel/ChannelType.h"

namespace PARTONS {

ChannelType::ChannelType() :
        m_type(ChannelType::UNDEFINED) {
}

ChannelType::ChannelType(Type type) :
        m_type(type) {
}
ChannelType::operator ChannelType::Type() const {
    return m_type;
}

std::string ChannelType::toString() const {
    switch (m_type) {
    case DVCS:
        return "DVCS";
        break;
    case DVMP:
        return "DVMP";
        break;
    case TCS:
        return "TCS";
        break;
    case GAM2:
        return "GAM2";
        break;
    case DDVCS:
        return "DDVCS";
        break;
    default:
        return "UNDEFINED";
    }
}

std::string ChannelType::getShortName() {
    switch (m_type) {
    case DVCS:
        return "DVCS";
        break;
    case DVMP:
        return "DVMP";
        break;
    case TCS:
        return "TCS";
        break;
    case GAM2:
        return "GAM2";
        break;
    case DDVCS:
        return "DDVCS";
        break;
    default:
        return "UNDEFINED";
    }
}

ChannelType::Type ChannelType::getType() const {
    return m_type;
}

void ChannelType::setType(Type type) {
    m_type = type;
}

ChannelType::~ChannelType() {
}

} /* namespace PARTONS */
