#include "../../../../../include/partons/beans/convol_coeff_function/JET/JETConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/JET/JETObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string JETConvolCoeffFunctionKinematic::JET_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME =
        "JETConvolCoeffFunctionKinematic";

const std::string JETConvolCoeffFunctionKinematic::KINEMATIC_PARAMETER_NAME_JET_CFF_TYPE =
        "jetCFF";

JETConvolCoeffFunctionKinematic::JETConvolCoeffFunctionKinematic() :
        ConvolCoeffFunctionKinematic("JETConvolCoeffFunctionKinematic", ChannelType::JET),
                m_z(PhysicalType<double>(PhysicalUnit::NONE)),
                m_qPerp2(PhysicalType<double>(PhysicalUnit::GEV2)),
                m_Q2(PhysicalType<double>(PhysicalUnit::GEV2)),
                m_jetType(JetType::UNDEFINED), m_jetCFFType(JetCFFType::UNDEFINED) {
}

JETConvolCoeffFunctionKinematic::JETConvolCoeffFunctionKinematic(double xi,
        double t, double z, double qPerp2, double Q2, double MuF2, double MuR2,
        JetType::Type jetType, JetCFFType::Type jetCFFType) :
        ConvolCoeffFunctionKinematic("JETConvolCoeffFunctionKinematic",
                ChannelType::JET, xi, t, MuF2, MuR2),
                m_z(PhysicalType<double>(z, PhysicalUnit::NONE)),
                m_qPerp2(PhysicalType<double>(qPerp2, PhysicalUnit::GEV2)),
                m_Q2(PhysicalType<double>(Q2, PhysicalUnit::GEV2)),
                m_jetType(jetType), m_jetCFFType(jetCFFType) {
}

JETConvolCoeffFunctionKinematic::JETConvolCoeffFunctionKinematic(
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &z, const PhysicalType<double> &qPerp2,
        const PhysicalType<double> &Q2, const PhysicalType<double> &MuF2,
        const PhysicalType<double> &MuR2,
        JetType::Type jetType, JetCFFType::Type jetCFFType) :
        ConvolCoeffFunctionKinematic("JETConvolCoeffFunctionKinematic",
                ChannelType::JET, xi, t, MuF2, MuR2),
                m_z(PhysicalType<double>(PhysicalUnit::NONE)),
                m_qPerp2(PhysicalType<double>(PhysicalUnit::GEV2)),
                m_Q2(PhysicalType<double>(PhysicalUnit::GEV2)),
                m_jetType(jetType), m_jetCFFType(jetCFFType) {

    m_z.checkIfSameUnitCategoryAs(z);
    m_qPerp2.checkIfSameUnitCategoryAs(qPerp2);
    m_Q2.checkIfSameUnitCategoryAs(Q2);

    m_z = z;
    m_qPerp2 = qPerp2;
    m_Q2 = Q2;
}

JETConvolCoeffFunctionKinematic::JETConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &z, const ElemUtils::GenericType &qPerp2,
        const ElemUtils::GenericType &Q2, const ElemUtils::GenericType &MuF2,
        const ElemUtils::GenericType &MuR2,
        JetType::Type jetType, JetCFFType::Type jetCFFType) :
        ConvolCoeffFunctionKinematic("JETConvolCoeffFunctionKinematic",
                ChannelType::JET, xi, t, MuF2, MuR2),
        m_z(PhysicalType<double>(z, PhysicalUnit::NONE)),
        m_qPerp2(PhysicalType<double>(qPerp2, PhysicalUnit::GEV2)),
        m_Q2(PhysicalType<double>(Q2, PhysicalUnit::GEV2)),
        m_jetType(jetType), m_jetCFFType(jetCFFType) {
}

JETConvolCoeffFunctionKinematic::JETConvolCoeffFunctionKinematic(
        const JETConvolCoeffFunctionKinematic &other) :
        ConvolCoeffFunctionKinematic(other), m_z(other.m_z),
        m_qPerp2(other.m_qPerp2), m_Q2(other.m_Q2),
        m_jetType(other.m_jetType), m_jetCFFType(other.m_jetCFFType) {
}

JETConvolCoeffFunctionKinematic::~JETConvolCoeffFunctionKinematic() {
}

