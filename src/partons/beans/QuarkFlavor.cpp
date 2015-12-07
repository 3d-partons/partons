#include "../../../include/partons/beans/QuarkFlavor.h"

const std::string QuarkFlavor::QUARK_FLAVOR_TYPE_DB_COLUMN_NAME =
        "quark_flavor_type";

QuarkFlavor::QuarkFlavor() :
        m_type(QuarkFlavor::UNDEFINED) {
}

QuarkFlavor::QuarkFlavor(Type type) :
        m_type(type) {
}
QuarkFlavor::operator QuarkFlavor::Type() const {
    return m_type;
}

std::string QuarkFlavor::toString() {
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

QuarkFlavor::Type QuarkFlavor::getType() const {
    return m_type;
}

void QuarkFlavor::setType(Type type) {
    m_type = type;
}
