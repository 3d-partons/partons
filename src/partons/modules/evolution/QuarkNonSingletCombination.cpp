#include "../../../../include/partons/modules/evolution/QuarkNonSingletCombination.h"

namespace PARTONS {

QuarkNonSingletCombination::QuarkNonSingletCombination() :
        m_type(QuarkNonSingletCombination::UNDEFINED) {
}

QuarkNonSingletCombination::QuarkNonSingletCombination(
        const QuarkNonSingletCombination& other) {
    m_type = other.m_type;
}

QuarkNonSingletCombination::QuarkNonSingletCombination(Type type) :
        m_type(type) {
}

QuarkNonSingletCombination::~QuarkNonSingletCombination() {
}

QuarkNonSingletCombination::operator QuarkNonSingletCombination::Type() const {
    return m_type;
}

std::string QuarkNonSingletCombination::toString() const {

    switch (m_type) {

    case UP_NONSINGLET:
        return "UP_NONSINGLET";
        break;
    case DOWN_NONSINGLET:
        return "DOWN_NONSINGLET";
        break;
    case STRANGE_NONSINGLET:
        return "STRANGE_NONSINGLET";
        break;
    case CHARM_NONSINGLET:
        return "CHARM_NONSINGLET";
        break;
    case BOTTOM_NONSINGLET:
        return "BOTTOM_NONSINGLET";
        break;
    case TOP_NONSINGLET:
        return "TOP_NONSINGLET";
        break;
    case UP_MINUS_DOWN:
        return "UP_MINUS_DOWN";
        break;
    case UP_MINUS_STRANGE:
        return "UP_MINUS_STRANGE";
        break;
    case UP_MINUS_CHARM:
        return "UP_MINUS_CHARM";
        break;
    case UP_MINUS_BOTTOM:
        return "UP_MINUS_BOTTOM";
        break;
    case UP_MINUS_TOP:
        return "UP_MINUS_TOP";
        break;
    default:
        return "UNDEFINED";
    }
}

QuarkNonSingletCombination::Type QuarkNonSingletCombination::getType() const {
    return m_type;
}

void QuarkNonSingletCombination::setType(Type type) {
    m_type = type;
}

} /* namespace PARTONS */
