#ifndef PARTON_TYPE_H
#define PARTON_TYPE_H

/**
 * @file PartonType.h
 * @author Pawel Sznajder (NCBJ)
 * @date 23 April 2019
 * @version 1.0
 */

#include <string>

#include "List.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class JetType
 *
 * @brief Definition of enumeration values for jet types.
 *
 * This class defines a set of enumeration values that are used to distinguish between jet types. In addition, a declared object of this class is always associated to one meson type (see PartonType::m_type), so member functions can act on it.
 */
class JetType {

public:

    /**
     * Definition of enumerate values corresponding to parton types.
     */
    enum Type {
        UNDEFINED = 0,  //!< Undefined type.

        GLUON = 1,      //!<  \f$g\f$
        UP = 2,         //!<  \f$u\f$
        DOWN = 3,       //!<  \f$d\f$
        STRANGE = 4     //!<  \f$s\f$
    };

    /**
     * Default constructor.
     */
    JetType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    JetType(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JetType(const JetType &other);

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
     * Relation operator that checks if the value of left operand is less than the value of right operand (based on values assigned in the definition of PartonType::Type).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const JetType &other) const;

    /**
     * Try to match meson type from given string.
     * @param mesonTypeStr String to be matched.
     * @return Matched type or PartonType::UNDEFINED if unable to match.
     */
    static JetType::Type fromString(const std::string & mesonTypeStr);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    JetType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    JetType::Type m_type;
};

/**
 * Stream operator to serialize class into Packet. See also PartonType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, JetType& mesonType);

/**
 * Stream operator to retrieve class from Packet. See also PartonType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, JetType& mesonType);

} /* namespace PARTONS */

#endif /* PARTON_TYPE_H */

