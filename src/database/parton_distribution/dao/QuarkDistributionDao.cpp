#include "QuarkDistributionDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <string>

#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

QuarkDistributionDao::QuarkDistributionDao() :
        BaseObject("QuarkDistributionDao") {
}

QuarkDistributionDao::~QuarkDistributionDao() {
}

int QuarkDistributionDao::insert(double quarkDistributionPlus,
        double quarkDistributionMinus, double quarkDistribution,
        int quarkFlavorTypeId, int partonDistributionId) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getDb());

    query.prepare(
            "INSERT INTO quark_distribution (quark_distribution_plus, quark_distribution_minus, quark_distribution, quark_flavor_id, parton_distribution_id) VALUES (:quarkDistributionPlus, :quarkDistributionMinus, :quarkDistribution, :quarkFlavorTypeId, :partonDistributionId )");

    query.bindValue(":quarkDistributionPlus", quarkDistributionPlus);
    query.bindValue(":quarkDistributionMinus", quarkDistributionMinus);
    query.bindValue(":quarkDistribution", quarkDistribution);
    query.bindValue(":quarkFlavorTypeId", quarkFlavorTypeId);
    query.bindValue(":partonDistributionId", partonDistributionId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__, Formatter() << query.lastError().text().toStdString());
    }

    query.clear();

    return result;
}
