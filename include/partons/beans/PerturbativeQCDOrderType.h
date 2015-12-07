#ifndef PERTURBATIVE_QCD_ORDER_TYPE_H
#define PERTURBATIVE_QCD_ORDER_TYPE_H

/**
 * @file QCDOrderType.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
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
 * - UNDEFINED: empty field\n
 * - LO: Leading Order\n
 * - NLO: Next-to-Leading Order\n
 * - NNLO : Next-to-Next-to-Leading Order\n
 * - LL: Leading Logarithm\n
 * - NLL: Next-to-Leading Logarithm\n
 */

#include <string>

class PerturbativeQCDOrderType {

public:
    static const std::string PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE;

    enum Type {
        UNDEFINED = 0, LO = 1, NLO = 2, NNLO = 3, LL = 4, NLL = 5
    };

    PerturbativeQCDOrderType();

    PerturbativeQCDOrderType(Type type);

    const std::string toString();

    PerturbativeQCDOrderType::Type getType() const;

    void setType(Type type);

    static PerturbativeQCDOrderType::Type fromString(
            const std::string& gpdTypeStr);

private:
    PerturbativeQCDOrderType::Type m_type;
};

//inline bool operator==(const QCDOrderType& lhs, const QCDOrderType& rhs) {
//    return (lhs.t_ == rhs.t_) ? true : false;
//}

#endif /* PERTURBATIVE_QCD_ORDER_TYPE_H */
