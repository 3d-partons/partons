#include "../../../../include/partons/utils/type/PhysicalUnit.h"

PhysicalUnit::PhysicalUnit() :
        m_type(PhysicalUnit::NONE) {
}

PhysicalUnit::PhysicalUnit(Type type) :
        m_type(type) {
}

PhysicalUnit::operator PhysicalUnit::Type() const {
    return m_type;
}

std::string PhysicalUnit::toString() {
    switch (m_type) {
    case GEV:
        return "GEV";
        break;
    case GEV2:
        return "GEV2";
        break;
    case DEGREE:
        return "DEGREE";
        break;
    default:
        return "NONE";
    }
}

std::string PhysicalUnit::getShortName() {
    switch (m_type) {
    case GEV:
        return "GeV";
        break;
    case GEV2:
        return "GeV2";
        break;
    case DEGREE:
        return "degree";
        break;
    default:
        return "none";
    }
}

PhysicalUnit::Type PhysicalUnit::getType() const {
    return m_type;
}

void PhysicalUnit::setType(Type type) {
    m_type = type;
}
