#ifndef DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H
#define DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H

/**
 * @file DVMPConvolCoeffFunctionKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionKinematic.h"
#include "../../../beans/List.h"
#include "../../../beans/MesonPolarization.h"
#include "../../../beans/MesonType.h"
#include "../../../utils/type/PhysicalType.h"
#include "ConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionKinematicDao
 *
 * @brief Compton form factor (CFF) kinematics Data Access Object (DAO).
 * */
class DVMPConvolCoeffFunctionKinematicDao: public ConvolCoeffFunctionKinematicDao {

public:

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionKinematicDao();

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionKinematicDao();

    /**
     * Insert DVMP CFF kinematics into the database.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @param mesonType Meson type.
     * @param mesonPolarization Meson polarization state.
     * @param hashSum Hash sum.
     * @return Unique id of inserted row in the database.
     */
    int insert(const PhysicalType<double>& xi, const PhysicalType<double>& t,
            const PhysicalType<double>& Q2, const PhysicalType<double>& MuF2,
            const PhysicalType<double>& MuR2, MesonType::Type mesonType,
            MesonPolarization::Type mesonPolarization,
            const std::string& hashSum) const;

    /**
     * Select DVMP CFF kinematics from the database.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param Q2 Virtual-photon virtuality.
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @param mesonType Meson type.
     * @param mesonPolarization Meson polarization state.
     * @return Unique id of selected row in the database.
     */
    int select(const PhysicalType<double>& xi, const PhysicalType<double>& t,
            const PhysicalType<double>& Q2, const PhysicalType<double>& MuF2,
            const PhysicalType<double>& MuR2, MesonType::Type mesonType,
            MesonPolarization::Type mesonPolarization) const;

    /**
     * Retrieve CFF kinematics from the database by given unique id of row.
     * @param id Unique id of row in the database to be selected.
     * @return DVMPConvolCoeffFunctionKinematic object containing CFF kinematics retrieved from the database.
     */
    DVMPConvolCoeffFunctionKinematic getKinematicById(const int id) const;

    /**
     * Retrieve list of CFF kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of DVMPConvolCoeffFunctionKinematic objects containing CFF kinematics retrieved from the database.
     */
    List<DVMPConvolCoeffFunctionKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Retrieve unique id of row containing CFF kinematics in the database by given hash sum.
     * @param hashSum Hash sum of CFF kinematics to be selected.
     * @return Unique id of selected row in the database.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Fill DVMPConvolCoeffFunctionKinematic object from given query.
     * @param kinematic DVMPConvolCoeffFunctionKinematic object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillKinematicFromQuery(DVMPConvolCoeffFunctionKinematic &kinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of DVMPConvolCoeffFunctionKinematic objects from given query.
     * @param kinematicList List of DVMPConvolCoeffFunctionKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillKinematicListFromQuery(
            List<DVMPConvolCoeffFunctionKinematic>& kinematicList,
            QSqlQuery& query) const;
};

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H */
