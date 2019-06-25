#include "../../../include/partons/database/Database.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qmap.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>

#include "../../../include/partons/Partons.h"

namespace PARTONS {

const std::string Database::TABLE_NAME_COMPUTATION = "computation";
const std::string Database::COLUMN_NAME_COMPUTATION_ID = "computation_id";
const std::string Database::COLUMN_NAME_COMPUTATION_MODULE_NAME =
        "computation_module_name";

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
const std::string Database::COLUMN_NAME_GPD_RESULT_ID = "gpd_result_id";

const std::string Database::COLUMN_NAME_PARTON_DISTRIBUTION_ID =
        "parton_distribution_id";

const std::string Database::TABLE_NAME_OBSERVABLE_KINEMATIC =
        "observable_kinematic";
const std::string Database::COLUMN_NAME_OBSERVABLE_KINEMATIC_ID =
        "observable_kinematic_id";

const std::string Database::TABLE_NAME_OBSERVABLE_RESULT = "observable_result";
const std::string Database::COLUMN_NAME_OBSERVABLE_RESULT_ID =
        "observable_result_id";
const std::string Database::COLUMN_NAME_OBSERVABLE_NAME = "observable_name";
const std::string Database::COLUMN_NAME_OBSERVABLE_VALUE = "observable_value";

const std::string Database::COLUMN_NAME_GPD_TYPE_ID = "gpd_type_id";

const std::string Database::COLUMN_NAME_GLUON_DISTRIBUTION =
        "gluon_distribution_value";

const std::string Database::COLUMN_NAME_QUARK_FLAVOR_ID = "quark_flavor_id";

const std::string Database::COLUMN_NAME_QUARK_DISTRIBUTION =
        "quark_distribution";
const std::string Database::COLUMN_NAME_QUARK_DISTRIBUTION_PLUS =
        "quark_distribution_plus";
const std::string Database::COLUMN_NAME_QUARK_DISTRIBUTION_MINUS =
        "quark_distribution_minus";

const std::string Database::TABLE_NAME_CCF_KINEMATIC = "ccf_kinematic";
const std::string Database::COLUMN_NAME_CCF_KINEMATIC_ID = "ccf_kinematic_id";

const std::string Database::TABLE_NAME_CCF_RESULT = "ccf_result";
const std::string Database::COLUMN_NAME_CCF_RESULT_ID = "ccf_result_id";

int Database::getNumberOfRows(const QSqlQuery& query) {

    //debug
    Partons::getInstance()->getLoggerManager()->debug("DatabaseManager",
            __func__, "Processing ...");

    //check if select
    if (query.isSelect()) {
        return query.size();
    } else {
        return query.numRowsAffected();
    }
}

int Database::execSelectQuery(QSqlQuery& query) {

    //execute
    if (query.exec()) {

        //debug
        Partons::getInstance()->getLoggerManager()->debug("Database", __func__,
                getLastExecutedQuery(query));

        //check if select
        if (!query.isSelect()) {
            throw ElemUtils::CustomException("Database", __func__,
                    ElemUtils::Formatter()
                            << "Executed query is not a SELECT query ; please check your implementation");
        }

        //check if active, if yes return number of rows
        if (!query.isActive()) {
            throw ElemUtils::CustomException("Database", __func__,
                    ElemUtils::Formatter()
                            << "Current query is inactive ; Cannot perform next task(s) on it : "
                            << getLastExecutedQuery(query));
        } else {
            return Database::getNumberOfRows(query);
        }
    } else {
        throw ElemUtils::CustomException("Database", __func__,
                ElemUtils::Formatter() << "Cannot execute query : "
                        << getLastExecutedQuery(query) << " because : "
                        << query.lastError().text().toStdString());
    }

    return 0;
}

std::string Database::getLastExecutedQuery(const QSqlQuery& query) {
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    while (it.hasNext()) {
        it.next();
        str.replace(it.key(), it.value().toString());
    }
    return str.toStdString();
}

bool Database::checkUniqueResult(const std::string &className,
        const std::string &funcName, const unsigned int resultSize,
        const QSqlQuery& query) {
    if (resultSize != 1) {
        throw ElemUtils::CustomException(className, funcName,
                ElemUtils::Formatter()
                        << "More than 1 result have been found - there is an integrity problem in your database for query : "
                        << getLastExecutedQuery(query));
    }

    return true;
}

bool Database::checkManyResults(const std::string &className,
        const std::string &funcName, const unsigned int resultSize,
        const QSqlQuery& query) {
    if (resultSize == 0) {
        throw ElemUtils::CustomException(className, funcName,
                ElemUtils::Formatter()
                        << "Cannot found any result with query : "
                        << getLastExecutedQuery(query));
    }

    return true;
}

std::string Database::getPreFormatedColumnNamesFromVector(
        const std::vector<std::string>& columnNames) {
    ElemUtils::Formatter formatter;

    for (unsigned int i = 0; i != columnNames.size(); i++) {
        if (i == 0) {
            formatter << "(" << columnNames[i];
        } else {
            formatter << ", " << columnNames[i];
        }

    }

    formatter << ")";

    return formatter.str();
}

std::string Database::getPreFormatedColumnValuesFromVector(
        const std::vector<std::string>& columnValues) {
    ElemUtils::Formatter formatter;

    for (unsigned int i = 0; i != columnValues.size(); i++) {
        if (i == 0) {
            formatter << "VALUES (" << columnValues[i];
        } else {
            formatter << ", " << columnValues[i];
        }

    }

    formatter << ")";

    return formatter.str();
}

} /* namespace PARTONS */
