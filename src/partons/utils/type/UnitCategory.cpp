#include "../../../../include/partons/utils/type/UnitCategory.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {

UnitCategory::UnitCategory() :
        m_type(UnitCategory::NONE) {
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

    case NONE:
        return "NONE";
        break;
    case EMP:
        return "EMP";
        break;
    case EMP2:
        return "EMP2";
        break;
    case DISTANCE:
        return "DISTANCE";
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

    case NONE:
        return "none";
        break;
    case EMP:
        return "energy, momentum or mass";
        break;
    case EMP2:
        return "square of energy momentum or mass";
        break;
    case DISTANCE:
        return "distance";
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

UnitCategory::Type UnitCategory::getType() const {
    return m_type;
}

void UnitCategory::setType(Type type) {
    m_type = type;
}

} /* namespace PARTONS */
