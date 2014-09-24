#ifndef QCD_ORDER_TYPE_H
#define QCD_ORDER_TYPE_H

/**
 * \author Bryan BERTHOU
 * \date 16/07/2014
 */

#include <string>

class QCDOrderType {

public:
    enum Type {
        LO, NLO, NNLO
    };

    QCDOrderType(Type type) :
            m_type(type) {
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
        default:
            return "";
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
