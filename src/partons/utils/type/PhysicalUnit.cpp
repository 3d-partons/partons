#include "../../../../include/partons/utils/type/PhysicalUnit.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {

PhysicalUnit::PhysicalUnit() :
        m_type(PhysicalUnit::NONE) {
}

PhysicalUnit::PhysicalUnit(Type type) :
        m_type(type) {
}

PhysicalUnit::PhysicalUnit(const PhysicalUnit &other) :
        m_type(other.m_type) {
}

PhysicalUnit::operator PhysicalUnit::Type() const {
    return m_type;
}

std::string PhysicalUnit::toString() const {

    switch (m_type) {

    case NONE:
        return "NONE";
        break;
    case GEV:
        return "GEV";
        break;
    case GEV2:
        return "GEV2";
        break;
    case DEGREE:
        return "DEGREE";
        break;
    case RADIAN:
        return "RADIAN";
        break;
    case NB:
        return "NB";
        break;
    default:
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

std::string PhysicalUnit::getShortName() {

    switch (m_type) {

    case NONE:
        return "none";
        break;
    case GEV:
        return "GeV";
        break;
    case GEV2:
        return "GeV2";
        break;
    case DEGREE:
        return "degree";
        break;
    case RADIAN:
        return "radian";
        break;
    case NB:
        return "nb";
        break;
    default:
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

UnitCategory::Type PhysicalUnit::getUnitCategory() const {

    switch (m_type) {

    case NONE:
        return UnitCategory::NONE;
        break;
    case GEV:
        return UnitCategory::EMP;
        break;
    case GEV2:
        return UnitCategory::EMP2;
        break;
    case DEGREE:
        return UnitCategory::ANGLE;
        break;
    case RADIAN:
        return UnitCategory::ANGLE;
        break;
    case NB:
        return UnitCategory::CROSS_SECTION;
        break;
    default:
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

PhysicalUnit::Type PhysicalUnit::getType() const {
    return m_type;
}

void PhysicalUnit::setType(Type type) {
    m_type = type;
}

} /* namespace PARTONS */
