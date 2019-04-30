#include "../../../../../include/partons/database/parton_distribution/dao/PartonDistributionDao.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/database/DatabaseManager.h"

namespace PARTONS {

PartonDistributionDao::PartonDistributionDao() :
        BaseObject("PartonDistributionDao") {
}

PartonDistributionDao::~PartonDistributionDao() {
}

int PartonDistributionDao::insert(double gluonDistributionValue) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO parton_distribution (gluon_distribution_value) VALUES (:gluonDistributionValue )");

    query.bindValue(":gluonDistributionValue", gluonDistributionValue);

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

int PartonDistributionDao::insertIntoPartonDistributionQuarkDistributionTable(
        const int partonDistributionId, const int quarkDistributionId) const {

    //result
    int result = -1;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "INSERT INTO parton_distribution_quark_distribution (parton_distribution_id, quark_distribution_id) VALUES (:partonDistributionId, :quarkDistributionId)");

    query.bindValue(":partonDistributionId", partonDistributionId);
    query.bindValue(":quarkDistributionId", quarkDistributionId);

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

PartonDistribution PartonDistributionDao::getPartonDistributionById(
        const int partonDistributionId) const {

    //result
    PartonDistribution partonDistribution;

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT * FROM parton_distribution WHERE id = :partonDistributionId");

    query.bindValue(":partonDistributionId", partonDistributionId);

    //execute query
    if (query.exec()) {
        fillPartonDistributionFromQuery(partonDistribution, query);
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    return partonDistribution;
}

void PartonDistributionDao::fillPartonDistributionFromQuery(
        PartonDistribution &partonDistribution, QSqlQuery &query) const {

    //get indices
    int f_parton_distribution_id = query.record().indexOf("id");
    int f_gluon_distribution = query.record().indexOf(
            "gluon_distribution_value");

    //first query
    if (query.first()) {

        //get values
        int partonDistributionId =
                query.value(f_parton_distribution_id).toInt();
        double gluonDistributionValue =
                query.value(f_gluon_distribution).toDouble();

        //fill
        partonDistribution.setGluonDistribution(
                GluonDistribution(gluonDistributionValue));

        fillPartonDistribution(partonDistribution, partonDistributionId);
    }
}

void PartonDistributionDao::fillPartonDistribution(
        PartonDistribution &partonDistribution,
        const int partonDistributionId) const {

    //create query
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    //prepare query
    query.prepare(
            "SELECT quark_distribution_id FROM parton_distribution_quark_distribution WHERE parton_distribution_id = :partonDistributionId");

    query.bindValue(":partonDistributionId", partonDistributionId);

    //first query
    if (query.exec()) {

        //get values
        int f_quark_distribution_id = query.record().indexOf(
                "quark_distribution_id");

        while (query.next()) {
            partonDistribution.addQuarkDistribution(
                    m_quarkDistributionDao.getQuarkDistributionById(
                            query.value(f_quark_distribution_id).toInt()));
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }
}

} /* namespace PARTONS */
