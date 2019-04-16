#include "../../../../../include/partons/database/gpd/dao/GPDResultDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>

#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/database/Database.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

GPDResultDao::GPDResultDao() :
        BaseObject("GPDResultDao") {
}

GPDResultDao::~GPDResultDao() {
}

int GPDResultDao::insertResult(const std::string &computationModuleName,
        int gpdKinematicId, int computationId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO gpd_result (computation_module_name, gpd_kinematic_id, computation_id) VALUES (:computationModuleName, :gpdKinematicId, :computationId)");

    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":gpdKinematicId", gpdKinematicId);
    query.bindValue(":computationId", computationId);

    //execute
    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return result;
}

int GPDResultDao::insertIntoGPDResultPartonDistributionTable(
        const int gpdTypeId, const int gpdResultId,
        const int partonDistributionId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO gpd_result_parton_distribution (gpd_type_id, gpd_result_id, parton_distribution_id) VALUES (:gpdTypeId, :gpdResultId, :partonDistributionId)");

    query.bindValue(":gpdTypeId", gpdTypeId);
    query.bindValue(":gpdResultId", gpdResultId);
    query.bindValue(":partonDistributionId", partonDistributionId);

    //execute
    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.lastInsertId().toInt();

    return result;
}

List<GPDResult> GPDResultDao::getGPDResultListByComputationId(
        const int computationId) const {

    //result
    List<GPDResult> resultList;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM gpd_result_view WHERE computation_id = :computationId");

    query.bindValue(":computationId", computationId);

    //execute
    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    //fill
    fillGPDResultList(resultList, query);

    return resultList;
}

void GPDResultDao::fillGPDResultList(List<GPDResult> &gpdResultList,
        QSqlQuery &query) const {

    //get indices
    int field_gpd_result_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_GPD_RESULT_ID.c_str()));
    int field_parton_distribution_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_PARTON_DISTRIBUTION_ID.c_str()));
    int field_computation_module_name = query.record().indexOf(
            QString(Database::COLUMN_NAME_COMPUTATION_MODULE_NAME.c_str()));
    int field_gpd_type_id = query.record().indexOf(
            QString(Database::COLUMN_NAME_GPD_TYPE_ID.c_str()));
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

    //results
    GPDResult gpdResult;
    PartonDistribution partonDistribution;

    int currentGPDResultId = -1;
    int currentPartonDistributionId = -1;
    int previousGPDTypeId = -1;

    //first query
    if (query.first()) {

        //retrieve gpd_type_id
        previousGPDTypeId = query.value(field_gpd_type_id).toInt();

        //retrieve gpd_result_id
        currentGPDResultId = query.value(field_gpd_result_id).toInt();
        gpdResult.setIndexId(currentGPDResultId);

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

        //retrieve gpd_result_id
        currentGPDResultId = query.value(field_gpd_result_id).toInt();

        //new gpd result
        if (currentGPDResultId != gpdResult.getIndexId()) {

            //add
            gpdResult.addPartonDistribution(
                    static_cast<GPDType::Type>(previousGPDTypeId),
                    partonDistribution);

            //make new for next one
            partonDistribution = PartonDistribution();
            partonDistribution.setIndexId(currentPartonDistributionId);
            partonDistribution.setGluonDistribution(
                    GluonDistribution(
                            query.value(field_gluon_distribution).toDouble()));

            //add
            gpdResultList.add(gpdResult);

            //make new for next one
            gpdResult = GPDResult();
            gpdResult.setIndexId(currentGPDResultId);
        }

        //new partons distribution result
        if (currentPartonDistributionId != partonDistribution.getIndexId()) {

            //add
            gpdResult.addPartonDistribution(
                    static_cast<GPDType::Type>(previousGPDTypeId),
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

        //store gpd_type_id as previous value
        previousGPDTypeId = query.value(field_gpd_type_id).toInt();
    }

    //store last parsed result
    gpdResult.addPartonDistribution(
            static_cast<GPDType::Type>(previousGPDTypeId), partonDistribution);
    gpdResultList.add(gpdResult);
}

} /* namespace PARTONS */
