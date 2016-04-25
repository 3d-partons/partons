#include "../../../../include/partons/beans/automation/Task.h"

#include <ElementaryUtils/string_utils/Formatter.h>

Task::Task() :
        BaseObject("Task"), m_storeInDB(false), m_scenarioTaskIndexNumber(0) {
}

Task::~Task() {
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

bool Task::isStoreInDB() const {
    return m_storeInDB;
}

void Task::setStoreInDB(bool storeInDb) {
    m_storeInDB = storeInDb;
}

//TODO implement
std::string Task::toString() const {
    ElemUtils::Formatter formatter;

    formatter << "Service name = " << m_serviceName << " function name = "
            << m_functionName;

    return formatter.str();
}

unsigned int Task::getScenarioTaskIndexNumber() const {
    return m_scenarioTaskIndexNumber;
}

void Task::setScenarioTaskIndexNumber(unsigned int scenarioTaskIndexNumber) {
    m_scenarioTaskIndexNumber = scenarioTaskIndexNumber;
}
