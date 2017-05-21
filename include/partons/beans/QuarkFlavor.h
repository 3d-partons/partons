#ifndef QUARK_FLAVOR_H
#define QUARK_FLAVOR_H

/**
 *
 * @file QuarkFlavor.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2014
 * @version 1.0
 */

#include <string>

/**
 * @class QuarkFlavor
 *
 * @brief Definition of enumeration values for quark flavors.
 *
 * This class defines a set of enumeration values that are used to distinguish between quark flavors. In addition, a declared object of this class is always associated to one quark flavor type (see QuarkFlavor::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 QuarkFlavor::Type enum_variable = QuarkFlavor::UP;

 //this is declared object
 QuarkFlavor enum_object;

 //let us assign some type (default is QuarkFlavor::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = QuarkFlavor(QuarkFlavor::DOWN).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Quark flavor is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Quark flavor is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:09:19 [INFO] (example::main) Quark flavor is: UP
 20-05-2017 12:09:19 [INFO] (example::main) Quark flavor is: DOWN
 \endcode
 */
class QuarkFlavor {
public:

    /**
     * Name of table in the database corresponding to this class.
     */
    static const std::string QUARK_FLAVOR_TYPE_DB_COLUMN_NAME;

    /**
     * Definition of enumerate values corresponding to quark flavors.
     */
    enum Type {
        UNDEFINED = 0,      //!< Undefined type.
        UP = 1,      //!< Quark flavor up.
        DOWN = 2,    //!< Quark flavor down.
        STRANGE = 3, //!< Quark flavor strange.
        CHARM = 4,   //!< Quark flavor charm.
        BOTTOM = 5,  //!< Quark flavor bottom.
        TOP = 6     //!< Quark flavor top.
    };

    /**
     * Default constructor.
     */
    QuarkFlavor();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    QuarkFlavor(const QuarkFlavor &other);

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    QuarkFlavor(Type type);

    /**
     * Destructor.
     */
    virtual ~QuarkFlavor();

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "UP" for QuarkFlavor::UP.
     */
    std::string toString();

    /**
     * Get short name representation of type being assigned to a declared object of this class.
     * @return Short string representation of assigned type, like "u" for QuarkFlavor::UP.
     */
    std::string getShortName();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    QuarkFlavor::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    QuarkFlavor::Type m_type;
};

#endif /* QUARK_FLAVOR_H */
