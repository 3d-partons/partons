#include "../../../../include/partons/beans/double_distribution/DDGauge.h"

DDGauge::DDGauge() :
        m_type(DDGauge::UNDEFINED) {
}

DDGauge::DDGauge(DDGauge::Type type) {
    m_type = type;
}

DDGauge::~DDGauge() {
}

DDGauge::operator DDGauge::Type() const {
    return m_type;
}

std::string DDGauge::toString() {

    switch (m_type) {
    case BMKS:
        return "BMKS";
        break;
    case Pobylitsa:
        return "Pobylitsa";
        break;
    default:
        return "UNDEFINED";
    }
}
