#include "PartonDistributionDaoService.h"

#include <QtSql/qsqldatabase.h>
#include <exception>
#include <map>
#include <utility>

#include "../../../beans/parton_distribution/GluonDistribution.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../../beans/parton_distribution/QuarkDistribution.h"
#include "../../../beans/QuarkFlavor.h"

PartonDistributionDaoService::PartonDistributionDaoService() :
        BaseObject("PartonDistributionDaoService") {
}

PartonDistributionDaoService::~PartonDistributionDaoService() {
}

int PartonDistributionDaoService::insert(
        const PartonDistribution &partonDistribution) const {
    int result = -1;

    // For multiple query it's better to use transaction to guarantee database's integrity and performance
    QSqlDatabase::database().transaction();

    try {

        result = insertWithoutTransaction(partonDistribution);

        // If there is no exception we can commit all query
        QSqlDatabase::database().commit();

    } catch (std::exception &e) {
        error(__func__, e.what());

        // Else return database in a stable state : n-1
        QSqlDatabase::database().rollback();
    }

    return result;
}

int PartonDistributionDaoService::insertWithoutTransaction(
        const PartonDistribution &partonDistribution) const {
    int partonDistributionId = m_partonDistributionDao.insert(
            partonDistribution.getGluonDistribution().getGluonDistribution());

    std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributionList =
            partonDistribution.getQuarkDistributions();

    for (std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator it =
            quarkDistributionList.begin(); it != quarkDistributionList.end();
            it++) {
        QuarkDistribution quarkDistribution = (it->second);
        m_quarkDistributionDao.insert(
                quarkDistribution.getQuarkDistributionPlus(),
                quarkDistribution.getQuarkDistributionMinus(),
                quarkDistribution.getQuarkDistribution(), (it->first),
                partonDistributionId);
    }

    return partonDistributionId;
}

//TODO re-implement
//int PartonDistributionDaoService::insert(int gpdResultId,
//        const std::vector<PartonDistribution> &partonDistributionList) const {
//    int result = -1;
//
//    for (unsigned int i = 0; i != partonDistributionList.size(); i++) {
//        result = insert(gpdResultId, partonDistributionList[i]);
//    }
//
//    return result;
//}
