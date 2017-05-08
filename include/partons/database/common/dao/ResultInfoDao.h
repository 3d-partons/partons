/**
 * @file ResultInfoDao.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 6, 2016
 * @version 1.0
 *
 * @class ResultInfoDao
 *
 * @brief Result information Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete result information from the database. This class in not intended to be used by regular users who should deal with ResultInfoDaoService instead.
 */

#ifndef RESULTINFODAO_H_
#define RESULTINFODAO_H_

#include <include/partons/BaseObject.h>
#include <QtSql/qsqlquery.h>

class ResultInfo;

class ResultInfoDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ResultInfoDao();

    /**
     * Destructor.
     */
    virtual ~ResultInfoDao();

    /**
     * Retrieve result information from the database by given unique id of row containing computation information.
     * @param computationId Unique id of row containing computation information in the database to be selected.
     * @return ResultInfo object containing result information retrieved from the database.
     */
    ResultInfo getResultInfoByComputationId(const int computationId) const;

private:

    /**
     * Fill ResultInfo object from given query.
     * @param resultInfo ResultInfo object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillResultInfo(ResultInfo &resultInfo, QSqlQuery& query) const;
};

#endif /* RESULTINFODAO_H_ */
