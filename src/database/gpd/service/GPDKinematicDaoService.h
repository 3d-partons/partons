#ifndef GPD_KINEMATIC_DAO_SERVICE
#define GPD_KINEMATIC_DAO_SERVICE

/**
 * @file GPDKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class GPDKinematicDaoService
 *
 * @brief
 */

#include "../../../beans/gpd/GPDKinematic.h"
#include "../dao/GPDKinematicDao.h"

class GPDKinematic;

class GPDKinematicDaoService: public BaseObject {
public:
    GPDKinematicDaoService();
    virtual ~GPDKinematicDaoService();

    int insert(const GPDKinematic &gpdKinematic) const;
    int select(const GPDKinematic &gpdKinematic) const;

private:
    GPDKinematicDao m_GPDKinematicDao;
};

#endif /* GPD_KINEMATIC_DAO_SERVICE */
