#ifndef QUARK_FLAVOR_H
#define QUARK_FLAVOR_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 22/07/2014
 */

#include <string>

struct QuarkFlavor {
    //prevent automatic conversion for any other built-in types such as bool, int, etc
    template<typename T>
    operator T() const;

public:
    enum Type {
        UP, DOWN, STRANGE, CHARM, BOTTOM, TOP
    };
    Type t_;
    QuarkFlavor(Type t) :
            t_(t) {
    }
    operator Type() const {
        return t_;
    }

    std::string toString() {
        switch (t_) {
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
        switch (t_) {
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
};

#endif /* QUARK_FLAVOR_H */
