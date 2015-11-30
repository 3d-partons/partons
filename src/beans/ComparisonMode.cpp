#include "ComparisonMode.h"

ComparisonMode::ComparisonMode() :
        m_type(ComparisonMode::UNDEFINED) {
}

ComparisonMode::ComparisonMode(Type type) :
        m_type(type) {
}

ComparisonMode::operator ComparisonMode::Type() const {
    return m_type;
}

std::string ComparisonMode::toString() {
    switch (m_type) {
    case EQUAL:
        return "EQUAL";
        break;
    case INTERSECTION:
        return "INTERSECTION";
        break;
    default:
        return "UNDEFINED";
    }
}

std::string ComparisonMode::getShortName() {
    switch (m_type) {
    case EQUAL:
        return "EQUAL";
        break;
    case INTERSECTION:
        return "INTERSECTION";
        break;
    default:
        return "UNDEFINED";
    }
}

ComparisonMode::Type ComparisonMode::getType() const {
    return m_type;
}

void ComparisonMode::setType(Type type) {
    m_type = type;
}
