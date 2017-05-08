/*
 * ResultInfoDaoService.h
 *
 *  Created on: May 23, 2016
 *      Author: debian
 */

/**
 * @class ResultInfoDaoService
 *
 * @brief Result information Data Access Object (DAO) service.
 *
 * It deals with ResultInfo C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove result information from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#ifndef RESULTINFODAOSERVICE_H_
#define RESULTINFODAOSERVICE_H_

#include <include/partons/database/common/dao/ComputationDao.h>
#include <include/partons/database/common/dao/ResultInfoDao.h>
#include <include/partons/database/common/service/ComputationDaoService.h>
#include <include/partons/database/common/service/EnvironmentConfigurationDaoService.h>
#include <include/partons/database/common/service/ScenarioDaoService.h>

class ResultInfo;

class ResultInfoDaoService: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ResultInfoDaoService();

    /**
     * Destructor.
     */
    virtual ~ResultInfoDaoService();

    /**
     * Insert into database a new ResultInfo object without transactions mechanisms.
     * @param resultInfo Reference to ResultInfo object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(const ResultInfo &resultInfo) const;

    /**
     * Try to find all entries in the database containing computation information that are associated with given unique computation id value.
     * @param computationId Computation id of database entries to be found.
     * @return ResultInfo object that matches with computation id.
     */
    ResultInfo getResultInfoByComputationId(const int computationId) const;

private:

    /**
     * ResultInfoDao object to perform database queries.
     */
    ResultInfoDao m_resultInfoDao;

    /**
     * ComputationDaoService object to perform database queries.
     */
    ComputationDaoService m_computationDaoService;

    /**
     * ScenarioDaoService object to perform database queries.
     */
    ScenarioDaoService m_scenarioDaoService;

    /**
     * EnvironmentConfigurationDaoService object to perform database queries.
     */
    EnvironmentConfigurationDaoService m_environmentConfigurationDaoService;

    /**
     * ComputationDao object to perform database queries.
     */
    ComputationDao m_computationDao;
};

#endif /* RESULTINFODAOSERVICE_H_ */
