#ifndef SCENARIO_MANAGER_H
#define SCENARIO_MANAGER_H

/**
 * @file ScenarioManager.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 January 2014
 * @version 1.0
 *
 * @class ScenarioManager
 * @brief \<singleton\>
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/parser/XMLParser.h>
#include <string>

#include "beans/automation/Scenario.h"
#include "beans/automation/Task.h"

class ScenarioManager: public BaseObject, public ElemUtils::XMLParser {
public:
    static const std::string SCENARIO_NODE_NAME;
    static const std::string TASK_NODE_NAME;

    /**
     * A static function that provides a unique pointer of this class
     *
     * @return A unique pointer of this class
     */
    static ScenarioManager* getInstance();

    void delete_();

    void playScenario(const std::string &scenarioFileName);

    virtual void startElement(const std::string &elementName,
            ElemUtils::XMLAttributs attributes, const std::string &elementData);
    virtual void endElement(const std::string &elementName);

private:
    static ScenarioManager* pInstance; ///< Private pointer of this class for a unique instance

    /**
     * Private default constructor for a unique instance of this class
     */
    ScenarioManager();

    /**
     * Default destructor
     */
    virtual ~ScenarioManager();

    Scenario m_scenario;

    // Temporary object
    Task m_task;
    // Temporary object
    ElemUtils::Parameters m_parameterList;
};

#endif /* SCENARIO_MANAGER_H */