void JETConvolCoeffFunctionKinematic::configure(
        const ElemUtils::Parameters &parameters) {
    //run for mother
    ConvolCoeffFunctionKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //z
    if (parameters.isAvailable(
            JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Z)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Z_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setZ(value, unit);
        } else {
            setZ(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Z);
    }

    //q2Perp
    if (parameters.isAvailable(
            JETObservableKinematic::KINEMATIC_PARAMETER_NAME_QPERP2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_QPERP2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQPerp2(value, unit);
        } else {
            setQPerp2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_QPERP2);
    }

    //Q2
    if (parameters.isAvailable(
            JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2(value, unit);
        } else {
            setQ2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    //jet type
    if (parameters.isAvailable(
            JETObservableKinematic::KINEMATIC_PARAMETER_NAME_JET_TYPE)) {
        setJetType(
                JetType::fromString(
                        parameters.getLastAvailable().getString()));
    } else {
        errorMissingParameter(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_JET_TYPE);
    }

    //jet CFF type
    if (parameters.isAvailable(
            JETConvolCoeffFunctionKinematic::KINEMATIC_PARAMETER_NAME_JET_CFF_TYPE)) {
        setJetCFFType(
                JetCFFType::fromString(
                        parameters.getLastAvailable().getString()));
            } else {
                errorMissingParameter(
                        JETConvolCoeffFunctionKinematic::KINEMATIC_PARAMETER_NAME_JET_CFF_TYPE);
            }
}

std::string JETConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionKinematic::toString();

    if (m_z.isInitialized())
        formatter << "z: " << m_z.toString() << ' ';

    if (m_qPerp2.isInitialized())
        formatter << "qPerp2: " << m_qPerp2.toString() << ' ';

    if (m_Q2.isInitialized())
        formatter << "Q2: " << m_Q2.toString() << ' ';

    formatter << "jetType: " << JetType(m_jetType).toString() << ' ';
    formatter << "jetCFFType: " << JetCFFType(m_jetCFFType).toString() << ' ';

    return formatter.str();
}

void JETConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi.toStdString()
                            << m_t.toStdString() << m_z.toStdString() << m_qPerp2.toStdString()
                            << m_Q2.toStdString() << m_MuF2.toStdString() << m_MuR2.toStdString() << m_jetType << m_jetCFFType));
}

void JETConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {

    ConvolCoeffFunctionKinematic::serialize(packet);

    packet << m_z;
    packet << m_qPerp2;
    packet << m_Q2;
    packet << JetType(m_jetType);
    packet << JetCFFType(m_jetCFFType);
}

void JETConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {

    ConvolCoeffFunctionKinematic::unserialize(packet);

    packet >> m_z;
    packet >> m_qPerp2;
    packet >> m_Q2;

    JetType jetType;
    packet >> jetType;
    m_jetType = jetType.getType();

    JetCFFType jetCFFType;
    packet >> jetCFFType;
    m_jetCFFType = jetCFFType.getType();

    updateHashSum();
}

void JETConvolCoeffFunctionKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ConvolCoeffFunctionKinematic::serializeIntoStdVector(vec);

    m_z.serializeIntoStdVector(vec);
    m_qPerp2.serializeIntoStdVector(vec);
    m_Q2.serializeIntoStdVector(vec);
    vec.push_back(static_cast<double>(m_jetType));
    vec.push_back(static_cast<double>(m_jetCFFType));
}

void JETConvolCoeffFunctionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ConvolCoeffFunctionKinematic::unserializeFromStdVector(it, end);

    m_z.unserializeFromStdVector(it, end);
    m_qPerp2.unserializeFromStdVector(it, end);
    m_Q2.unserializeFromStdVector(it, end);
    m_jetType = static_cast<JetType::Type>(*it);
    m_jetCFFType = static_cast<JetCFFType::Type>(*it);
    it++;

    updateHashSum();
}

bool JETConvolCoeffFunctionKinematic::operator ==(
        const JETConvolCoeffFunctionKinematic& other) const {
    return m_xi == other.getXi() && m_t == other.getT()
            && m_z == other.getZ() && m_qPerp2 == other.getQPerp2()
            && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2()
            && m_Q2 == other.getQ2() && m_jetType == other.getJetType()
            && m_jetCFFType == other.getJetCFFType();
}

bool JETConvolCoeffFunctionKinematic::operator !=(
        const JETConvolCoeffFunctionKinematic& other) const {
    return !((*this) == other);
}

const PhysicalType<double>& JETConvolCoeffFunctionKinematic::getZ() const {
    return m_z;
}

void JETConvolCoeffFunctionKinematic::setZ(const PhysicalType<double>& z) {

    m_z.checkIfSameUnitCategoryAs(z);
    m_z = z;
    updateHashSum();
}

 void JETConvolCoeffFunctionKinematic::setZ(double z,
            PhysicalUnit::Type unit) {
    setZ(PhysicalType<double>(z, unit));
}

const PhysicalType<double>& JETConvolCoeffFunctionKinematic::getQPerp2() const {
    return m_qPerp2;
}

void JETConvolCoeffFunctionKinematic::setQPerp2(const PhysicalType<double>& qPerp2) {

    m_qPerp2.checkIfSameUnitCategoryAs(qPerp2);
    m_qPerp2 = qPerp2;
    updateHashSum();
}

void JETConvolCoeffFunctionKinematic::setQPerp2(double qPerp2,
            PhysicalUnit::Type unit) {
    setQPerp2(PhysicalType<double>(qPerp2, unit));
}

const PhysicalType<double>& JETConvolCoeffFunctionKinematic::getQ2() const {
    return m_Q2;
}

void JETConvolCoeffFunctionKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void JETConvolCoeffFunctionKinematic::setQ2(double Q2,
        PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

JetType::Type JETConvolCoeffFunctionKinematic::getJetType() const {
    return m_jetType;
}

void JETConvolCoeffFunctionKinematic::setJetType(JetType::Type jetType) {

    m_jetType = jetType;
    updateHashSum();
}

JetCFFType::Type JETConvolCoeffFunctionKinematic::getJetCFFType() const {
    return m_jetCFFType;
}

void JETConvolCoeffFunctionKinematic::setJetCFFType(JetCFFType::Type jetCFFType) {

    m_jetCFFType = jetCFFType;
    updateHashSum();
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        JETConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        JETConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
