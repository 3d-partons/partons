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

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"

class ConvolCoeffFunctionKinematicDao: public BaseObject {
public:
    ConvolCoeffFunctionKinematicDao();
    virtual ~ConvolCoeffFunctionKinematicDao();

    int insert(double xi, double t, double Q2, double MuF2, double MuR2) const;

    int select(double xi, double t, double Q2, double MuF2, double MuR2) const;

    DVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    List<DVCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:
    void fillKinematicListFromQuery(
            List<DVCSConvolCoeffFunctionKinematic>& kinematicList,
            QSqlQuery& query) const;

    void fillKinematicFromQuery(DVCSConvolCoeffFunctionKinematic &kinematic,
            QSqlQuery &query) const;
};

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H */
