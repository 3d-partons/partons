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

class PartonDistributionDao {
public:
    static int insert(int gpdResultId, int gpdTypeId, double gluonDistribution);

private:

};

#endif /* PARTON_DISTRIBUTION_DAO */
