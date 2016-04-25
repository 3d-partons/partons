#include "../../../../include/partons/services/automation/DefaultXMLParser.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/parser/XMLAttributs.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../include/partons/beans/automation/Task.h"

DefaultXMLParser::DefaultXMLParser() :
        XMLParserI("DefaultXMLParser") {
}

DefaultXMLParser::~DefaultXMLParser() {
}

Scenario DefaultXMLParser::parseScenarioXMLFile(
        const std::string& xmlFilePath) {
    // parse XML file
    analyse(xmlFilePath);

    m_scenario.setFilePath(xmlFilePath);

    return m_scenario;
}

void DefaultXMLParser::startElement(const std::string &elementName,
        ElemUtils::XMLAttributs attributes, const std::string &elementData) {

    debug(__func__,
            ElemUtils::Formatter() << "StartElementName = " << elementName);
    debug(__func__,
            ElemUtils::Formatter() << "Attributs : \n"
                    << attributes.toString());

    // if reached start element node is scenario
    // then retrieve its description attribute
    if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::SCENARIO_NODE_NAME)) {
        m_scenario = Scenario();

        std::string scenarioDescription = attributes.getStringValueOf(
                "description");
        m_scenario.setDescription(scenarioDescription);

        //TODO how to get date format ?
    }

    // if reached start element node is task
    // then retrieve its service, method and storeInDB attributes
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::TASK_NODE_NAME)) {
        // create a new temporary Task objet.
        m_task = Task();

        m_task.setServiceName(
                attributes.getStringValueOf(
                        XMLParserI::TASK_SERVICE_ATTRIBUT_NAME));
        m_task.setFunctionName(
                attributes.getStringValueOf(
                        XMLParserI::TASK_METHOD_ATTRIBUT_NAME));

        if (attributes.isAvailable(
                XMLParserI::TASK_STORE_IN_DB_ATTRIBUT_NAME)) {
            m_task.setStoreInDB(
                    attributes.getBooleanValueOf(
                            XMLParserI::TASK_STORE_IN_DB_ATTRIBUT_NAME));
        }
    }

    // if reached start element node is kinematics
    // then retrieve its type attribute
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::KINEMATICS_NODE_NAME)) {
        m_tempObjectType = attributes.getStringValueOf(
                XMLParserI::NODE_TYPE_ATTRIBUT_NAME);
    }

    // if reached start element node is module
    // then retrieve its type attribute
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::MODULE_NODE_NAME)) {
        m_tempObjectType = attributes.getStringValueOf(
                XMLParserI::NODE_TYPE_ATTRIBUT_NAME);
    }

    // if reached start element node is param
    // then retrieve its name and value attributes and add them into the temporary parameters list.
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::PARAM_NODE_NAME)) {
        m_tempObjectParameters.add(
                attributes.getStringValueOf(
                        XMLParserI::PARAM_NAME_ATTRIBUT_NAME),
                attributes.getStringValueOf(
                        XMLParserI::PARAM_VALUE_ATTRIBUT_NAME));
    }
}

void DefaultXMLParser::endElement(const std::string& elementName) {
    debug(__func__,
            ElemUtils::Formatter() << "EndElementName = " << elementName);

    // If the end node task is reached is that the temporary task is fill and it can be stored into the scenario.
    if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::TASK_NODE_NAME)) {
        m_task.setScenarioTaskIndexNumber(m_scenario.size());
        m_scenario.add(m_task);
    }

    // else it's that the object parameterization is over and it can be stored into the temporary task object
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::KINEMATICS_NODE_NAME)
            || ElemUtils::StringUtils::equals(elementName,
                    XMLParserI::MODULE_NODE_NAME)) {
        m_task.addParameters(m_tempObjectType, m_tempObjectParameters);

        // temporary parameterList object need to be cleared for the next object parameterization
        m_tempObjectParameters.clear();
    }
}
