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

#include <QtSql/qsqlquery.h>

#include "../../../beans/gpd/GPDKinematic.h"
#include "../../../beans/List.h"

class GPDKinematicDao: public BaseObject {
public:
    GPDKinematicDao();
    virtual ~GPDKinematicDao();

    int insert(double x, double xi, double t, double MuF2, double MuR2) const;

    int select(double x, double xi, double t, double MuF2, double MuR2) const;

    GPDKinematic getKinematicById(const int id) const;
    List<GPDKinematic> getKinematicListByComputationId(
            const int computationId) const;

private:
    void fillGPDKinematicFromQuery(GPDKinematic &gpdKinematic,
            QSqlQuery &query) const;
    void fillGPDKinematicListFromQuery(List<GPDKinematic> &gpdKinematicList,
            QSqlQuery &query) const;
};

#endif /* GPD_KINEMATIC_DAO */
