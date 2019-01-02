#include "../../../../../include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../../../include/partons/beans/channel/ChannelType.h"
#include "../../../../../include/partons/Partons.h"
#include "../../../../../include/partons/services/hash_sum/CryptographicHashService.h"
#include "../../../../../include/partons/ServiceObjectRegistry.h"
#include "../../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

const std::string DVCSObservableKinematic::PARAMETER_NAME_XB = "xB";
const std::string DVCSObservableKinematic::PARAMETER_NAME_T = "t";
const std::string DVCSObservableKinematic::PARAMETER_NAME_Q2 = "Q2";
const std::string DVCSObservableKinematic::PARAMETER_NAME_PHI = "phi";
const std::string DVCSObservableKinematic::PARAMETER_NAME_BEAM_ENERGY = "E";

DVCSObservableKinematic::DVCSObservableKinematic() :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                0.), m_t(0.), m_Q2(0.), m_E(0.), m_phi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    updateHashSum();
}

DVCSObservableKinematic::DVCSObservableKinematic(
        const ElemUtils::Parameters &parameters) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                0.), m_t(0.), m_Q2(0.), m_E(0.), m_phi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {

    if (parameters.isAvailable(DVCSObservableKinematic::PARAMETER_NAME_XB)) {
        m_xB = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(DVCSObservableKinematic::PARAMETER_NAME_XB);
    }

    if (parameters.isAvailable(DVCSObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(DVCSObservableKinematic::PARAMETER_NAME_T);
    }

    if (parameters.isAvailable(DVCSObservableKinematic::PARAMETER_NAME_Q2)) {
        m_Q2 = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(DVCSObservableKinematic::PARAMETER_NAME_Q2);
    }

    if (parameters.isAvailable(
            DVCSObservableKinematic::PARAMETER_NAME_BEAM_ENERGY)) {
        m_E = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(
                DVCSObservableKinematic::PARAMETER_NAME_BEAM_ENERGY);
    }

    if (parameters.isAvailable(DVCSObservableKinematic::PARAMETER_NAME_PHI)) {
        m_phi.setValue(parameters.getLastAvailable().toDouble());
        m_phi.setInitialized(true);
    }

    updateHashSum();
}

DVCSObservableKinematic::DVCSObservableKinematic(double xB, double t, double Q2,
        double E, double phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                xB), m_t(t), m_Q2(Q2), m_E(E), m_phi(
                PhysicalType<double>(phi,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {

    m_phi.setInitialized(true);
    updateHashSum();
}

DVCSObservableKinematic::DVCSObservableKinematic(
        const ElemUtils::GenericType& xB, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& Q2, const ElemUtils::GenericType& E,
        const ElemUtils::GenericType& phi) :
        ObservableKinematic("DVCSObservableKinematic", ChannelType::DVCS), m_xB(
                xB.toDouble()), m_t(t.toDouble()), m_Q2(Q2.toDouble()), m_E(
                E.toDouble()), m_phi(
                PhysicalType<double>(phi.toDouble(),
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {

    m_phi.setInitialized(true);
    updateHashSum();
}

DVCSObservableKinematic::~DVCSObservableKinematic() {
}

std::string DVCSObservableKinematic::toString() const {
    ElemUtils::Formatter formatter;
    formatter << Kinematic::toString() << " m_xB = " << m_xB << " m_t = " << m_t
            << " (GeV2) m_Q2 = " << m_Q2 << " (GeV2) m_E = " << m_E << " (GeV)";

    if (m_phi.isInitialized()) {
        formatter << " phi = " << m_phi.toString();
    }

    return formatter.str();
}

//TODO serialize PhysicalType<T>
void DVCSObservableKinematic::serialize(ElemUtils::Packet &packet) const {

    Kinematic::serialize(packet);

    packet << m_xB << m_t << m_Q2 << m_E << m_phi.getValue();
}

//TODO serialize PhysicalType<T>
void DVCSObservableKinematic::unserialize(ElemUtils::Packet &packet) {

    Kinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2;
    packet >> m_E;

    double phi = 0.;
    packet >> phi;

    m_phi.setValue(phi);
}

void DVCSObservableKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xB << m_t << m_Q2 << m_E
                            << m_phi.getValue()));
}

double DVCSObservableKinematic::getXB() const {
    return m_xB;
}

void DVCSObservableKinematic::setXB(double xB) {

    m_xB = xB;
    updateHashSum();
}

double DVCSObservableKinematic::getT() const {
    return m_t;
}

void DVCSObservableKinematic::setT(double t) {

    m_t = t;
    updateHashSum();
}

double DVCSObservableKinematic::getQ2() const {
    return m_Q2;
}

void DVCSObservableKinematic::setQ2(double Q2) {

    m_Q2 = Q2;
    updateHashSum();
}

double DVCSObservableKinematic::getE() const {
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
