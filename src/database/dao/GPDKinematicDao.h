#ifndef GPD_KINEMATIC_DAO
#define GPD_KINEMATIC_DAO

/**
 * @file GPDKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class GPDKinematicDao
 *
 * @brief
 */

class GPDKinematicDao {
public:
    static int insert(int scenarioId, int kinematicType, double x, double xi,
            double t, double MuF, double MuR);

private:

};

#endif /* GPD_KINEMATIC_DAO */
