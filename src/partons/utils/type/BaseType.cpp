#include "../../../../include/partons/utils/type/BaseType.h"

BaseType::BaseType() :
        m_initialized(false), m_unit("none") {
}

BaseType::BaseType(bool initialized) :
        m_initialized(initialized), m_unit("none") {
}

BaseType::~BaseType() {
}

bool BaseType::isInitialized() const {
    return m_initialized;
}

void BaseType::setInitialized(bool initialized) {
    m_initialized = initialized;
}

const std::string& BaseType::getUnit() const {
    return m_unit;
}

void BaseType::setUnit(const std::string& unit) {
    m_unit = unit;
}
