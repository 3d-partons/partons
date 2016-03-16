#include "../../../include/partons/beans/Computation.h"

#include "../../../include/partons/beans/automation/ComputationConfiguration.h"
#include "../../../include/partons/beans/system/EnvironmentConfiguration.h"
#include "../../../include/partons/Partons.h"

Computation::Computation() :
        DatabaseObject("Computation"), m_dateTime(time(0)), m_pComputationConfiguration(
                0), m_pEnvironmentConfiguration(0) {
    setEnvironmentConfiguration(
            Partons::getInstance()->getEnvironmentConfiguration());
}

Computation::Computation(int indexId, time_t dateTime,
        ComputationConfiguration* pComputationConfiguration,
        EnvironmentConfiguration* pEnvironmentConfiguration) :
        DatabaseObject("Computation", indexId), m_dateTime(dateTime), m_pComputationConfiguration(
                pComputationConfiguration), m_pEnvironmentConfiguration(
                pEnvironmentConfiguration) {
}

Computation::~Computation() {
    // if (indexId != -1) it means that Computation object come from database so we need to destroy related objects.
    if (getIndexId() != -1) {
        if (m_pComputationConfiguration) {
            delete m_pComputationConfiguration;
            m_pComputationConfiguration = 0;
        }

        if (m_pEnvironmentConfiguration) {
            delete m_pEnvironmentConfiguration;
            m_pEnvironmentConfiguration = 0;
        }
    }
}

const time_t Computation::getDateTime() const {
    return m_dateTime;
}

void Computation::setDateTime(time_t dateTime) {
    m_dateTime = dateTime;
}

ComputationConfiguration* Computation::getComputationConfiguration() const {
    return m_pComputationConfiguration;
}

void Computation::setComputationConfiguration(
        ComputationConfiguration* pComputationConfiguration) {
    m_pComputationConfiguration = pComputationConfiguration;
}

EnvironmentConfiguration* Computation::getEnvironmentConfiguration() const {
    return m_pEnvironmentConfiguration;
}

void Computation::setEnvironmentConfiguration(
        EnvironmentConfiguration* pEnvironmentConfiguration) {
    m_pEnvironmentConfiguration = pEnvironmentConfiguration;
}
