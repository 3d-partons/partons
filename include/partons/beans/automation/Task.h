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

namespace PARTONS {

class Scenario;

/**
 * @class Task
 *
 * @brief Class representing single task.
 *
 * This class represents a single task to be evaluated by one of services.
 * An object of this class is intended to be created by a xml parser during running PARTONS with a specific xml file, which can be local or can be retrieved from a database.
 * Therefore, this class may be seen as a representation of a single task encoded in a xml file, like:
 \code{.py}
 <task service="GPDService" method="computeGPDModel" storeInDB="0">

     <kinematics type="GPDKinematic">
         <param name="x" value="0.1" />
         <param name="xi" value="0.05" />
         <param name="t" value="-0.3" />
         <param name="MuF2" value="8." />
         <param name="MuR2" value="8." />
     </kinematics>

     <computation_configuration>
         <module type="GPDModule">
             <param name="className" value="GK11Model" />
         </module>
     </computation_configuration>
 </task>
 \endcode
 */
class Task: public BaseObject {

public:

    /**
     * Default constructor.
     */
    Task();

    /**
     * Destructor.
     */
    virtual ~Task();

    virtual std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get pointer to the scenario that holds this task.
     */
    Scenario* getScenario() const;

    /**
     * Set pointer to the scenario that holds this task.
     */
    void setScenario(Scenario* pScenario);

    /**
     * Get name of service responsible for running this scenario.
     */
    const std::string& getServiceName() const;

    /**
     * Set name of service responsible for running this scenario.
     */
    void setServiceName(const std::string& serviceName);

    /**
     * Get name of function to be evaluated by this task.
     */
    const std::string& getFunctionName() const;

    /**
     * Set name of function to be evaluated by this task.
     */
    void setFunctionName(const std::string& functionName);

    /**
     * Check if the result of this task is going to be store in the database.
     */
    bool isStoreInDB() const;

    /**
     * Set if the result of this task is going to be store in the database.
     */
    void setStoreInDB(bool storeInDb);

    /**
     * Get index of this task in a given scenario holding it.
     */
    unsigned int getScenarioTaskIndexNumber() const;

    /**
     * Set index of this task in a given scenario holding it.
     */
    void setScenarioTaskIndexNumber(unsigned int scenarioTaskIndexNumber);

    /**
     * Get base object data stored in xml block given by XMLParserI::COMPUTATION_CONFIGURATION_NODE_NAME tag.
     */
    const BaseObjectData& getModuleComputationConfiguration() const;

    /**
     * Set base object data stored in xml block given by XMLParserI::COMPUTATION_CONFIGURATION_NODE_NAME tag.
     */
    void setModuleComputationConfiguration(
            const BaseObjectData& moduleComputationConfiguration);

    /**
     * Get base object data stored in xml block given by XMLParserI::KINEMATICS_NODE_NAME tag.
     */
    const BaseObjectData& getKinematicsData() const;

    /**
     * Set base object data stored in xml block given by XMLParserI::KINEMATICS_NODE_NAME tag.
     */
    void setKinematicsData(const BaseObjectData& kinematicsData);

    /**
     * Get base object data stored in xml block given by XMLParserI::TASK_NODE_NAME tag.
     */
    const BaseObjectData& getTaskParameters() const;

    /**
     * Set base object data stored in xml block given by XMLParserI::TASK_NODE_NAME tag.
     */
    void setTaskParameters(const BaseObjectData& taskParameters);

private:

    /**
     * Pointer to the scenario that holds this task.
     */
    Scenario* m_pScenario;

    /**
     * Name of service responsible for running this scenario.
     */
    std::string m_serviceName;

    /**
     * Name of function to be evaluated by this task.
     */
    std::string m_functionName;

    /**
     * Switch indicating if the result of this task is going to be store in the database.
     */
    bool m_storeInDB;

    /**
     * Index of this task in a given scenario holding it.
     */
    unsigned int m_scenarioTaskIndexNumber;

    /**
     * Base object data stored in xml block given by XMLParserI::COMPUTATION_CONFIGURATION_NODE_NAME tag.
     */
    BaseObjectData m_moduleComputationConfiguration;

    /**
     * Base object data stored in xml block given by XMLParserI::KINEMATICS_NODE_NAME tag.
     */
    BaseObjectData m_kinematicsData;

    /**
     * Base object data stored in xml block given by XMLParserI::TASK_NODE_NAME tag.
     */
    BaseObjectData m_taskParameters;
};

} /* namespace PARTONS */

#endif /* TASK_H */
