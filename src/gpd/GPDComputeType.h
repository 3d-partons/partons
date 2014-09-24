#ifndef GPD_COMPUTE_TYPE_H
#define GPD_COMPUTE_TYPE_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 30 juil 2014
 * @version 1.0
 */

#include <string>

class GPDComputeType {

public:
    enum Type {
        ALL, H, Ht, E, Et
    };

    GPDComputeType(Type type) :
            m_type(type) {
    }
    operator Type() const {
        return m_type;
    }

    std::string toString() {
        switch (m_type) {
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

    GPDComputeType::Type getType() const {
        return m_type;
    }

    void setType(Type type) {
        m_type = type;
    }

private:
    GPDComputeType::Type m_type;
};

#endif /* GPD_COMPUTE_TYPE_H */
