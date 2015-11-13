#ifndef PARTON_DISTRIBUTION_DAO
#define PARTON_DISTRIBUTION_DAO

/**
 * @file PartonDistributionDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class PartonDistributionDao
 *
 * @brief
 */

#include "../../../BaseObject.h"

class PartonDistributionDao: public BaseObject {
public:
    PartonDistributionDao();
    virtual ~PartonDistributionDao();

    int insert(double gluonDistributionValue) const;
};

#endif /* PARTON_DISTRIBUTION_DAO */
