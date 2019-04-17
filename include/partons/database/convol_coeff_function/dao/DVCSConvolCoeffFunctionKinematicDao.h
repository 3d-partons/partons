#ifndef DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H
#define DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H

/**
 * @file DVCSConvolCoeffFunctionKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../../../utils/type/PhysicalType.h"
#include "ConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionKinematicDao
 *
 * @brief Compton form factor (CFF) kinematics Data Access Object (DAO).
 * */
class DVCSConvolCoeffFunctionKinematicDao: public ConvolCoeffFunctionKinematicDao {

public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionKinematicDao();

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionKinematicDao();

    /**
     * Insert DVCS CFF kinematics into the database.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @return Unique id of inserted row in the database.
     */
    int insert(const PhysicalType<double>& xi, const PhysicalType<double>& t,
            const PhysicalType<double>& Q2, const PhysicalType<double>& MuF2,
            const PhysicalType<double>& MuR2) const;

    /**
     * Select DVCS CFF kinematics from the database.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @return Unique id of selected row in the database.
     */
    int select(const PhysicalType<double>& xi, const PhysicalType<double>& t,
            const PhysicalType<double>& Q2, const PhysicalType<double>& MuF2,
            const PhysicalType<double>& MuR2) const;

    /**
     * Retrieve CFF kinematics from the database by given unique id of row.
     * @param id Unique id of row in the database to be selected.
     * @return DVCSConvolCoeffFunctionKinematic object containing CFF kinematics retrieved from the database.
     */
    DVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Retrieve list of CFF kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of DVCSConvolCoeffFunctionKinematic objects containing CFF kinematics retrieved from the database.
     */
    List<DVCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Retrieve unique id of row containing CFF kinematics in the database by given hash sum.
     * @param hashSum Hash sum of CFF kinematics to be selected.
     * @return Unique id of selected row in the database.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Fill DVCSConvolCoeffFunctionKinematic object from given query.
     * @param kinematic DVCSConvolCoeffFunctionKinematic object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillKinematicFromQuery(DVCSConvolCoeffFunctionKinematic &kinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of DVCSConvolCoeffFunctionKinematic objects from given query.
     * @param kinematicList List of DVCSConvolCoeffFunctionKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillKinematicListFromQuery(
            List<DVCSConvolCoeffFunctionKinematic>& kinematicList,
            QSqlQuery& query) const;
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H */
