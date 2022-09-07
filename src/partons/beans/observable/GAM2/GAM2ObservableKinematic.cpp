#include "../../../../../include/partons/beans/observable/GAM2/GAM2ObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string GAM2ObservableKinematic::GAM2_OBSERVABLE_KINEMATIC_CLASS_NAME =
        "GAM2ObservableKinematic";

const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_UPRIM =
        "uPrim";
const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_MGG2 =
        "Mgg2";
const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY =
        "E";
const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI = "phi";

const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_UPRIM_UNIT =
        "uPrim_unit";
const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_MGG2_UNIT =
        "Mgg2_unit";
const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT =
        "E_unit";
const std::string GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT =
        "phi_unit";

GAM2ObservableKinematic::GAM2ObservableKinematic() :
        ObservableKinematic("GAM2ObservableKinematic", ChannelType::GAM2), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_uPrim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)) {
}

GAM2ObservableKinematic::GAM2ObservableKinematic(double t, double uPrim,
        double Mgg2, double E, double phi) :
        ObservableKinematic("GAM2ObservableKinematic", ChannelType::GAM2), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_uPrim(
                PhysicalType<double>(uPrim, PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(Mgg2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)) {
}

GAM2ObservableKinematic::GAM2ObservableKinematic(const PhysicalType<double>& t,
        const PhysicalType<double>& uPrim, const PhysicalType<double>& Mgg2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi) :
        ObservableKinematic("GAM2ObservableKinematic", ChannelType::GAM2), m_t(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_uPrim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_uPrim.checkIfSameUnitCategoryAs(uPrim);
    m_Mgg2.checkIfSameUnitCategoryAs(Mgg2);
    m_E.checkIfSameUnitCategoryAs(E);
    m_phi.checkIfSameUnitCategoryAs(phi);

    m_t = t;
    m_uPrim = uPrim;
    m_Mgg2 = Mgg2;
    m_E = E;
    m_phi = phi;
}

GAM2ObservableKinematic::GAM2ObservableKinematic(
        const ElemUtils::GenericType& t, const ElemUtils::GenericType& uPrim,
        const ElemUtils::GenericType& Mgg2, const ElemUtils::GenericType& E,
        const ElemUtils::GenericType& phi) :
        ObservableKinematic("GAM2ObservableKinematic", ChannelType::GAM2), m_t(
                PhysicalType<double>(t, PhysicalUnit::GEV2)), m_uPrim(
                PhysicalType<double>(uPrim, PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(Mgg2, PhysicalUnit::GEV2)), m_E(
                PhysicalType<double>(E, PhysicalUnit::GEV)), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)) {
}

GAM2ObservableKinematic::GAM2ObservableKinematic(
        const GAM2ObservableKinematic &other) :
        ObservableKinematic(other), m_t(other.m_t), m_uPrim(other.m_uPrim), m_Mgg2(
                other.m_Mgg2), m_E(other.m_E), m_phi(other.m_phi) {
}

GAM2ObservableKinematic::~GAM2ObservableKinematic() {
}

void GAM2ObservableKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ObservableKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

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

    //u'
    if (parameters.isAvailable(
            GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_UPRIM)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_UPRIM_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setUPrim(value, unit);
        } else {
            setUPrim(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_UPRIM);
    }

    //Mgg2
    if (parameters.isAvailable(
            GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_MGG2)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_MGG2_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setMgg2(value, unit);
        } else {
            setMgg2(value, PhysicalUnit::GEV2);
        }
    } else {
        errorMissingParameter(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_MGG2);
    }

    //E
    if (parameters.isAvailable(
            GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setE(value, unit);
        } else {
            setE(value, PhysicalUnit::GEV);
        }
    } else {
        errorMissingParameter(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY);
    }

    //phi
    if (parameters.isAvailable(
            GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI)) {

        value = parameters.getLastAvailable().toDouble();

        if (parameters.isAvailable(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT)) {

            unit =
                    PhysicalUnit(parameters.getLastAvailable().getString()).getType();
            setPhi(value, unit);
        } else {
            setPhi(value, PhysicalUnit::DEG);
        }
    } else {
        errorMissingParameter(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
    }
}

std::string GAM2ObservableKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ObservableKinematic::toString() << '\n';

    if (m_t.isInitialized())
        formatter << "t: " << m_t.toString() << ' ';
    if (m_uPrim.isInitialized())
        formatter << "u': " << m_uPrim.toString() << ' ';
    if (m_Mgg2.isInitialized())
        formatter << "Mgg2: " << m_Mgg2.toString() << ' ';
    if (m_E.isInitialized())
        formatter << "E: " << m_E.toString() << ' ';
    if (m_phi.isInitialized())
        formatter << "phi: " << m_phi.toString() << ' ';

    return formatter.str();
}

