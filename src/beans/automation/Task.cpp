/*
 * Task.cpp
 *
 *  Created on: Jul 3, 2015
 *      Author: debian
 */

#include "Task.h"

#include <utility>

Task::Task() :
        BaseObject("Task") {
}

Task::~Task() {
    // TODO Auto-generated destructor stub
}

const ParameterList& Task::getParameterList(
        const std::string& className) const {
    std::map<std::string, ParameterList>::const_iterator it =
            m_functionParameters.find(className);
    if (it == m_functionParameters.end()) {
        throwException(__func__,
                "Missing parameters for className = " + className);
    }

    return it->second;
}

const std::string& Task::getFunctionName() const {
    return m_functionName;
}

void Task::setFunctionName(const std::string& functionName) {
    m_functionName = functionName;
}

const std::map<std::string, ParameterList>& Task::getFunctionParameters() const {
    return m_functionParameters;
}

void Task::setFunctionParameters(
        const std::map<std::string, ParameterList>& functionParameters) {
    m_functionParameters = functionParameters;
}

const std::string& Task::getServiceName() const {
    return m_serviceName;
}

void Task::setServiceName(const std::string& serviceName) {
    m_serviceName = serviceName;
}
