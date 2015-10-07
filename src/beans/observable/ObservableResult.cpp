#include "ObservableResult.h"

#include <sstream>

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
    std::ostringstream os;

    if (m_observableType == ObservableType::PHI) {
        os << "phi(°) = " << m_phi << " - observable = " << m_value;
    } else if (m_observableType == ObservableType::FOURIER) {
        os << "observable = " << m_value;
    } else {
        os << "unknow observable = " << m_value;
    }

    return os.str();
}

void ObservableResult::setObservableType(ObservableType::Type observableType) {
    m_observableType = observableType;
}
