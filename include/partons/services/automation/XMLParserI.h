#ifndef XML_PARSER_I_H
#define XML_PARSER_I_H

/**
 * @file XMLParserI.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 23, 2016
 * @version 1.0
 */

#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/automation/Task.h"
#include "../../beans/List.h"

namespace PARTONS {

class Scenario;

/**
 * @class XMLParserI
 *
 * @brief
 */
class XMLParserI: public BaseObject {
public:
    static const std::string SCENARIO_NODE_NAME;
    static const std::string TASK_NODE_NAME;
    static const std::string KINEMATICS_NODE_NAME;
    static const std::string COMPUTATION_CONFIGURATION_NODE_NAME;
    static const std::string MODULE_NODE_NAME;
    static const std::string NODE_MODULE_TYPE_ATTRIBUT_NAME;
    static const std::string NODE_MODULE_NAME_ATTRIBUT_NAME;
    static const std::string PARAM_NODE_NAME;
    static const std::string TASK_SERVICE_ATTRIBUT_NAME;
    static const std::string TASK_METHOD_ATTRIBUT_NAME;
    static const std::string TASK_STORE_IN_DB_ATTRIBUT_NAME;
    static const std::string PARAM_NAME_ATTRIBUT_NAME;
    static const std::string PARAM_VALUE_ATTRIBUT_NAME;
    static const std::string TASK_PARAM_NODE_NAME;

    /**
     * Constructor
     *
     * @param className
     */
    XMLParserI(const std::string &className);

    /**
     * Destructor
     */
    virtual ~XMLParserI();

    /**
     *
     * @param xmlDocument
     * @return
     */
    virtual Scenario* parseScenario(Scenario* pSenario) = 0;

protected:
    Scenario* m_pScenario; ///<
    // Temporary object
    Task m_task;    ///<

    // Temporary object
    // unsigned int m_computationConfigurationXMLFileIndex;    ///<

    BaseObjectData m_modulesData;
    BaseObjectData m_kinematicsData;
    BaseObjectData m_taskParametersData;

    List<BaseObjectData*> m_currentModuleHierarchy;
};

} /* namespace PARTONS */

#endif /* XML_PARSER_I_H */
