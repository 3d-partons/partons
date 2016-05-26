#include "../../../../include/partons/beans/system/ResultInfo.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../include/partons/ResourceManager.h"

ResultInfo::ResultInfo() :
        BaseObject("ResultInfo"), m_scenarioHashSum(
                ElemUtils::StringUtils::EMPTY), m_environmentConfigurationHashSum(
                ElemUtils::StringUtils::EMPTY), m_scenarioTaskIndexNumber(0) {
}

ResultInfo::ResultInfo(const ResultInfo &other) :
        BaseObject(other) {
    m_computation = other.m_computation;
    m_scenarioHashSum = other.m_scenarioHashSum;
    m_environmentConfigurationHashSum = other.m_environmentConfigurationHashSum;
    m_scenarioTaskIndexNumber = other.m_scenarioTaskIndexNumber;
}

ResultInfo::~ResultInfo() {
    // Nothing to do.
}

const Computation& ResultInfo::getComputation() const {
    return m_computation;
}

void ResultInfo::setComputation(const Computation& computation) {
    m_computation = computation;
}

std::string ResultInfo::toString() const {
    ElemUtils::Formatter formatter;

    //TODO implement toString() for Computation class
    formatter << m_computation.toString() << '\n';

    Scenario* pScenario = ResourceManager::getInstance()->getScenarioByHashSum(
            m_scenarioHashSum);

    //TODO improve toString output for Scenario class
    if (pScenario) {
        formatter << pScenario->toString() << '\n';
    } else {
        formatter << "Scenario is not available" << '\n';
    }

    //TODO implement toString for EnvironmentConfiguration

    return formatter.str();
}

const std::string& ResultInfo::getEnvironmentConfigurationHashSum() const {
    return m_environmentConfigurationHashSum;
}

void ResultInfo::setEnvironmentConfigurationHashSum(
        const std::string& environmentConfigurationHashSum) {
    m_environmentConfigurationHashSum = environmentConfigurationHashSum;
}

const std::string& ResultInfo::getScenarioHashSum() const {
    return m_scenarioHashSum;
}

void ResultInfo::setScenarioHashSum(const std::string& scenarioHashSum) {
    m_scenarioHashSum = scenarioHashSum;
}

unsigned int ResultInfo::getScenarioTaskIndexNumber() const {
    return m_scenarioTaskIndexNumber;
}

void ResultInfo::setScenarioTaskIndexNumber(
        unsigned int scenarioTaskIndexNumber) {
    m_scenarioTaskIndexNumber = scenarioTaskIndexNumber;
}
