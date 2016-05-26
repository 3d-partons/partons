#include "../../../../../include/partons/database/gpd/service/GPDResultDaoService.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtSql/qsqldatabase.h>
#include <exception>
#include <map>

#include "../../../../../include/partons/beans/Computation.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/system/ResultInfo.h"

GPDResultDaoService::GPDResultDaoService() :
        BaseObject("GPDResultDaoService"), m_previousComputationId(
                std::make_pair<time_t, int>(0, -1)) {
}

GPDResultDaoService::~GPDResultDaoService() {

}

int GPDResultDaoService::insert(const GPDResult &gpdResult) const {

    int computationId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {
        computationId = insertWithoutTransaction(gpdResult);

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {
        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return computationId;
}

//int GPDResultDaoService::insertWithoutTransactionResultList(
//        const ResultList<GPDResult> &gpdResultList) const {
//
//
//
//}

int GPDResultDaoService::insertWithoutTransaction(
        const GPDResult &gpdResult) const {

    int computationId = -1;
    time_t computationDateTime =
            gpdResult.getResultInfo().getComputation().getDateTime();

    // Check if previousComputationId stored is not the same as current
    if (computationDateTime == m_previousComputationId.first) {
        computationId = m_previousComputationId.second;
    } else {
        // Check if this computation date already exists and retrieve Id
        computationId = m_computationDaoService.getComputationIdByDateTime(
                computationDateTime);

        // If not, insert new entry in database and retrieve its id
        if (computationId == -1) {
            computationId = m_resultInfoDaoService.insertWithoutTransaction(
                    gpdResult.getResultInfo());
        }

        m_previousComputationId = std::make_pair<time_t, int>(
                computationDateTime, computationId);
    }

    // Check if this gpd_kinematic already exists
    int gpdKinematicId = m_gpdKinematicDaoService.getIdByKinematicObject(
            gpdResult.getKinematic());

    // If not, insert new entry in database and retrieve its id
    if (gpdKinematicId == -1) {
        gpdKinematicId = m_gpdKinematicDaoService.insertWithoutTransaction(
                gpdResult.getKinematic());
    }

    // Insert new gpd_result entry in database
    int gpdResultId = m_gpdResultDao.insertResult(
            gpdResult.getComputationModuleName(), gpdKinematicId,
            computationId);

    // Get all PartonDistribution objects indexed by GPDType
    std::map<GPDType::Type, PartonDistribution> partonDistributionMap =
            gpdResult.getPartonDistributions();

    // Defined a parton_distribution_id
    int partonDistributionId = -1;

    // Then loop over GPDType to store PartonDistribution objets into database.
    for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
            partonDistributionMap.begin(); it != partonDistributionMap.end();
            it++) {
        // Insert new PartonDistribution object into database ; retrieve its id
        partonDistributionId = m_partonDistributionDaoService.insert(
                (it->second));

        // Fill gpd_result_parton_distribution association table with previous retrieved ids.
        m_gpdResultDao.insertIntoGPDResultPartonDistributionTable((it->first),
                gpdResultId, partonDistributionId);
    }

    return computationId;
}

int GPDResultDaoService::insert(const List<GPDResult> &gpdResultList) const {

    info(__func__,
            ElemUtils::Formatter() << "Inserting object size = "
                    << gpdResultList.size());

    int computationId = -1;

//    unsigned int batchSize = 250;
//    unsigned int startListIndex = 0;
//    unsigned int numberOfIteration = gpdResultList.size() / batchSize();
//
//    for()
//    {
//
//    }
//
    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        for (unsigned int i = 0; i != gpdResultList.size(); i++) {
            computationId = insertWithoutTransaction(gpdResultList.get(i));
        }

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {

        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return computationId;
}

List<GPDResult> GPDResultDaoService::getGPDResultListByComputationId(
        const int computationId) const {
    return m_gpdResultDao.getGPDResultListByComputationId(computationId);
}
