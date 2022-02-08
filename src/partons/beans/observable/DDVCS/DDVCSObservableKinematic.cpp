#include "../../../../../include/partons/beans/observable/DDVCS/DDVCSObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string DDVCSObservableKinematic::DDVCS_OBSERVABLE_KINEMATIC_CLASS_NAME =
        "DDVCSObservableKinematic";

const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB = "xB";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2 = "Q2";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM =
        "Q2Prim";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY =
        "E";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI = "phi";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA =
        "theta";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT =
        "xB_unit";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT =
        "Q2_unit";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM_UNIT =
        "Q2Prim_unit";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT =
        "E_unit";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT =
        "phi_unit";
const std::string DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA_UNIT =
        "theta_unit";

DDVCSObservableKinematic::DDVCSObservableKinematic() :
        ObservableKinematic("DDVCSObservableKinematic", ChannelType::DDVCS), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)), m_theta(
                PhysicalType<double>(PhysicalUnit::RAD)) {
}

DDVCSObservableKinematic::DDVCSObservableKinematic(double xB, double t,
        double Q2, double Q2Prim, double E, double phi, double theta) :
        ObservableKinematic("DDVCSObservableKinematic", ChannelType::DDVCS), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)), m_theta(
                PhysicalType<double>(theta, PhysicalUnit::RAD)) {
}

DDVCSObservableKinematic::DDVCSObservableKinematic(
        const PhysicalType<double>& xB, const PhysicalType<double>& t,
        const PhysicalType<double>& Q2, const PhysicalType<double>& Q2Prim,
        const PhysicalType<double>& E, const PhysicalType<double>& phi,
        const PhysicalType<double>& theta) :
        ObservableKinematic("DDVCSObservableKinematic", ChannelType::DDVCS), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)), m_theta(
                PhysicalType<double>(PhysicalUnit::RAD)) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_t.checkIfSameUnitCategoryAs(t);
    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);
    m_E.checkIfSameUnitCategoryAs(E);
    m_phi.checkIfSameUnitCategoryAs(phi);
    m_theta.checkIfSameUnitCategoryAs(theta);

    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;
    m_Q2Prim = Q2Prim;
    m_E = E;
    m_phi = phi;
    m_theta = theta;
}

DDVCSObservableKinematic::DDVCSObservableKinematic(
        const ElemUtils::GenericType& xB, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& Q2, const ElemUtils::GenericType& Q2Prim,
        const ElemUtils::GenericType& E, const ElemUtils::GenericType& phi,
        const ElemUtils::GenericType& theta) :
        ObservableKinematic("DDVCSObservableKinematic", ChannelType::DDVCS), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)), m_theta(
                PhysicalType<double>(theta, PhysicalUnit::RAD)) {
}

DDVCSObservableKinematic::DDVCSObservableKinematic(
        const DDVCSObservableKinematic &other) :
        ObservableKinematic(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_Q2Prim(other.m_Q2Prim), m_E(other.m_E), m_phi(
                other.m_phi), m_theta(other.m_theta) {
}

DDVCSObservableKinematic::~DDVCSObservableKinematic() {
}

void DDVCSObservableKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ObservableKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //xB
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setXB(value, unit);
        } else {
            setXB(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB);
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

    //Q2
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2(value, unit);
        } else {
            setQ2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    //Q2'
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2Prim(value, unit);
        } else {
            setQ2Prim(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM);
    }

    //E
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setE(value, unit);
        } else {
            setE(value, PhysicalUnit::GEV);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY);
    }

    //phi
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setPhi(value, unit);
        } else {
            setPhi(value, PhysicalUnit::DEG);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
    }

    //theta
    if (parameters.isAvailable(
            DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setTheta(value, unit);
        } else {
            setTheta(value, PhysicalUnit::DEG);
        }
    } else {
        errorMissingParameter(
                DDVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA);
    }
}

std::string DDVCSObservableKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ObservableKinematic::toString() << '\n';

    if (m_xB.isInitialized())
        formatter << "xB: " << m_xB.toString() << ' ';
    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
    if (m_Q2.isInitialized())
        formatter << "Q2: " << m_Q2.toString() << ' ';
    if (m_Q2Prim.isInitialized())
        formatter << "Q2': " << m_Q2Prim.toString() << ' ';
    if (m_E.isInitialized())
        formatter << "E: " << m_E.toString() << ' ';
    if (m_phi.isInitialized())
        formatter << "phi: " << m_phi.toString() << ' ';
    if (m_theta.isInitialized())
        formatter << "theta: " << m_theta.toString() << ' ';

    return formatter.str();
}

void DDVCSObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    ObservableKinematic::serialize(packet);

    packet << m_xB << m_t << m_Q2 << m_Q2Prim << m_E << m_phi << m_theta;
}

void DDVCSObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    ObservableKinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2;
    packet >> m_Q2Prim;
    packet >> m_E;
    packet >> m_phi;
    packet >> m_theta;

    updateHashSum();
}

void DDVCSObservableKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ObservableKinematic::serializeIntoStdVector(vec);

    m_xB.serializeIntoStdVector(vec);
    m_t.serializeIntoStdVector(vec);
    m_Q2.serializeIntoStdVector(vec);
    m_Q2Prim.serializeIntoStdVector(vec);
    m_E.serializeIntoStdVector(vec);
    m_phi.serializeIntoStdVector(vec);
    m_theta.serializeIntoStdVector(vec);
}

void DDVCSObservableKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ObservableKinematic::unserializeFromStdVector(it, end);

    m_xB.unserializeFromStdVector(it, end);
    m_t.unserializeFromStdVector(it, end);
    m_Q2.unserializeFromStdVector(it, end);
    m_Q2Prim.unserializeFromStdVector(it, end);
    m_E.unserializeFromStdVector(it, end);
    m_phi.unserializeFromStdVector(it, end);
    m_theta.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool DDVCSObservableKinematic::operator ==(
        const DDVCSObservableKinematic& other) const {
    return m_xB == other.getXB() && m_t == other.getT() && m_Q2 == other.getQ2()
            && m_Q2Prim == other.getQ2Prim() && m_E == other.getE()
            && m_phi == other.getPhi() && m_theta == other.getTheta();
}

bool DDVCSObservableKinematic::operator !=(
        const DDVCSObservableKinematic& other) const {
    return !((*this) == other);
}

void DDVCSObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xB.toStdString()
                            << m_t.toStdString() << m_Q2.toStdString()
                            << m_Q2Prim.toStdString() << m_E.toStdString()
                            << m_phi.toStdString() << m_theta.toStdString()));
}

const PhysicalType<double>& DDVCSObservableKinematic::getXB() const {
    return m_xB;
}

const PhysicalType<double>& DDVCSObservableKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& DDVCSObservableKinematic::getQ2() const {
    return m_Q2;
}

const PhysicalType<double>& DDVCSObservableKinematic::getQ2Prim() const {
    return m_Q2Prim;
}

const PhysicalType<double>& DDVCSObservableKinematic::getE() const {
    return m_E;
}

const PhysicalType<double>& DDVCSObservableKinematic::getPhi() const {
    return m_phi;
}

const PhysicalType<double>& DDVCSObservableKinematic::getTheta() const {
    return m_theta;
}

void DDVCSObservableKinematic::setXB(const PhysicalType<double>& xB) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_xB = xB;
    updateHashSum();
}

void DDVCSObservableKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void DDVCSObservableKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void DDVCSObservableKinematic::setQ2Prim(const PhysicalType<double>& Q2Prim) {

    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);
    m_Q2Prim = Q2Prim;
    updateHashSum();
}

void DDVCSObservableKinematic::setE(const PhysicalType<double>& E) {

    m_E.checkIfSameUnitCategoryAs(E);
    m_E = E;
    updateHashSum();
}

void DDVCSObservableKinematic::setPhi(const PhysicalType<double>& phi) {

    m_phi.checkIfSameUnitCategoryAs(phi);
    m_phi = phi;
    updateHashSum();
}

void DDVCSObservableKinematic::setTheta(const PhysicalType<double>& theta) {

    m_theta.checkIfSameUnitCategoryAs(theta);
    m_theta = theta;
    updateHashSum();
}

void DDVCSObservableKinematic::setXB(double xB, PhysicalUnit::Type unit) {
    setXB(PhysicalType<double>(xB, unit));
}

void DDVCSObservableKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void DDVCSObservableKinematic::setQ2(double Q2, PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

void DDVCSObservableKinematic::setQ2Prim(double Q2Prim,
        PhysicalUnit::Type unit) {
    setQ2Prim(PhysicalType<double>(Q2Prim, unit));
}

void DDVCSObservableKinematic::setE(double E, PhysicalUnit::Type unit) {
    setE(PhysicalType<double>(E, unit));
}

void DDVCSObservableKinematic::setPhi(double phi, PhysicalUnit::Type unit) {
    setPhi(PhysicalType<double>(phi, unit));
}

void DDVCSObservableKinematic::setTheta(double theta, PhysicalUnit::Type unit) {
    setTheta(PhysicalType<double>(theta, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DDVCSObservableKinematic& DDVCSObservableKinematic) {

    DDVCSObservableKinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DDVCSObservableKinematic& DDVCSObservableKinematic) {

    DDVCSObservableKinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
