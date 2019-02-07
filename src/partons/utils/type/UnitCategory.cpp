#include "../../../../include/partons/utils/type/UnitCategory.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>

namespace PARTONS {

UnitCategory::UnitCategory() :
        m_type(UnitCategory::UNDEFINED) {
}

UnitCategory::UnitCategory(Type type) :
        m_type(type) {
}

UnitCategory::UnitCategory(const UnitCategory &other) :
        m_type(other.m_type) {
}

UnitCategory::operator UnitCategory::Type() const {
    return m_type;
}

std::string UnitCategory::toString() const {

    switch (m_type) {

    case UNDEFINED:
        return "UNDEFINED";
        break;
    case NONE:
        return "NONE";
        break;
    case EMP:
        return "EMP";
        break;
    case EMP2:
        return "EMP2";
        break;
    case DISTANCE_TIME:
        return "DISTANCE_TIME";
        break;
    case CROSS_SECTION:
        return "CROSS_SECTION";
        break;
    case ANGLE:
        return "NANGLEB";
        break;
    default:
        throw ElemUtils::CustomException("UnitCategory", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

std::string UnitCategory::getShortName() {

    switch (m_type) {

    case UNDEFINED:
        return "undefined";
        break;
    case NONE:
        return "none";
        break;
    case EMP:
        return "energy, momentum or mass";
        break;
    case EMP2:
        return "square of energy momentum or mass";
        break;
    case DISTANCE_TIME:
        return "distance or time";
        break;
    case CROSS_SECTION:
        return "cross section";
        break;
    case ANGLE:
        return "angle";
        break;
    default:
        throw ElemUtils::CustomException("UnitCategory", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

void UnitCategory::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void UnitCategory::unserialize(ElemUtils::Packet &packet) {
    int i = 0;
    packet >> i;
    m_type = static_cast<UnitCategory::Type>(i);
}

UnitCategory::Type UnitCategory::getType() const {
    return m_type;
}

void UnitCategory::setType(Type type) {
    m_type = type;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        UnitCategory& unitCategory) {

    unitCategory.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        UnitCategory& unitCategory) {

    unitCategory.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
