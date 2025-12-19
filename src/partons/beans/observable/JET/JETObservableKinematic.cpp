#include "../../../../../include/partons/beans/observable/JET/JETObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string JETObservableKinematic::JET_OBSERVABLE_KINEMATIC_CLASS_NAME =
        "JETObservableKinematic";

const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_XB = "xB";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Z = "z";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_QPERP2 = "qPerp2";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2 = "Q2";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY =
        "E";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI = "phi";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT =
        "xB_unit";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Z_UNIT =
        "z_unit";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_QPERP2_UNIT =
        "qPerp2_unit";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT =
        "Q2_unit";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT =
        "E_unit";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT =
        "phi_unit";
const std::string JETObservableKinematic::KINEMATIC_PARAMETER_NAME_JET_TYPE =
        "jet";

JETObservableKinematic::JETObservableKinematic() :
        ObservableKinematic("JETObservableKinematic", ChannelType::JET), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_z(
                PhysicalType<double>(PhysicalUnit::NONE)), m_qPerp2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)), m_jetType(JetType::UNDEFINED) {
}

JETObservableKinematic::JETObservableKinematic(double xB, double t, double z, double q2Perp, double Q2,
        double E, double phi, JetType::Type jetType) :
        ObservableKinematic("JETObservableKinematic", ChannelType::JET), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_z(
                PhysicalType<double>(z, PhysicalUnit::NONE)), m_qPerp2(
                PhysicalType<double>(q2Perp, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)), m_jetType(jetType) {
}

JETObservableKinematic::JETObservableKinematic(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& z,
        const PhysicalType<double>& q2Perp, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi, JetType::Type jetType) :
        ObservableKinematic("JETObservableKinematic", ChannelType::JET), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_z(
                PhysicalType<double>(PhysicalUnit::NONE)), m_qPerp2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)), m_jetType(jetType) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_t.checkIfSameUnitCategoryAs(t);
    m_z.checkIfSameUnitCategoryAs(z);
    m_qPerp2.checkIfSameUnitCategoryAs(q2Perp);
    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_E.checkIfSameUnitCategoryAs(E);
    m_phi.checkIfSameUnitCategoryAs(phi);

    m_xB = xB;
    m_t = t;
    m_z = z;
    m_qPerp2 = q2Perp;
    m_Q2 = Q2;
    m_E = E;
    m_phi = phi;
}

JETObservableKinematic::JETObservableKinematic(
        const ElemUtils::GenericType& xB, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& z, const ElemUtils::GenericType& qPerp2,
        const ElemUtils::GenericType& Q2, const ElemUtils::GenericType& E,
        const ElemUtils::GenericType& phi, JetType::Type jetType) :
        ObservableKinematic("JETObservableKinematic", ChannelType::JET), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_z(
                PhysicalType<double>(z, PhysicalUnit::NONE)), m_qPerp2(
                PhysicalType<double>(qPerp2, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)), m_jetType(jetType) {
}

JETObservableKinematic::JETObservableKinematic(
        const JETObservableKinematic &other) :
        ObservableKinematic(other), m_xB(other.m_xB), m_t(other.m_t),
        m_z(other.m_z), m_qPerp2(other.m_qPerp2),
        m_Q2(other.m_Q2), m_E(other.m_E), m_phi(other.m_phi), m_jetType(other.m_jetType) {
}

JETObservableKinematic::~JETObservableKinematic() {
}

void JETObservableKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ObservableKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //xB
    if (parameters.isAvailable(
            JETObservableKinematic::KINEMATIC_PARAMETER_NAME_XB)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setXB(value, unit);
        } else {
            setXB(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_XB);
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

    //z
    if (parameters.isAvailable(JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Z)) {

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
        errorMissingParameter(JETObservableKinematic::KINEMATIC_PARAMETER_NAME_Z);
    }

    //qPerp2
    if (parameters.isAvailable(JETObservableKinematic::KINEMATIC_PARAMETER_NAME_QPERP2)) {

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
        errorMissingParameter(JETObservableKinematic::KINEMATIC_PARAMETER_NAME_QPERP2);
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

    //E
    if (parameters.isAvailable(
            JETObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setE(value, unit);
        } else {
            setE(value, PhysicalUnit::GEV);
        }
    } else {
        errorMissingParameter(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY);
    }

    //phi
    if (parameters.isAvailable(
            JETObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setPhi(value, unit);
        } else {
            setPhi(value, PhysicalUnit::DEG);
        }
    } else {
        errorMissingParameter(
                JETObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
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
}

std::string JETObservableKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ObservableKinematic::toString() << '\n';

    if (m_xB.isInitialized())
        formatter << "xB: " << m_xB.toString() << ' ';
    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
    if (m_z.isInitialized())
        formatter << "z: " << m_z.toString() << ' ';
    if (m_qPerp2.isInitialized())
        formatter << "qPerp2: " << m_qPerp2.toString() << ' ';
    if (m_Q2.isInitialized())
        formatter << "Q2: " << m_Q2.toString() << ' ';
    if (m_E.isInitialized())
        formatter << "E: " << m_E.toString() << ' ';
    if (m_phi.isInitialized())
        formatter << "phi: " << m_phi.toString() << ' ';
    formatter << "jetType: " << JetType(m_jetType).toString() << ' ';

    return formatter.str();
}

void JETObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    ObservableKinematic::serialize(packet);

    packet << m_xB << m_t << m_z << m_qPerp2 << m_Q2 << m_E << m_phi<< JetType(m_jetType);;
}

void JETObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    ObservableKinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_z;
    packet >> m_qPerp2;
    packet >> m_Q2;
    packet >> m_E;
    packet >> m_phi;

    JetType jetType;
    packet >> jetType;
    m_jetType = jetType.getType();

    updateHashSum();
}

void JETObservableKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ObservableKinematic::serializeIntoStdVector(vec);

    m_xB.serializeIntoStdVector(vec);
    m_t.serializeIntoStdVector(vec);
    m_z.serializeIntoStdVector(vec);
    m_qPerp2.serializeIntoStdVector(vec);
    m_Q2.serializeIntoStdVector(vec);
    m_E.serializeIntoStdVector(vec);
    m_phi.serializeIntoStdVector(vec);
    vec.push_back(static_cast<double>(m_jetType));
}

void JETObservableKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ObservableKinematic::unserializeFromStdVector(it, end);

    m_xB.unserializeFromStdVector(it, end);
    m_t.unserializeFromStdVector(it, end);
    m_z.unserializeFromStdVector(it, end);
    m_qPerp2.unserializeFromStdVector(it, end);
    m_Q2.unserializeFromStdVector(it, end);
    m_E.unserializeFromStdVector(it, end);
    m_phi.unserializeFromStdVector(it, end);
    m_jetType = static_cast<JetType::Type>(*it);
    it++;

    updateHashSum();
}

bool JETObservableKinematic::operator ==(
        const JETObservableKinematic& other) const {
    return m_xB == other.getXB() && m_t == other.getT() && m_z == other.getZ()
            && m_qPerp2 == other.getQPerp2() && m_Q2 == other.getQ2()
            && m_E == other.getE() && m_phi == other.getPhi() && m_jetType == other.getJetType();
}

bool JETObservableKinematic::operator !=(
        const JETObservableKinematic& other) const {
    return !((*this) == other);
}

void JETObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xB.toStdString()
                            << m_t.toStdString() << m_z.toStdString() << m_qPerp2.toStdString()
                            << m_Q2.toStdString() << m_E.toStdString() << m_phi.toStdString() << m_jetType));
}

const PhysicalType<double>& JETObservableKinematic::getXB() const {
    return m_xB;
}

const PhysicalType<double>& JETObservableKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& JETObservableKinematic::getZ() const {
    return m_z;
}

const PhysicalType<double>& JETObservableKinematic::getQPerp2() const {
    return m_qPerp2;
}

const PhysicalType<double>& JETObservableKinematic::getQ2() const {
    return m_Q2;
}

const PhysicalType<double>& JETObservableKinematic::getE() const {
    return m_E;
}

const PhysicalType<double>& JETObservableKinematic::getPhi() const {
    return m_phi;
}

JetType::Type JETObservableKinematic::getJetType() const {
    return m_jetType;
}

void JETObservableKinematic::setXB(const PhysicalType<double>& xB) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_xB = xB;
    updateHashSum();
}

void JETObservableKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void JETObservableKinematic::setZ(const PhysicalType<double>& z) {

    m_z.checkIfSameUnitCategoryAs(z);
    m_z = z;
    updateHashSum();
}

void JETObservableKinematic::setQPerp2(const PhysicalType<double>& qPerp2) {

    m_qPerp2.checkIfSameUnitCategoryAs(qPerp2);
    m_qPerp2 = qPerp2;
    updateHashSum();
}

void JETObservableKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void JETObservableKinematic::setE(const PhysicalType<double>& E) {

    m_E.checkIfSameUnitCategoryAs(E);
    m_E = E;
    updateHashSum();
}

void JETObservableKinematic::setPhi(const PhysicalType<double>& phi) {

    m_phi.checkIfSameUnitCategoryAs(phi);
    m_phi = phi;
    updateHashSum();
}

void JETObservableKinematic::setJetType(JetType::Type jetType) {

    m_jetType = jetType;
    updateHashSum();
}

void JETObservableKinematic::setXB(double xB, PhysicalUnit::Type unit) {
    setXB(PhysicalType<double>(xB, unit));
}

void JETObservableKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void JETObservableKinematic::setZ(double z, PhysicalUnit::Type unit) {
    setZ(PhysicalType<double>(z, unit));
}

void JETObservableKinematic::setQPerp2(double qPerp2, PhysicalUnit::Type unit) {
    setQPerp2(PhysicalType<double>(qPerp2, unit));
}

void JETObservableKinematic::setQ2(double Q2, PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

void JETObservableKinematic::setE(double E, PhysicalUnit::Type unit) {
    setE(PhysicalType<double>(E, unit));
}

void JETObservableKinematic::setPhi(double phi, PhysicalUnit::Type unit) {
    setPhi(PhysicalType<double>(phi, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        JETObservableKinematic& JETObservableKinematic) {

    JETObservableKinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        JETObservableKinematic& JETObservableKinematic) {

    JETObservableKinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
