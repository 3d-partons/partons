#include "../../../../../include/partons/beans/observable/TCS/TCSObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string TCSObservableKinematic::TCS_OBSERVABLE_KNEMATIC_CLASS_NAME =
        "TCSObservableKinematic";

const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB = "xB";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM =
        "Q2Prim";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI = "phi";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY =
        "E";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA =
        "theta";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT =
        "xB_unit";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM_UNIT =
        "Q2Prim_unit";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT =
        "phi_unit";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT =
        "E_unit";
const std::string TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA_UNIT =
        "theta_unit";

TCSObservableKinematic::TCSObservableKinematic() :
        ObservableKinematic("TCSObservableKinematic", ChannelType::TCS), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::DEG)), m_theta(
                PhysicalType<double>(PhysicalUnit::DEG)) {
}

TCSObservableKinematic::TCSObservableKinematic(
        const ElemUtils::Parameters &parameters) :
        ObservableKinematic("TCSObservableKinematic", ChannelType::TCS), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::DEG)), m_theta(
                PhysicalType<double>(PhysicalUnit::DEG)) {

    double value;
    PhysicalUnit::Type unit;

    //xB
    if (parameters.isAvailable(
            TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setXB(value, unit);
        } else {
            setXB(value);
        }
    } else {
        errorMissingParameter(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB);
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
            setT(value);
        }
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_T);
    }

    //Q2'
    if (parameters.isAvailable(
            TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2Prim(value, unit);
        } else {
            setQ2Prim(value);
        }
    } else {
        errorMissingParameter(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2PRIM);
    }

    //E
    if (parameters.isAvailable(
            TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setE(value, unit);
        } else {
            setE(value);
        }
    } else {
        errorMissingParameter(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY);
    }

    //phi
    if (parameters.isAvailable(
            TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setPhi(value, unit);
        } else {
            setPhi(value);
        }
    } else {
        errorMissingParameter(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
    }

    //theta
    if (parameters.isAvailable(
            TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setTheta(value, unit);
        } else {
            setTheta(value);
        }
    } else {
        errorMissingParameter(
                TCSObservableKinematic::KINEMATIC_PARAMETER_NAME_THETA);
    }
}

TCSObservableKinematic::TCSObservableKinematic(double xB, double t,
        double Q2Prim, double E, double phi, double theta) :
        ObservableKinematic("TCSObservableKinematic", ChannelType::TCS), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::DEG)), m_theta(
                PhysicalType<double>(theta, PhysicalUnit::DEG)) {
}

TCSObservableKinematic::TCSObservableKinematic(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2Prim,
        const PhysicalType<double>& E, const PhysicalType<double>& phi,
        const PhysicalType<double>& theta) :
        ObservableKinematic("TCSObservableKinematic", ChannelType::TCS), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::DEG)), m_theta(
                PhysicalType<double>(PhysicalUnit::DEG)) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_t.checkIfSameUnitCategoryAs(t);
    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);
    m_E.checkIfSameUnitCategoryAs(E);
    m_phi.checkIfSameUnitCategoryAs(phi);
    m_theta.checkIfSameUnitCategoryAs(theta);

    m_xB = xB;
    m_t = t;
    m_Q2Prim = Q2Prim;
    m_E = E;
    m_phi = phi;
    m_theta = theta;
}

TCSObservableKinematic::TCSObservableKinematic(const ElemUtils::GenericType& xB,
        const ElemUtils::GenericType& t, const ElemUtils::GenericType& Q2Prim,
        const ElemUtils::GenericType& E, const ElemUtils::GenericType& phi,
        const ElemUtils::GenericType& theta) :
        ObservableKinematic("TCSObservableKinematic", ChannelType::TCS), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2Prim(
                PhysicalType<double>(Q2Prim, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::DEG)), m_theta(
                PhysicalType<double>(theta, PhysicalUnit::DEG)) {
}

TCSObservableKinematic::TCSObservableKinematic(
        const TCSObservableKinematic &other) :
        ObservableKinematic(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2Prim(
                other.m_Q2Prim), m_E(other.m_E), m_phi(other.m_phi), m_theta(
                other.m_theta) {
}

TCSObservableKinematic::~TCSObservableKinematic() {
}

std::string TCSObservableKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << Kinematic::toString() << '\n';

    if (m_xB.isInitialized())
        formatter << "xB: " << m_xB.toString() << ' ';
    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
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

void TCSObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_xB << m_t << m_Q2Prim << m_E << m_phi << m_theta;
}

void TCSObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2Prim;
    packet >> m_E;
    packet >> m_phi;
    packet >> m_theta;
}

void TCSObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xB.getValue() << m_t.getValue()
                            << m_Q2Prim.getValue() << m_E.getValue()
                            << m_phi.getValue() << m_theta.getValue()));
}

const PhysicalType<double>& TCSObservableKinematic::getXB() const {
    return m_xB;
}

const PhysicalType<double>& TCSObservableKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& TCSObservableKinematic::getQ2Prim() const {
    return m_Q2Prim;
}

const PhysicalType<double>& TCSObservableKinematic::getE() const {
    return m_E;
}

const PhysicalType<double>& TCSObservableKinematic::getPhi() const {
    return m_phi;
}

const PhysicalType<double>& TCSObservableKinematic::getTheta() const {
    return m_theta;
}

void TCSObservableKinematic::setXB(const PhysicalType<double>& xB) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_xB = xB;
    updateHashSum();
}

void TCSObservableKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void TCSObservableKinematic::setQ2Prim(const PhysicalType<double>& Q2Prim) {

    m_Q2Prim.checkIfSameUnitCategoryAs(Q2Prim);
    m_Q2Prim = Q2Prim;
    updateHashSum();
}

void TCSObservableKinematic::setE(const PhysicalType<double>& E) {

    m_E.checkIfSameUnitCategoryAs(E);
    m_E = E;
    updateHashSum();
}

void TCSObservableKinematic::setPhi(const PhysicalType<double>& phi) {

    m_phi.checkIfSameUnitCategoryAs(phi);
    m_phi = phi;
    updateHashSum();
}

void TCSObservableKinematic::setTheta(const PhysicalType<double>& theta) {

    m_theta.checkIfSameUnitCategoryAs(theta);
    m_theta = theta;
    updateHashSum();
}

void TCSObservableKinematic::setXB(double xB, PhysicalUnit::Type unit) {
    setXB(PhysicalType<double>(xB, unit));
}

void TCSObservableKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void TCSObservableKinematic::setQ2Prim(double Q2Prim, PhysicalUnit::Type unit) {
    setQ2Prim(PhysicalType<double>(Q2Prim, unit));
}

void TCSObservableKinematic::setE(double E, PhysicalUnit::Type unit) {
    setE(PhysicalType<double>(E, unit));
}

void TCSObservableKinematic::setPhi(double phi, PhysicalUnit::Type unit) {
    setPhi(PhysicalType<double>(phi, unit));
}

void TCSObservableKinematic::setTheta(double theta, PhysicalUnit::Type unit) {
    setTheta(PhysicalType<double>(theta, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        TCSObservableKinematic& TCSObservableKinematic) {

    TCSObservableKinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        TCSObservableKinematic& TCSObservableKinematic) {

    TCSObservableKinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
