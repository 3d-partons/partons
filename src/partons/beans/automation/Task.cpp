#include "../../../../include/partons/beans/automation/Task.h"

#include <utility>

Task::Task() :
        BaseObject("Task"), m_storeInDB(false) {
}

Task::~Task() {
}

const ElemUtils::Parameters& Task::getParameters(
        const std::string& className) const {
    std::map<std::string, ElemUtils::Parameters>::const_iterator it =
            m_functionParameters.find(className);
    if (it == m_functionParameters.end()) {
        error(__func__, "Missing parameters for className = " + className);
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

void Task::addParameters(const std::string& className,
        const ElemUtils::Parameters& parameterList) {
    m_functionParameters.insert(std::make_pair(className, parameterList));
}

bool Task::isAvailableParameters(const std::string &className) {
    m_it = m_functionParameters.find(className);

    return (m_it != m_functionParameters.end()) ? true : false;
}

ElemUtils::Parameters& Task::getLastAvailableParameters() const {
    return (m_it->second);
}

std::vector<ElemUtils::Parameters> Task::getListOfLastAvailableParameters(
        const std::string &className) {
    std::vector<ElemUtils::Parameters> listOfParameterList;

    std::multimap<std::string, ElemUtils::Parameters>::iterator itUpperBound =
            m_functionParameters.upper_bound(className);

    while (m_it != itUpperBound && m_it != m_functionParameters.end()) {
        listOfParameterList.push_back(m_it->second);
        m_it++;
    }

    return listOfParameterList;
}

bool Task::isStoreInDB() const {
    return m_storeInDB;
}

void Task::setStoreInDB(bool storeInDb) {
    m_storeInDB = storeInDb;
}
