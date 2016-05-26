#ifndef TASK_H
#define TASK_H

/**
 * @file Task.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 03, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/MultimapParameters.h>
#include <string>

#include "../../BaseObject.h"

class Scenario;

/**
 * @class Task
 *
 * @brief
 */
class Task: public BaseObject, public ElemUtils::MultimapParameters {
public:
    /**
     * Default constructor
     */
    Task();

    /**
     * Destructor
     */
    virtual ~Task();

    const std::string& getFunctionName() const;
    void setFunctionName(const std::string& functionName);

    const std::string& getServiceName() const;
    void setServiceName(const std::string& serviceName);

    bool isStoreInDB() const;
    void setStoreInDB(bool storeInDb);

    const ElemUtils::MultimapParameters& getObjectsParameters() const;
    ElemUtils::MultimapParameters& getObjectsParameters();

    virtual std::string toString() const;
    unsigned int getScenarioTaskIndexNumber() const;
    void setScenarioTaskIndexNumber(unsigned int scenarioTaskIndexNumber);

    Scenario* getScenario() const;
    void setScenario(Scenario* pScenario);

private:
    Scenario* m_pScenario; ///< reference to the scenario that hold this task.

    std::string m_serviceName; ///<
    std::string m_functionName; ///<
    bool m_storeInDB; ///<

    unsigned int m_scenarioTaskIndexNumber;
};

#endif /* TASK_H */
