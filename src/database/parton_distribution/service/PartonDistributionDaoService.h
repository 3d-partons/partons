#ifndef PARTON_DISTRIBUTION_DAO_SERVICE
#define PARTON_DISTRIBUTION_DAO_SERVICE

/**
 * @file PartonDistributionDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class PartonDistributionDaoService
 *
 * @brief
 */

#include "../dao/PartonDistributionDao.h"
#include "../dao/QuarkDistributionDao.h"

class PartonDistribution;

class PartonDistributionDaoService: public BaseObject {
public:
    PartonDistributionDaoService();
    virtual ~PartonDistributionDaoService();

    int insert(const PartonDistribution &partonDistribution) const;
    int insertWithoutTransaction(
            const PartonDistribution &partonDistribution) const;
//    int insert(
//            const std::vector<PartonDistribution> &partonDistributionList) const;

private:
    PartonDistributionDao m_partonDistributionDao;
    QuarkDistributionDao m_quarkDistributionDao;
};

#endif /* PARTON_DISTRIBUTION_DAO_SERVICE */
