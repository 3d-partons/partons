#include "../../../../include/partons/beans/observable/ObservableType.h"

namespace PARTONS {


ObservableType::ObservableType() :
        m_type(ObservableType::UNDEFINED) {
}

ObservableType::ObservableType(Type type) :
        m_type(type) {
}
ObservableType::operator ObservableType::Type() const {
    return m_type;
}

std::string ObservableType::toString() const {
    switch (m_type) {
    case PHI:
        return "PHI";
        break;
    case FOURIER:
        return "FOURRIER";
        break;
    default:
        return "UNDEFINED";
    }
}

std::string ObservableType::getShortName() {
    switch (m_type) {
    case PHI:
        return "phi";
        break;
    case FOURIER:
        return "fourrier";
        break;
    default:
        return "UNDEFINED";
    }
}

ObservableType::Type ObservableType::getType() const {
    return m_type;
}

void ObservableType::setType(Type type) {
    m_type = type;
}

} /* namespace PARTONS */
