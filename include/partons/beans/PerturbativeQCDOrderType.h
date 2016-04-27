#ifndef PERTURBATIVE_QCD_ORDER_TYPE_H
#define PERTURBATIVE_QCD_ORDER_TYPE_H

/**
 * @file PerturbativeQCDOrderType.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 16, 2014
 * @version 1.0
 */

#include <string>

#include "../BaseObject.h"

/**
 * @class PerturbativeQCDOrderType
 *
 * @brief Defines available orders of perturbative QCD computations.\n
 * Current values are:\n
 * - UNDEFINED: empty field\n
 * - LO: Leading Order\n
 * - NLO: Next-to-Leading Order\n
 * - NNLO : Next-to-Next-to-Leading Order\n
 * - LL: Leading Logarithm\n
 * - NLL: Next-to-Leading Logarithm\n
 */
class PerturbativeQCDOrderType: public BaseObject {

public:
    static const std::string PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE;

    enum Type {
        UNDEFINED = 0, LO = 1, NLO = 2, NNLO = 3, LL = 4, NLL = 5
    };

    PerturbativeQCDOrderType();

    PerturbativeQCDOrderType(Type type);

    PerturbativeQCDOrderType(const std::string &perturbativeQCDOrderTypeString);

    const std::string toString();

    PerturbativeQCDOrderType::Type getType() const;

    void setType(Type type);

private:
    PerturbativeQCDOrderType::Type m_type;
};

//inline bool operator==(const QCDOrderType& lhs, const QCDOrderType& rhs) {
//    return (lhs.t_ == rhs.t_) ? true : false;
//}

#endif /* PERTURBATIVE_QCD_ORDER_TYPE_H */
