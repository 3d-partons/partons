#include "../../../include/partons/beans/Computation.h"

Computation::Computation() :
        DatabaseObject("Computation"), m_dateTime(time(0)) {
}

Computation::Computation(int indexId, time_t dateTime) :
        DatabaseObject("Computation", indexId), m_dateTime(dateTime) {
}

Computation::Computation(const Computation &other) :
        DatabaseObject(other) {
    m_dateTime = other.m_dateTime;
}

Computation::~Computation() {
    // Nothing to do
    // Members pointers are handle by ResourceManager.
}

time_t Computation::getDateTime() const {
    return m_dateTime;
}

void Computation::setDateTime(time_t dateTime) {
    m_dateTime = dateTime;
}
