#include "../../../include/partons/beans/Kinematic.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

//#include "../../../include/partons/BaseObject.h"

namespace PARTONS {


Kinematic::Kinematic(const std::string &className) :
        BaseObject(className)/*, m_listEntryPosition(-1)*/, m_hashSum(
                ElemUtils::StringUtils::EMPTY) {
}

Kinematic::Kinematic(const Kinematic& other) :
        BaseObject(other) {
    m_hashSum = other.m_hashSum;
}

Kinematic::~Kinematic() {
}

//bool Kinematic::operator <(const Kinematic& other) const {
//    return (m_listEntryPosition < other.m_listEntryPosition);
//}

std::string Kinematic::toString() const {
//    return ElemUtils::Formatter() << "listEntryPosition = "
//            << m_listEntryPosition;

    return BaseObject::toString();
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

//int Kinematic::getListEntryPosition() const {
//    return m_listEntryPosition;
//}
//
//void Kinematic::setListEntryPosition(int listEntryPosition) {
//    m_listEntryPosition = listEntryPosition;
//}

void Kinematic::serialize(ElemUtils::Packet& packet) const {
    BaseObject::serialize(packet);
    //packet << m_listEntryPosition;
}

void Kinematic::unserialize(ElemUtils::Packet& packet) {
    BaseObject::unserialize(packet);
    //packet >> m_listEntryPosition;
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
