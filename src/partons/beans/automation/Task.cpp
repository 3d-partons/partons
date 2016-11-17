#include "../../../../include/partons/beans/automation/Task.h"

#include <ElementaryUtils/string_utils/Formatter.h>

Task::Task() :
        BaseObject("Task"), m_pScenario(0), m_storeInDB(false), m_scenarioTaskIndexNumber(
                0) {
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
            << m_functionName << '\n' << "[Parameters]" << '\n';

    formatter << m_moduleComputationConfiguration.toString();

    return formatter.str();
}

unsigned int Task::getScenarioTaskIndexNumber() const {
    return m_scenarioTaskIndexNumber;
}

void Task::setScenarioTaskIndexNumber(unsigned int scenarioTaskIndexNumber) {
    m_scenarioTaskIndexNumber = scenarioTaskIndexNumber;
}

Scenario* Task::getScenario() const {
    return m_pScenario;
}

void Task::setScenario(Scenario* pScenario) {
    m_pScenario = pScenario;
}

const BaseObjectData& Task::getModuleComputationConfiguration() const {
    return m_moduleComputationConfiguration;
}

void Task::setModuleComputationConfiguration(
        const BaseObjectData& moduleComputationConfiguration) {
    m_moduleComputationConfiguration = moduleComputationConfiguration;

    debug(__func__,
            ElemUtils::Formatter()
                    << "Task's moduleComputationConfiguration member has been updating with value : "
                    << '\n' << moduleComputationConfiguration.toString());
}

const BaseObjectData& Task::getKinematicsData() const {
    return m_kinematicsData;
}

void Task::setKinematicsData(const BaseObjectData& kinematicsData) {
    m_kinematicsData = kinematicsData;
}

const BaseObjectData& Task::getTaskParameters() const {
    return m_taskParameters;
}

void Task::setTaskParameters(const BaseObjectData& taskParameters) {
    m_taskParameters = taskParameters;
}
