#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string DVCSObservableKinematic::DVCS_OBSERVABLE_KINEMATIC_CLASS_NAME =
        "DVCSObservableKinematic";

const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB = "xB";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2 = "Q2";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY =
        "E";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI = "phi";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT =
        "xB_unit";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT =
        "Q2_unit";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT =
        "E_unit";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT =
        "phi_unit";

DVCSObservableKinematic::DVCSObservableKinematic() :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)) {
}

DVCSObservableKinematic::DVCSObservableKinematic(double xB, double t, double Q2,
        double E, double phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)) {
}

DVCSObservableKinematic::DVCSObservableKinematic(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_t.checkIfSameUnitCategoryAs(t);
    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_E.checkIfSameUnitCategoryAs(E);
    m_phi.checkIfSameUnitCategoryAs(phi);

    m_xB = xB;
    m_t = t;
    m_Q2 = Q2;
    m_E = E;
    m_phi = phi;
}

DVCSObservableKinematic::DVCSObservableKinematic(
        const ElemUtils::GenericType& xB, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& Q2, const ElemUtils::GenericType& E,
        const ElemUtils::GenericType& phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)) {
}

DVCSObservableKinematic::DVCSObservableKinematic(
        const DVCSObservableKinematic &other) :
        ObservableKinematic(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_E(other.m_E), m_phi(other.m_phi) {
}

DVCSObservableKinematic::~DVCSObservableKinematic() {
}

void DVCSObservableKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ObservableKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //xB
    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setXB(value, unit);
        } else {
            setXB(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB);
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
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2(value, unit);
        } else {
            setQ2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    //E
    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setE(value, unit);
        } else {
            setE(value, PhysicalUnit::GEV);
        }
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY);
    }

    //phi
    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setPhi(value, unit);
        } else {
            setPhi(value, PhysicalUnit::DEG);
        }
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
    }
}

std::string DVCSObservableKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ObservableKinematic::toString() << '\n';

    if (m_xB.isInitialized())
        formatter << "xB: " << m_xB.toString() << ' ';
    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
    if (m_Q2.isInitialized())
        formatter << "Q2: " << m_Q2.toString() << ' ';
    if (m_E.isInitialized())
        formatter << "E: " << m_E.toString() << ' ';
    if (m_phi.isInitialized())
        formatter << "phi: " << m_phi.toString() << ' ';

    return formatter.str();
}

void DVCSObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    ObservableKinematic::serialize(packet);

    packet << m_xB << m_t << m_Q2 << m_E << m_phi;
}

void DVCSObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    ObservableKinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2;
    packet >> m_E;
    packet >> m_phi;

    updateHashSum();
}

void DVCSObservableKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ObservableKinematic::serializeIntoStdVector(vec);

    m_xB.serializeIntoStdVector(vec);
    m_t.serializeIntoStdVector(vec);
    m_Q2.serializeIntoStdVector(vec);
    m_E.serializeIntoStdVector(vec);
    m_phi.serializeIntoStdVector(vec);
}

void DVCSObservableKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ObservableKinematic::unserializeFromStdVector(it, end);

    m_xB.unserializeFromStdVector(it, end);
    m_t.unserializeFromStdVector(it, end);
    m_Q2.unserializeFromStdVector(it, end);
    m_E.unserializeFromStdVector(it, end);
    m_phi.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool DVCSObservableKinematic::operator ==(
        const DVCSObservableKinematic& other) const {
    return m_xB == other.getXB() && m_t == other.getT() && m_Q2 == other.getQ2()
            && m_E == other.getE() && m_phi == other.getPhi();
}

bool DVCSObservableKinematic::operator !=(
        const DVCSObservableKinematic& other) const {
    return !((*this) == other);
}

void DVCSObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xB.toStdString()
                            << m_t.toStdString() << m_Q2.toStdString()
                            << m_E.toStdString() << m_phi.toStdString()));
}

const PhysicalType<double>& DVCSObservableKinematic::getXB() const {
    return m_xB;
}

const PhysicalType<double>& DVCSObservableKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& DVCSObservableKinematic::getQ2() const {
    return m_Q2;
}

const PhysicalType<double>& DVCSObservableKinematic::getE() const {
    return m_E;
}

const PhysicalType<double>& DVCSObservableKinematic::getPhi() const {
    return m_phi;
}

void DVCSObservableKinematic::setXB(const PhysicalType<double>& xB) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_xB = xB;
    updateHashSum();
}

void DVCSObservableKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void DVCSObservableKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void DVCSObservableKinematic::setE(const PhysicalType<double>& E) {

    m_E.checkIfSameUnitCategoryAs(E);
    m_E = E;
    updateHashSum();
}

void DVCSObservableKinematic::setPhi(const PhysicalType<double>& phi) {

    m_phi.checkIfSameUnitCategoryAs(phi);
    m_phi = phi;
    updateHashSum();
}

void DVCSObservableKinematic::setXB(double xB, PhysicalUnit::Type unit) {
    setXB(PhysicalType<double>(xB, unit));
}

void DVCSObservableKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void DVCSObservableKinematic::setQ2(double Q2, PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

void DVCSObservableKinematic::setE(double E, PhysicalUnit::Type unit) {
    setE(PhysicalType<double>(E, unit));
}

void DVCSObservableKinematic::setPhi(double phi, PhysicalUnit::Type unit) {
    setPhi(PhysicalType<double>(phi, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVCSObservableKinematic& DVCSObservableKinematic) {

    DVCSObservableKinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVCSObservableKinematic& DVCSObservableKinematic) {

    DVCSObservableKinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
