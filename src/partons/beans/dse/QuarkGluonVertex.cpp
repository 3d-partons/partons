#include "../../../../include/partons/beans/dse/QuarkGluonVertex.h"

QuarkGluonVertex::QuarkGluonVertex(const std::string &className) :
        BaseObject(className) {
    m_v.assign(12, false);
}

QuarkGluonVertex::~QuarkGluonVertex() {
    // TODO Auto-generated destructor stub
}

QuarkGluonVertex::QuarkGluonVertex(const QuarkGluonVertex& other) :
        BaseObject(other), m_v(other.m_v) {

}

std::string QuarkGluonVertex::toString() const {
    return BaseObject::toString();
}

const std::vector<bool>& QuarkGluonVertex::getV() const {
    return m_v;
}

void QuarkGluonVertex::setV(const std::vector<bool>& v) {
    m_v = v;
}
