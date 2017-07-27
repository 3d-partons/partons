#include "../../../../include/partons/beans/dse/QuarkGluonVertex.h"

namespace PARTONS {


QuarkGluonVertex::QuarkGluonVertex(const std::string &className,
        unsigned int numberOfBasisElements) :
        BaseObject(className), m_N(numberOfBasisElements) {
}

QuarkGluonVertex::~QuarkGluonVertex() {
    // TODO Auto-generated destructor stub
}

QuarkGluonVertex::QuarkGluonVertex(const QuarkGluonVertex& other) :
        BaseObject(other), m_N(other.m_N) {

}

std::string QuarkGluonVertex::toString() const {
    return BaseObject::toString();
}

unsigned int QuarkGluonVertex::numberOfBasisElementsUsed() const {
    return m_N;
}

void QuarkGluonVertex::basisElementsWarning(const std::string& functionName) const {
    warn(functionName, "Vertex used with more basis elements than supported!"
            "Returned 0 for unsupported basis elements!"
            "Beware unphysical result.");
}

} /* namespace PARTONS */
