#include "../../../../include/partons/beans/observable/ObservableKinematic.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

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

ObservableKinematic::ObservableKinematic(
        const ElemUtils::Parameters &parameters) :
        Kinematic("ObservableKinematic"), m_xB(0.), m_t(0.), m_Q2(0.), m_phi(
                PhysicalType<double>(0.,
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_XB)) {
        m_xB = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_XB << ">");
    }

    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_T << ">");
    }

    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_Q2)) {
        m_Q2 = parameters.getLastAvailable().toDouble();
    } else {
        error(__func__,
                ElemUtils::Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_Q2 << ">");
    }

    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_PHI)) {
        m_phi.setValue(parameters.getLastAvailable().toDouble());
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

ObservableKinematic::ObservableKinematic(const ElemUtils::GenericType& xB,
        const ElemUtils::GenericType& t, const ElemUtils::GenericType& Q2,
        const ElemUtils::GenericType& phi) :
        Kinematic("ObservableKinematic"), m_xB(xB.toDouble()), m_t(
                t.toDouble()), m_Q2(Q2.toDouble()), m_phi(
                PhysicalType<double>(phi.toDouble(),
                        PhysicalUnit(PhysicalUnit::DEGREE).getShortName())) {
    m_phi.setInitialized(true);
}

ObservableKinematic::~ObservableKinematic() {
}

std::string ObservableKinematic::toString() const {
    ElemUtils::Formatter formatter;
    formatter << Kinematic::toString() << " m_xB = " << m_xB << " m_t = " << m_t
            << " (GeV2) m_Q2 = " << m_Q2 << " (GeV2)";

    if (m_phi.isInitialized()) {
        formatter << " phi = " << m_phi.toString();
    }

    return formatter.str();
}

//ComparisonReport ObservableKinematic::compare(
//        const ObservableKinematic& referenceObject,
//        const NumA::Tolerances& tolerances) const {
//    ComparisonReport comparisonReport(getClassName());
//
//    ComparisonData xb_comparisonData = CompareUtils::compareDouble(
//            ObservableKinematic::PARAMETER_NAME_XB, getXB(),
//            referenceObject.getXB(), tolerances);
//    comparisonReport.addComparisonData(xb_comparisonData);
//
//    ComparisonData t_comparisonData = CompareUtils::compareDouble(
//            ObservableKinematic::PARAMETER_NAME_T, getT(),
//            referenceObject.getT(), tolerances);
//    comparisonReport.addComparisonData(t_comparisonData);
//
//    ComparisonData q2_comparisonData = CompareUtils::compareDouble(
//            ObservableKinematic::PARAMETER_NAME_Q2, getQ2(),
//            referenceObject.getQ2(), tolerances);
//    comparisonReport.addComparisonData(q2_comparisonData);
//
//    //TODO handle phi
//
//    return comparisonReport;
//}

// ##### GETTERS & SETTERS #####

double ObservableKinematic::getQ2() const {
    return m_Q2;
}

void ObservableKinematic::setQ2(double Q2) {
    m_Q2 = Q2;
    updateHashSum();
}

double ObservableKinematic::getT() const {
    return m_t;
}

void ObservableKinematic::setT(double t) {
    m_t = t;
    updateHashSum();
}

double ObservableKinematic::getXB() const {
    return m_xB;
}

void ObservableKinematic::setXB(double xB) {
    m_xB = xB;
    updateHashSum();
}

PhysicalType<double> ObservableKinematic::getPhi() const {
    return m_phi;
}

void ObservableKinematic::setPhi(double phi) {
    m_phi = phi;
}

//TODO serialize PhysicalType<T>
void ObservableKinematic::serialize(ElemUtils::Packet &packet) const {
    Kinematic::serialize(packet);

    packet << m_xB << m_t << m_Q2 << m_phi.getValue();
}

//TODO serialize PhysicalType<T>
void ObservableKinematic::unserialize(ElemUtils::Packet &packet) {
    Kinematic::unserialize(packet);

    packet >> m_xB;
    packet >> m_t;
    packet >> m_Q2;

    double phi = 0.;
    packet >> phi;

    m_phi.setValue(phi);
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic) {
    observableKinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic) {

    observableKinematic.unserialize(packet);
    return packet;
}

void ObservableKinematic::updateHashSum() const {
    //TODO
}
