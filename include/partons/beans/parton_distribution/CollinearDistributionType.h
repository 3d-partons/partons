#ifndef COLLINEAR_DISTRIBUTION_TYPE_H
#define COLLINEAR_DISTRIBUTION_TYPE_H

/**
 * @file CollinearDistributionType.h
 * @author: Valerio BERTONE (CEA Saclay)
 * @date 18 July 2020
 * @version 1.0
 */

#include <string>

#include "../List.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class CollinearDistributionType
 *
 * @brief Definition of enumeration values for collinear distribution types.
 *
 * This class defines a set of enumeration values that are used to
 distinguish between collinear distributions. In addition, a declared
 object of this class is always associated to one
 collinear-distribution type (see CollinearDistributionType::m_type),
 so member functions can act on it.
 */
class CollinearDistributionType {

public:

    /**
     * Name of table in the database corresponding to this class.
     */
    static const std::string COLLINEAR_DISTRIBUTION_TYPE_DB_COLUMN_NAME;

    /**
     * Definition of enumerate values corresponding to
     * collinear-distribution types.
     */
    enum Type {
        UNDEFINED = 0,     //!< Undefined type.
        ALL = 1,           //!< All-like type, useful to indicate all available types.
        UnpolPDF = 2,      //!< Twist-2 collinear unpolarised PDFs
        PolPDF = 3,        //!< Twist-2 collinear logitudinally polarised PDFs
        TransPDF = 4,      //!< Twist-2 collinear transversely polarised PDFs
        UnpolFF = 5,       //!< Twist-2 collinear unpolarised FFs
        PolFF = 6,         //!< Twist-2 collinear logitudinally polarised FFs
        TransFF = 7,       //!< Twist-2 collinear transversely polarised FFs
        END                //!< End-like type, useful to define loops over all types.
    };

    /**
     * Default constructor.
     */
    CollinearDistributionType();

    /**
     * Assignment constructor.
     * @param type Type to be assigned.
     */
    CollinearDistributionType(Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    CollinearDistributionType(const CollinearDistributionType &other);

    /**
     * Automatic cast to enum.
     */
    operator Type() const;

    /**
     * Get string representation of type being assigned to a declared object of this class.
     * @return String representation of assigned type, like "H" for CollinearDistributionType::H.
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
     * Relation operator that checks if the value of left operand is less than the value of right operand (based on values assigned in the definition of CollinearDistributionType::Type).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const CollinearDistributionType &other) const;

    /**
     * Try to match collinear distribution type from given string.
     * @param colldistTypeStr String to be matched.
     * @return Matched type or CollinearDistributionType::UNDEFINED if unable to match.
     */
    static CollinearDistributionType::Type fromString(const std::string & colldistTypeStr);

    /**
     * Try to match list of collinear distribution types from given string. Types should be separated by the pipe symbol, e.g. "H|E|..."
     * @param colldistTypeListAsString String to be matched.
     * @return List of matched CollinearDistributionType objects.
     */
    static List<CollinearDistributionType> getListOfCollinearDistributionTypeFromString(
            const std::string &colldistTypeListAsString);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get type being assigned to a declared object of this class.
     */
    CollinearDistributionType::Type getType() const;

    /**
     * Assign type to a declared object of this class.
     */
    void setType(Type type);

private:

    /**
     * Type associated to a declared object of this class.
     */
    CollinearDistributionType::Type m_type;
};

/**
 * Stream operator to serialize class into Packet. See also CollinearDistributionType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, CollinearDistributionType& colldistType);

/**
 * Stream operator to retrieve class from Packet. See also CollinearDistributionType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, CollinearDistributionType& colldistType);

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_TYPE_H */
