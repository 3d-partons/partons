#include "ObservableKinematic.h"

#include "../../utils/GenericType.h"
#include "../../utils/ParameterList.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/stringUtils/StringUtils.h"

const std::string ObservableKinematic::PARAMETER_NAME_XB = "xB";
const std::string ObservableKinematic::PARAMETER_NAME_T = "t";
const std::string ObservableKinematic::PARAMETER_NAME_Q2 = "Q2";
const std::string ObservableKinematic::PARAMETER_NAME_PHI = "phi";

ObservableKinematic::ObservableKinematic() :
        Kinematic("ObservableKinematic"), m_xB(0.), m_t(0.), m_Q2(0.), m_phi(0.) {
}

ObservableKinematic::ObservableKinematic(ParameterList &parameterList) :
        Kinematic("ObservableKinematic"), m_xB(0.), m_t(0.), m_Q2(0.), m_phi(0.) {
    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_XB)) {
        m_xB = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_XB << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_T << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_Q2)) {
        m_Q2 = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_Q2 << ">");
    }

    if (parameterList.isAvailable(ObservableKinematic::PARAMETER_NAME_PHI)) {

        m_phi = parameterList.getLastAvailable().toDouble();
    } else {
        throwException(__func__,
                Formatter() << "Missing parameter <"
                        << ObservableKinematic::PARAMETER_NAME_PHI << ">");
    }
}

ObservableKinematic::ObservableKinematic(double xB, double t, double Q2,
        double phi) :
        Kinematic("ObservableKinematic"), m_xB(xB), m_t(t), m_Q2(Q2), m_phi(phi) {
}

ObservableKinematic::ObservableKinematic(const std::string &xB,
        const std::string &t, const std::string &Q2, const std::string &phi) :
        Kinematic("ObservableKinematic"), m_xB(
                StringUtils::fromStringToDouble(xB)), m_t(
                StringUtils::fromStringToDouble(t)), m_Q2(
                StringUtils::fromStringToDouble(Q2)), m_phi(
                StringUtils::fromStringToDouble(phi)) {
}

ObservableKinematic::~ObservableKinematic() {
}

std::string ObservableKinematic::toString() const {
    Formatter formatter;

    formatter << toStringWithoutPhi() << '\n';

    formatter << "m_phi = " << m_phi << "\n";

    return formatter;
}

const std::string ObservableKinematic::toStringWithoutPhi() const {
    Formatter formatter;

    formatter << Kinematic::toString() << " m_xB = " << m_xB << " m_t = " << m_t
            << " (GeV2) m_Q2 = " << m_Q2 << " (GeV2)";

    return formatter;
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

double ObservableKinematic::getPhi() const {
    return m_phi;
}

void ObservableKinematic::setPhi(double phi) {
    m_phi = phi;
}
