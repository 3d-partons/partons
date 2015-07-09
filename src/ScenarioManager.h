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

#include <string>

#include "beans/automation/Scenario.h"
#include "beans/automation/Task.h"
#include "utils/parser/xml/XMLParser.h"

class Attributs;

class ScenarioManager: public XMLParser /* ParserXML */{
public:
    /**
     * A static function that provides a unique pointer of this class
     *
     * @return A unique pointer of this class
     */
    static ScenarioManager* getInstance();

    /**
     * Default destructor
     */
    virtual ~ScenarioManager();

    void playScenario(const std::string &scenarioFilePath);

//    virtual void startElement(std::string tagName, Attributs attributes,
//            std::string tagValue);

    virtual void startElement(const std::string &elementName,
            Attributs attributes, const std::string &elementData);
    virtual void endElement(const std::string &elementName);

private:
    static ScenarioManager* pInstance; ///< Private pointer of this class for a unique instance

    /**
     * Private default constructor for a unique instance of this class
     */
    ScenarioManager();

    Scenario m_scenario;
    Task m_task;
};

#endif /* SCENARIO_MANAGER_H */
