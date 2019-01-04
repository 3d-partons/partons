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

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

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
        UNDEFINED = 0,  ///< undefined
        NONE = 1,       ///< none
        GEV = 2,        ///< GeV
        GEV2 = 3,       ///< GeV2
        DEGREE = 4,     ///< degree
        RADIAN = 5,     ///< radian
        NB = 6          ///< nb
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
     * Serialize into given Packet.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Retrieve data from given Packet.
     * @param packet Input Packet.
     */
    void unserialize(ElemUtils::Packet &packet);

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

/**
 * Stream operator to serialize class into Packet. See also PhysicalUnit::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PhysicalUnit& physicalUnit);

/**
 * Stream operator to retrieve class from Packet. See also PhysicalUnit::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        PhysicalUnit& physicalUnit);

} /* namespace PARTONS */

#endif /* PHYSICAL_UNIT_H */
