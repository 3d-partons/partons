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

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/ResultList.h"
#include "../../common/service/CommonDaoService.h"
#include "../../parton_distribution/service/PartonDistributionDaoService.h"
#include "../dao/GPDResultDao.h"
#include "GPDKinematicDaoService.h"

class GPDResultDaoService: public BaseObject {
public:
    GPDResultDaoService();
    virtual ~ GPDResultDaoService();

    int insert(const GPDResult &gpdResult) const;
    int insert(const ResultList<GPDResult> &gpdResultList) const;

    ResultList<GPDResult> getGPDResultListByComputationId(
            const int computationId) const;

private:
    GPDResultDao m_gpdResultDao;

    GPDKinematicDaoService m_gpdKinematicDaoService;
    PartonDistributionDaoService m_partonDistributionDaoService;
    CommonDaoService m_commonDaoService;

    int insertWithoutTransaction(const GPDResult &gpdResult) const;
};

#endif /* GPD_RESULT_DAO_SERVICE */
