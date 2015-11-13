#include "GPDResultDaoService.h"

#include <QtSql/qsqldatabase.h>
#include <exception>
#include <map>
#include <utility>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/gpd/GPDResultList.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"

GPDResultDaoService::GPDResultDaoService() :
        BaseObject("GPDResultDaoService") {
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

int GPDResultDaoService::insertWithoutTransaction(
        const GPDResult &gpdResult) const {

    // Check if this gpd_kinematic already exists
    int gpdKinematicId = m_gpdKinematicDaoService.select(
            gpdResult.getKinematic());

    // If not, insert new entry in database and retrieve its id
    if (gpdKinematicId == -1) {
        gpdKinematicId = m_gpdKinematicDaoService.insert(
                gpdResult.getKinematic());
    }

    // Check if this computation date already exists and retrieve Id
    int computationId = m_commonDaoService.getComputationId(
            gpdResult.getComputationDateTime());
    // If not, insert new entry in database and retrieve its id
    if (computationId == -1) {
        computationId = m_commonDaoService.insertComputation(
                gpdResult.getComputationDateTime());
    }

    // Insert new gpd_result entry in database
    int gpdResultId = m_gpdResultDao.insert(
            gpdResult.getComputationModuleName(), gpdKinematicId,
            computationId);

    std::map<GPDType::Type, PartonDistribution> partonDistributionMap =
            gpdResult.getPartonDistributions();

    int partonDistributionId = 0;

    for (std::map<GPDType::Type, PartonDistribution>::const_iterator it =
            partonDistributionMap.begin(); it != partonDistributionMap.end();
            it++) {
        PartonDistribution partonDistribution = (it->second);
        partonDistributionId = m_partonDistributionDaoService.insert(
                partonDistribution);

        m_gpdResultPartonDistributionDao.insert((it->first), gpdResultId,
                partonDistributionId);
    }

    return computationId;
}

int GPDResultDaoService::insert(const GPDResultList &gpdResultList) const {
    int computationId = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        for (unsigned int i = 0; i != gpdResultList.getSize(); i++) {
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
