#include "../../../../../include/partons/database/parton_distribution/dao/QuarkDistributionDao.h"

#include <QSqlError>
#include <QVariant>
#include <QString>
#include <QSqlRecord>
#include <string>

#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/database/DatabaseManager.h"
#include "../../../../../include/partons/utils/stringUtils/Formatter.h"

QuarkDistributionDao::QuarkDistributionDao() :
        BaseObject("QuarkDistributionDao") {
}

QuarkDistributionDao::~QuarkDistributionDao() {
}

int QuarkDistributionDao::insert(double quarkDistributionPlus,
        double quarkDistributionMinus, double quarkDistribution,
        int quarkFlavorTypeId, int partonDistributionId) const {

    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO quark_distribution (quark_distribution_plus, quark_distribution_minus, quark_distribution, quark_flavor_id) VALUES (:quarkDistributionPlus, :quarkDistributionMinus, :quarkDistribution, :quarkFlavorTypeId )");

    query.bindValue(":quarkDistributionPlus", quarkDistributionPlus);
    query.bindValue(":quarkDistributionMinus", quarkDistributionMinus);
    query.bindValue(":quarkDistribution", quarkDistribution);
    query.bindValue(":quarkFlavorTypeId", quarkFlavorTypeId);

    if (query.exec()) {
        result = query.lastInsertId().toInt();
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return result;
}

QuarkDistribution QuarkDistributionDao::getQuarkDistributionById(
        const int quarkDistributionId) const {

    QuarkDistribution quarkDistribution;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM quark_distribution WHERE id = :quarkDistributionId");

    query.bindValue(":quarkDistributionId", quarkDistributionId);

    if (query.exec()) {

        fillQuarkDistributionFromQuery(quarkDistribution, query);

    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return quarkDistribution;

}

void QuarkDistributionDao::fillQuarkDistributionFromQuery(
        QuarkDistribution &quarkDistribution, QSqlQuery &query) const {

    // mapping index - column name
    int f_quark_flavor_id = query.record().indexOf("quark_flavor_id");
    int f_quark_distribution = query.record().indexOf("quark_distribution");
    int f_quark_distribution_minus = query.record().indexOf(
            "quark_distribution_minus");
    int f_quark_distribution_plus = query.record().indexOf(
            "quark_distribution_plus");

    if (query.first()) {
        // retrieve column value
        int quarkFlavorId = query.value(f_quark_flavor_id).toInt();
        double quarkDistributionValue =
                query.value(f_quark_distribution).toDouble();
        double quarkDistributionMinus =
                query.value(f_quark_distribution_minus).toDouble();
        double quarkDistributionPlus =
                query.value(f_quark_distribution_plus).toDouble();

        // fill QuarkDistribution object
        quarkDistribution.setQuarkFlavor(
                static_cast<QuarkFlavor::Type>(quarkFlavorId));
        quarkDistribution.setQuarkDistribution(quarkDistributionValue);
        quarkDistribution.setQuarkDistributionMinus(quarkDistributionMinus);
        quarkDistribution.setQuarkDistributionPlus(quarkDistributionPlus);
    }
}
