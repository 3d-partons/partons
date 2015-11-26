#ifndef PARTON_DISTRIBUTION_DAO
#define PARTON_DISTRIBUTION_DAO

/**
 * @file PartonDistributionDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class PartonDistributionDao
 *
 * @brief
 */

#include <Qt/qsqlquery.h>

#include "QuarkDistributionDao.h"

class PartonDistribution;

class PartonDistributionDao: public BaseObject {
public:
    PartonDistributionDao();
    virtual ~PartonDistributionDao();

    int insert(double gluonDistributionValue) const;

    int insertIntoPartonDistributionQuarkDistributionTable(
            const int partonDistributionId,
            const int quarkDistributionId) const;

    PartonDistribution getPartonDistributionById(
            const int partonDistributionId) const;

private:
    QuarkDistributionDao m_quarkDistributionDao;

    void fillPartonDistributionFromQuery(PartonDistribution &partonDistribution,
            QSqlQuery &query) const;

    void fillPartonDistribution(PartonDistribution &partonDistribution,
            const int partonDistributionId) const;
};

#endif /* PARTON_DISTRIBUTION_DAO */