void GAM2ObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    ObservableKinematic::serialize(packet);

    packet << m_t << m_uPrim << m_Mgg2 << m_E << m_phi;
}

void GAM2ObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    ObservableKinematic::unserialize(packet);

    packet >> m_t;
    packet >> m_uPrim;
    packet >> m_Mgg2;
    packet >> m_E;
    packet >> m_phi;

    updateHashSum();
}

void GAM2ObservableKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ObservableKinematic::serializeIntoStdVector(vec);

    m_t.serializeIntoStdVector(vec);
    m_uPrim.serializeIntoStdVector(vec);
    m_Mgg2.serializeIntoStdVector(vec);
    m_E.serializeIntoStdVector(vec);
    m_phi.serializeIntoStdVector(vec);
}

void GAM2ObservableKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ObservableKinematic::unserializeFromStdVector(it, end);

    m_t.unserializeFromStdVector(it, end);
    m_uPrim.unserializeFromStdVector(it, end);
    m_Mgg2.unserializeFromStdVector(it, end);
    m_E.unserializeFromStdVector(it, end);
    m_phi.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool GAM2ObservableKinematic::operator ==(
        const GAM2ObservableKinematic& other) const {
    return m_t == other.getT() && m_uPrim == other.getUPrim()
            && m_Mgg2 == other.getMgg2() && m_E == other.getE()
            && m_phi == other.getPhi();
}

bool GAM2ObservableKinematic::operator !=(
        const GAM2ObservableKinematic& other) const {
    return !((*this) == other);
}

void GAM2ObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_t.toStdString()
                            << m_uPrim.toStdString() << m_Mgg2.toStdString()
                            << m_E.toStdString() << m_phi.toStdString()));
}

const PhysicalType<double>& GAM2ObservableKinematic::getT() const {
    return m_t;
}

const PhysicalType<double>& GAM2ObservableKinematic::getUPrim() const {
    return m_uPrim;
}

const PhysicalType<double>& GAM2ObservableKinematic::getMgg2() const {
    return m_Mgg2;
}

const PhysicalType<double>& GAM2ObservableKinematic::getE() const {
    return m_E;
}

const PhysicalType<double>& GAM2ObservableKinematic::getPhi() const {
    return m_phi;
}

void GAM2ObservableKinematic::setT(const PhysicalType<double>& t) {

    m_t.checkIfSameUnitCategoryAs(t);
    m_t = t;
    updateHashSum();
}

void GAM2ObservableKinematic::setUPrim(const PhysicalType<double>& uPrim) {

    m_uPrim.checkIfSameUnitCategoryAs(uPrim);
    m_uPrim = uPrim;
    updateHashSum();
}

void GAM2ObservableKinematic::setMgg2(const PhysicalType<double>& Mgg2) {

    m_Mgg2.checkIfSameUnitCategoryAs(Mgg2);
    m_Mgg2 = Mgg2;
    updateHashSum();
}

void GAM2ObservableKinematic::setE(const PhysicalType<double>& E) {

    m_E.checkIfSameUnitCategoryAs(E);
    m_E = E;
    updateHashSum();
}

void GAM2ObservableKinematic::setPhi(const PhysicalType<double>& phi) {

    m_phi.checkIfSameUnitCategoryAs(phi);
    m_phi = phi;
    updateHashSum();
}

void GAM2ObservableKinematic::setT(double t, PhysicalUnit::Type unit) {
    setT(PhysicalType<double>(t, unit));
}

void GAM2ObservableKinematic::setUPrim(double uPrim, PhysicalUnit::Type unit) {
    setUPrim(PhysicalType<double>(uPrim, unit));
}

void GAM2ObservableKinematic::setMgg2(double Mgg2, PhysicalUnit::Type unit) {
    setMgg2(PhysicalType<double>(Mgg2, unit));
}

void GAM2ObservableKinematic::setE(double E, PhysicalUnit::Type unit) {
    setE(PhysicalType<double>(E, unit));
}

void GAM2ObservableKinematic::setPhi(double phi, PhysicalUnit::Type unit) {
    setPhi(PhysicalType<double>(phi, unit));
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GAM2ObservableKinematic& GAM2ObservableKinematic) {

    GAM2ObservableKinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GAM2ObservableKinematic& GAM2ObservableKinematic) {

    GAM2ObservableKinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
