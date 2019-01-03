#ifndef PHYSICAL_UNIT_H
#define PHYSICAL_UNIT_H

/**
 * @file PhysicalUnit.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 20 January 2016
 * @version 1.0
 */

#include <string>

#include "UnitCategory.h"

namespace PARTONS {

/**
 * @class PhysicalUnit
 *
 * @brief Definition of units.
 */
class PhysicalUnit {

public:

    /**
     * Enum types.
     */
    enum Type {
        NONE = 0,       ///< None
        GEV = 1,        ///< GeV
        GEV2 = 2,       ///< GeV2
        DEGREE = 3,     ///< degree
        RADIAN = 4,     ///< radian
        NB = 5          ///< nb
    };

    /**
     * Default constructor.
     */
    PhysicalUnit();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    PhysicalUnit(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    PhysicalUnit(const PhysicalUnit &other);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representing class content.
     */
    std::string toString() const;

    /**
     * Get short string representing PhysicalUnit::m_type.
     */
    std::string getShortName();

    /**
     * Get unit category.
     */
    UnitCategory::Type getUnitCategory() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get PhysicalUnit::m_type.
     */
    PhysicalUnit::Type getType() const;

    /**
     * Set PhysicalUnit::m_type.
     */
    void setType(Type type);

private:

    /**
     * Type.
     */
    PhysicalUnit::Type m_type;
};

} /* namespace PARTONS */

#endif /* PHYSICAL_UNIT_H */
