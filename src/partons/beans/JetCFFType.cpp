#include "../../../include/partons/beans/JetCFFType.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

JetCFFType::JetCFFType() :
        m_type(JetCFFType::UNDEFINED) {
}

JetCFFType::JetCFFType(Type type) :
        m_type(type) {
}

JetCFFType::JetCFFType(const JetCFFType &other) {
    m_type = other.m_type;
}

JetCFFType::operator JetCFFType::Type() const {
    return m_type;
}

std::string JetCFFType::toString() const {

    switch (m_type) {

    case LL:
        return "LL";
        break;
    case TL:
        return "TL";
        break;
    case TT1:
        return "TT1";
        break;
    case TT2:
        return "TT2";
        break;

    default:
        return "UNDEFINED";
    }
}

JetCFFType::Type JetCFFType::getType() const {
    return m_type;
}

void JetCFFType::setType(Type type) {
    m_type = type;
}

JetCFFType::Type JetCFFType::fromString(const std::string& partonTypeStr) {

    JetCFFType::Type partonType = JetCFFType::UNDEFINED;

    if (ElemUtils::StringUtils::equals(partonTypeStr, "LL")) {
        partonType = JetCFFType::LL;
    } else if (ElemUtils::StringUtils::equals(partonTypeStr, "TL")) {
        partonType = JetCFFType::TL;
    } else if (ElemUtils::StringUtils::equals(partonTypeStr, "TT1")) {
        partonType = JetCFFType::TT1;
    } else if (ElemUtils::StringUtils::equals(partonTypeStr, "TT2")) {
        partonType = JetCFFType::TT2;
    }

    return partonType;
}

void JetCFFType::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void JetCFFType::unserialize(ElemUtils::Packet &packet) {

    int i = 0;
    packet >> i;
    m_type = static_cast<JetCFFType::Type>(i);
}

bool JetCFFType::operator <(const JetCFFType& other) const {
    return (m_type < other.m_type);
}

ElemUtils::Packet & operator <<(ElemUtils::Packet & packet,
        JetCFFType & partonType) {

    partonType.serialize(packet);
    return packet;
}

ElemUtils::Packet & operator >>(ElemUtils::Packet & packet,
        JetCFFType & partonType) {

    partonType.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */

