#include "../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/beans/gpd/GPDKinematic.h"

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

void ConvolCoeffFunctionKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    Kinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //xi
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_XI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setXi(value, unit);
        } else {
            setXi(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_XI);
    }

    //t
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_T)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_T_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setT(value, unit);
        } else {
            setT(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_T);
    }

    //muF2
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuF2(value, unit);
        } else {
            setMuF2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2);
    }

    //muR2
    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMuR2(value, unit);
        } else {
            setMuR2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2);
    }
}

std::string ConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Kinematic::toString() << '\n';

    if (m_xi.isInitialized())
        formatter << "xi: " << m_xi.toString() << ' ';
    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
    if (m_MuF2.isInitialized())
        formatter << "muF2: " << m_MuF2.toString() << ' ';
    if (m_MuR2.isInitialized())
        formatter << "muR2: " << m_MuR2.toString() << ' ';

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

void ConvolCoeffFunctionKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    Kinematic::serializeIntoStdVector(vec);

    m_xi.serializeIntoStdVector(vec);
    m_t.serializeIntoStdVector(vec);
    m_MuF2.serializeIntoStdVector(vec);
    m_MuR2.serializeIntoStdVector(vec);
}

void ConvolCoeffFunctionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    Kinematic::unserializeFromStdVector(it, end);

    m_xi.unserializeFromStdVector(it, end);
    m_t.unserializeFromStdVector(it, end);
    m_MuF2.unserializeFromStdVector(it, end);
    m_MuR2.unserializeFromStdVector(it, end);
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

void ConvolCoeffFunctionKinematic::setXi(double xi, PhysicalUnit::Type unit) {
    setXi(PhysicalType<double>(xi, unit));
}

void ConvolCoeffFunctionKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void ConvolCoeffFunctionKinematic::setMuF2(double MuF2,
        PhysicalUnit::Type unit) {
    setMuF2(PhysicalType<double>(MuF2, unit));
}

void ConvolCoeffFunctionKinematic::setMuR2(double MuR2,
        PhysicalUnit::Type unit) {
    setMuR2(PhysicalType<double>(MuR2, unit));
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
