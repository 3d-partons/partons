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
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO gpd_result (computation_module_name, gpd_kinematic_id, computation_id) VALUES (:computationModuleName, :gpdKinematicId, :computationId)");

    query.bindValue(":computationModuleName",
            QString(computationModuleName.c_str()));
    query.bindValue(":gpdKinematicId", gpdKinematicId);
    query.bindValue(":computationId", computationId);

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
    int result = -1;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO gpd_result_parton_distribution (gpd_type_id, gpd_result_id, parton_distribution_id) VALUES (:gpdTypeId, :gpdResultId, :partonDistributionId)");

    query.bindValue(":gpdTypeId", gpdTypeId);
    query.bindValue(":gpdResultId", gpdResultId);
    query.bindValue(":partonDistributionId", partonDistributionId);

    Database::checkUniqueResult(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    result = query.lastInsertId().toInt();

    return result;
}

List<GPDResult> GPDResultDao::getGPDResultListByComputationId(
        const int computationId) const {
    debug(__func__, "Processing ...");

    List<GPDResult> resultList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

//    query.prepare(
//            "SELECT gr.id, pd.id, gr.computation_module_name, grpd.gpd_type_id, pd.gluon_distribution_value, qd.quark_flavor_id, qd.quark_distribution, qd.quark_distribution_plus, qd.quark_distribution_minus FROM gpd_result gr INNER JOIN computation c ON gr.computation_id = c.id INNER JOIN gpd_result_parton_distribution grpd ON gr.id = grpd.gpd_result_id INNER JOIN parton_distribution pd ON grpd.parton_distribution_id = pd.id INNER JOIN parton_distribution_quark_distribution pdqd ON pd.id = pdqd.parton_distribution_id INNER JOIN quark_distribution qd ON pdqd.quark_distribution_id = qd.id WHERE gr.computation_id = :computationId ORDER BY gr.id");

//TODO check view
    query.prepare(
            "SELECT * FROM gpd_result_view WHERE computation_id = :computationId");

    query.bindValue(":computationId", computationId);

    info(__func__, "Executing query ...");

    Database::checkManyResults(getClassName(), __func__,
            Database::execSelectQuery(query), query);

    fillGPDResultList(resultList, query);

    return resultList;
}

//TODO refactoring : remove redundancies
void GPDResultDao::fillGPDResultList(List<GPDResult> &gpdResultList,
        QSqlQuery &query) const {

    info(__func__, "Preparing retrieved data ...");

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

    GPDResult gpdResult;
    PartonDistribution partonDistribution;

    int currentGPDResultId = -1;
    int currentPartonDistributionId = -1;
    int previousGPDTypeId = -1;

    if (query.first()) {
        // retrieve gpd_type_id
        previousGPDTypeId = query.value(field_gpd_type_id).toInt();

        // retrieve parton_distribution_id
        currentPartonDistributionId =
                query.value(field_parton_distribution_id).toInt();

        // retrieve gpd_result_id
        currentGPDResultId = query.value(field_gpd_result_id).toInt();

        gpdResult.setIndexId(currentGPDResultId);

        partonDistribution.setIndexId(currentPartonDistributionId);
        partonDistribution.setGluonDistribution(
                GluonDistribution(
                        query.value(field_gluon_distribution).toDouble()));

        QuarkDistribution quarkDistribution = QuarkDistribution(
                static_cast<QuarkFlavor::Type>(query.value(
                        field_quark_flavor_id).toInt()),
                query.value(field_quark_distribution).toDouble(),
                query.value(field_quark_distribution_plus).toDouble(),
                query.value(field_quark_distribution_minus).toDouble());

        // retrieve QuarkDistribution with quark_flavor_id, quark_distribution, quark_distribution_plus & quark_distribution_minus
        partonDistribution.addQuarkDistribution(quarkDistribution);
    }

    while (query.next()) {
        // retrieve parton_distribution_id
        currentPartonDistributionId =
                query.value(field_parton_distribution_id).toInt();

        // retrieve gpd_result_id
        currentGPDResultId = query.value(field_gpd_result_id).toInt();

        if (currentGPDResultId != gpdResult.getIndexId()) {
            gpdResult.addPartonDistribution(
                    static_cast<GPDType::Type>(previousGPDTypeId),
                    partonDistribution);

            partonDistribution = PartonDistribution();
            partonDistribution.setIndexId(currentPartonDistributionId);
            partonDistribution.setGluonDistribution(
                    GluonDistribution(
                            query.value(field_gluon_distribution).toDouble()));

            gpdResultList.add(gpdResult);
            gpdResult = GPDResult();
            gpdResult.setIndexId(currentGPDResultId);
        }

        if (currentPartonDistributionId != partonDistribution.getIndexId()) {
            gpdResult.addPartonDistribution(
                    static_cast<GPDType::Type>(previousGPDTypeId),
                    partonDistribution);
            partonDistribution = PartonDistribution();
            partonDistribution.setIndexId(currentPartonDistributionId);
            partonDistribution.setGluonDistribution(
                    GluonDistribution(
                            query.value(field_gluon_distribution).toDouble()));
        }

        QuarkDistribution quarkDistribution = QuarkDistribution(
                static_cast<QuarkFlavor::Type>(query.value(
                        field_quark_flavor_id).toInt()),
                query.value(field_quark_distribution).toDouble(),
                query.value(field_quark_distribution_plus).toDouble(),
                query.value(field_quark_distribution_minus).toDouble());

        // retrieve QuarkDistribution with quark_flavor_id, quark_distribution, quark_distribution_plus & quark_distribution_minus
        partonDistribution.addQuarkDistribution(quarkDistribution);

        // store gpd_type_id as previous value
        previousGPDTypeId = query.value(field_gpd_type_id).toInt();
    }

    // store last parsed result
    gpdResult.addPartonDistribution(
            static_cast<GPDType::Type>(previousGPDTypeId), partonDistribution);
    gpdResultList.add(gpdResult);

    info(__func__, "Done !");
}

} /* namespace PARTONS */
