#ifndef CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H
#define CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H

/**
 * @file ConvolCoeffFunctionKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionKinematicDao
 *
 * @brief
 */

#include <Qt/qsqlquery.h>

#include "../../../BaseObject.h"

class DVCSConvolCoeffFunctionKinematic;

class ConvolCoeffFunctionKinematicDao: public BaseObject {
public:
    ConvolCoeffFunctionKinematicDao();
    virtual ~ConvolCoeffFunctionKinematicDao();

    int insert(double xi, double t, double Q2, double MuF2, double MuR2) const;

    int select(double xi, double t, double Q2, double MuF2, double MuR2) const;

    DVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

private:
    DVCSConvolCoeffFunctionKinematic getKinematicFromQuery(
            QSqlQuery &query) const;
};

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H */
