#include "../../../include/partons/beans/Computation.h"

Computation::Computation() :
        m_dateTime(time(0)) {
}

Computation::~Computation() {
}

const time_t Computation::getDateTime() const {
    return m_dateTime;
}

void Computation::setDateTime(time_t dateTime) {
    m_dateTime = dateTime;
}
