#include "../../../../include/partons/beans/kinematic/KinematicType.h"

KinematicType::KinematicType() :
        m_type(KinematicType::UNDEFINED) {
}

KinematicType::KinematicType(Type type) :
        m_type(type) {
}

KinematicType::KinematicType(const KinematicType &other) {
    m_type = other.m_type;
}

KinematicType::operator KinematicType::Type() const {
    return m_type;
}

std::string KinematicType::toString() {
    switch (m_type) {
    case THEO:
        return "THEO";
        break;
    case EXP:
        return "EXP";
        break;
    default:
        return "UNDEFINED";
    }
}

KinematicType::Type KinematicType::getType() const {
    return m_type;
}

void KinematicType::setType(Type type) {
    m_type = type;
}
