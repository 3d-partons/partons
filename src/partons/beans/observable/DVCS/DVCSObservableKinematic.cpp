#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB = "xB";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2 = "Q2";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI = "phi";
const std::string DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY =
        "E";

DVCSObservableKinematic::DVCSObservableKinematic() :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS) {
    setHashSum(ElemUtils::StringUtils::EMPTY);
}

DVCSObservableKinematic::DVCSObservableKinematic(
        const ElemUtils::Parameters &parameters) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_t(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_Q2(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_E(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::GEV).getShortName())), m_phi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {

    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB)) {
        m_xB.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB);
    }

    if (parameters.isAvailable(GPDKinematic::KINEMATIC_PARAMETER_NAME_T)) {
        m_t.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(GPDKinematic::KINEMATIC_PARAMETER_NAME_T);
    }

    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2)) {
        m_Q2.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2);
    }

    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY)) {
        m_E.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY);
    }

    if (parameters.isAvailable(
            DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI)) {
        m_phi.setValue(parameters.getLastAvailable().toDouble());
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI);
    }

    updateHashSum();
}

DVCSObservableKinematic::DVCSObservableKinematic(double xB, double t, double Q2,
        double E, double phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                PhysicalType<double>(xB,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_t(
                PhysicalType<double>(t,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_Q2(
                PhysicalType<double>(Q2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_E(
                PhysicalType<double>(E,
                        PhysicalUnit(PhysicalUnit::GEV).getShortName())), m_phi(
                PhysicalType<double>(phi,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    updateHashSum();
}

DVCSObservableKinematic::DVCSObservableKinematic(const PhysicalType<double>& xB,
        const PhysicalType<double>& t, const PhysicalType<double>& Q2,
        const PhysicalType<double>& E, const PhysicalType<double>& phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                xB), m_t(t), m_Q2(Q2), m_E(E), m_phi(phi) {
    updateHashSum();
}

DVCSObservableKinematic::DVCSObservableKinematic(
        const ElemUtils::GenericType& xB, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& Q2, const ElemUtils::GenericType& E,
        const ElemUtils::GenericType& phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                PhysicalType<double>(xB,
                        PhysicalUnit(PhysicalUnit::NONE).getShortName())), m_t(
                PhysicalType<double>(t,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_Q2(
                PhysicalType<double>(Q2,
                        PhysicalUnit(PhysicalUnit::GEV2).getShortName())), m_E(
                PhysicalType<double>(E,
                        PhysicalUnit(PhysicalUnit::GEV).getShortName())), m_phi(
                PhysicalType<double>(phi,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    updateHashSum();
}

DVCSObservableKinematic::DVCSObservableKinematic(
        const DVCSObservableKinematic &other) :
        ObservableKinematic(other), m_xB(other.m_xB), m_t(other.m_t), m_Q2(
                other.m_Q2), m_E(other.m_E), m_phi(other.m_phi) {
}

DVCSObservableKinematic::~DVCSObservableKinematic() {
}

std::string DVCSObservableKinematic::toString() const {

    ElemUtils::Formatter formatter;

    formatter << ObservableKinematic::toString();

    formatter << " xB = " << m_xB.toString();
    formatter << " t = " << m_t.toString();
    formatter << " Q2 = " << m_Q2.toString();
    formatter << " E = " << m_E.toString();
    formatter << " phi = " << m_phi.toString();

    return formatter.str();
}

void DVCSObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_xB << m_t << m_Q2 << m_E << m_phi;
}

void DVCSObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2;
    packet >> m_E;
    packet >> m_phi;
}

void DVCSObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xB.getValue() << m_t.getValue()
                            << m_Q2.getValue() << m_E.getValue()
                            << m_phi.getValue()));
}

PhysicalType<double> DVCSObservableKinematic::getXB() const {
    return m_xB;
}

void DVCSObservableKinematic::setXB(double xB) {

    m_xB = xB;
    updateHashSum();
}

PhysicalType<double> DVCSObservableKinematic::getT() const {
    return m_t;
}

void DVCSObservableKinematic::setT(double t) {

    m_t = t;
    updateHashSum();
}

PhysicalType<double> DVCSObservableKinematic::getQ2() const {
    return m_Q2;
}

void DVCSObservableKinematic::setQ2(double Q2) {

    m_Q2 = Q2;
    updateHashSum();
}

PhysicalType<double> DVCSObservableKinematic::getE() const {
    return m_E;
}

void DVCSObservableKinematic::setE(double E) {

    m_E = E;
    updateHashSum();
}

PhysicalType<double> DVCSObservableKinematic::getPhi() const {
    return m_phi;
}

void DVCSObservableKinematic::setPhi(double phi) {

    m_phi = phi;
    updateHashSum();
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
