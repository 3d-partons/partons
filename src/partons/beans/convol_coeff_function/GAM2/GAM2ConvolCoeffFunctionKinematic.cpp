#include "../../../../../include/partons/beans/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>
#include <iterator>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/observable/GAM2/GAM2ObservableKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

const std::string GAM2ConvolCoeffFunctionKinematic::GAM2_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME =
        "GAM2ConvolCoeffFunctionKinematic";

GAM2ConvolCoeffFunctionKinematic::GAM2ConvolCoeffFunctionKinematic() :
        ConvolCoeffFunctionKinematic("GAM2ConvolCoeffFunctionKinematic",
                ChannelType::GAM2), m_uPrim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_polG0(
                PolarizationType::UNDEFINED), m_polG1(
                PolarizationType::UNDEFINED), m_polG2(
                PolarizationType::UNDEFINED), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)) {
}

GAM2ConvolCoeffFunctionKinematic::GAM2ConvolCoeffFunctionKinematic(double xi,
        double t, double uPrim, double Mgg2, double MuF2, double MuR2,
        PolarizationType::Type polG0, PolarizationType::Type polG1,
        PolarizationType::Type polG2, double phi) :
        ConvolCoeffFunctionKinematic("GAM2ConvolCoeffFunctionKinematic",
                ChannelType::GAM2, xi, t, MuF2, MuR2), m_uPrim(
                PhysicalType<double>(uPrim, PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(Mgg2, PhysicalUnit::GEV2)), m_polG0(polG0), m_polG1(
                polG1), m_polG2(polG2), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)) {
}

GAM2ConvolCoeffFunctionKinematic::GAM2ConvolCoeffFunctionKinematic(
        const PhysicalType<double> &xi, const PhysicalType<double> &t,
        const PhysicalType<double> &uPrim, const PhysicalType<double> &Mgg2,
        const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2,
        PolarizationType::Type polG0, PolarizationType::Type polG1,
        PolarizationType::Type polG2, const PhysicalType<double> &phi) :
        ConvolCoeffFunctionKinematic("GAM2ConvolCoeffFunctionKinematic",
                ChannelType::GAM2, xi, t, MuF2, MuR2), m_uPrim(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(PhysicalUnit::GEV2)), m_polG0(polG0), m_polG1(
                polG1), m_polG2(polG2), m_phi(
                PhysicalType<double>(PhysicalUnit::RAD)) {

    m_uPrim.checkIfSameUnitCategoryAs(uPrim);
    m_Mgg2.checkIfSameUnitCategoryAs(Mgg2);
    m_phi.checkIfSameUnitCategoryAs(phi);

    m_uPrim = uPrim;
    m_Mgg2 = Mgg2;
    m_phi = phi;
}

GAM2ConvolCoeffFunctionKinematic::GAM2ConvolCoeffFunctionKinematic(
        const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
        const ElemUtils::GenericType &uPrim, const ElemUtils::GenericType &Mgg2,
        const ElemUtils::GenericType &MuF2, const ElemUtils::GenericType &MuR2,
        PolarizationType::Type polG0, PolarizationType::Type polG1,
        PolarizationType::Type polG2, const ElemUtils::GenericType &phi) :
        ConvolCoeffFunctionKinematic("GAM2ConvolCoeffFunctionKinematic",
                ChannelType::GAM2, xi, t, MuF2, MuR2), m_uPrim(
                PhysicalType<double>(uPrim, PhysicalUnit::GEV2)), m_Mgg2(
                PhysicalType<double>(Mgg2, PhysicalUnit::GEV2)), m_polG0(polG0), m_polG1(
                polG1), m_polG2(polG2), m_phi(
                PhysicalType<double>(phi, PhysicalUnit::RAD)) {
}

