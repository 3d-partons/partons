#include "../../../include/partons/beans/Result.h"

Result::Result(const std::string &className) :
        BaseObject(className), m_computationModuleName("UNDEFINED"), m_computationDateTime(
                time(0)), m_id(-1) {
}

Result::~Result() {
}

void Result::setComputationDateTime(time_t computationDateTime) {
    m_computationDateTime = computationDateTime;
}

void Result::setComputationModuleName(const std::string& moduleName) {
    m_computationModuleName = moduleName;
}

void Result::setId(int id) {
    m_id = id;
}

const std::string& Result::getComputationModuleName() const {
    return m_computationModuleName;
}

time_t Result::getComputationDateTime() const {
    return m_computationDateTime;
}

int Result::getId() const {
    return m_id;
}

