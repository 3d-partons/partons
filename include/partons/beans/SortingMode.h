/*
 * SortingMode.h
 *
 *  Created on: Dec 7, 2015
 *      Author: debian
 */

#ifndef SORTING_MODE_H
#define SORTING_MODE_H

#include <string>

/**
 * @class SortingMode
 *
 * @brief Definition of sorting modes.
 *
 * This class defines a set of enumeration values that are used to distinguish between sorting modes. In addition, a declared object of this class is always associated to one sorting mode (see SortingMode::m_type), so member functions can act on it. E.g.
 \code{.cpp}
 //this is single enum variable - nothing to play with
 SortingMode::Type enum_variable = SortingMode::ASCENDING;

 //this is declared object
 SortingMode enum_object;

 //let us assign some type (default is SortingMode::UNDEFINED)
 enum_object.setType(enum_variable);

 //with objects you can use available functions, e.g. you can represent enumeration type by a corresponding string
 std::string enum_string_1 = enum_object.toString();

 //you can achieve some basic operations without the explicit declaration of objects by using the assignment constructor
 std::string enum_string_2 = SortingMode(SortingMode::DESCENDING).toString();

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Sorting mode is: " << enum_string_1);
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Sorting mode is: " << enum_string_2);
 \endcode
 which gives via Logger:
 \code
 20-05-2017 12:13:02 [INFO] (example::main) Sorting mode is: ASCENDING
 20-05-2017 12:13:02 [INFO] (example::main) Sorting mode is: DESCENDING
 \endcode
 */
class SortingMode {

public:

    enum Type {
        UNDEFINED = 0, //!< Undefined type.
        ASCENDING = 1, //!< Ascending sorting.
        DESCENDING = 2 //!< Descending sorting.
    };

    /**
     * Default constructor.
     */
    SortingMode();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    SortingMode(Type type);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "ASCENDING" for SortingMode::ASCENDING.
     */
    std::string toString() const;

    /**
     * Get short name representation of type being assigned to a declared object of this class.
     * @return Short string representation of assigned type, like "ASCENDING" for SortingMode::ASCENDING.
     */
    std::string getShortName();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    SortingMode::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    SortingMode::Type m_type;
};

#endif /* SORTING_MODE_H */
