#ifndef QUARK_FLAVOR_H
#define QUARK_FLAVOR_H

/**
 *
 * @file QuarkFlavor.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 22 July 2014
 * @version 1.0
 *
 * @class QuarkFlavor
 *
 * @brief Defines existing quark flavors and associates them with an enum type.\n
 * It represents the dependence on quark flavors of hadron structure distributions.
 * Current values are:\n
 * UNDEFINED: empty field\n
 * UP, DOWN, STRANGE, CHARM, BOTTOM, TOP: explicit quark names\n
 */

#include <string>

class QuarkFlavor {
public:
    enum Type {
        UNDEFINED, UP, DOWN, STRANGE, CHARM, BOTTOM, TOP
    };

    QuarkFlavor()
            : m_type(QuarkFlavor::UNDEFINED) {
    }

    QuarkFlavor(Type type)
            : m_type(type) {
    }
    operator Type() const {
        return m_type;
    }

    std::string toString() {
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

    std::string getShortName() {
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

    QuarkFlavor::Type getType() const {
        return m_type;
    }

    void setType(Type type) {
        m_type = type;
    }

private:
    QuarkFlavor::Type m_type;
};

#endif /* QUARK_FLAVOR_H */
