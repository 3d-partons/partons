#ifndef DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H
#define DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H

/**
 * @file DDVCSConvolCoeffFunctionKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../../../utils/type/PhysicalType.h"
#include "ConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionKinematicDao
 *
 * @brief Compton form factor (CFF) kinematics Data Access Object (DAO).
 * */
class DDVCSConvolCoeffFunctionKinematicDao: public ConvolCoeffFunctionKinematicDao {

public:

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionKinematicDao();

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionKinematicDao();

    /**
     * Insert DDVCS CFF kinematics into the database.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param Q2 Virtual-photon virtuality.
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @param hashSum Hash sum.
     * @return Unique id of inserted row in the database.
     */
    int insert(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &Q2,
            const PhysicalType<double> &Q2Prim,
            const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2,
            const std::string& hashSum) const;

    /**
     * Select DDVCS CFF kinematics from the database.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param Q2 Virtual-photon virtuality.
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @return Unique id of selected row in the database.
     */
    int select(const PhysicalType<double> &xi,
            const PhysicalType<double> &t, const PhysicalType<double> &Q2,
            const PhysicalType<double> &Q2Prim,
            const PhysicalType<double> &MuF2, const PhysicalType<double> &MuR2) const;

    /**
     * Retrieve CFF kinematics from the database by given unique id of row.
     * @param id Unique id of row in the database to be selected.
     * @return DDVCSConvolCoeffFunctionKinematic object containing CFF kinematics retrieved from the database.
     */
    DDVCSConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Retrieve list of CFF kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of DDVCSConvolCoeffFunctionKinematic objects containing CFF kinematics retrieved from the database.
     */
    List<DDVCSConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Retrieve unique id of row containing CFF kinematics in the database by given hash sum.
     * @param hashSum Hash sum of CFF kinematics to be selected.
     * @return Unique id of selected row in the database.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Fill DDVCSConvolCoeffFunctionKinematic object from given query.
     * @param kinematic DDVCSConvolCoeffFunctionKinematic object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillKinematicFromQuery(DDVCSConvolCoeffFunctionKinematic &kinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of DDVCSConvolCoeffFunctionKinematic objects from given query.
     * @param kinematicList List of DDVCSConvolCoeffFunctionKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillKinematicListFromQuery(
            List<DDVCSConvolCoeffFunctionKinematic>& kinematicList,
            QSqlQuery& query) const;
};

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H */
