#include "ObservableResult.h"

#include "../../utils/stringUtils/Formatter.h"

ObservableResult::ObservableResult() :
        m_phi(0.), m_value(0.), m_observableType(ObservableType::UNDEFINED) {
}

ObservableResult::ObservableResult(double value) :
        m_phi(0.), m_value(value), m_observableType(ObservableType::UNDEFINED) {
}

ObservableResult::ObservableResult(double phi, double value) :
        m_phi(phi), m_value(value), m_observableType(ObservableType::UNDEFINED) {
}

ObservableResult::~ObservableResult() {
    // TODO Auto-generated destructor stub
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

std::string ObservableResult::toString() {
    Formatter formatter;

    if (m_observableType == ObservableType::PHI) {
        formatter << "phi(°) = " << m_phi << " - observable = " << m_value;
    } else if (m_observableType == ObservableType::FOURIER) {
        formatter << "observable = " << m_value;
    } else {
        formatter << "unknow observable = " << m_value;
    }

    return formatter.str();
}

void ObservableResult::setObservableType(ObservableType::Type observableType) {
    m_observableType = observableType;
}
