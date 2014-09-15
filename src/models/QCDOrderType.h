#ifndef QCD_ORDER_TYPE_H
#define QCD_ORDER_TYPE_H

/**
 * \author Bryan BERTHOU
 * \date 16/07/2014
 */

#include <string>

struct QCDOrderType {
    //prevent automatic conversion for any other built-in types such as bool, int, etc
    template<typename T>
    operator T() const;

public:
    enum Type {
        LO, NLO, NNLO
    };
    Type t_;
    QCDOrderType(Type t) :
            t_(t) {
    }
    operator Type() const {
        return t_;
    }

    std::string toString() {
        switch (t_) {
        case LO:
            return "LO";
            break;
        case NLO:
            return "NLO";
            break;
        case NNLO:
            return "NNLO";
            break;
        default:
            return "";
        }
    }
};

#endif /* QCD_ORDER_TYPE_H */
