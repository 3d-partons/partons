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
 * @brief Definition of enumeration values for pQCD orders of calculation.
 *
 * This class defines a set of enumeration values that are used to distinguish between pQCD orders of calculation. In addition, a declared object of this class is always associated to one pQCD order (see PerturbativeQCDOrderType::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 PerturbativeQCDOrderType::Type enum_variable = PerturbativeQCDOrderType::LO;

 //this is declared object
 PerturbativeQCDOrderType enum_object;

 //let us assign some type (default is PerturbativeQCDOrderType::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = PerturbativeQCDOrderType(PerturbativeQCDOrderType::NLO).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "pQCD order is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "pQCD order is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:13:02 [INFO] (example::main) pQCD order is: LO
 20-05-2017 12:13:02 [INFO] (example::main) pQCD order is: NLO
 \endcode
 */
class PerturbativeQCDOrderType: public BaseObject {

public:

    static const std::string PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE;

    enum Type {
        UNDEFINED = 0,  //!< Undefined type.
        LO = 1,     //!< Leading Order.
        NLO = 2,    //!< Next-to-Leading Order.
        NNLO = 3,   //!< Next-to-Next-to-Leading Order.
        LL = 4,     //!< Leading Logarithm.
        NLL = 5    //!< Next-to-Leading Logarithm.
    };

    /**
     * Default constructor.
     */
    PerturbativeQCDOrderType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    PerturbativeQCDOrderType(Type type);

    /**
     * Assignment constructor trying to match pQCD order type from given string. If unable to match set PerturbativeQCDOrderType::UNDEFINED.
     * @param perturbativeQCDOrderTypeString String to be matched.
     */
    PerturbativeQCDOrderType(const std::string &perturbativeQCDOrderTypeString);

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "LO" for PerturbativeQCDOrderType::LO.
     */
    const std::string toString();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    PerturbativeQCDOrderType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    PerturbativeQCDOrderType::Type m_type;
};

//inline bool operator==(const QCDOrderType& lhs, const QCDOrderType& rhs) {
//    return (lhs.t_ == rhs.t_) ? true : false;
//}

#endif /* PERTURBATIVE_QCD_ORDER_TYPE_H */
