#include "../../../../include/partons/services/automation/XMLParserI.h"

namespace PARTONS {


const std::string XMLParserI::SCENARIO_NODE_NAME = "scenario";
const std::string XMLParserI::TASK_NODE_NAME = "task";
const std::string XMLParserI::KINEMATICS_NODE_NAME = "kinematics";
const std::string XMLParserI::COMPUTATION_CONFIGURATION_NODE_NAME =
        "computation_configuration";
const std::string XMLParserI::MODULE_NODE_NAME = "module";
const std::string XMLParserI::NODE_MODULE_TYPE_ATTRIBUT_NAME = "type";
const std::string XMLParserI::NODE_MODULE_NAME_ATTRIBUT_NAME = "name";
const std::string XMLParserI::PARAM_NODE_NAME = "param";
const std::string XMLParserI::TASK_SERVICE_ATTRIBUT_NAME = "service";
const std::string XMLParserI::TASK_METHOD_ATTRIBUT_NAME = "method";
const std::string XMLParserI::TASK_STORE_IN_DB_ATTRIBUT_NAME = "storeInDB";
const std::string XMLParserI::PARAM_NAME_ATTRIBUT_NAME = "name";
const std::string XMLParserI::PARAM_VALUE_ATTRIBUT_NAME = "value";
const std::string XMLParserI::TASK_PARAM_NODE_NAME = "task_param";

XMLParserI::XMLParserI(const std::string &className) :
        BaseObject(className), m_pScenario(0) /*, m_computationConfigurationXMLFileIndex(
 0)*/{
}

XMLParserI::~XMLParserI() {
}


} /* namespace PARTONS */
