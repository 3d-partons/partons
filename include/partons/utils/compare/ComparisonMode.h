/*
 * ComparisonMode.h
 *
 *  Created on: Nov 30, 2015
 *      Author: debian
 */

#ifndef COMPARISON_MODE_H
#define COMPARISON_MODE_H

/**
 * @file ComparisonMode.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 November 2015
 * @version 1.0
*/

#include <string>

namespace PARTONS {

/**
 * @class ComparisonMode
 *
 * @brief Definition of comparison modes.
 *
 * This class defines a set of enumeration values that are used to distinguish between comparison modes. In addition, a declared object of this class is always associated to one comparison mode (see ComparisonMode::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 ComparisonMode::Type enum_variable = ComparisonMode::EQUAL;

 //this is declared object
 ComparisonMode enum_object;

 //let us assign some type (default is ComparisonMode::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = ComparisonMode(ComparisonMode::INTERSECTION).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Comparison mode is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Comparison mode is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:13:02 [INFO] (example::main) Comparison mode is: EQUAL
 20-05-2017 12:13:02 [INFO] (example::main) Comparison mode is: INTERSECTION
 \endcode
 */
class ComparisonMode {

public:

    enum Type {
        UNDEFINED = 0, 		///< Undefined type.
		EQUAL = 1, 			///< Equal mode of comparison.
		INTERSECTION = 2	///< Intersection mode of comparison.
    };

    /**
     * Default constructor.
     */
    ComparisonMode();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    ComparisonMode(Type type);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "ASCENDING" for ComparisonMode::EQUAL.
     */
    std::string toString() const;

    /**
     * Get short name representation of type being assigned to a declared object of this class.
     * @return Short string representation of assigned type, like "ASCENDING" for ComparisonMode::EQUAL.
     */
    std::string getShortName() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    ComparisonMode::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    ComparisonMode::Type m_type;
};

} /* namespace PARTONS */

#endif /* COMPARISONMODE_H_ */
