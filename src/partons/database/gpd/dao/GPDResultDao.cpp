#include "../../../../../include/partons/database/gpd/dao/GPDResultDao.h"

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
#include "../../../../../include/partons/database/DatabaseManager.h"

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
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

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

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

//List<GPDResult> GPDResultDao::getGPDResultListByComputationId(
//        const int computationId) const {
//    List<GPDResult> resultList;
//
//    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());
//
//    query.prepare(
//            "SELECT * FROM gpd_result WHERE computation_id = :computationId");
//
//    query.bindValue(":computationId", computationId);
//
//    if (query.exec()) {
//        // TODO implement this test in other dao classes
//        if (DatabaseManager::getNumberOfRows(query) != 0) {
//            fillGPDResultList(resultList, query);
//        } else {
//            warn(__func__,
//                    ElemUtils::Formatter() << "No entry for computationId = "
//                            << computationId);
//        }
//    }
//
//    query.clear();
//
//    return resultList;
//}

List<GPDResult> GPDResultDao::getGPDResultListByComputationId(
        const int computationId) const {
    List<GPDResult> resultList;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

//    query.prepare(
//            "SELECT gr.id, pd.id, gr.computation_module_name, grpd.gpd_type_id, pd.gluon_distribution_value, qd.quark_flavor_id, qd.quark_distribution, qd.quark_distribution_plus, qd.quark_distribution_minus FROM gpd_result gr INNER JOIN computation c ON gr.computation_id = c.id INNER JOIN gpd_result_parton_distribution grpd ON gr.id = grpd.gpd_result_id INNER JOIN parton_distribution pd ON grpd.parton_distribution_id = pd.id INNER JOIN parton_distribution_quark_distribution pdqd ON pd.id = pdqd.parton_distribution_id INNER JOIN quark_distribution qd ON pdqd.quark_distribution_id = qd.id WHERE gr.computation_id = :computationId ORDER BY gr.id");

    //TODO check view
    query.prepare(
                "SELECT * FROM gpd_result_view WHERE computation_id = :computationId");

    query.bindValue(":computationId", computationId);

    info(__func__, "Executing query ...");

    if (query.exec()) {
        // TODO implement this test in other dao classes
        if (DatabaseManager::getNumberOfRows(query) != 0) {
            fillGPDResultList(resultList, query);
        } else {
            warn(__func__,
                    ElemUtils::Formatter()
                            << "No entries found for computationId = "
                            << computationId);
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return resultList;
}

//void GPDResultDao::fillGPDResultList(List<GPDResult> &gpdResultList,
//        QSqlQuery &query) const {
//
//    int f_gpd_result_id = query.record().indexOf("id");
//    int f_computation_module_name = query.record().indexOf(
//            "computation_module_name");
//    int f_kinematic_id = query.record().indexOf("gpd_kinematic_id");
//
//    while (query.next()) {
//        int gpdResultId = query.value(f_gpd_result_id).toInt();
//        int kinematicId = query.value(f_kinematic_id).toInt();
//        std::string computationModuleName = query.value(
//                f_computation_module_name).toString().toStdString();
//
//        //TODO create Computation, ResultInfo, ...
//
//        GPDResult gpdResult;
//
//        gpdResult.setKinematic(m_gpdKinematicDao.getKinematicById(kinematicId));
//        gpdResult.setComputationModuleName(computationModuleName);
//        gpdResult.setIndexId(gpdResultId);
//
//        fillGPDResult(gpdResult);
//
//        gpdResultList.add(gpdResult);
//    }
//}

void GPDResultDao::fillGPDResultList(List<GPDResult> &gpdResultList,
        QSqlQuery &query) const {

    info(__func__, "Preparing retrieved data ...");

    //TODO create Computation, ResultInfo, ...

    GPDResult previousGPDResult;
    PartonDistribution previousPartonDistribution;
    GPDType previousGPDType;

    int tempGPDResultId = -1;
    int tempPartonDistributionId = -1;

    GPDType tempGPDType;
    QuarkFlavor tempQuarkFlavor;
    QuarkDistribution tempQuarkDistribution;

    while (query.next()) {

        tempQuarkFlavor = QuarkFlavor(
                static_cast<QuarkFlavor::Type>(query.value(5).toInt()));
        tempGPDType = GPDType(
                static_cast<GPDType::Type>(query.value(3).toInt()));

        tempGPDResultId = query.value(0).toInt();
        tempPartonDistributionId = query.value(1).toInt();

        tempQuarkDistribution =
                QuarkDistribution(
                        QuarkFlavor(
                                static_cast<QuarkFlavor::Type>(query.value(5).toInt())).getType(),
                        query.value(6).toDouble(), query.value(7).toDouble(),
                        query.value(8).toDouble());

        if (tempGPDResultId != previousGPDResult.getIndexId()) {

            if (previousGPDResult.getIndexId() != -1) {
                previousGPDResult.addPartonDistribution(previousGPDType,
                        previousPartonDistribution);
                gpdResultList.add(previousGPDResult);
            }

            previousGPDResult = GPDResult();
            previousGPDResult.setIndexId(tempGPDResultId);
            previousGPDResult.setComputationModuleName(
                    query.value(2).toString().toStdString());
        }

        if (tempPartonDistributionId
                != previousPartonDistribution.getIndexId()) {

            if (previousPartonDistribution.getIndexId() != -1) {
                previousGPDResult.addPartonDistribution(previousGPDType,
                        previousPartonDistribution);
            }

            previousGPDType = tempGPDType;
            previousPartonDistribution = PartonDistribution();
            previousPartonDistribution.setIndexId(tempPartonDistributionId);
            previousPartonDistribution.setGluonDistribution(
                    GluonDistribution(query.value(4).toDouble()));
        }

        previousPartonDistribution.addQuarkDistribution(tempQuarkDistribution);
    }

    previousGPDResult.addPartonDistribution(previousGPDType,
            previousPartonDistribution);
    gpdResultList.add(previousGPDResult);

    info(__func__, "Done !");
}

void GPDResultDao::fillGPDResult(GPDResult &gpdResult) const {
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM gpd_result_parton_distribution WHERE gpd_result_id = :gpdResultId");

    query.bindValue(":gpdResultId", gpdResult.getIndexId());

    if (query.exec()) {

        int field_gpd_type_id = query.record().indexOf("gpd_type_id");
        int field_parton_distribution_id = query.record().indexOf(
                "parton_distribution_id");

        while (query.next()) {
            int gpd_type_id = query.value(field_gpd_type_id).toInt();
            int parton_distribution_id = query.value(
                    field_parton_distribution_id).toInt();

            gpdResult.addPartonDistribution(
                    static_cast<GPDType::Type>(gpd_type_id),
                    m_partonDistributionDao.getPartonDistributionById(
                            parton_distribution_id));
        }
    } else {
        error(__func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}
