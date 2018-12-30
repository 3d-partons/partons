#include "../../../include/partons/beans/Kinematic.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

namespace PARTONS {

Kinematic::Kinematic(const std::string &className,
        ChannelType::Type channelType) :
        BaseObject(className), m_channelType(channelType), m_hashSum(
                ElemUtils::StringUtils::EMPTY) {
}

Kinematic::Kinematic(const Kinematic& other) :
        BaseObject(other), m_channelType(other.m_channelType), m_hashSum(
                other.m_hashSum) {
}

Kinematic::~Kinematic() {
}

std::string Kinematic::toString() const {
    return BaseObject::toString();
}

ChannelType::Type Kinematic::getChannelType() const {
    return m_channelType;
}

const std::string& Kinematic::getHashSum() const {

    if (m_hashSum.empty()) {
        updateHashSum();
    }

    return m_hashSum;
}

void Kinematic::setHashSum(const std::string& hashSum) const {
    m_hashSum = hashSum;
}

void Kinematic::serialize(ElemUtils::Packet& packet) const {
    BaseObject::serialize(packet);
}

void Kinematic::unserialize(ElemUtils::Packet& packet) {
    BaseObject::unserialize(packet);
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        Kinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        Kinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
