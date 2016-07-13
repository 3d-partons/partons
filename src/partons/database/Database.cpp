#include "../../../include/partons/database/Database.h"

const std::string Database::TABLE_NAME_COMPUTATION = "computation";
const std::string Database::COLUMN_NAME_COMPUTATION_ID = "computation_id";

const std::string Database::TABLE_NAME_SCENARIO = "scenario";
const std::string Database::COLUMN_NAME_SCENARIO_ID = "scenario_id";
const std::string Database::COLUMN_NAME_SCENARIO_STORE_DATE =
        "scenario_store_date";
const std::string Database::COLUMN_NAME_SCENARIO_DESCRIPTION =
        "scenario_description";
const std::string Database::COLUMN_NAME_SCENARIO_XML_FILE = "scenario_xml_file";
const std::string Database::COLUMN_NAME_SCENARIO_HASH_SUM = "scenario_hash_sum";

const std::string Database::TABLE_NAME_ENVIRONMENT_CONFIGURATION =
        "environment_configuration";
const std::string Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_ID =
        "env_conf_id";
const std::string Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_STORE_DATE =
        "env_conf_store_date";
const std::string Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_CONFIGURATION =
        "env_conf_configuration";
const std::string Database::COLUMN_NAME_ENVIRONMENT_CONFIGURATION_HASH_SUM =
        "env_conf_hash_sum";

const std::string Database::TABLE_NAME_GPD_KINEMATIC = "gpd_kinematic";
const std::string Database::COLUMN_NAME_GPD_KINEMATIC_ID = "gpd_kinematic_id";

const std::string Database::TABLE_NAME_GPD_RESULT = "gpd_result";

const std::string Database::TABLE_NAME_OBSERVABLE_KINEMATIC =
        "observable_kinematic";
const std::string Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID =
        "observable_kinematic_id";

const std::string Database::TABLE_NAME_OBSERVABLE_RESULT = "observable_result";
const std::string Database::COLUMN_NAME_OBSERVABLE_RESULT_ID =
        "observable_result_id";
