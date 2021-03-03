#include "../../../../../include/partons/database/collinear_distribution/dao/CollinearDistributionResultDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/beans/collinear_distribution/CollinearDistributionType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

CollinearDistributionResultDao::CollinearDistributionResultDao() :
        BaseObject("CollinearDistributionResultDao") {
}

CollinearDistributionResultDao::~CollinearDistributionResultDao() {
}

int CollinearDistributionResultDao::insertCollinearDistributionResult(const std::string &computationModuleName,
        int colldistKinematicId, int computationId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO collinear_distribution_result (computation_module_name, collinear_distribution_kinematic_id, computation_id) VALUES (:computationModuleName, :colldistKinematicId, :computationId)");

    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":colldistKinematicId", colldistKinematicId);
    query.bindValue(":computationId", computationId);

    //execute
    if (query.exec()) {

        //get result
        result = query.lastInsertId().toInt();
    } else {

        //thrown if error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

int CollinearDistributionResultDao::insertIntoCollinearDistributionResultPartonDistributionTable(
        const int colldistTypeId, const int colldistResultId,
        const int partonDistributionId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO collinear_distribution_result_parton_distribution (collinear_distribution_type_id, collinear_distribution_result_id, parton_distribution_id) VALUES (:colldistTypeId, :colldistResultId, :partonDistributionId)");

    query.bindValue(":colldistTypeId", colldistTypeId);
    query.bindValue(":colldistResultId", colldistResultId);
    query.bindValue(":partonDistributionId", partonDistributionId);

    //execute
    if (query.exec()) {

        //get result
        result = query.lastInsertId().toInt();
    } else {

        //thrown if error
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

List<CollinearDistributionResult> CollinearDistributionResultDao::getCollinearDistributionResultListByComputationId(
        const int computationId) const {

    //result
    List<CollinearDistributionResult> resultList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM collinear_distribution_result_view WHERE computation_id = :computationId");

    query.bindValue(":computationId", computationId);

    //execute
    if (Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query)) {

        //fill
        fillCollinearDistributionResultList(resultList, query);
    }

    return resultList;
}

void CollinearDistributionResultDao::fillCollinearDistributionResultList(List<CollinearDistributionResult> &colldistResultList,
        QSqlQuery &query) const {

    //get indices
    int field_collinear_distribution_result_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_COLLINEAR_DISTRIBUTION_RESULT_ID.c_str()));
    int field_parton_distribution_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_PARTON_DISTRIBUTION_ID.c_str()));
    int field_computation_module_name = query.record().indexOf(
            QString(Database::COLUMN_NAME_COMPUTATION_MODULE_NAME.c_str()));
    int field_collinear_distribution_type_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_COLLINEAR_DISTRIBUTION_TYPE_ID.c_str()));
    int field_gluon_distribution = query.record().indexOf(
            QString(Database::COLUMN_NAME_GLUON_DISTRIBUTION.c_str()));
    int field_quark_flavor_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_QUARK_FLAVOR_ID.c_str()));
    int field_quark_distribution = query.record().indexOf(
            QString(Database::COLUMN_NAME_QUARK_DISTRIBUTION.c_str()));
    int field_quark_distribution_plus = query.record().indexOf(
            QString(Database::COLUMN_NAME_QUARK_DISTRIBUTION_PLUS.c_str()));
    int field_quark_distribution_minus = query.record().indexOf(
            QString(Database::COLUMN_NAME_QUARK_DISTRIBUTION_MINUS.c_str()));
    int field_kinematic_id = query.record().indexOf("collinear_distribution_kinematic_id");

    //results
    CollinearDistributionResult colldistResult;
    PartonDistribution partonDistribution;

    int currentCollinearDistributionResultId = -1;
    int currentPartonDistributionId = -1;
    int previousCollinearDistributionTypeId = -1;

    //first query
    if (query.first()) {

        //retrieve collinear_distribution_type_id
        previousCollinearDistributionTypeId = query.value(field_collinear_distribution_type_id).toInt();

        //retrieve collinear_distribution_result_id
        currentCollinearDistributionResultId = query.value(field_collinear_distribution_result_id).toInt();
        colldistResult.setIndexId(currentCollinearDistributionResultId);

        //set kinematics
        colldistResult.setKinematic(
                m_colldistKinematicDao.getKinematicById(
                        query.value(field_kinematic_id).toInt()));

        //retrieve parton_distribution_id
        currentPartonDistributionId =
                query.value(field_parton_distribution_id).toInt();

        //set index id
        partonDistribution.setIndexId(currentPartonDistributionId);

        //set gluons
        partonDistribution.setGluonDistribution(
                GluonDistribution(
                        query.value(field_gluon_distribution).toDouble()));

        //set quarks
        QuarkDistribution quarkDistribution = QuarkDistribution(
                static_cast<QuarkFlavor::Type>(query.value(
                        field_quark_flavor_id).toInt()),
                query.value(field_quark_distribution).toDouble(),
                query.value(field_quark_distribution_plus).toDouble(),
                query.value(field_quark_distribution_minus).toDouble());

        //add
        partonDistribution.addQuarkDistribution(quarkDistribution);
    }

    //loop over single queries
    while (query.next()) {

        //retrieve parton_distribution_id
        currentPartonDistributionId =
                query.value(field_parton_distribution_id).toInt();

        //retrieve collinear_distribution_result_id
        currentCollinearDistributionResultId = query.value(field_collinear_distribution_result_id).toInt();

        //new collinear_distribution result
        if (currentCollinearDistributionResultId != colldistResult.getIndexId()) {

            //add
            colldistResult.addPartonDistribution(
                    static_cast<CollinearDistributionType::Type>(previousCollinearDistributionTypeId),
                    partonDistribution);

            //make new for next one
            partonDistribution = PartonDistribution();
            partonDistribution.setIndexId(currentPartonDistributionId);
            partonDistribution.setGluonDistribution(
                    GluonDistribution(
                            query.value(field_gluon_distribution).toDouble()));

            //add
            colldistResultList.add(colldistResult);

            //make new for next one
            colldistResult = CollinearDistributionResult();
            colldistResult.setIndexId(currentCollinearDistributionResultId);

            //set kinematics
            colldistResult.setKinematic(
                    m_colldistKinematicDao.getKinematicById(
                            query.value(field_kinematic_id).toInt()));
        }

        //new partons distribution result
        if (currentPartonDistributionId != partonDistribution.getIndexId()) {

            //add
            colldistResult.addPartonDistribution(
                    static_cast<CollinearDistributionType::Type>(previousCollinearDistributionTypeId),
                    partonDistribution);

            //make new for next one
            partonDistribution = PartonDistribution();
            partonDistribution.setIndexId(currentPartonDistributionId);
            partonDistribution.setGluonDistribution(
                    GluonDistribution(
                            query.value(field_gluon_distribution).toDouble()));
        }

        //set quarks
        QuarkDistribution quarkDistribution = QuarkDistribution(
                static_cast<QuarkFlavor::Type>(query.value(
                        field_quark_flavor_id).toInt()),
                query.value(field_quark_distribution).toDouble(),
                query.value(field_quark_distribution_plus).toDouble(),
                query.value(field_quark_distribution_minus).toDouble());

        //add
        partonDistribution.addQuarkDistribution(quarkDistribution);

        //store collinear_distribution_type_id as previous value
        previousCollinearDistributionTypeId = query.value(field_collinear_distribution_type_id).toInt();
    }

    //store last parsed result
    colldistResult.addPartonDistribution(
            static_cast<CollinearDistributionType::Type>(previousCollinearDistributionTypeId), partonDistribution);
    colldistResultList.add(colldistResult);
}

} /* namespace PARTONS */
