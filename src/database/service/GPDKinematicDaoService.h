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

class GPDKinematic;

class GPDKinematicDaoService {
public:
    static int insert(const GPDKinematic &gpdKinematic);
};

#endif /* GPD_KINEMATIC_DAO_SERVICE */
