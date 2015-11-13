#ifndef GPD_RESULT_PARTON_DISTRIBUTION_DAO_H
#define GPD_RESULT_PARTON_DISTRIBUTION_DAO_H

/**
 * @file GPDResultPartonDistributionDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 13 November 2015
 * @version 1.0
 *
 * @class GPDResultPartonDistributionDao
 *
 * @brief
 */

#include "../../../BaseObject.h"

class GPDResultPartonDistributionDao: public BaseObject {
public:
    GPDResultPartonDistributionDao();
    virtual ~GPDResultPartonDistributionDao();

    int insert(int gpdTypeId, int gpdResultId, int partonDistributionId) const;
};

#endif /* GPD_RESULT_PARTON_DISTRIBUTION_DAO_H */
