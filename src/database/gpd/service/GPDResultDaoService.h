#ifndef GPD_RESULT_DAO_SERVICE
#define GPD_RESULT_DAO_SERVICE

#include "../dao/GPDResultDao.h"
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

    GPDKinematicDaoService m_gpdKinematicDaoService;

    int insertWithoutTransaction(const GPDResult &gpdResult) const;
};

#endif /* GPD_RESULT_DAO_SERVICE */
