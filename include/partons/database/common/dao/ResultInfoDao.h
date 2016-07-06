/*
 * ResultInfoDao.h
 *
 *  Created on: 6 juil. 2016
 *      Author: bryan
 */

#ifndef RESULTINFODAO_H_
#define RESULTINFODAO_H_

#include <QtSql/qsqlquery.h>

#include "../../../BaseObject.h"

class ResultInfo;

class ResultInfoDao: public BaseObject {
public:
    ResultInfoDao();
    virtual ~ResultInfoDao();

    ResultInfo getResultInfoByComputationId(const int computationId) const;

private:
    void fillResultInfo(ResultInfo &resultInfo, QSqlQuery& query) const;
};

#endif /* RESULTINFODAO_H_ */
