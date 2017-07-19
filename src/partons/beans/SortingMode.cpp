#include "../../../include/partons/beans/SortingMode.h"

SortingMode::SortingMode() :
        m_type(SortingMode::UNDEFINED) {
}

SortingMode::SortingMode(Type type) :
        m_type(type) {
}
SortingMode::operator SortingMode::Type() const {
    return m_type;
}

std::string SortingMode::toString() const {
    switch (m_type) {
    case ASCENDING:
        return "ASCENDING";
        break;
    case DESCENDING:
        return "DESCENDING";
        break;
    default:
        return "UNDEFINED";
    }
}

std::string SortingMode::getShortName() {
    switch (m_type) {
    case ASCENDING:
        return "ASCENDING";
        break;
    case DESCENDING:
        return "DESCENDING";
        break;
    default:
        return "UNDEFINED";
    }
}

SortingMode::Type SortingMode::getType() const {
    return m_type;
}

void SortingMode::setType(Type type) {
    m_type = type;
}
