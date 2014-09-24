#ifndef QUARK_FLAVOR_H
#define QUARK_FLAVOR_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 22/07/2014
 */

#include <string>

class QuarkFlavor {
public:
    enum Type {
        UP, DOWN, STRANGE, CHARM, BOTTOM, TOP
    };

    QuarkFlavor(Type type) :
            m_type(type) {
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
            return "default";
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
            return "default";
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
