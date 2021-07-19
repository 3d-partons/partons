#ifndef MESON_TYPE_H
#define MESON_TYPE_H

/**
 * @file MesonType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 23 April 2019
 * @version 1.0
 */

#include <string>

#include "gpd/GPDType.h"
#include "List.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class MesonType
 *
 * @brief Definition of enumeration values for meson types.
 *
 * This class defines a set of enumeration values that are used to distinguish between meson types. In addition, a declared object of this class is always associated to one meson type (see MesonType::m_type), so member functions can act on it.
 */
class MesonType {

public:

    /**
     * Definition of enumerate values corresponding to meson types.
     */
    enum Type {
        UNDEFINED = 0,  //!< Undefined type.

        RHOMINUS = 1,   //!<  \f$\rho^{-}\f$
        RHO0 = 2,       //!<  \f$\rho^{0}\f$
        RHOPLUS = 3,    //!<  \f$\rho^{+}\f$
        OMEGA = 4,      //!<  \f$\omega\f$
        PHI = 5,        //!<  \f$\phi\f$
        JPSI = 6,       //!<  \f$J/\Psi\f$
        UPSILON = 7,    //!<  \f$\Upsilon\f$

        PIMINUS = 8,    //!<  \f$\pi^{-}\f$
        PI0 = 9,        //!<  \f$\pi^{0}\f$
        PIPLUS = 10     //!<  \f$\pi^{+}\f$
    };

    /**
     * Default constructor.
     */
    MesonType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    MesonType(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    MesonType(const MesonType &other);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get list of GPD types probed by the meson.
     */
    List<GPDType> getPossibleGPDTypes() const;

    /**
     * Get mass associated to set meson type.
     */
    double getMass() const;

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
     * Relation operator that checks if the value of left operand is less than the value of right operand (based on values assigned in the definition of MesonType::Type).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const MesonType &other) const;

    /**
     * Try to match meson type from given string.
     * @param mesonTypeStr String to be matched.
     * @return Matched type or MesonType::UNDEFINED if unable to match.
     */
    static MesonType::Type fromString(const std::string & mesonTypeStr);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    MesonType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    MesonType::Type m_type;
};

/**
 * Stream operator to serialize class into Packet. See also MesonType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, MesonType& mesonType);

/**
 * Stream operator to retrieve class from Packet. See also MesonType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, MesonType& mesonType);

} /* namespace PARTONS */

#endif /* MESON_TYPE_H */

