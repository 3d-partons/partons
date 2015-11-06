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

#include "../../../BaseObject.h"

class GPDKinematicDao: public BaseObject {
public:
    GPDKinematicDao();
    virtual ~GPDKinematicDao();

    int insert(double x, double xi, double t, double MuF2, double MuR2) const;

    int select(double x, double xi, double t, double MuF2, double MuR2) const;
};

#endif /* GPD_KINEMATIC_DAO */
