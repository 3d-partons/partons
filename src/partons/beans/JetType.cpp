#include "../../../include/partons/beans/JetType.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

JetType::JetType() :
        m_type(JetType::UNDEFINED) {
}

JetType::JetType(Type type) :
        m_type(type) {
}

JetType::JetType(const JetType &other) {
    m_type = other.m_type;
}

JetType::operator JetType::Type() const {
    return m_type;
}

std::string JetType::toString() const {

    switch (m_type) {

    case GLUON:
        return "g";
        break;
    case UP:
        return "u";
        break;
    case DOWN:
        return "d";
        break;
    case STRANGE:
        return "s";
        break;
    case CHARM:
        return "c";
        break;

    default:
        return "UNDEFINED";
    }
}

JetType::Type JetType::getType() const {
    return m_type;
}

void JetType::setType(Type type) {
    m_type = type;
}

JetType::Type JetType::fromString(const std::string& partonTypeStr) {

    JetType::Type partonType = JetType::UNDEFINED;

    if (ElemUtils::StringUtils::equals(partonTypeStr, "g")) {
        partonType = JetType::GLUON;
    } else if (ElemUtils::StringUtils::equals(partonTypeStr, "u")) {
        partonType = JetType::UP;
    } else if (ElemUtils::StringUtils::equals(partonTypeStr, "d")) {
        partonType = JetType::DOWN;
    } else if (ElemUtils::StringUtils::equals(partonTypeStr, "s")) {
        partonType = JetType::STRANGE;
    } else if (ElemUtils::StringUtils::equals(partonTypeStr, "c")) {
        partonType = JetType::CHARM;
    }

    return partonType;
}

void JetType::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void JetType::unserialize(ElemUtils::Packet &packet) {

    int i = 0;
    packet >> i;
    m_type = static_cast<JetType::Type>(i);
}

bool JetType::operator <(const JetType& other) const {
    return (m_type < other.m_type);
}

ElemUtils::Packet & operator <<(ElemUtils::Packet & packet,
        JetType & partonType) {

    partonType.serialize(packet);
    return packet;
}

ElemUtils::Packet & operator >>(ElemUtils::Packet & packet,
        JetType & partonType) {

    partonType.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */

