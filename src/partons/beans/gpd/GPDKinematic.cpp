#include "../../../../include/partons/beans/gpd/GPDKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../include/partons/Partons.h"
#include "../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_X = "x";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_XI = "xi";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_T = "t";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

GPDKinematic::GPDKinematic() :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED) {
    setHashSum(ElemUtils::StringUtils::EMPTY);
}

GPDKinematic::GPDKinematic(const ElemUtils::Parameters &parameters) :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_xi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_t(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_MuF2(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_MuR2(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())) {

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_X)) {
        m_x.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_X);
    }

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI)) {
        m_xi.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI);
    }

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_T)) {
        m_t.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_T);
    }

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2)) {
        m_MuF2.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2);
    }

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2)) {
        m_MuR2.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2);
    }

    updateHashSum();
}

GPDKinematic::GPDKinematic(double x, double xi, double t, double MuF2,
        double MuR2) :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_xi(
                PhysicalType<double>(xi,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_t(
                PhysicalType<double>(t,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_MuF2(
                PhysicalType<double>(MuF2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_MuR2(
                PhysicalType<double>(MuR2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())) {
    updateHashSum();
}

GPDKinematic::GPDKinematic(const PhysicalType<double> &x,
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(x), m_xi(xi), m_t(
                t), m_MuF2(MuF2), m_MuR2(MuR2) {
    updateHashSum();
}

GPDKinematic::GPDKinematic(const ElemUtils::GenericType& x,
        const ElemUtils::GenericType& xi, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& MuF2, const ElemUtils::GenericType& MuR2) :
        Kinematic("GPDKinematic", ChannelType::UNDEFINED), m_x(
                PhysicalType<double>(x,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_xi(
                PhysicalType<double>(xi,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_t(
                PhysicalType<double>(t,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_MuF2(
                PhysicalType<double>(MuF2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_MuR2(
                PhysicalType<double>(MuR2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())) {
    updateHashSum();
}

GPDKinematic::GPDKinematic(const GPDKinematic& other) :
        Kinematic(other), m_x(other.m_x), m_xi(other.m_xi), m_t(other.m_t), m_MuF2(
                other.m_MuF2), m_MuR2(other.m_MuR2) {
    updateHashSum();
}

GPDKinematic::~GPDKinematic() {
}

std::string GPDKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Kinematic::toString();
    formatter << "\tx: " << m_x.toString() << " xi: " << m_xi.toString()
            << " t: " << m_t.toString() << " muF2: " << m_MuF2.toString()
            << " muR2: " << m_MuR2.toString() << '\n';

    return formatter.str();
}

void GPDKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_x.getValue() << m_xi.getValue()
                            << m_t.getValue() << m_MuF2.getValue()
                            << m_MuR2.getValue()));
}

void GPDKinematic::setX(double x) {
    m_x = x;
    updateHashSum();
}

void GPDKinematic::setXi(double xi) {
    m_xi = xi;
    updateHashSum();
}

void GPDKinematic::setT(double t) {
    m_t = t;
    updateHashSum();
}

void GPDKinematic::setMuF2(double muF2) {
    m_MuF2 = muF2;
    updateHashSum();
}

void GPDKinematic::setMuR2(double muR2) {
    m_MuR2 = muR2;
    updateHashSum();
}

PhysicalType<double> GPDKinematic::getX() const {
    return m_x;
}

PhysicalType<double> GPDKinematic::getXi() const {
    return m_xi;
}

PhysicalType<double> GPDKinematic::getT() const {
    return m_t;
}

PhysicalType<double> GPDKinematic::getMuF2() const {
    return m_MuF2;
}

PhysicalType<double> GPDKinematic::getMuR2() const {
    return m_MuR2;
}

void GPDKinematic::serialize(ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_x << m_xi << m_t << m_MuF2 << m_MuR2;
}

void GPDKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_x;
    packet >> m_xi;
    packet >> m_t;
    packet >> m_MuF2;
    packet >> m_MuR2;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GPDKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GPDKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