GAM2ConvolCoeffFunctionKinematic::GAM2ConvolCoeffFunctionKinematic(
        const GAM2ConvolCoeffFunctionKinematic &other) :
        ConvolCoeffFunctionKinematic(other), m_uPrim(other.m_uPrim), m_Mgg2(
                other.m_Mgg2), m_polG0(other.m_polG0), m_polG1(other.m_polG1), m_polG2(
                other.m_polG2), m_phi(other.m_phi) {
}

GAM2ConvolCoeffFunctionKinematic::~GAM2ConvolCoeffFunctionKinematic() {
}

void GAM2ConvolCoeffFunctionKinematic::configure(
        const ElemUtils::Parameters &parameters) {

    //run for mother
    ConvolCoeffFunctionKinematic::configure(parameters);

    double value;
    PhysicalUnit::Type unit;

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

    //TODO polarization is missing

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
            setPhi(value, PhysicalUnit::RAD);
        }
    } else {
        errorMissingParameter(
                GAM2ObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
    }
}

std::string GAM2ConvolCoeffFunctionKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ConvolCoeffFunctionKinematic::toString();

    if (m_uPrim.isInitialized())
        formatter << "u': " << m_uPrim.toString() << ' ';

    if (m_Mgg2.isInitialized())
        formatter << "Mgg2: " << m_Mgg2.toString() << ' ';

    formatter << "polG0: " << PolarizationType(m_polG0).toString() << ' ';
    formatter << "polG1: " << PolarizationType(m_polG1).toString() << ' ';
    formatter << "polG2: " << PolarizationType(m_polG2).toString() << ' ';

    if (m_phi.isInitialized())
        formatter << "phi: " << m_phi.toString() << ' ';

    return formatter.str();
}

void GAM2ConvolCoeffFunctionKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi.toStdString()
                            << m_t.toStdString() << m_uPrim.toStdString()
                            << m_Mgg2.toStdString() << m_MuF2.toStdString()
                            << m_MuR2.toStdString()
                            << PolarizationType(m_polG0).toString()
                            << PolarizationType(m_polG1).toString()
                            << PolarizationType(m_polG2).toString()
                            << m_phi.toStdString()));
}

void GAM2ConvolCoeffFunctionKinematic::serialize(
        ElemUtils::Packet& packet) const {

    ConvolCoeffFunctionKinematic::serialize(packet);

    packet << m_uPrim;
    packet << m_Mgg2;
    packet << PolarizationType(m_polG0);
    packet << PolarizationType(m_polG1);
    packet << PolarizationType(m_polG2);
    packet << m_phi;
}

void GAM2ConvolCoeffFunctionKinematic::unserialize(ElemUtils::Packet& packet) {

    ConvolCoeffFunctionKinematic::unserialize(packet);

    packet >> m_uPrim;
    packet >> m_Mgg2;

    PolarizationType polarization;

    packet >> polarization;
    m_polG0 = polarization.getType();

    packet >> polarization;
    m_polG1 = polarization.getType();

    packet >> polarization;
    m_polG2 = polarization.getType();

    packet >> m_phi;

    updateHashSum();
}

void GAM2ConvolCoeffFunctionKinematic::serializeIntoStdVector(
        std::vector<double>& vec) const {

    ConvolCoeffFunctionKinematic::serializeIntoStdVector(vec);

    m_uPrim.serializeIntoStdVector(vec);
    m_Mgg2.serializeIntoStdVector(vec);
    vec.push_back(static_cast<double>(m_polG0));
    vec.push_back(static_cast<double>(m_polG1));
    vec.push_back(static_cast<double>(m_polG2));
    m_phi.serializeIntoStdVector(vec);
}

void GAM2ConvolCoeffFunctionKinematic::unserializeFromStdVector(
        std::vector<double>::const_iterator& it,
        const std::vector<double>::const_iterator& end) {

    ConvolCoeffFunctionKinematic::unserializeFromStdVector(it, end);

    m_uPrim.unserializeFromStdVector(it, end);
    m_Mgg2.unserializeFromStdVector(it, end);

    m_polG0 = static_cast<PolarizationType::Type>(*it);
    it++;
    m_polG1 = static_cast<PolarizationType::Type>(*it);
    it++;
    m_polG2 = static_cast<PolarizationType::Type>(*it);
    it++;

    m_phi.unserializeFromStdVector(it, end);

    updateHashSum();
}

bool GAM2ConvolCoeffFunctionKinematic::operator ==(
        const GAM2ConvolCoeffFunctionKinematic& other) const {
    return m_xi == other.getXi() && m_t == other.getT()
            && m_MuF2 == other.getMuF2() && m_MuR2 == other.getMuR2()
            && m_uPrim == other.getUPrim() && m_Mgg2 == other.getMgg2()
            && m_polG0 == other.getPolG0() && m_polG1 == other.getPolG1()
            && m_polG2 == other.getPolG2() && m_phi == other.getPhi();
}

bool GAM2ConvolCoeffFunctionKinematic::operator !=(
        const GAM2ConvolCoeffFunctionKinematic& other) const {
    return !((*this) == other);
}

const PhysicalType<double>& GAM2ConvolCoeffFunctionKinematic::getUPrim() const {
    return m_uPrim;
}

const PhysicalType<double>& GAM2ConvolCoeffFunctionKinematic::getMgg2() const {
    return m_Mgg2;
}

const PhysicalType<double>& GAM2ConvolCoeffFunctionKinematic::getPhi() const {
    return m_phi;
}

void GAM2ConvolCoeffFunctionKinematic::setUPrim(
        const PhysicalType<double>& uPrim) {

    m_uPrim.checkIfSameUnitCategoryAs(uPrim);
    m_uPrim = uPrim;
    updateHashSum();
}

void GAM2ConvolCoeffFunctionKinematic::setMgg2(
        const PhysicalType<double>& Mgg2) {

    m_Mgg2.checkIfSameUnitCategoryAs(Mgg2);
    m_Mgg2 = Mgg2;
    updateHashSum();
}

void GAM2ConvolCoeffFunctionKinematic::setPhi(const PhysicalType<double>& phi) {

    m_phi.checkIfSameUnitCategoryAs(phi);
    m_phi = phi;
    updateHashSum();
}

void GAM2ConvolCoeffFunctionKinematic::setUPrim(double uPrim,
        PhysicalUnit::Type unit) {
    setUPrim(PhysicalType<double>(uPrim, unit));
}

void GAM2ConvolCoeffFunctionKinematic::setMgg2(double Mgg2,
        PhysicalUnit::Type unit) {
    setMgg2(PhysicalType<double>(Mgg2, unit));
}

void GAM2ConvolCoeffFunctionKinematic::setPhi(double phi,
        PhysicalUnit::Type unit) {
    setPhi(PhysicalType<double>(phi, unit));
}

PolarizationType::Type GAM2ConvolCoeffFunctionKinematic::getPolG0() const {
    return m_polG0;
}

void GAM2ConvolCoeffFunctionKinematic::setPolG0(PolarizationType::Type polG0) {

    m_polG0 = polG0;
    updateHashSum();
}

PolarizationType::Type GAM2ConvolCoeffFunctionKinematic::getPolG1() const {
    return m_polG1;
}

void GAM2ConvolCoeffFunctionKinematic::setPolG1(PolarizationType::Type polG1) {

    m_polG1 = polG1;
    updateHashSum();
}

PolarizationType::Type GAM2ConvolCoeffFunctionKinematic::getPolG2() const {
    return m_polG2;
}

void GAM2ConvolCoeffFunctionKinematic::setPolG2(PolarizationType::Type polG2) {

    m_polG2 = polG2;
    updateHashSum();
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        GAM2ConvolCoeffFunctionKinematic& kinematic) {

    kinematic.serialize(packet);
    return packet;
}

ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        GAM2ConvolCoeffFunctionKinematic& kinematic) {

    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
