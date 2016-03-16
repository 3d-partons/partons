#ifndef COMMON_DAO_H
#define COMMON_DAO_H

/**
 * @file ComputationDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 02 November 2015
 * @version 1.0
 *
 * @class ComputationDao
 *
 * @brief
 */

#include <QtSql/qsqlquery.h>
#include <ctime>

#include "../../../BaseObject.h"

class Computation;

class ComputationDao: public BaseObject {
public:
    ComputationDao();
    virtual ~ComputationDao();

    int insert(const time_t &dateTime,
            const int environmentConfigurationId) const;
    Computation selectByIndexId(const int indexId) const;
    int getComputationIdByDateTime(const time_t &dateTime) const;

    int insertIntoComputationConfigurationComputation(
            const int computationConfigurationId,
            const int computationId) const;

private:
    void fillComputation(Computation &computation, QSqlQuery &query) const;
};

#endif /* COMMON_DAO_H */
