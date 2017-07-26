#include "../../../../include/partons/utils/type/Double.h"

namespace PARTONS {


Double::Double() :
        BaseType(), m_value(0.) {
}

Double::Double(double value) :
        BaseType(true), m_value(value) {
}

Double::~Double() {
}

} /* namespace PARTONS */
