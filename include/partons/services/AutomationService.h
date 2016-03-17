#ifndef AUTOMATION_SERVICE_H
#define AUTOMATION_SERVICE_H

/**
 * @file AutomationService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 03 March 2016
 * @version 1.0
 *
 * @class AutomationService
 *
 * @brief
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/parser/XMLParser.h>
#include <string>

#include "../beans/automation/Scenario.h"
#include "../beans/automation/Task.h"

class AutomationService: public BaseObject, public ElemUtils::XMLParser {
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
    ElemUtils::Parameters m_parameterList;

    // ##### XML PARSER METHOD #####
    virtual void startElement(const std::string &elementName,
            ElemUtils::XMLAttributs attributes, const std::string &elementData);
    virtual void endElement(const std::string &elementName);
};

#endif /* AUTOMATION_SERVICE_H */
