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

#include <Qt/qsqlquery.h>

#include "../../../beans/gpd/GPDKinematic.h"

class GPDKinematicDao: public BaseObject {
public:
    GPDKinematicDao();
    virtual ~GPDKinematicDao();

    int insert(double x, double xi, double t, double MuF2, double MuR2) const;

    int select(double x, double xi, double t, double MuF2, double MuR2) const;

    GPDKinematic getKinematicById(const int id) const;

private:
    void getGPDKinematicFromQuery(GPDKinematic &gpdKinematic,
            QSqlQuery &query) const;
};

#endif /* GPD_KINEMATIC_DAO */
