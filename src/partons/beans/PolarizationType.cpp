#include "../../../include/partons/beans/PolarizationType.h"

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

namespace PARTONS {

PolarizationType::PolarizationType() :
        m_type(PolarizationType::UNDEFINED) {
}

PolarizationType::PolarizationType(Type type) :
        m_type(type) {
}

PolarizationType::PolarizationType(const PolarizationType &other) {
    m_type = other.m_type;
}

PolarizationType::operator PolarizationType::Type() const {
    return m_type;
}

std::string PolarizationType::toString() const {

    switch (m_type) {

    case LIN_LONG_PLUS:
        return "lin_L_p";
        break;
    case LIN_LONG_MINUS:
        return "lin_L_m";
        break;
    case LIN_TRANS_X_PLUS:
        return "lin_Tx_p";
        break;
    case LIN_TRANS_X_MINUS:
        return "lin_Tx_m";
        break;
    case LIN_TRANS_Y_PLUS:
        return "lin_Ty_p";
        break;
    case LIN_TRANS_Y_MINUS:
        return "lin_Ty_m";
        break;
    case CIR_L:
        return "cir_L";
        break;
    case CIR_R:
        return "cir_R";
        break;

    default:
        return "UNDEFINED";
    }
}

PolarizationType::Type PolarizationType::getType() const {
    return m_type;
}

void PolarizationType::setType(Type type) {
    m_type = type;
}

PolarizationType::Type PolarizationType::fromString(
        const std::string& polarizationTypeStr) {

    PolarizationType::Type polarizationType = PolarizationType::UNDEFINED;

    if (ElemUtils::StringUtils::equals(polarizationTypeStr, "lin_L_p")) {
        polarizationType = PolarizationType::LIN_LONG_PLUS;
    } else if (ElemUtils::StringUtils::equals(polarizationTypeStr, "lin_L_m")) {
        polarizationType = PolarizationType::LIN_LONG_MINUS;
    } else if (ElemUtils::StringUtils::equals(polarizationTypeStr,
            "lin_Tx_p")) {
        polarizationType = PolarizationType::LIN_TRANS_X_PLUS;
    } else if (ElemUtils::StringUtils::equals(polarizationTypeStr,
            "lin_Tx_m")) {
        polarizationType = PolarizationType::LIN_TRANS_X_MINUS;
    } else if (ElemUtils::StringUtils::equals(polarizationTypeStr,
            "lin_Ty_p")) {
        polarizationType = PolarizationType::LIN_TRANS_Y_PLUS;
    } else if (ElemUtils::StringUtils::equals(polarizationTypeStr,
            "lin_Ty_m")) {
        polarizationType = PolarizationType::LIN_TRANS_Y_MINUS;
    } else if (ElemUtils::StringUtils::equals(polarizationTypeStr, "cir_L")) {
        polarizationType = PolarizationType::CIR_L;
    } else if (ElemUtils::StringUtils::equals(polarizationTypeStr, "cir_R")) {
        polarizationType = PolarizationType::CIR_R;
    }

    return polarizationType;
}

void PolarizationType::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void PolarizationType::unserialize(ElemUtils::Packet &packet) {

    int i;
    packet >> i;
    m_type = static_cast<PolarizationType::Type>(i);
}

bool PolarizationType::operator <(const PolarizationType& other) const {
    return (m_type < other.m_type);
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PolarizationType& polarizationType) {

    polarizationType.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        PolarizationType& polarizationType) {

    polarizationType.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */

