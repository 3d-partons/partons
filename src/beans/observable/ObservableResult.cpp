#include "ObservableResult.h"

#include "../../utils/stringUtils/Formatter.h"

ObservableResult::ObservableResult() :
        Result(), m_observbleName("UNDEFINED"), m_value(0.), m_phi(0.), m_totalError(
                0.), m_observableType(ObservableType::UNDEFINED) {
}

ObservableResult::ObservableResult(const std::string &observableName,
        double value) :
        Result(), m_observbleName(observableName), m_phi(0.), m_value(value), m_totalError(
                0.), m_observableType(ObservableType::UNDEFINED) {
}

ObservableResult::ObservableResult(const std::string &observableName,
        double phi, double value) :
        Result(), m_observbleName(observableName), m_phi(phi), m_value(value), m_totalError(
                0.), m_observableType(ObservableType::UNDEFINED) {
}

ObservableResult::~ObservableResult() {
}

std::string ObservableResult::toString() {
    Formatter formatter;

    formatter << m_kinematic.toStringWithoutPhi();

    if (m_observableType == ObservableType::PHI) {
        formatter << " phi = " << m_phi << " (degree) - observable = "
                << m_value;
    } else if (m_observableType == ObservableType::FOURIER) {
        formatter << " observable = " << m_value;
    } else {
        formatter << "unknow observable = " << m_value;
    }

    return formatter.str();
}

void ObservableResult::setObservableType(ObservableType::Type observableType) {
    m_observableType = observableType;
}

const ObservableKinematic& ObservableResult::getKinematic() const {
    return m_kinematic;
}

ObservableType::Type ObservableResult::getObservableType() const {
    return m_observableType;
}

const std::string& ObservableResult::getObservableName() const {
    return m_observbleName;
}

double ObservableResult::getPhi() const {
    return m_phi;
}

const ErrorBar& ObservableResult::getStatError() const {
    return m_statError;
}

const ErrorBar& ObservableResult::getSystError() const {
    return m_systError;
}

double ObservableResult::getTotalError() const {
    return m_totalError;
}

double ObservableResult::getValue() const {
    return m_value;
}

void ObservableResult::setPhi(double phi) {
    m_phi = phi;
}

void ObservableResult::setKinematic(const ObservableKinematic &kinematic) {
    m_kinematic = kinematic;
}

void ObservableResult::setStatError(const ErrorBar& statError) {
    m_statError = statError;
}

void ObservableResult::setSystError(const ErrorBar& systError) {
    m_systError = systError;
}

void ObservableResult::setTotalError(double totalError) {
    m_totalError = totalError;
}

void ObservableResult::setValue(double value) {
    m_value = value;
}
