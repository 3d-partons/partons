#ifndef UNIT_CATEGORY_H
#define UNIT_CATEGORY_H

/**
 * @file UnitCategory.h
 * @author: Pawel Sznajder (NCBJ/Warsaw)
 * @date 3 January 2018
 * @version 1.0
 */

#include <string>

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

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
        UNDEFINED = 0,          ///< Undefined
        NONE = 1,               ///< None
        EMP = 2,                ///< Energy, mass or momentum (eV in natural units)
        EMP2 = 3,               ///< Square of energy, mass or momentum (eV^2 in natural units)
        DISTANCE_TIME = 4,      ///< Distance or time (eV^-1 in natural units)
        CROSS_SECTION = 5,      ///< Cross section (eV^-2 in natural units)
        ANGLE = 6               ///< Angle
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
     * Serialize into given Packet.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Retrieve data from given Packet.
     * @param packet Input Packet.
     */
    void unserialize(ElemUtils::Packet &packet);

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

/**
 * Stream operator to serialize class into Packet. See also UnitCategory::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        UnitCategory& unitCategory);

/**
 * Stream operator to retrieve class from Packet. See also UnitCategory::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        UnitCategory& unitCategory);

} /* namespace PARTONS */

#endif /* UNIT_CATEGORY_H */
