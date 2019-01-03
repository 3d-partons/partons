#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic() :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS) {
    setHashSum(ElemUtils::StringUtils::EMPTY);
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const ElemUtils::Parameters &parameters) :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS, parameters), m_Q2(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())) {

    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {
        m_Q2.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    updateHashSum();
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(double xi,
        double t, double Q2, double MuF2, double MuR2) :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())) {
    updateHashSum();
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &Q2, const PhysicalType<double> &MuF2,
        const PhysicalType<double> &MuR2) :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS, xi, t, MuF2, MuR2), m_Q2(Q2) {
    updateHashSum();
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &Q2, const ElemUtils::GenericType &MuF2,
        const ElemUtils::GenericType &MuR2) :
        ConvolCoeffFunctionKinematic("DVCSConvolCoeffFunctionKinematic",
                ChannelType::DVCS, xi, t, MuF2, MuR2), m_Q2(
                PhysicalType<double>(Q2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())) {
    updateHashSum();
}

DVCSConvolCoeffFunctionKinematic::DVCSConvolCoeffFunctionKinematic(
        const DVCSConvolCoeffFunctionKinematic &other) :
        ConvolCoeffFunctionKinematic(other), m_Q2(other.m_Q2) {
}

DVCSConvolCoeffFunctionKinematic::~DVCSConvolCoeffFunctionKinematic() {
}

std::string DVCSConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionKinematic::toString();
    formatter << " Q2: " << m_Q2.toString() << '\n';

    return formatter.str();
}

void DVCSConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi.getValue() << m_t.getValue()
                            << m_Q2.getValue() << m_MuF2.getValue()
                            << m_MuR2.getValue()));
}

void DVCSConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {

    ConvolCoeffFunctionKinematic::serialize(packet);

    packet << m_Q2;
}

void DVCSConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {

    ConvolCoeffFunctionKinematic::unserialize(packet);

    packet >> m_Q2;

    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setXi(double xi) {

    m_xi = xi;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setT(double t) {
    m_t = t;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setMuF2(double MuF2) {
    m_MuF2 = MuF2;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setMuR2(double MuR2) {
    m_MuR2 = MuR2;
    updateHashSum();
}

void DVCSConvolCoeffFunctionKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
    updateHashSum();
}

PhysicalType<double> DVCSConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVCSConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
