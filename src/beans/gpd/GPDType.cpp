#include "GPDType.h"

const std::string GPDType::GPD_TYPE_DB_COLUMN_NAME = "gpd_type";

GPDType::GPDType()
        : m_type(GPDType::UNDEFINED) {
}

GPDType::GPDType(Type type)
        : m_type(type) {
}

/**
 * Copy constructor
 * @param other
 */
GPDType::GPDType(const GPDType &other) {
    m_type = other.m_type;
}

GPDType::operator GPDType::Type() const {
    return m_type;
}

std::string GPDType::toString() {
    switch (m_type) {
    case H:
        return "H";
        break;
    case Ht:
        return "Ht";
        break;
    case E:
        return "E";
        break;
    case Et:
        return "Et";
        break;
    default:
        return "UNDEFINED";
    }
}

GPDType::Type GPDType::getType() const {
    return m_type;
}

void GPDType::setType(Type type) {
    m_type = type;
}
