#include "../../../include/partons/beans/MesonType.h"

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

namespace PARTONS {

MesonType::MesonType() :
        m_type(MesonType::UNDEFINED) {
}

MesonType::MesonType(Type type) :
        m_type(type) {
}

MesonType::MesonType(const MesonType &other) {
    m_type = other.m_type;
}

MesonType::operator MesonType::Type() const {
    return m_type;
}

std::string MesonType::toString() const {

    switch (m_type) {

    case RHOMINUS:
        return "RHOMINUS";
        break;
    case RHO0:
        return "RHO0";
        break;
    case RHOPLUS:
        return "RHOPLUS";
        break;
    case OMEGA:
        return "OMEGA";
        break;
    case PHI:
        return "PHI";
        break;
    case JPSI:
        return "JPSI";
        break;
    case UPSILON:
        return "UPSILON";
        break;

    case PIMINUS:
        return "PIMINUS";
        break;
    case PI0:
        return "PI0";
        break;
    case PIPLUS:
        return "PIPLUS";
        break;

    default:
        return "UNDEFINED";
    }
}

MesonType::Type MesonType::getType() const {
    return m_type;
}

void MesonType::setType(Type type) {
    m_type = type;
}

MesonType::Type MesonType::fromString(const std::string& mesonTypeStr) {

    MesonType::Type mesonType = MesonType::UNDEFINED;

    if (ElemUtils::StringUtils::equals(mesonTypeStr, "rho-")) {
        mesonType = MesonType::RHOMINUS;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "rho0")) {
        mesonType = MesonType::RHO0;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "rho+")) {
        mesonType = MesonType::RHOPLUS;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "omega")) {
        mesonType = MesonType::OMEGA;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "phi")) {
        mesonType = MesonType::PHI;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "J/Psi")) {
        mesonType = MesonType::JPSI;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "upsilon")) {
        mesonType = MesonType::UPSILON;
    }

    else if (ElemUtils::StringUtils::equals(mesonTypeStr, "pi-")) {
        mesonType = MesonType::PIMINUS;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "pi0")) {
        mesonType = MesonType::PI0;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "pi+")) {
        mesonType = MesonType::PIPLUS;
    }

    return mesonType;
}

void MesonType::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void MesonType::unserialize(ElemUtils::Packet &packet) {

    int i = 0;
    packet >> i;
    m_type = static_cast<MesonType::Type>(i);
}

bool MesonType::operator <(const MesonType& other) const {
    return (m_type < other.m_type);
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        MesonType& mesonType) {

    mesonType.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        MesonType& mesonType) {

    mesonType.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */

