#include "../../../include/partons/beans/MesonPolarization.h"

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

namespace PARTONS {

MesonPolarization::MesonPolarization() :
        m_type(MesonPolarization::UNDEFINED) {
}

MesonPolarization::MesonPolarization(Type type) :
        m_type(type) {
}

MesonPolarization::MesonPolarization(const MesonPolarization &other) {
    m_type = other.m_type;
}

MesonPolarization::operator MesonPolarization::Type() const {
    return m_type;
}

std::string MesonPolarization::toString() const {

    switch (m_type) {

    case L:
        return "L";
        break;
    case T:
        return "T";
        break;

    default:
        return "UNDEFINED";
    }
}

MesonPolarization::Type MesonPolarization::getType() const {
    return m_type;
}

void MesonPolarization::setType(Type type) {
    m_type = type;
}

MesonPolarization::Type MesonPolarization::fromString(
        const std::string& mesonPolarizationStr) {

    MesonPolarization::Type mesonPolarization = MesonPolarization::UNDEFINED;

    if (ElemUtils::StringUtils::equals(mesonPolarizationStr, "L")) {
        mesonPolarization = MesonPolarization::L;
    } else if (ElemUtils::StringUtils::equals(mesonPolarizationStr, "T")) {
        mesonPolarization = MesonPolarization::T;
    }

    return mesonPolarization;
}

void MesonPolarization::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void MesonPolarization::unserialize(ElemUtils::Packet &packet) {

    int i;
    packet >> i;
    m_type = static_cast<MesonPolarization::Type>(i);
}

bool MesonPolarization::operator <(const MesonPolarization& other) const {
    return (m_type < other.m_type);
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        MesonPolarization& mesonPolarization) {

    mesonPolarization.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        MesonPolarization& mesonPolarization) {

    mesonPolarization.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */

