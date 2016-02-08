#include "../../../../include/partons/beans/observable/ObservableKinematic.h"

#include "../../../../include/partons/utils/compare/CompareUtils.h"
#include "../../../../include/partons/utils/compare/ComparisonData.h"
#include "../../../../include/partons/utils/GenericType.h"
#include "../../../../include/partons/utils/ParameterList.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../../include/partons/utils/stringUtils/StringUtils.h"
#include "../../../../include/partons/utils/test/ComparisonReport.h"
#include "../../../../include/partons/utils/thread/Packet.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

const std::string ObservableKinematic::PARAMETER_NAME_XB = "xB";
const std::string ObservableKinematic::PARAMETER_NAME_T = "t";
const std::string ObservableKinematic::PARAMETER_NAME_Q2 = "Q2";
const std::string ObservableKinematic::PARAMETER_NAME_PHI = "phi";

ObservableKinematic::ObservableKinematic() :
        Kinematic("ObservableKinematic"), m_xB(0.), m_t(0.), m_Q2(0.), m_phi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
}

ObservableKinematic::ObservableKinematic(ParameterList &parameterList) :
        Kinematic("ObservableKinematic"), m_xB(0.), m_t(0.), m_Q2(0.), m_phi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_XB)) {
        m_xB = parameterList.getLastAvailable().toDouble();
    } else {
        error(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_XB << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameterList.getLastAvailable().toDouble();
    } else {
        error(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_T << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_Q2)) {
        m_Q2 = parameterList.getLastAvailable().toDouble();
    } else {
        error(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_Q2 << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_PHI)) {
        m_phi.setValue(parameterList.getLastAvailable().toDouble());
        m_phi.setInitialized(true);
    }

    /*
     else {
     error(__func__,
     Formatter() << "Missing parameter <"
     << ObservableKinematic::PARAMETER_NAME_PHI << ">");
     }
     */
}

ObservableKinematic::ObservableKinematic(double xB, double t, double Q2,
        double phi) :
        Kinematic("ObservableKinematic"), m_xB(xB), m_t(t), m_Q2(Q2), m_phi(
                PhysicalType<double>(phi,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    m_phi.setInitialized(true);
}

ObservableKinematic::ObservableKinematic(double xB, double t, double Q2) :
        Kinematic("ObservableKinematic"), m_xB(xB), m_t(t), m_Q2(Q2), m_phi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
}

ObservableKinematic::ObservableKinematic(const std::string &xB,
        const std::string &t, const std::string &Q2, const std::string &phi) :
        Kinematic("ObservableKinematic"), m_xB(
                StringUtils::fromStringToDouble(xB)), m_t(
                StringUtils::fromStringToDouble(t)), m_Q2(
                StringUtils::fromStringToDouble(Q2)), m_phi(
                PhysicalType<double>(StringUtils::fromStringToDouble(phi),
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    m_phi.setInitialized(true);
}

ObservableKinematic::~ObservableKinematic() {
}

std::string ObservableKinematic::toString() const {
    return Formatter() << Kinematic::toString() << " m_xB = " << m_xB
            << " m_t = " << m_t << " (GeV2) m_Q2 = " << m_Q2 << " (GeV2) phi = "
            << m_phi.toString();
}

ComparisonReport ObservableKinematic::compare(
        const ObservableKinematic& referenceObject,
        const Tolerances& tolerances) const {
    ComparisonReport comparisonReport(getClassName());

    ComparisonData xb_comparisonData = CompareUtils::compareDouble(
            ObservableKinematic::PARAMETER_NAME_XB, getXB(),
            referenceObject.getXB(), tolerances);
    comparisonReport.addComparisonData(xb_comparisonData);

    ComparisonData t_comparisonData = CompareUtils::compareDouble(
            ObservableKinematic::PARAMETER_NAME_T, getT(),
            referenceObject.getT(), tolerances);
    comparisonReport.addComparisonData(t_comparisonData);

    ComparisonData q2_comparisonData = CompareUtils::compareDouble(
            ObservableKinematic::PARAMETER_NAME_Q2, getQ2(),
            referenceObject.getQ2(), tolerances);
    comparisonReport.addComparisonData(q2_comparisonData);

    //TODO handle phi

    return comparisonReport;
}

// ##### GETTERS & SETTERS #####

double ObservableKinematic::getQ2() const {
    return m_Q2;
}

void ObservableKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
}

double ObservableKinematic::getT() const {
    return m_t;
}

void ObservableKinematic::setT(double t) {
    m_t = t;
}

double ObservableKinematic::getXB() const {
    return m_xB;
}

void ObservableKinematic::setXB(double xB) {
    m_xB = xB;
}

PhysicalType<double> ObservableKinematic::getPhi() const {
    return m_phi;
}

void ObservableKinematic::setPhi(double phi) {
    m_phi = phi;
}

//TODO serialize PhysicalType<T>
void ObservableKinematic::serialize(Packet &packet) const {
    packet << m_xB << m_t << m_Q2 << m_phi.getValue();
}

//TODO serialize PhysicalType<T>
void ObservableKinematic::unserialize(Packet &packet) {
    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2;

    double phi = 0.;
    packet >> phi;

    m_phi.setValue(phi);
}

Packet& operator <<(Packet& packet, ObservableKinematic& observableKinematic) {
    observableKinematic.serialize(packet);
    return packet;
}
Packet& operator >>(Packet& packet, ObservableKinematic& observableKinematic) {

    observableKinematic.unserialize(packet);
    return packet;
}
