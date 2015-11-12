#include "GPDResultDaoService.h"

#include <QtSql/qsqldatabase.h>
#include <exception>
#include <vector>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/gpd/GPDResultList.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../service/ModuleDaoService.h"
#include "../../service/PartonDistributionDaoService.h"
#include "../../service/QuarkDistributionDaoService.h"

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

    // Retrieve module_id from moduleClassName
    int moduleId = ModuleDaoService::getModuleIdByClassName(
            gpdResult.getComputationModuleName());

    // Insert new gpd_result entry in database
    int gpdResultId = m_gpdResultDao.insert(moduleId, gpdKinematicId);

    std::vector<PartonDistribution> partonDistributionList =
            gpdResult.getPartonDistributionList();

    int partonDistributionId = 0;

    for (unsigned int i = 0; i != partonDistributionList.size(); i++) {
        partonDistributionId = PartonDistributionDaoService::insert(gpdResultId,
                partonDistributionList[i]);

        QuarkDistributionDaoService::insert(partonDistributionId,
                partonDistributionList[i].getVectorOfQuarkDistribution());
    }

    return gpdResultId;
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
