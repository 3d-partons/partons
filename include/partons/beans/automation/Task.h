#ifndef TASK_H
#define TASK_H

/**
 * @file Task.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 03, 2015
 * @version 1.0
 */

#include <string>

#include "BaseObjectData.h"

class Scenario;

/**
 * @class Task
 *
 * @brief
 */
class Task: public BaseObject {
public:
    /**
     * Default constructor
     */
    Task();

    /**
     * Destructor
     */
    virtual ~Task();

    virtual std::string toString() const;

    // ##### GETTERS & SETTERS #####

    const std::string& getFunctionName() const;
    void setFunctionName(const std::string& functionName);

    const std::string& getServiceName() const;
    void setServiceName(const std::string& serviceName);

    bool isStoreInDB() const;
    void setStoreInDB(bool storeInDb);

    unsigned int getScenarioTaskIndexNumber() const;
    void setScenarioTaskIndexNumber(unsigned int scenarioTaskIndexNumber);

    Scenario* getScenario() const;
    void setScenario(Scenario* pScenario);

    const BaseObjectData& getModuleComputationConfiguration() const;
    void setModuleComputationConfiguration(
            const BaseObjectData& moduleComputationConfiguration);

    const BaseObjectData& getKinematicsData() const;
    void setKinematicsData(const BaseObjectData& kinematicsData);

    const BaseObjectData& getTaskParameters() const;
    void setTaskParameters(const BaseObjectData& taskParameters);

private:
    Scenario* m_pScenario; ///< reference to the scenario that hold this task.

    std::string m_serviceName; ///<
    std::string m_functionName; ///<
    bool m_storeInDB; ///<

    unsigned int m_scenarioTaskIndexNumber;

    BaseObjectData m_moduleComputationConfiguration;
    BaseObjectData m_kinematicsData;
    BaseObjectData m_taskParameters;
};

#endif /* TASK_H */
