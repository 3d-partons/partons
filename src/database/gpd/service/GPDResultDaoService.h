#ifndef GPD_RESULT_DAO_SERVICE
#define GPD_RESULT_DAO_SERVICE

/**
 * @file GPDResultDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 13 November 2015
 * @version 1.0
 *
 * @class GPDResultDaoService
 *
 * @brief
 */

#include "../../common/service/CommonDaoService.h"
#include "../../parton_distribution/service/PartonDistributionDaoService.h"
#include "../dao/GPDResultDao.h"
#include "../dao/GPDResultPartonDistributionDao.h"
#include "GPDKinematicDaoService.h"

class GPDResult;
class GPDResultList;

class GPDResultDaoService: public BaseObject {
public:
    GPDResultDaoService();
    virtual ~ GPDResultDaoService();

    int insert(const GPDResult &gpdResult) const;
    int insert(const GPDResultList &gpdResultList) const;

private:
    GPDResultDao m_gpdResultDao;
    GPDResultPartonDistributionDao m_gpdResultPartonDistributionDao;

    GPDKinematicDaoService m_gpdKinematicDaoService;
    PartonDistributionDaoService m_partonDistributionDaoService;
    CommonDaoService m_commonDaoService;

    int insertWithoutTransaction(const GPDResult &gpdResult) const;
};

#endif /* GPD_RESULT_DAO_SERVICE */
