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

        //undefined
        UNDEFINED = 0,  ///< undefined

        //none
        NONE = 1,       ///< none

        //energy, momentum or mass
        EV = 2,         ///< eV
        KEV = 3,        ///< keV
        MEV = 4,        ///< MeV
        GEV = 5,        ///< GeV
        TEV = 6,        ///< TeV

        //energy, momentum or mass squared
        EV2 = 7,         ///< eV^2
        KEV2 = 8,        ///< keV^2
        MEV2 = 9,        ///< MeV^2
        GEV2 = 10,       ///< GeV^2
        TEV2 = 11,       ///< TeV^2

        //distance or time
        EVm1 = 12,       ///< eV^-1
        KEVm1 = 13,      ///< keV^-1
        MEVm1 = 14,      ///< MeV^-1
        GEVm1 = 15,      ///< GeV^-1
        TEVm1 = 16,      ///< TeV^-1

        FM = 17,         ///< fm
        PM = 18,         ///< pm
        NM = 19,         ///< nm
        UM = 20,         ///< um
        MM = 21,         ///< mm
        M = 22,          ///< m

        FS = 23,         ///< fs
        PS = 24,         ///< ps
        NS = 25,         ///< ns
        US = 26,         ///< us
        MS = 27,         ///< ms
        S = 28,          ///< s

        //cross-section
        FB = 29,         ///< fb
        PB = 30,         ///< pb
        NB = 31,         ///< nb
        UB = 32,         ///< ub
        MB = 33,         ///< mb
        B = 34,          ///< b

        //angle
        DEG = 35,        ///< degree
        RAD = 36,        ///< radian
        MRAD = 37        ///< mradian
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
