#include "../../../include/partons/beans/Computation.h"

#include "../../../include/partons/Partons.h"

Computation::Computation() :
        DatabaseObject("Computation"), m_dateTime(time(0)), m_pScenario(0), m_pEnvironmentConfiguration(
                0) {
    setEnvironmentConfiguration(
            Partons::getInstance()->getEnvironmentConfiguration());
}

Computation::Computation(int indexId, time_t dateTime, Scenario* pScenario,
        EnvironmentConfiguration* pEnvironmentConfiguration) :
        DatabaseObject("Computation", indexId), m_dateTime(dateTime), m_pScenario(
                pScenario), m_pEnvironmentConfiguration(
                pEnvironmentConfiguration) {
}

Computation::Computation(const Computation &other) :
        DatabaseObject(other) {
    m_dateTime = other.m_dateTime;
    m_pScenario = other.m_pScenario;
    m_pEnvironmentConfiguration = other.m_pEnvironmentConfiguration;
}

Computation::~Computation() {
    // Nothing to do
    // Members pointers are handle by ResourceManager.
}

time_t Computation::getDateTime() const {
    return m_dateTime;
}

void Computation::setDateTime(time_t dateTime) {
    m_dateTime = dateTime;
}

EnvironmentConfiguration* Computation::getEnvironmentConfiguration() const {
    return m_pEnvironmentConfiguration;
}

void Computation::setEnvironmentConfiguration(
        EnvironmentConfiguration* pEnvironmentConfiguration) {
    m_pEnvironmentConfiguration = pEnvironmentConfiguration;
}

Scenario* Computation::getScenario() const {
    return m_pScenario;
}

void Computation::setScenario(Scenario* pScenario) {
    m_pScenario = pScenario;
}
