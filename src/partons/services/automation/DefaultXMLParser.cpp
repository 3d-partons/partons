#include "../../../../include/partons/services/automation/DefaultXMLParser.h"

#include <ElementaryUtils/logger/CustomException.h>
//#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>

#include "../../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../../include/partons/beans/automation/Scenario.h"
#include "../../../../include/partons/beans/automation/Task.h"
#include "../../../../include/partons/beans/List.h"

DefaultXMLParser::DefaultXMLParser() :
        XMLParserI("DefaultXMLParser"), m_isModuleNodePreviouslyCreated(false) {
}

DefaultXMLParser::~DefaultXMLParser() {
}

Scenario* DefaultXMLParser::parseScenario(Scenario* pScenario) {
    if (!pScenario) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Scenario provided pointer is NULL");
    }

    m_pScenario = pScenario;

    // parse XML file
    analyse(pScenario->getFile());

    debug(__func__, pScenario->toString());

    return m_pScenario;
}

void DefaultXMLParser::startElement(const std::string &elementName,
        ElemUtils::Parameters attributes, const std::string &elementData) {

    debug(__func__,
            ElemUtils::Formatter() << "StartElementName = " << elementName);
    debug(__func__,
            ElemUtils::Formatter() << "Attributs : \n"
                    << attributes.toString());

    // if reached start element node is scenario
    // then retrieve its description attribute
    if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::SCENARIO_NODE_NAME)) {
        if (attributes.isAvailable("description")) {
            m_pScenario->setDescription(
                    attributes.getLastAvailable().getString());
        }

        //TODO how to get date format ?
    }

    // if reached start element node is task
    // then retrieve its service, method and storeInDB attributes
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::TASK_NODE_NAME)) {
        // create a new temporary Task objet.
        m_task = Task();

        if (attributes.isAvailable(XMLParserI::TASK_SERVICE_ATTRIBUT_NAME)) {
            m_task.setServiceName(attributes.getLastAvailable().getString());
        }
        if (attributes.isAvailable(XMLParserI::TASK_METHOD_ATTRIBUT_NAME)) {
            m_task.setFunctionName(attributes.getLastAvailable().getString());
        }
        if (attributes.isAvailable(
                XMLParserI::TASK_STORE_IN_DB_ATTRIBUT_NAME)) {
            m_task.setStoreInDB(attributes.getLastAvailable().toBoolean());
        }
    }

    // if reached start element node is kinematics
    // then retrieve its type attribute
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::KINEMATICS_NODE_NAME)) {
        if (attributes.isAvailable(
                XMLParserI::NODE_MODULE_TYPE_ATTRIBUT_NAME)) {
            m_kinematicsData = BaseObjectData(
                    attributes.getLastAvailable().getString(),
                    attributes.getLastAvailable().getString());
        }

        m_currentModuleHierarchy.add(&m_kinematicsData);
    }

    // if reached start element node is module
    // then retrieve its type attribute
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::MODULE_NODE_NAME)) {
        m_isModuleNodePreviouslyCreated = true;

        std::string moduleType = ElemUtils::StringUtils::EMPTY;
        std::string moduleClassName = ElemUtils::StringUtils::EMPTY;

        if (attributes.isAvailable(
                XMLParserI::NODE_MODULE_TYPE_ATTRIBUT_NAME)) {
            moduleType = attributes.getLastAvailable().getString();
        }
        if (attributes.isAvailable(
                XMLParserI::NODE_MODULE_NAME_ATTRIBUT_NAME)) {
            moduleClassName = attributes.getLastAvailable().getString();
        }

        if (m_currentModuleHierarchy.size() == 0) {
            m_modulesData = BaseObjectData(moduleType, moduleClassName);
            m_currentModuleHierarchy.add(&m_modulesData);

            debug(__func__,
                    ElemUtils::Formatter() << m_modulesData.getModuleType()
                            << "[" << m_modulesData.getModuleClassName()
                            << "] added");
        } else {
            try {
                m_currentModuleHierarchy.add(
                        &m_currentModuleHierarchy.getLast()->addSubModule(
                                moduleType, moduleClassName));
            } catch (const ElemUtils::CustomException &e) {
                throw ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter() << e.what() << " : "
                                << elementName);
            }
        }
    }

    // if reached start element node is task_param
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::TASK_PARAM_NODE_NAME)) {

        if (attributes.isAvailable(
                XMLParserI::NODE_MODULE_TYPE_ATTRIBUT_NAME)) {

            m_currentModuleHierarchy.add(
                    &(m_taskParametersData.addSubModule(
                            attributes.getLastAvailable().getString(),
                            attributes.getLastAvailable().getString())));
        }

    }
}

