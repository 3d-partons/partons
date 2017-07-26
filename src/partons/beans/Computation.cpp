#include "../../../include/partons/beans/Computation.h"

namespace PARTONS {


Computation::Computation() :
        BaseObject("Computation"), m_dateTime(time(0)) {
}

Computation::Computation(int indexId, time_t dateTime) :
        BaseObject("Computation"), m_dateTime(dateTime) {
    setIndexId(indexId);
}

Computation::Computation(const Computation &other) :
        BaseObject(other) {
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

} /* namespace PARTONS */
