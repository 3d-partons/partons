#ifndef OBSERVABLE_TYPE_H
#define OBSERVABLE_TYPE_H

/**
 * @file ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 September 2015
 * @version 1.0
 */

#include <string>

/**
 * @class ObservableType
 *
 * @brief Definition of enumeration values for observable types.
 *
 * This class defines a set of enumeration values that are used to distinguish between observable types. In addition, a declared object of this class is always associated to one observable type (see ObservableType::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 ObservableType::Type enum_variable = ObservableType::PHI;

 //this is declared object
 ObservableType enum_object;

 //let us assign some type (default is ObservableType::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = ObservableType(ObservableType::FOURIER).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Observable type is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Observable type is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:05:44 [INFO] (example::main) Observable type is: PHI
 20-05-2017 12:05:44 [INFO] (example::main) Observable type is: FOURRIER
 \endcode
 */
class ObservableType {
public:

    /**
     * Definition of enumerate values corresponding to observable types.
     */
    enum Type {
        UNDEFINED = 0, //!< Undefined type.
        PHI = 1, //!< Observable dependent of phi angle.
        FOURIER = 2 //!< Fourier-like observable related to one of phi angle modulations.
    };

    /**
     * Default constructor.
     */
    ObservableType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    ObservableType(Type type);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "PHI" for ObservableType::PHI.
     */
    std::string toString();

    /**
     * Get short name representation of type being assigned to a declared object of this class.
     * @return Short string representation of assigned type, like "phi" for ObservableType::PHI.
     */
    std::string getShortName();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    ObservableType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    ObservableType::Type m_type;
};

#endif /* OBSERVABLE_TYPE_H */