void DefaultXMLParser::emptyStartElement(const std::string &elementName,
        ElemUtils::Parameters attributes) {

    debug(__func__,
            ElemUtils::Formatter() << "EmptyStartElementName = "
                    << elementName);
    debug(__func__,
            ElemUtils::Formatter() << "Attributs : \n"
                    << attributes.toString());

    if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::MODULE_NODE_NAME)) {

        std::string moduleType = ElemUtils::StringUtils::EMPTY;
        std::string moduleClassName = ElemUtils::StringUtils::EMPTY;

        if (attributes.isAvailable(
                XMLParserI::NODE_MODULE_TYPE_ATTRIBUT_NAME)) {
            moduleType = attributes.getLastAvailable().getString();
        }
        if (attributes.isAvailable(
                XMLParserI::NODE_MODULE_NAME_ATTRIBUT_NAME)) {
            moduleClassName = attributes.getLastAvailable().getString();
        }

        try {
            m_currentModuleHierarchy.getLast()->addSubModule(moduleType,
                    moduleClassName);
        } catch (const ElemUtils::CustomException &e) {
            m_modulesData = BaseObjectData(moduleType, moduleClassName);
        }
    }

    // if reached start element node is param
    // then retrieve its name and value attributes and add them into the temporary parameters list.
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::PARAM_NODE_NAME)) {
        try {

            std::string paramName = ElemUtils::StringUtils::EMPTY;
            std::string paramValue = ElemUtils::StringUtils::EMPTY;

            if (attributes.isAvailable(XMLParserI::PARAM_NAME_ATTRIBUT_NAME)) {
                paramName = attributes.getLastAvailable().getString();
            }
            if (attributes.isAvailable(XMLParserI::PARAM_VALUE_ATTRIBUT_NAME)) {
                paramValue = attributes.getLastAvailable().getString();
            }

            m_currentModuleHierarchy.getLast()->addParameter(
                    ElemUtils::Parameter(paramName, paramValue));

        } catch (const ElemUtils::CustomException &e) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << e.what() << " : " << elementName
                            << " : " << attributes.toString());
        }
    }

}
void DefaultXMLParser::endElement(const std::string& elementName) {
    debug(__func__,
            ElemUtils::Formatter() << "EndElementName = " << elementName);

    // If the end node task is reached is that the temporary task is fill and it can be stored into the scenario.
    if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::TASK_NODE_NAME)) {

        m_task.setScenarioTaskIndexNumber(m_pScenario->size());
        m_task.setModuleComputationConfiguration(m_modulesData);
        m_task.setTaskParameters(m_taskParametersData);
        m_pScenario->add(m_task);

        //clear previous temporary data
        m_kinematicsData = BaseObjectData();
        m_modulesData = BaseObjectData();
        m_taskParametersData = BaseObjectData();
    }

    // else it's that the object parameterization is over and it can be stored into the temporary task object
    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::KINEMATICS_NODE_NAME)
            || ElemUtils::StringUtils::equals(elementName,
                    XMLParserI::TASK_PARAM_NODE_NAME)) {

        try {
            m_currentModuleHierarchy.removeLast();
        } catch (const ElemUtils::CustomException &e) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    e.what());
        }

        m_task.setKinematicsData(m_kinematicsData);
    }

    else if (ElemUtils::StringUtils::equals(elementName,
            XMLParserI::MODULE_NODE_NAME)) {
        if (m_isModuleNodePreviouslyCreated) {
            m_isModuleNodePreviouslyCreated = false;
        }

        try {
            m_currentModuleHierarchy.removeLast();
        } catch (const ElemUtils::CustomException &e) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    e.what());
        }
    }
}
