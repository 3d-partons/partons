#include "../../../../include/partons/utils/type/Double.h"

Double::Double() :
        BaseType(), m_value(0.) {
}

Double::Double(double value) :
        BaseType(true), m_value(value) {
}

Double::~Double() {
}
