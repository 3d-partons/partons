#ifndef MESON_POLARIZATION_H
#define MESON_POLARIZATION_H

/**
 * @file MesonPolarization.h
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
 * @class MesonPolarization
 *
 * @brief Definition of enumeration values for meson polarization state.
 *
 * This class defines a set of enumeration values that are used to distinguish between meson polarization state. In addition, a declared object of this class is always associated to one meson type (see MesonPolarization::m_type), so member functions can act on it.
 */
class MesonPolarization {

public:

    /**
     * Definition of enumerate values corresponding to meson polarization state.
     */
    enum Type {
        UNDEFINED = 0,      //!< Undefined type.

        L = 1,   //!< Longitudinal.
        T = 2    //!< Transverse.
    };

    /**
     * Default constructor.
     */
    MesonPolarization();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    MesonPolarization(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    MesonPolarization(const MesonPolarization &other);

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
     * Relation operator that checks if the value of left operand is less than the value of right operand (based on values assigned in the definition of MesonPolarization::Type).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const MesonPolarization &other) const;

    /**
     * Try to match meson polarization from given string.
     * @param mesonPolarizationStr String to be matched.
     * @return Matched type or MesonPolarization::UNDEFINED if unable to match.
     */
    static MesonPolarization::Type fromString(
            const std::string & mesonPolarizationStr);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    MesonPolarization::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    MesonPolarization::Type m_type;
};

/**
 * Stream operator to serialize class into Packet. See also MesonPolarization::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        MesonPolarization& mesonPolarization);

/**
 * Stream operator to retrieve class from Packet. See also MesonPolarization::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        MesonPolarization& mesonPolarization);

} /* namespace PARTONS */

#endif /* MESON_POLARIZATION_H */

