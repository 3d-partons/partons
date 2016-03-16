#include "../../../include/partons/beans/Result.h"

Result::Result(const std::string &className) :
        DatabaseObject(className), m_computationModuleName("UNDEFINED") {
}

Result::~Result() {
}

void Result::setComputationModuleName(const std::string& moduleName) {
    m_computationModuleName = moduleName;
}

const std::string& Result::getComputationModuleName() const {
    return m_computationModuleName;
}

const Computation& Result::getComputation() const {
    return m_computation;
}

Computation& Result::getComputation() {
    return m_computation;
}

void Result::setComputation(const Computation& computation) {
    m_computation = computation;
}
