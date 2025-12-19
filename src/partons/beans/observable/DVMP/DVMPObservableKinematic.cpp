#include "../../../../../include/partons/beans/observable/DVMP/DVMPObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <iterator>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string DVMPObservableKinematic::DVMP_OBSERVABLE_KNEMATIC_CLASS_NAME =
        "DVMPObservableKinematic";

const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_XB = "xB";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2 = "Q2";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY =
        "E";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI = "phi";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_MESON_TYPE =
        "meson";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT =
        "xB_unit";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT =
        "Q2_unit";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT =
        "E_unit";
const std::string DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT =
        "phi_unit";

DVMPObservableKinematic::DVMPObservableKinematic() :
        ObservableKinematic("DVMPObservableKinematic", ChannelType::DVMP), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)), m_mesonType(
                MesonType::UNDEFINED) {
}

DVMPObservableKinematic::DVMPObservableKinematic(double xB, double t, double Q2,
        double E, double phi, MesonType::Type mesonType) :
        ObservableKinematic("DVMPObservableKinematic", ChannelType::DVMP), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)), m_mesonType(
                mesonType) {
}

DVMPObservableKinematic::DVMPObservableKinematic(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi,
        MesonType::Type mesonType) :
        ObservableKinematic("DVMPObservableKinematic", ChannelType::DVMP), m_xB(
                PhysicalType<double>(PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)), m_mesonType(mesonType) {

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

DVMPObservableKinematic::DVMPObservableKinematic(
        const ElemUtils::GenericType& xB, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& Q2, const ElemUtils::GenericType& E,
        const ElemUtils::GenericType& phi, MesonType::Type mesonType) :
        ObservableKinematic("DVMPObservableKinematic", ChannelType::DVMP), m_xB(
                PhysicalType<double>(xB, PhysicalUnit::NONE)), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_Q2(
                PhysicalType<double>(Q2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)), m_mesonType(
                mesonType) {
}

DVMPObservableKinematic::DVMPObservableKinematic(
        const DVMPObservableKinematic &other) :
        ObservableKinematic(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_E(other.m_E), m_phi(other.m_phi), m_mesonType(
                other.m_mesonType) {
}

DVMPObservableKinematic::~DVMPObservableKinematic() {
}

void DVMPObservableKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ObservableKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

    //xB
    if (parameters.isAvailable(
            DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_XB)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setXB(value, unit);
        } else {
            setXB(value, PhysicalUnit::NONE);
        }
    } else {
        errorMissingParameter(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_XB);
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
            DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setQ2(value, unit);
        } else {
            setQ2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    //E
    if (parameters.isAvailable(
            DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setE(value, unit);
        } else {
            setE(value, PhysicalUnit::GEV);
        }
    } else {
        errorMissingParameter(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY);
    }

    //phi
    if (parameters.isAvailable(
            DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setPhi(value, unit);
        } else {
            setPhi(value, PhysicalUnit::DEG);
        }
    } else {
        errorMissingParameter(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
    }

    //meson type
    if (parameters.isAvailable(
            DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_MESON_TYPE)) {
        setMesonType(
                MesonType::fromString(
                        parameters.getLastAvailable().getString()));
    } else {
        errorMissingParameter(
                DVMPObservableKinematic::KINEMATIC_PARAMETER_NAME_MESON_TYPE);
    }
}

std::string DVMPObservableKinematic::toString() const {

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

    formatter << "meson type: " << MesonType(m_mesonType).toString() << ' ';

    return formatter.str();
}

void DVMPObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    ObservableKinematic::serialize(packet);

    packet << m_xB << m_t << m_Q2 << m_E << m_phi << MesonType(m_mesonType);
}

void DVMPObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    ObservableKinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2;
    packet >> m_E;
    packet >> m_phi;

    MesonType mesonType;
    packet >> mesonType;
    m_mesonType = mesonType.getType();

    updateHashSum();
}

void DVMPObservableKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ObservableKinematic::serializeIntoStdVector(vec);

    m_xB.serializeIntoStdVector(vec);
    m_t.serializeIntoStdVector(vec);
    m_Q2.serializeIntoStdVector(vec);
    m_E.serializeIntoStdVector(vec);
    m_phi.serializeIntoStdVector(vec);
    vec.push_back(static_cast<double>(m_mesonType));
}

void DVMPObservableKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ObservableKinematic::unserializeFromStdVector(it, end);

    m_xB.unserializeFromStdVector(it, end);
    m_t.unserializeFromStdVector(it, end);
    m_Q2.unserializeFromStdVector(it, end);
    m_E.unserializeFromStdVector(it, end);
    m_phi.unserializeFromStdVector(it, end);
    m_mesonType = static_cast<MesonType::Type>(*it);
    it++;

    updateHashSum();
}

bool DVMPObservableKinematic::operator ==(
        const DVMPObservableKinematic& other) const {
    return m_xB == other.getXB() && m_t == other.getT() && m_Q2 == other.getQ2()
            && m_E == other.getE() && m_phi == other.getPhi()
            && m_mesonType == other.getMesonType();
}

bool DVMPObservableKinematic::operator !=(
        const DVMPObservableKinematic& other) const {
    return !((*this) == other);
}

void DVMPObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xB.toStdString()
                            << m_t.toStdString() << m_Q2.toStdString()
                            << m_E.toStdString() << m_phi.toStdString()
                            << MesonType(m_mesonType).toString()));
}

const PhysicalType<double>& DVMPObservableKinematic::getXB() const {
    return m_xB;
}

const PhysicalType<double>& DVMPObservableKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& DVMPObservableKinematic::getQ2() const {
    return m_Q2;
}

const PhysicalType<double>& DVMPObservableKinematic::getE() const {
    return m_E;
}

const PhysicalType<double>& DVMPObservableKinematic::getPhi() const {
    return m_phi;
}

MesonType::Type DVMPObservableKinematic::getMesonType() const {
    return m_mesonType;
}

void DVMPObservableKinematic::setXB(const PhysicalType<double>& xB) {

    m_xB.checkIfSameUnitCategoryAs(xB);
    m_xB = xB;
    updateHashSum();
}

void DVMPObservableKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void DVMPObservableKinematic::setQ2(const PhysicalType<double>& Q2) {

    m_Q2.checkIfSameUnitCategoryAs(Q2);
    m_Q2 = Q2;
    updateHashSum();
}

void DVMPObservableKinematic::setE(const PhysicalType<double>& E) {

    m_E.checkIfSameUnitCategoryAs(E);
    m_E = E;
    updateHashSum();
}

void DVMPObservableKinematic::setPhi(const PhysicalType<double>& phi) {

    m_phi.checkIfSameUnitCategoryAs(phi);
    m_phi = phi;
    updateHashSum();
}

void DVMPObservableKinematic::setXB(double xB, PhysicalUnit::Type unit) {
    setXB(PhysicalType<double>(xB, unit));
}

void DVMPObservableKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void DVMPObservableKinematic::setQ2(double Q2, PhysicalUnit::Type unit) {
    setQ2(PhysicalType<double>(Q2, unit));
}

void DVMPObservableKinematic::setE(double E, PhysicalUnit::Type unit) {
    setE(PhysicalType<double>(E, unit));
}

void DVMPObservableKinematic::setPhi(double phi, PhysicalUnit::Type unit) {
    setPhi(PhysicalType<double>(phi, unit));
}

void DVMPObservableKinematic::setMesonType(MesonType::Type mesonType) {

    m_mesonType = mesonType;
    updateHashSum();
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        DVMPObservableKinematic& DVMPObservableKinematic) {

    DVMPObservableKinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        DVMPObservableKinematic& DVMPObservableKinematic) {

    DVMPObservableKinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
