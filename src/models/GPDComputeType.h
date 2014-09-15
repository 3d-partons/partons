#ifndef GPD_COMPUTE_TYPE_H
#define GPD_COMPUTE_TYPE_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 30 juil 2014
 * @version 1.0
 */

#include <string>

struct GPDComputeType {
    //prevent automatic conversion for any other built-in types such as bool, int, etc
    template<typename T>
    operator T() const;

public:
    enum Type {
        ALL, H, Ht, E, Et
    };
    Type t_;
    GPDComputeType(Type t) :
            t_(t) {
    }
    operator Type() const {
        return t_;
    }

    std::string toString() {
        switch (t_) {
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
            return "default";
        }
    }
};

#endif /* GPD_COMPUTE_TYPE_H */
