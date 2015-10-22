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

const std::string& Task::getServiceName() const {
    return m_serviceName;
}

void Task::setServiceName(const std::string& serviceName) {
    m_serviceName = serviceName;
}

void Task::addParameterList(const std::string& className,
        const ParameterList& parameterList) {
    m_functionParameters.insert(std::make_pair(className, parameterList));
}

bool Task::isAvailableParameterList(const std::string &className) {
    m_it = m_functionParameters.find(className);

    return (m_it != m_functionParameters.end()) ? true : false;
}

ParameterList& Task::getLastAvailableParameterList() const {
    return (m_it->second);
}

std::vector<ParameterList> Task::getListOfLastAvailableParameterList(
        const std::string &className) {
    std::vector<ParameterList> listOfParameterList;

    std::multimap<std::string, ParameterList>::iterator itUpperBound =
            m_functionParameters.upper_bound(className);

    while (m_it != itUpperBound && m_it != m_functionParameters.end()) {
        listOfParameterList.push_back(m_it->second);
        m_it++;
    }

    return listOfParameterList;
}
