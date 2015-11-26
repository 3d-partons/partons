#include "PartonDistributionDao.h"

#include <Qt/qsqlerror.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>
#include <QtSql/qsqlrecord.h>
#include <string>

#include "../../../beans/parton_distribution/GluonDistribution.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../../beans/parton_distribution/QuarkDistribution.h"
#include "../../../utils/stringUtils/Formatter.h"
#include "../../DatabaseManager.h"

PartonDistributionDao::PartonDistributionDao() :
        BaseObject("PartonDistributionDao") {
}

PartonDistributionDao::~PartonDistributionDao() {
}

int PartonDistributionDao::insert(double gluonDistributionValue) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO parton_distribution (gluon_distribution_value) VALUES (:gluonDistributionValue )");

    query.bindValue(":gluonDistributionValue", gluonDistributionValue);

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

int PartonDistributionDao::insertIntoPartonDistributionQuarkDistributionTable(
        const int partonDistributionId, const int quarkDistributionId) const {
    int result = -1;
    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "INSERT INTO parton_distribution_quark_distribution (parton_distribution_id, quark_distribution_id) VALUES (:partonDistributionId, :quarkDistributionId)");

    query.bindValue(":partonDistributionId", partonDistributionId);
    query.bindValue(":quarkDistributionId", quarkDistributionId);

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

PartonDistribution PartonDistributionDao::getPartonDistributionById(
        const int partonDistributionId) const {

    PartonDistribution partonDistribution;

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT * FROM parton_distribution WHERE id = :partonDistributionId");

    query.bindValue(":partonDistributionId", partonDistributionId);

    if (query.exec()) {
        fillPartonDistributionFromQuery(partonDistribution, query);
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();

    return partonDistribution;
}

void PartonDistributionDao::fillPartonDistributionFromQuery(
        PartonDistribution &partonDistribution, QSqlQuery &query) const {

    int f_parton_distribution_id = query.record().indexOf("id");
    int f_gluon_distribution = query.record().indexOf(
            "gluon_distribution_value");

    if (query.first()) {

        int partonDistributionId =
                query.value(f_parton_distribution_id).toInt();
        double gluonDistributionValue =
                query.value(f_gluon_distribution).toDouble();

        partonDistribution.setGluonDistribution(
                GluonDistribution(gluonDistributionValue));

        fillPartonDistribution(partonDistribution, partonDistributionId);
    }
}

void PartonDistributionDao::fillPartonDistribution(
        PartonDistribution &partonDistribution,
        const int partonDistributionId) const {

    QSqlQuery query(DatabaseManager::getInstance()->getProductionDatabase());

    query.prepare(
            "SELECT quark_distribution_id FROM parton_distribution_quark_distribution WHERE parton_distribution_id = :partonDistributionId");

    query.bindValue(":partonDistributionId", partonDistributionId);

    if (query.exec()) {

        int f_quark_distribution_id = query.record().indexOf(
                "quark_distribution_id");

        while (query.next()) {
            partonDistribution.addQuarkDistribution(
                    m_quarkDistributionDao.getQuarkDistributionById(
                            query.value(f_quark_distribution_id).toInt()));
        }
    } else {
        error(__func__,
                Formatter() << query.lastError().text().toStdString()
                        << " for sql query = "
                        << query.executedQuery().toStdString());
    }

    query.clear();
}
