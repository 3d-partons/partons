#include "QuarkDistributionDao.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>

#include "../DatabaseManager.h"

int QuarkDistributionDao::insert(int partonDistributionId,
        int quarkFlavorTypeId, double quarkDistribution,
        double quarkDistributionMinus, double quarkDistributionPlus) {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO quark_distribution (parton_distribution_id, quark_flavor_id, quark_distribution, quark_distribution_minus, quark_distribution_plus) VALUES (:partonDistributionId, :quarkFlavorTypeId, :quarkDistribution, :quarkDistributionMinus, :quarkDistributionPlus )");

    query.bindValue(":partonDistributionId", partonDistributionId);
    query.bindValue(":quarkFlavorTypeId", quarkFlavorTypeId);
    query.bindValue(":quarkDistribution", quarkDistribution);
    query.bindValue(":quarkDistributionMinus", quarkDistributionMinus);
    query.bindValue(":quarkDistributionPlus", quarkDistributionPlus);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    }

    query.clear();

    return result;
}
