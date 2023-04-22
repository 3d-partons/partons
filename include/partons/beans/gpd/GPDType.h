#ifndef GPD_TYPE_H
#define GPD_TYPE_H

/**
 * @file GPDType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 July 2014
 * @version 1.0
 */

#include <string>

#include "../List.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class GPDType
 *
 * @brief Definition of enumeration values for GPD types.
 *
 * This class defines a set of enumeration values that are used to distinguish between GPD types. In addition, a declared object of this class is always associated to one GPD type (see GPDType::m_type), so member functions can act on it.
 */
class GPDType {

public:

//    /**
//     * Name of table in the database corresponding to this class.
//     */
//    static const std::string GPD_TYPE_DB_COLUMN_NAME;

    /**
     * Definition of enumerate values corresponding to GPD types.
     */
    enum Type {

        UNDEFINED = 0, //!< Undefined type.

        ALL = 1,     //!< All-like type, useful to indicate all available types.

        H = 2,        //!< Twist-2 GPD \f$H\f$
        E = 3,        //!< Twist-2 GPD \f$E\f$
        Ht = 4,       //!< Twist-2 GPD \f$\tilde{H}\f$
        Et = 5,       //!< Twist-2 GPD \f$\tilde{E}\f$
        HTrans = 6,   //!< Twist-2 GPD \f$H_{T}\f$
        ETrans = 7,   //!< Twist-2 GPD \f$E_{T}\f$
        HtTrans = 8,  //!< Twist-2 GPD \f$\tilde{H}_{T}\f$
        EtTrans = 9,  //!< Twist-2 GPD \f$\tilde{E}_{T}\f$
        H3p = 10,     //!< Twist-3 GPD \f$H_{3}^{+}\f$
        E3p = 11,     //!< Twist-3 GPD \f$E_{3}^{+}\f$
        Ht3p = 12,    //!< Twist-3 GPD \f$\tilde{H}_{3}^{+}\f$
        Et3p = 13,    //!< Twist-3 GPD \f$\tilde{E}_{3}^{+}\f$
        H3m = 14,     //!< Twist-3 GPD \f$H_{3}^{-}\f$
        E3m = 15,     //!< Twist-3 GPD \f$E_{3}^{-}\f$
        Ht3m = 16,    //!< Twist-3 GPD \f$\tilde{H}_{3}^{-}\f$
        Et3m = 17,    //!< Twist-3 GPD \f$\tilde{E}_{3}^{-}\f$

        //Artificial types:

        EbarTrans = 18, //!< \f$2\tilde{H}_{T} + E_{T}\f$
        HL = 19, //!< Used in DDVCS to mark CFF longitudinal helicity amplitudes related to GPD H
        EL = 20, //!< Used in DDVCS to mark CFF longitudinal helicity amplitudes related to GPD E

        END       //!< End-like type, useful to define loops over all GPD types.
    };

    /**
     * Default constructor.
     */
    GPDType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    GPDType(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDType(const GPDType &other);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "H" for GPDType::H.
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
     * Relation operator that checks if the value of left operand is less than the value of right operand (based on values assigned in the definition of GPDType::Type).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const GPDType &other) const;

    /**
     * Try to match GPD type from given string.
     * @param gpdTypeStr String to be matched.
     * @return Matched type or GPDType::UNDEFINED if unable to match.
     */
    static GPDType::Type fromString(const std::string & gpdTypeStr);

    /**
     * Try to match list of GPD types from given string. Types should be separated by the pipe symbol, e.g. "H|E|..."
     * @param gpdTypeListAsString String to be matched.
     * @return List of matched GPDType objects.
     */
    static List<GPDType> getListOfGPDTypeFromString(
            const std::string &gpdTypeListAsString);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    GPDType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    GPDType::Type m_type;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, GPDType& gpdType);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, GPDType& gpdType);

} /* namespace PARTONS */

#endif /* GPD_COMPUTE_TYPE_H */
