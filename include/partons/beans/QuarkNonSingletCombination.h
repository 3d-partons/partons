#ifndef QUARK_NON_SINGLET_COMBINATION_H
#define QUARK_NON_SINGLET_COMBINATION_H

/**
 * @file QuarkNonSingletCombination.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date 06 July 2017
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class QuarkNonSingletCombination
 *
 * @brief Definition of enumeration values for quark non-singlet combinations.
 *
 * This class defines a set of enumeration values that are used to distinguish between quark non-singlet combinations. In addition, a declared object of this class is always associated to one quark flavor combination type (see QuarkNonSingletCombination::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 QuarkNonSingletCombination::Type enum_variable = QuarkNonSingletCombination::UP_NONSINGLET;

 //this is declared object
 QuarkNonSingletCombination enum_object;

 //let us assign some type (default is QuarkNonSingletCombination::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = QuarkNonSingletCombination(QuarkNonSingletCombination::DOWN_NONSINGLET).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Quark non-singlet combination is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Quark non-singlet combination is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:09:19 [INFO] (example::main) Quark non-singlet combination is: UP_NONSINGLET
 20-05-2017 12:09:19 [INFO] (example::main) Quark non-singlet combination is: DOWN_NONSINGLET
 \endcode
 */
class QuarkNonSingletCombination {

public:

    /**
     * Definition of enumerate values corresponding to quark flavors.
     */
    enum Type {
        UNDEFINED = 0,          //!< Undefined type.
        UP_NONSINGLET = 1,      //!< Non-singlet combination for quark flavor u.
        DOWN_NONSINGLET = 2,    //!< Non-singlet combination for quark flavor d.
        STRANGE_NONSINGLET = 3, //!< Non-singlet combination for quark flavor s.
        CHARM_NONSINGLET = 4,   //!< Non-singlet combination for quark flavor c.
        BOTTOM_NONSINGLET = 5, 	//!< Non-singlet combination for quark flavor b.
        TOP_NONSINGLET = 6,    	//!< Non-singlet combination for quark flavor t.
        UP_MINUS_DOWN = 7,     	//!< GPD u minus GPD d.
        UP_MINUS_STRANGE = 8,  	//!< GPD u minus GPD s.
        UP_MINUS_CHARM = 9,    	//!< GPD u minus GPD c.
        UP_MINUS_BOTTOM = 10,   //!< GPD u minus GPD b.
        UP_MINUS_TOP = 11      	//!< GPD u minus GPD t.
    };

    /**
     * Default constructor.
     */
    QuarkNonSingletCombination();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    QuarkNonSingletCombination(const QuarkNonSingletCombination &other);

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    QuarkNonSingletCombination(Type type);

    /**
     * Destructor.
     */
    virtual ~QuarkNonSingletCombination();

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "UP" for QuarkNonSingletCombination::UP.
     */
    std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    QuarkNonSingletCombination::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    QuarkNonSingletCombination::Type m_type;
};

} /* namespace PARTONS */

#endif /* QUARK_NON_SINGLET_COMBINATION_H */
