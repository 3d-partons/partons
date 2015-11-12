#include "Result.h"

Result::Result() :
        m_computationModuleName("UNDEFINED"), m_computationDateTime(time(0)) {
}

Result::~Result() {
}

time_t Result::getComputationDateTime() const {
    return m_computationDateTime;
}

void Result::setComputationDateTime(time_t computationDateTime) {
    m_computationDateTime = computationDateTime;
}

const std::string& Result::getComputationModuleName() const {
    return m_computationModuleName;
}

void Result::setComputationModuleName(const std::string& moduleName) {
    m_computationModuleName = moduleName;
}
