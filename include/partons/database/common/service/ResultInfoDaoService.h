/*
 * ResultInfoDaoService.h
 *
 *  Created on: May 23, 2016
 *      Author: debian
 */

#ifndef RESULTINFODAOSERVICE_H_
#define RESULTINFODAOSERVICE_H_

#include "../dao/ComputationDao.h"
#include "../dao/ResultInfoDao.h"
#include "ComputationDaoService.h"
#include "EnvironmentConfigurationDaoService.h"
#include "ScenarioDaoService.h"

class ResultInfo;

class ResultInfoDaoService: public BaseObject {
public:
    ResultInfoDaoService();
    virtual ~ResultInfoDaoService();

    int insertWithoutTransaction(const ResultInfo &resultInfo) const;
    ResultInfo getResultInfoByComputationId(const int computationId) const;

private:
    ResultInfoDao m_resultInfoDao;

    ComputationDaoService m_computationDaoService;
    ScenarioDaoService m_scenarioDaoService;
    EnvironmentConfigurationDaoService m_environmentConfigurationDaoService;

    ComputationDao m_computationDao;
};

#endif /* RESULTINFODAOSERVICE_H_ */
