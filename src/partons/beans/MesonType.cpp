#include "../../../include/partons/beans/MesonType.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/FundamentalPhysicalConstants.h"

namespace PARTONS {

MesonType::MesonType() :
        m_type(MesonType::UNDEFINED) {
}

MesonType::MesonType(Type type) :
        m_type(type) {
}

MesonType::MesonType(const MesonType &other) {
    m_type = other.m_type;
}

MesonType::operator MesonType::Type() const {
    return m_type;
}

List<GPDType> MesonType::getPossibleGPDTypes() const {

    List<GPDType> list;

    //pseudo-scalars
    if (m_type == PIMINUS || m_type == PI0 || m_type == PIPLUS) {

        list.add(GPDType(GPDType::Ht));
        list.add(GPDType(GPDType::Et));
        list.add(GPDType(GPDType::HTrans));
        list.add(GPDType(GPDType::ETrans));
        list.add(GPDType(GPDType::EbarTrans));
    }

    //vector
    else if (m_type == RHOMINUS || m_type == RHO0 || m_type == RHOPLUS
            || m_type == OMEGA || m_type == PHI || m_type == JPSI
            || m_type == UPSILON

            ) {

        list.add(GPDType(GPDType::H));
        list.add(GPDType(GPDType::E));
        list.add(GPDType(GPDType::HTrans));
        list.add(GPDType(GPDType::ETrans));
        list.add(GPDType(GPDType::EbarTrans));
    } else {
        throw ElemUtils::CustomException("MesonType", __func__,
                ElemUtils::Formatter() << "Not able to return value for type "
                        << toString());
    }

    return list;
}

double MesonType::getMass() const {

    switch (m_type) {

    case RHOMINUS:
        return Constant::MESON_RHOMINUS_MASS;
        break;
    case RHO0:
        return Constant::MESON_RHO0_MASS;
        break;
    case RHOPLUS:
        return Constant::MESON_RHOPLUS_MASS;
        break;
    case OMEGA:
        return Constant::MESON_OMEGA_MASS;
        break;
    case PHI:
        return Constant::MESON_PHI_MASS;
        break;
    case JPSI:
        return Constant::MESON_JPSI_MASS;
        break;
    case UPSILON:
        return Constant::MESON_UPSILON_MASS;
        break;

    case PIMINUS:
        return Constant::MESON_PIMINUS_MASS;
        break;
    case PI0:
        return Constant::MESON_PI0_MASS;
        break;
    case PIPLUS:
        return Constant::MESON_PIPLUS_MASS;
        break;

    default:

        throw ElemUtils::CustomException("MesonType", __func__,
                ElemUtils::Formatter() << "Not able to return value for type "
                        << toString());
        return 0.;
    }
}

std::string MesonType::toString() const {

    switch (m_type) {

    case RHOMINUS:
        return "RHOMINUS";
        break;
    case RHO0:
        return "RHO0";
        break;
    case RHOPLUS:
        return "RHOPLUS";
        break;
    case OMEGA:
        return "OMEGA";
        break;
    case PHI:
        return "PHI";
        break;
    case JPSI:
        return "JPSI";
        break;
    case UPSILON:
        return "UPSILON";
        break;

    case PIMINUS:
        return "PIMINUS";
        break;
    case PI0:
        return "PI0";
        break;
    case PIPLUS:
        return "PIPLUS";
        break;

    default:
        return "UNDEFINED";
    }
}

MesonType::Type MesonType::getType() const {
    return m_type;
}

void MesonType::setType(Type type) {
    m_type = type;
}

MesonType::Type MesonType::fromString(const std::string& mesonTypeStr) {

    MesonType::Type mesonType = MesonType::UNDEFINED;

    if (ElemUtils::StringUtils::equals(mesonTypeStr, "rho-")) {
        mesonType = MesonType::RHOMINUS;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "rho0")) {
        mesonType = MesonType::RHO0;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "rho+")) {
        mesonType = MesonType::RHOPLUS;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "omega")) {
        mesonType = MesonType::OMEGA;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "phi")) {
        mesonType = MesonType::PHI;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "J/Psi")) {
        mesonType = MesonType::JPSI;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "upsilon")) {
        mesonType = MesonType::UPSILON;
    }

    else if (ElemUtils::StringUtils::equals(mesonTypeStr, "pi-")) {
        mesonType = MesonType::PIMINUS;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "pi0")) {
        mesonType = MesonType::PI0;
    } else if (ElemUtils::StringUtils::equals(mesonTypeStr, "pi+")) {
        mesonType = MesonType::PIPLUS;
    }

    return mesonType;
}

void MesonType::serialize(ElemUtils::Packet &packet) const {
    packet << static_cast<int>(m_type);
}

void MesonType::unserialize(ElemUtils::Packet &packet) {

    int i = 0;
    packet >> i;
    m_type = static_cast<MesonType::Type>(i);
}

bool MesonType::operator <(const MesonType& other) const {
    return (m_type < other.m_type);
}

ElemUtils::Packet & operator <<(ElemUtils::Packet & packet,
        MesonType & mesonType) {

    mesonType.serialize(packet);
    return packet;
}

ElemUtils::Packet & operator >>(ElemUtils::Packet & packet,
        MesonType & mesonType) {

    mesonType.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */

