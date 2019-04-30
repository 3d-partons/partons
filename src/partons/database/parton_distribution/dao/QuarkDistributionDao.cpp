#include "../../../../../include/partons/database/parton_distribution/dao/QuarkDistributionDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

QuarkDistributionDao::QuarkDistributionDao() :
        BaseObject("QuarkDistributionDao") {
}

QuarkDistributionDao::~QuarkDistributionDao() {
}

int QuarkDistributionDao::insert(double quarkDistributionPlus,
        double quarkDistributionMinus, double quarkDistribution,
        int quarkFlavorTypeId, int partonDistributionId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO quark_distribution (quark_distribution_plus, quark_distribution_minus, quark_distribution, quark_flavor_id) VALUES (:quarkDistributionPlus, :quarkDistributionMinus, :quarkDistribution, :quarkFlavorTypeId )");

    query.bindValue(":quarkDistributionPlus", quarkDistributionPlus);
    query.bindValue(":quarkDistributionMinus", quarkDistributionMinus);
    query.bindValue(":quarkDistribution", quarkDistribution);
    query.bindValue(":quarkFlavorTypeId", quarkFlavorTypeId);

    //execute query
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

QuarkDistribution QuarkDistributionDao::getQuarkDistributionById(
        const int quarkDistributionId) const {

    //result
    QuarkDistribution quarkDistribution;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM quark_distribution WHERE id = :quarkDistributionId");

    query.bindValue(":quarkDistributionId", quarkDistributionId);

    //execute query
    if (query.exec()) {
        fillQuarkDistributionFromQuery(quarkDistribution, query);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return quarkDistribution;
}

void QuarkDistributionDao::fillQuarkDistributionFromQuery(
        QuarkDistribution &quarkDistribution, QSqlQuery &query) const {

    //get indices
    int f_quark_flavor_id = query.record().indexOf("quark_flavor_id");
    int f_quark_distribution = query.record().indexOf("quark_distribution");
    int f_quark_distribution_minus = query.record().indexOf(
            "quark_distribution_minus");
    int f_quark_distribution_plus = query.record().indexOf(
            "quark_distribution_plus");

    //first query
    if (query.first()) {

        //retrieve
        int quarkFlavorId = query.value(f_quark_flavor_id).toInt();
        double quarkDistributionValue =
                query.value(f_quark_distribution).toDouble();
        double quarkDistributionMinus =
                query.value(f_quark_distribution_minus).toDouble();
        double quarkDistributionPlus =
                query.value(f_quark_distribution_plus).toDouble();

        //fill
        quarkDistribution.setQuarkFlavor(
                static_cast<QuarkFlavor::Type>(quarkFlavorId));
        quarkDistribution.setQuarkDistribution(quarkDistributionValue);
        quarkDistribution.setQuarkDistributionMinus(quarkDistributionMinus);
        quarkDistribution.setQuarkDistributionPlus(quarkDistributionPlus);
    }
}

} /* namespace PARTONS */
