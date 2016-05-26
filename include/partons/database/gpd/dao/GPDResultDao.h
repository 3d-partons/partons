#ifndef GPD_RESULT_DAO
#define GPD_RESULT_DAO

/**
 * @file GPDResultDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 13 November 2015
 * @version 1.0
 *
 * @class GPDResultDao
 *
 * @brief
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/gpd/GPDResult.h"
#include "../../../beans/List.h"
#include "../../parton_distribution/dao/PartonDistributionDao.h"
#include "GPDKinematicDao.h"

class GPDResultDao: public BaseObject {
public:
    GPDResultDao();
    virtual ~GPDResultDao();

    int insertResult(const std::string &computationModuleName,
            int gpdKinematicId, int computationId) const;

    int insertIntoGPDResultPartonDistributionTable(const int gpdTypeId,
            const int gpdResultId, const int partonDistributionId) const;

    List<GPDResult> getGPDResultListByComputationId(
            const int computationId) const;

private:
    GPDKinematicDao m_gpdKinematicDao;
    PartonDistributionDao m_partonDistributionDao;

    void fillGPDResultList(List<GPDResult> &gpdResultList,
            QSqlQuery &query) const;

    // a supprimer

    void fillGPDResult(GPDResult &gpdResult) const;
};

#endif /* GPD_RESULT_DAO */
