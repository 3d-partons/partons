#include "../../../include/partons/beans/QuarkFlavor.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

namespace PARTONS {

const std::string QuarkFlavor::QUARK_FLAVOR_TYPE_DB_COLUMN_NAME =
        "quark_flavor_type";

QuarkFlavor::QuarkFlavor() :
        m_type(QuarkFlavor::UNDEFINED) {
}

QuarkFlavor::QuarkFlavor(const QuarkFlavor& other) {
    m_type = other.m_type;
}

QuarkFlavor::QuarkFlavor(Type type) :
        m_type(type) {
}
QuarkFlavor::operator QuarkFlavor::Type() const {
    return m_type;
}

std::string QuarkFlavor::toString() const {
    switch (m_type) {
    case UP:
        return "UP";
        break;
    case DOWN:
        return "DOWN";
        break;
    case STRANGE:
        return "STRANGE";
        break;
    case CHARM:
        return "CHARM";
        break;
    case BOTTOM:
        return "BOTTOM";
        break;
    case TOP:
        return "TOP";
        break;
    default:
        return "UNDEFINED";
    }
}

std::string QuarkFlavor::getShortName() {
    switch (m_type) {
    case UP:
        return "u";
        break;
    case DOWN:
        return "d";
        break;
    case STRANGE:
        return "s";
        break;
    case CHARM:
        return "c";
        break;
    case BOTTOM:
        return "b";
        break;
    case TOP:
        return "t";
        break;
    default:
        return "UNDEFINED";
    }
}

QuarkFlavor::Type QuarkFlavor::fromString(const std::string & quarkFlavorStr) {

    QuarkFlavor::Type quarkFlavor = QuarkFlavor::UNDEFINED;

    if (ElemUtils::StringUtils::equals(quarkFlavorStr, "u")) {
        quarkFlavor = QuarkFlavor::UP;
    } else if (ElemUtils::StringUtils::equals(quarkFlavorStr, "d")) {
        quarkFlavor = QuarkFlavor::DOWN;
    } else if (ElemUtils::StringUtils::equals(quarkFlavorStr, "s")) {
        quarkFlavor = QuarkFlavor::STRANGE;
    } else if (ElemUtils::StringUtils::equals(quarkFlavorStr, "c")) {
        quarkFlavor = QuarkFlavor::CHARM;
    } else if (ElemUtils::StringUtils::equals(quarkFlavorStr, "b")) {
        quarkFlavor = QuarkFlavor::BOTTOM;
    } else if (ElemUtils::StringUtils::equals(quarkFlavorStr, "t")) {
        quarkFlavor = QuarkFlavor::TOP;
    }

    return quarkFlavor;

}

QuarkFlavor::Type QuarkFlavor::getType() const {
    return m_type;
}

void QuarkFlavor::setType(Type type) {
    m_type = type;
}

QuarkFlavor::~QuarkFlavor() {
}

} /* namespace PARTONS */
