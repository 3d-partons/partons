#ifndef UNIT_CATEGORY_H
#define UNIT_CATEGORY_H

/**
 * @file UnitCategory.h
 * @author: Pawel Sznajder (NCBJ/Warsaw)
 * @date 3 January 2018
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class UnitCategory
 *
 * @brief Definition of units.
 */
class UnitCategory {

public:

    /**
     * Enum types.
     */
    enum Type {
        NONE = 0,               ///< None
        EMP = 1,                ///< Energy, momentum or mass (eV in natural units)
        EMP2 = 2,               ///< Square of energy, momentum or mass (eV^2 in natural units)
        DISTANCE = 3,           ///< Distance (eV^-1 in natural units)
        CROSS_SECTION = 4,      ///< Cross section (eV^-2 in natural units)
        ANGLE = 5,              ///< Angle
    };

    /**
     * Default constructor.
     */
    UnitCategory();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    UnitCategory(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    UnitCategory(const UnitCategory &other);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representing class content.
     */
    std::string toString() const;

    /**
     * Get short string representing UnitCategory::m_type.
     */
    std::string getShortName();

    /**
     * Get category. The categories are:
     * 0: none
     * 1: ev
     */
    int getCategory() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get UnitCategory::m_type.
     */
    UnitCategory::Type getType() const;

    /**
     * Set UnitCategory::m_type.
     */
    void setType(Type type);

private:

    /**
     * Type.
     */
    UnitCategory::Type m_type;
};

} /* namespace PARTONS */

#endif /* UNIT_CATEGORY_H */
