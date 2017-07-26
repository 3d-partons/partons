#include "../../../include/partons/beans/Result.h"

#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {


Result::Result(const std::string &className) :
        BaseObject(className), m_computationModuleName("UNDEFINED") {
}

Result::Result(const Result &other) :
        BaseObject(other) {
    m_resultInfo = other.m_resultInfo;
    m_computationModuleName = other.m_computationModuleName;
}

Result::~Result() {
}

void Result::setComputationModuleName(const std::string& moduleName) {
    m_computationModuleName = moduleName;
}

const std::string& Result::getComputationModuleName() const {
    return m_computationModuleName;
}

const ResultInfo& Result::getResultInfo() const {
    return m_resultInfo;
}

void Result::setResultInfo(const ResultInfo& resultInfo) {
    m_resultInfo = resultInfo;
}

std::string Result::toString() const {
    ElemUtils::Formatter formatter;

    formatter << m_resultInfo.toString() << '\n';

    return formatter.str();
}

} /* namespace PARTONS */
