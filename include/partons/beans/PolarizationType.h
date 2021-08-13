#ifndef POLARIZATION_TYPE_H
#define POLARIZATION_TYPE_H

/**
 * @file PolarizationType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 23 April 2019
 * @version 1.0
 */

#include <string>

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class PolarizationType
 *
 * @brief Definition of enumeration values for meson polarization state.
 *
 * This class defines a set of enumeration values that are used to distinguish between meson polarization state. In addition, a declared object of this class is always associated to one meson type (see PolarizationType::m_type), so member functions can act on it.
 */
class PolarizationType {

public:

    /**
     * Definition of enumerate values corresponding to meson polarization state.
     */
    enum Type {

        UNDEFINED = 0,      //!< Undefined type.

        LIN_LONG_PLUS = 1,   //!< Linear longitudinal (along z-axis) +.
        LIN_LONG_MINUS = 2,   //!< Linear longitudinal (along z-axis) -.
        LIN_TRANS_X_PLUS = 3,   //!< Linear transverse (along x-axis) +.
        LIN_TRANS_X_MINUS = 4,   //!< Linear transverse (along x-axis) -.
        LIN_TRANS_Y_PLUS = 5,   //!< Linear transverse (along y-axis) +.
        LIN_TRANS_Y_MINUS = 6,   //!< Linear transverse (along y-axis)-+.
        CIR_L = 7,   //!< Circular left.
        CIR_R = 8,   //!< Circular right.
    };

    /**
     * Default constructor.
     */
    PolarizationType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    PolarizationType(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    PolarizationType(const PolarizationType &other);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type.
     */
    std::string toString() const;

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
     * Relation operator that checks if the value of left operand is less than the value of right operand (based on values assigned in the definition of PolarizationType::Type).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const PolarizationType &other) const;

    /**
     * Try to match meson polarization from given string.
     * @param polarizationTypeStr String to be matched.
     * @return Matched type or PolarizationType::UNDEFINED if unable to match.
     */
    static PolarizationType::Type fromString(
            const std::string & polarizationTypeStr);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    PolarizationType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    PolarizationType::Type m_type;
};

/**
 * Stream operator to serialize class into Packet. See also PolarizationType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PolarizationType& polarizationType);

/**
 * Stream operator to retrieve class from Packet. See also PolarizationType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        PolarizationType& polarizationType);

} /* namespace PARTONS */

#endif /* POLARIZATION_TYPE_H */

