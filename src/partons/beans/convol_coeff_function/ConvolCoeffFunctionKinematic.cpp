#include "../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType) :
        Kinematic(className, channelType), m_xi(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType,
        const ElemUtils::Parameters &parameters) :
        Kinematic(className, channelType), m_xi(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

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
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType, double xi,
        double t, double MuF2, double MuR2) :
        Kinematic(className, channelType), m_xi(
                PhysicalType<double>(xi, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType,
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) :
        Kinematic(className, channelType), m_xi(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(PhysicalUnit::GEV2)) {

    m_xi.checkIfSameUnitCategoryAs(xi);
    m_t.checkIfSameUnitCategoryAs(t);
    m_MuF2.checkIfSameUnitCategoryAs(MuF2);
    m_MuR2.checkIfSameUnitCategoryAs(MuR2);

    m_xi = xi;
    m_t = t;
    m_MuF2 = MuF2;
    m_MuR2 = MuR2;
}

ConvolCoeffFunctionKinematic::ConvolCoeffFunctionKinematic(
        const std::string &className, ChannelType::Type channelType,
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &MuF2, const ElemUtils::GenericType &MuR2) :
        Kinematic(className, channelType), m_xi(
                PhysicalType<double>(xi, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_MuF2(
                PhysicalType<double>(MuF2, PhysicalUnit::GEV2)), m_MuR2(
                PhysicalType<double>(MuR2, PhysicalUnit::GEV2)) {
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
    formatter << "\txi: " << m_xi.toString() << " t: " << m_t.toString()
            << " muF2: " << m_MuF2.toString() << " muR2: " << m_MuR2.toString();

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

const PhysicalType<double>& ConvolCoeffFunctionKinematic::getMuF2() const {
    return m_MuF2;
}

const PhysicalType<double>& ConvolCoeffFunctionKinematic::getMuR2() const {
    return m_MuR2;
}

const PhysicalType<double>& ConvolCoeffFunctionKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& ConvolCoeffFunctionKinematic::getXi() const {
    return m_xi;
}

void ConvolCoeffFunctionKinematic::setXi(const PhysicalType<double>& xi) {

    m_xi.checkIfSameUnitCategoryAs(xi);
    m_xi = xi;
    updateHashSum();
}

void ConvolCoeffFunctionKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void ConvolCoeffFunctionKinematic::setMuF2(const PhysicalType<double>& MuF2) {

    m_MuF2.checkIfSameUnitCategoryAs(MuF2);
    m_MuF2 = MuF2;
    updateHashSum();
}

void ConvolCoeffFunctionKinematic::setMuR2(const PhysicalType<double>& MuR2) {

    m_MuR2.checkIfSameUnitCategoryAs(MuR2);
    m_MuR2 = MuR2;
    updateHashSum();
}

void ConvolCoeffFunctionKinematic::setXi(double xi) {

    m_xi.setValue(xi);
    updateHashSum();
}

void ConvolCoeffFunctionKinematic::setT(double t) {

    m_t.setValue(t);
    updateHashSum();
}

void ConvolCoeffFunctionKinematic::setMuF2(double MuF2) {

    m_MuF2.setValue(MuF2);
    updateHashSum();
}

void ConvolCoeffFunctionKinematic::setMuR2(double MuR2) {

    m_MuR2.setValue(MuR2);
    updateHashSum();
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
