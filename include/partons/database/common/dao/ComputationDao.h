#ifndef COMMON_DAO_H
#define COMMON_DAO_H

/**
 * @file ComputationDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
//#include <stddef.h>
#include <ctime>

#include "../../../BaseObject.h"

class Computation;

/**
 * @class ComputationDao
 *
 * @brief
 */
class ComputationDao: public BaseObject {
public:
    ComputationDao();
    virtual ~ComputationDao();

    int insert(const time_t &dateTime,
            const int environmentConfigurationId) const;
    Computation selectByIndexId(const int indexId) const;
    int getComputationIdByDateTime(const time_t &dateTime) const;

    int insertIntoScenarioComputation(
            const unsigned int scenarioTaskIndexNumber, const int scenarioId,
            const int computationId) const;

    bool isAvailable(const int computationId) const;

private:
    void fillComputation(Computation &computation, QSqlQuery &query) const;
};

#endif /* COMMON_DAO_H */
