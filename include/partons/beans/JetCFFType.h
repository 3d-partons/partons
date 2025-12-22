#ifndef JET_CFF_TYPE_H
#define JET_CFF_TYPE_H

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
 * @class JetCFFType
 *
 * @brief Definition of enumeration values for jet types.
 *
 * This class defines a set of enumeration values that are used to distinguish between jet CFF types. In addition, a declared object of this class is always associated to one meson type (see PartonType::m_type), so member functions can act on it.
 */
class JetCFFType {

public:

    /**
     * Definition of enumerate values corresponding to parton types.
     */
    enum Type {
        UNDEFINED = 0,   //!< Undefined type.

        LL = 1,          //!< \f$...\f$
        TL = 2,          //!<  \f$...\f$
        TT1 = 3,         //!<  \f$...\f$
        TT2 = 4          //!<  \f$...\f$
    };

    /**
     * Default constructor.
     */
    JetCFFType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    JetCFFType(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JetCFFType(const JetCFFType &other);

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
    bool operator <(const JetCFFType &other) const;

    /**
     * Try to match meson type from given string.
     * @param mesonTypeStr String to be matched.
     * @return Matched type or PartonType::UNDEFINED if unable to match.
     */
    static JetCFFType::Type fromString(const std::string & mesonTypeStr);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    JetCFFType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    JetCFFType::Type m_type;
};

/**
 * Stream operator to serialize class into Packet. See also PartonType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, JetCFFType& mesonType);

/**
 * Stream operator to retrieve class from Packet. See also PartonType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, JetCFFType& mesonType);

} /* namespace PARTONS */

#endif /* JET_CFF_TYPE_H */

