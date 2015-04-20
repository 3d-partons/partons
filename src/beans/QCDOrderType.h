#ifndef QCD_ORDER_TYPE_H
#define QCD_ORDER_TYPE_H

/**
 * @file QCDOrderType.h
 * @author Bryan BERTHOU
 * @date 16 July 2014
 * @version 1.0
 *
 * @class QCDOrderType
 *
 * @brief
 *
 * @class QCDOrderType
 * @brief Defines available orders of perturbative QCD computations.\n
 * Current values are:\n
 * UNDEFINED: empty field\n
 * LO: Leading Order\n
 * NLO: Next-to-Leading Order\n
 * NNLO : Next-to-Next-to-Leading Order\n
 * LL: Leading Logarithm\n
 * NLL: Next-to-Leading Logarithm\n
 */

#include <string>

class QCDOrderType {

public:
    enum Type {
        UNDEFINED, LO, NLO, NNLO, LL, NLL
    };

    QCDOrderType()
            : m_type(QCDOrderType::UNDEFINED) {
    }

    QCDOrderType(Type type)
            : m_type(type) {
    }

    const std::string toString() {
        switch (m_type) {
        case LO:
            return "LO";
            break;
        case NLO:
            return "NLO";
            break;
        case NNLO:
            return "NNLO";
            break;
        case LL:
            return "LL";
            break;
        case NLL:
            return "NLL";
            break;
        default:
            return "UNDEFINED";
        }
    }

    QCDOrderType::Type getType() const {
        return m_type;
    }

    void setType(Type type) {
        m_type = type;
    }

private:
    QCDOrderType::Type m_type;
};

//inline bool operator==(const QCDOrderType& lhs, const QCDOrderType& rhs) {
//    return (lhs.t_ == rhs.t_) ? true : false;
//}

#endif /* QCD_ORDER_TYPE_H */
