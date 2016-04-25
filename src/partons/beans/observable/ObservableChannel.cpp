#include "../../../../include/partons/beans/observable/ObservableChannel.h"

ObservableChannel::ObservableChannel() :
        m_type(ObservableChannel::UNDEFINED) {
}

ObservableChannel::ObservableChannel(Type type) :
        m_type(type) {
}
ObservableChannel::operator ObservableChannel::Type() const {
    return m_type;
}

std::string ObservableChannel::toString() {
    switch (m_type) {
    case DVCS:
        return "DVCS";
        break;
    case DVMP:
        return "DVMP";
        break;
    case TCS:
        return "TCS";
        break;
    default:
        return "UNDEFINED";
    }
}

std::string ObservableChannel::getShortName() {
    switch (m_type) {
    case DVCS:
        return "DVCS";
        break;
    case DVMP:
        return "DVMP";
        break;
    case TCS:
        return "TCS";
        break;
    default:
        return "UNDEFINED";
    }
}

ObservableChannel::Type ObservableChannel::getType() const {
    return m_type;
}

void ObservableChannel::setType(Type type) {
    m_type = type;
}

ObservableChannel::~ObservableChannel() {
}
