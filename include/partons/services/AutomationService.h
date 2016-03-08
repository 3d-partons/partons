/*
 * AutomationService.h
 *
 *  Created on: Mar 3, 2016
 *      Author: debian
 */

#ifndef AUTOMATIONSERVICE_H_
#define AUTOMATIONSERVICE_H_

#include <string>

#include "../beans/automation/Scenario.h"
#include "../beans/automation/Task.h"
#include "../utils/parser/xml/XMLParser.h"
#include "../utils/ParameterList.h"

class Attributs;

class AutomationService: public BaseObject, public XMLParser {
public:
    static const std::string SCENARIO_NODE_NAME;
    static const std::string TASK_NODE_NAME;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    AutomationService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~AutomationService();

    Scenario parseScenarioFile(const std::string &scenarioFilePath);

private:
    Scenario m_scenario;

    // Temporary object
    Task m_task;
    // Temporary object
    ParameterList m_parameterList;

    // ##### XML PARSER METHOD #####
    virtual void startElement(const std::string &elementName,
            Attributs attributes, const std::string &elementData);
    virtual void endElement(const std::string &elementName);
};

#endif /* AUTOMATIONSERVICE_H_ */
