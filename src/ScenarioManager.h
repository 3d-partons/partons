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

#include "beans/Scenario.h"
#include "utils/parser/xml/ParserXML.h"

class ScenarioManager: public ParserXML {
private:
    static ScenarioManager* pInstance; ///< Private pointer of this class for a unique instance

    /**
     * Private default constructor for a unique instance of this class
     */
    ScenarioManager();

    Scenario m_scenario;

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

    virtual void startElement(std::string tagName, Attributs attributes,
            std::string tagValue);
};

#endif /* SCENARIO_MANAGER_H */
