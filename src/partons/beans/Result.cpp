#include "../../../include/partons/beans/Result.h"

#include <ElementaryUtils/string_utils/Formatter.h>

namespace PARTONS {

Result::Result(const std::string &className, ChannelType::Type channelType) :
        BaseObject(className), m_channelType(channelType), m_computationModuleName(
                "UNDEFINED") {
}

Result::Result(const Result &other) :
        BaseObject(other), m_channelType(other.m_channelType), m_computationModuleName(
                other.m_computationModuleName) {
    m_resultInfo = other.m_resultInfo;
}

Result::~Result() {
}

ChannelType::Type Result::getChannelType() const {
    return m_channelType;
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

    formatter << BaseObject::toString();
    formatter << "Channel type: " << ChannelType(m_channelType).toString()
            << '\n';
    formatter << "Computation module name: "
            << ChannelType(m_channelType).toString() << '\n';
    formatter << "Result info: " << m_resultInfo.toString() << '\n';

    return formatter.str();
}

} /* namespace PARTONS */

