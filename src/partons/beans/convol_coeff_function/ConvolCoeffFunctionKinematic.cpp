#include "../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/beans/observable/ObservableKinematic.h"

namespace PARTONS {

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType) :
        Kinematic(className, channelType), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(
                0.) {
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType,
        const ElemUtils::Parameters &parameters) :
        Kinematic(className, channelType), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(
                0.) {
    if (parameters.isAvailable(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI)) {
        m_xi = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI);
    }
    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(ObservableKinematic::PARAMETER_NAME_T);
    }
    if (parameters.isAvailable(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2)) {
        m_MuF2 = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2);
    }

    if (parameters.isAvailable(
            GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2)) {
        m_MuR2 = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2);
    }
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType, double xi,
        double t, double MuF2, double MuR2) :
        Kinematic(className, channelType), m_xi(xi), m_t(t), m_MuF2(MuF2), m_MuR2(
                MuR2) {
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType,
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &MuF2, const ElemUtils::GenericType &MuR2) :
        Kinematic(className, channelType), m_xi(xi.toDouble()), m_t(
                t.toDouble()), m_MuF2(MuF2.toDouble()), m_MuR2(MuR2.toDouble()) {
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const ConvolCoeffFunctionKinematic &other) :
        Kinematic(other), m_xi(other.m_xi), m_t(other.m_t), m_MuF2(
                other.m_MuF2), m_MuR2(other.m_MuR2) {
}

ConvolCoeffFunctionKinematic::~ConvolCoeffFunctionKinematic() {
}

std::string ConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Kinematic::toString();
    formatter << "\txi: " << m_xi << " t: " << m_t << " muF2: " << m_MuF2
            << " muR2: " << m_MuR2;

    return formatter.str();
}

void ConvolCoeffFunctionKinematic::serialize(ElemUtils::Packet& packet) const {
    Kinematic::serialize(packet);

    packet << m_xi << m_t << m_MuF2 << m_MuR2;
}

void ConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {
    Kinematic::unserialize(packet);

    packet >> m_xi;
    packet >> m_t;
    packet >> m_MuF2;
    packet >> m_MuR2;
}

double ConvolCoeffFunctionKinematic::getMuF2() const {
    return m_MuF2;
}

double ConvolCoeffFunctionKinematic::getMuR2() const {
    return m_MuR2;
}

double ConvolCoeffFunctionKinematic::getT() const {
    return m_t;
}

double ConvolCoeffFunctionKinematic::getXi() const {
    return m_xi;
}

void ConvolCoeffFunctionKinematic::setXi(double xi) {
    m_xi = xi;
}

void ConvolCoeffFunctionKinematic::setT(double t) {
    m_t = t;
}

void ConvolCoeffFunctionKinematic::setMuF2(double muF2) {
    m_MuF2 = muF2;
}

void ConvolCoeffFunctionKinematic::setMuR2(double muR2) {
    m_MuR2 = muR2;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        ConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        ConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
