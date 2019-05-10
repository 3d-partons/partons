#include "../../../../include/partons/utils/type/PhysicalUnit.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

PhysicalUnit::PhysicalUnit() :
        m_type(PhysicalUnit::UNDEFINED) {
}

PhysicalUnit::PhysicalUnit(Type type) :
        m_type(type) {
}

/**
 * Assignment constructor.
 * @param type Type to be assigned.
 */
PhysicalUnit::PhysicalUnit(const std::string& type) {

    //undefined
    if (type == "UNDEFINED") {
        m_type = PhysicalUnit::UNDEFINED;
    }

    //none
    else if (type == "none") {
        m_type = PhysicalUnit::NONE;
    }

    //energy, momentum or mass
    else if (type == "eV") {
        m_type = PhysicalUnit::EV;
    }

    else if (type == "KeV") {
        m_type = PhysicalUnit::KEV;
    }

    else if (type == "MeV") {
        m_type = PhysicalUnit::MEV;
    }

    else if (type == "GeV") {
        m_type = PhysicalUnit::GEV;
    }

    else if (type == "TeV") {
        m_type = PhysicalUnit::TEV;
    }

    //energy, momentum or mass squared
    else if (type == "eV2") {
        m_type = PhysicalUnit::EV2;
    }

    else if (type == "KeV2") {
        m_type = PhysicalUnit::KEV2;
    }

    else if (type == "MeV2") {
        m_type = PhysicalUnit::MEV2;
    }

    else if (type == "GeV2") {
        m_type = PhysicalUnit::GEV2;
    }

    else if (type == "TeV2") {
        m_type = PhysicalUnit::TEV2;
    }

    //distance or time
    else if (type == "eVm1") {
        m_type = PhysicalUnit::EVm1;
    }

    else if (type == "KeVm1") {
        m_type = PhysicalUnit::KEVm1;
    }

    else if (type == "MeVm1") {
        m_type = PhysicalUnit::MEVm1;
    }

    else if (type == "GeVm1") {
        m_type = PhysicalUnit::GEVm1;
    }

    else if (type == "TeVm1") {
        m_type = PhysicalUnit::TEVm1;
    }

    else if (type == "fm") {
        m_type = PhysicalUnit::FM;
    }

    else if (type == "pm") {
        m_type = PhysicalUnit::PM;
    }

    else if (type == "nm") {
        m_type = PhysicalUnit::NM;
    }

    else if (type == "um") {
        m_type = PhysicalUnit::UM;
    }

    else if (type == "mm") {
        m_type = PhysicalUnit::MM;
    }

    else if (type == "m") {
        m_type = PhysicalUnit::M;
    }

    else if (type == "fs") {
        m_type = PhysicalUnit::FS;
    }

    else if (type == "ps") {
        m_type = PhysicalUnit::PS;
    }

    else if (type == "ns") {
        m_type = PhysicalUnit::NS;
    }

    else if (type == "us") {
        m_type = PhysicalUnit::US;
    }

    else if (type == "ms") {
        m_type = PhysicalUnit::MS;
    }

    else if (type == "s") {
        m_type = PhysicalUnit::S;
    }

    //cross-section
    else if (type == "GeVm2") {
        m_type = PhysicalUnit::GEVm2;
    }

    else if (type == "fm2") {
        m_type = PhysicalUnit::FM2;
    }

    else if (type == "fb") {
        m_type = PhysicalUnit::FB;
    }

    else if (type == "pb") {
        m_type = PhysicalUnit::PB;
    }

    else if (type == "nb") {
        m_type = PhysicalUnit::NB;
    }

    else if (type == "ub") {
        m_type = PhysicalUnit::UB;
    }

    else if (type == "mb") {
        m_type = PhysicalUnit::MB;
    }

    else if (type == "b") {
        m_type = PhysicalUnit::B;
    }

    //angle
    else if (type == "deg") {
        m_type = PhysicalUnit::DEG;
    }

    else if (type == "rad") {
        m_type = PhysicalUnit::RAD;
    }

    else if (type == "mrad") {
        m_type = PhysicalUnit::MRAD;
    }

    //no such type
    else {
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Cannot determine unit from string "
                        << type);
    }
}

PhysicalUnit::PhysicalUnit(const PhysicalUnit &other) :
        m_type(other.m_type) {
}

PhysicalUnit::operator PhysicalUnit::Type() const {
    return m_type;
}

std::string PhysicalUnit::toString() const {

    switch (m_type) {

    //undefined
    case UNDEFINED:
        return "UNDEFINED";
        break;

        //none
    case NONE:
        return "NONE";
        break;

        //energy, momentum or mass
    case EV:
        return "EV";
        break;
    case KEV:
        return "KEV";
        break;
    case MEV:
        return "MEV";
        break;
    case GEV:
        return "GEV";
        break;
    case TEV:
        return "TEV";
        break;

        //energy, momentum or mass squared
    case EV2:
        return "EV2";
        break;
    case KEV2:
        return "KEV2";
        break;
    case MEV2:
        return "MEV2";
        break;
    case GEV2:
        return "GEV2";
        break;
    case TEV2:
        return "TEV2";
        break;

        //distance or time
    case EVm1:
        return "EVm1";
        break;
    case KEVm1:
        return "KEVm1";
        break;
    case MEVm1:
        return "MEVm1";
        break;
    case GEVm1:
        return "GEVm1";
        break;
    case TEVm1:
        return "TEVm1";
        break;

    case FM:
        return "FM";
        break;
    case PM:
        return "PM";
        break;
    case NM:
        return "NM";
        break;
    case UM:
        return "UM";
        break;
    case MM:
        return "MM";
        break;
    case M:
        return "M";
        break;

    case FS:
        return "FS";
        break;
    case PS:
        return "PS";
        break;
    case NS:
        return "NS";
        break;
    case US:
        return "US";
        break;
    case MS:
        return "MS";
        break;
    case S:
        return "S";
        break;

        //cross-section
    case GEVm2:
        return "GEVm2";
        break;
    case FM2:
        return "FM2";
        break;
    case FB:
        return "FB";
        break;
    case PB:
        return "PB";
        break;
    case NB:
        return "NB";
        break;
    case UB:
        return "UB";
        break;
    case MB:
        return "B";
        break;
    case B:
        return "B";
        break;

        //angle
    case DEG:
        return "DEG";
        break;
    case RAD:
        return "RAD";
        break;
    case MRAD:
        return "MRAD";
        break;

        //no such type
    default:
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

std::string PhysicalUnit::getShortName() {

    switch (m_type) {

    //undefined
    case UNDEFINED:
        return "UNDEFINED";
        break;

        //none
    case NONE:
        return "none";
        break;

        //energy, momentum or mass
    case EV:
        return "eV";
        break;
    case KEV:
        return "KeV";
        break;
    case MEV:
        return "MeV";
        break;
    case GEV:
        return "GeV";
        break;
    case TEV:
        return "TeV";
        break;

        //energy, momentum or mass squared
    case EV2:
        return "eV2";
        break;
    case KEV2:
        return "KeV2";
        break;
    case MEV2:
        return "MeV2";
        break;
    case GEV2:
        return "GeV2";
        break;
    case TEV2:
        return "TeV2";
        break;

        //distance or time
    case EVm1:
        return "eVm1";
        break;
    case KEVm1:
        return "KeVm1";
        break;
    case MEVm1:
        return "MeVm1";
        break;
    case GEVm1:
        return "GeVm1";
        break;
    case TEVm1:
        return "TeVm1";
        break;

    case FM:
        return "fm";
        break;
    case PM:
        return "pm";
        break;
    case NM:
        return "nm";
        break;
    case UM:
        return "um";
        break;
    case MM:
        return "mm";
        break;
    case M:
        return "m";
        break;

    case FS:
        return "fs";
        break;
    case PS:
        return "ps";
        break;
    case NS:
        return "ns";
        break;
    case US:
        return "us";
        break;
    case MS:
        return "ms";
        break;
    case S:
        return "s";
        break;

        //cross-section
    case GEVm2:
        return "GeVm2";
        break;
    case FM2:
        return "fm2";
        break;
    case FB:
        return "fb";
        break;
    case PB:
        return "pb";
        break;
    case NB:
        return "nb";
        break;
    case UB:
        return "ub";
        break;
    case MB:
        return "mb";
        break;
    case B:
        return "b";
        break;

        //angle
    case DEG:
        return "deg";
        break;
    case RAD:
        return "rad";
        break;
    case MRAD:
        return "mrad";
        break;

        //no such type
    default:
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

void PhysicalUnit::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void PhysicalUnit::unserialize(ElemUtils::Packet &packet) {
    int i = 0;
    packet >> i;
    m_type = static_cast<PhysicalUnit::Type>(i);
}

UnitCategory::Type PhysicalUnit::getUnitCategory() const {

    switch (m_type) {

    //undefined
    case UNDEFINED:
        return UnitCategory::UNDEFINED;
        break;

        //none
    case NONE:
        return UnitCategory::NONE;
        break;

        //energy, momentum or mass
    case EV:
        return UnitCategory::EMP;
        break;
    case KEV:
        return UnitCategory::EMP;
        break;
    case MEV:
        return UnitCategory::EMP;
        break;
    case GEV:
        return UnitCategory::EMP;
        break;
    case TEV:
        return UnitCategory::EMP;
        break;

        //energy, momentum or mass squared
    case EV2:
        return UnitCategory::EMP2;
        break;
    case KEV2:
        return UnitCategory::EMP2;
        break;
    case MEV2:
        return UnitCategory::EMP2;
        break;
    case GEV2:
        return UnitCategory::EMP2;
        break;
    case TEV2:
        return UnitCategory::EMP2;
        break;

        //distance or time
    case EVm1:
        return UnitCategory::DISTANCE_TIME;
        break;
    case KEVm1:
        return UnitCategory::DISTANCE_TIME;
        break;
    case MEVm1:
        return UnitCategory::DISTANCE_TIME;
        break;
    case GEVm1:
        return UnitCategory::DISTANCE_TIME;
        break;
    case TEVm1:
        return UnitCategory::DISTANCE_TIME;
        break;

    case FM:
        return UnitCategory::DISTANCE_TIME;
        break;
    case PM:
        return UnitCategory::DISTANCE_TIME;
        break;
    case NM:
        return UnitCategory::DISTANCE_TIME;
        break;
    case UM:
        return UnitCategory::DISTANCE_TIME;
        break;
    case MM:
        return UnitCategory::DISTANCE_TIME;
        break;
    case M:
        return UnitCategory::DISTANCE_TIME;
        break;

    case FS:
        return UnitCategory::DISTANCE_TIME;
        break;
    case PS:
        return UnitCategory::DISTANCE_TIME;
        break;
    case NS:
        return UnitCategory::DISTANCE_TIME;
        break;
    case US:
        return UnitCategory::DISTANCE_TIME;
        break;
    case MS:
        return UnitCategory::DISTANCE_TIME;
        break;
    case S:
        return UnitCategory::DISTANCE_TIME;
        break;

        //cross-section
    case GEVm2:
        return UnitCategory::CROSS_SECTION;
        break;
    case FM2:
        return UnitCategory::CROSS_SECTION;
        break;
    case FB:
        return UnitCategory::CROSS_SECTION;
        break;
    case PB:
        return UnitCategory::CROSS_SECTION;
        break;
    case NB:
        return UnitCategory::CROSS_SECTION;
        break;
    case UB:
        return UnitCategory::CROSS_SECTION;
        break;
    case MB:
        return UnitCategory::CROSS_SECTION;
        break;
    case B:
        return UnitCategory::CROSS_SECTION;
        break;

        //angle
    case DEG:
        return UnitCategory::ANGLE;
        break;
    case RAD:
        return UnitCategory::ANGLE;
        break;
    case MRAD:
        return UnitCategory::ANGLE;
        break;

        //no such type
    default:
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

double PhysicalUnit::getConversionFactor() const {

    switch (m_type) {

    //undefined
    case UNDEFINED:
        return 1.;
        break;

        //none
    case NONE:
        return 1.;
        break;

        //energy, momentum or mass
    case EV:
        return 1.E0;
        break;
    case KEV:
        return 1.E3;
        break;
    case MEV:
        return 1.E6;
        break;
    case GEV:
        return 1.E9;
        break;
    case TEV:
        return 1.E12;
        break;

        //energy, momentum or mass squared
    case EV2:
        return 1.E0;
        break;
    case KEV2:
        return 1.E6;
        break;
    case MEV2:
        return 1.E12;
        break;
    case GEV2:
        return 1.E18;
        break;
    case TEV2:
        return 1.E24;
        break;

        //distance or time
    case EVm1:
        return 1.E0;
        break;
    case KEVm1:
        return 1.E-3;
        break;
    case MEVm1:
        return 1.E-6;
        break;
    case GEVm1:
        return 1.E-9;
        break;
    case TEVm1:
        return 1.E-12;
        break;

    case FM:
        return 1.E-6 / Constant::HBarC;
        break;
    case PM:
        return 1.E-3 / Constant::HBarC;
        break;
    case NM:
        return 1.E0 / Constant::HBarC;
        break;
    case UM:
        return 1.E3 / Constant::HBarC;
        break;
    case MM:
        return 1.E6 / Constant::HBarC;
        break;
    case M:
        return 1.E9 / Constant::HBarC;
        break;

    case FS:
        return 1.E-15 / Constant::PLANCK_CONSTANT_REDUCED;
        break;
    case PS:
        return 1.E-12 / Constant::PLANCK_CONSTANT_REDUCED;
        break;
    case NS:
        return 1.E-9 / Constant::PLANCK_CONSTANT_REDUCED;
        break;
    case US:
        return 1.E-6 / Constant::PLANCK_CONSTANT_REDUCED;
        break;
    case MS:
        return 1.E-3 / Constant::PLANCK_CONSTANT_REDUCED;
        break;
    case S:
        return Constant::PLANCK_CONSTANT_REDUCED;
        break;

        //cross-section
    case GEVm2:
        return 1.E0;
        break;

    case FM2:
        return 1.E7 / Constant::CONV_GEVm2_TO_NBARN;
        break;

    case FB:
        return 1.E-6 / Constant::CONV_GEVm2_TO_NBARN;
        break;
    case PB:
        return 1.E-3 / Constant::CONV_GEVm2_TO_NBARN;
        break;
    case NB:
        return 1.E0 / Constant::CONV_GEVm2_TO_NBARN;
        break;
    case UB:
        return 1.E3 / Constant::CONV_GEVm2_TO_NBARN;
        break;
    case MB:
        return 1.E6 / Constant::CONV_GEVm2_TO_NBARN;
        break;
    case B:
        return 1.E9 / Constant::CONV_GEVm2_TO_NBARN;
        break;

        //angle
    case DEG:
        return 1.;
        break;
    case RAD:
        return 180. / Constant::PI;
        break;
    case MRAD:
        return 1.E-3 * 180. / Constant::PI;
        break;

        //no such type
    default:
        throw ElemUtils::CustomException("PhysicalType", __func__,
                ElemUtils::Formatter() << "Type with id " << int(m_type)
                        << " not implemented here");
    }
}

PhysicalUnit::Type PhysicalUnit::getType() const {
    return m_type;
}

void PhysicalUnit::setType(Type type) {
    m_type = type;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        PhysicalUnit& physicalUnit) {

    physicalUnit.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        PhysicalUnit& physicalUnit) {

    physicalUnit.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
