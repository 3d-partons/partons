#ifndef DVMP_OBSERVABLE_KINEMATIC_DAO_H
#define DVMP_OBSERVABLE_KINEMATIC_DAO_H

/**
 * @file DVMPObservableKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/List.h"
#include "../../../beans/MesonType.h"
#include "../../../beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "ObservableKinematicDao.h"

namespace PARTONS {

/**
 * @class DVMPObservableKinematicDao
 *
 * @brief DVMP Observable kinematics Data Access Object (DAO).
 */
class DVMPObservableKinematicDao: public ObservableKinematicDao {
public:

    /**
     * Default constructor.
     */
    DVMPObservableKinematicDao();

    /**
     * Destructor.
     */
    virtual ~DVMPObservableKinematicDao();

    /**
     * Insert observable kinematics into the database.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV^2).
     * @param Q2 Virtual-photon virtuality.
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     * @param mesonType Meson type.
     * @param hashSum Hash sum.
     * @return Unique id of inserted row in the database.
     */
    int insert(const PhysicalType<double>& xB, const PhysicalType<double>& t,
            const PhysicalType<double>& Q2, const PhysicalType<double>& E,
            const PhysicalType<double>& phi, MesonType::Type mesonType,
            const std::string& hashSum) const;

    /**
     * Select GPD kinematics from the database.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV^2).
     * @param Q2 Virtual-photon virtuality.
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     * @param mesonType Meson type.
     * @return Unique id of selected row in the database.
     */
    int select(const PhysicalType<double>& xB, const PhysicalType<double>& t,
            const PhysicalType<double>& Q2, const PhysicalType<double>& E,
            const PhysicalType<double>& phi, MesonType::Type mesonType) const;

    /**
     * Retrieve observable kinematics from the database by given unique id of row.
     * @param id Unique id of row in the database to be selected.
     * @return ObservableKinematic object containing GPD kinematics retrieved from the database.
     */
    DVMPObservableKinematic getKinematicById(const int id) const;

    /**
     * Retrieve list of observable kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of ObservableKinematic objects containing GPD kinematics retrieved from the database.
     */
    List<DVMPObservableKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Retrieve unique id of row containing observable kinematics in the database by given hash sum.
     * @param hashSum Hash sum of observable kinematics to be selected.
     * @return Unique id of selected row in the database.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Fill ObservableKinematic object from given query.
     * @param observableKinematic ObservableKinematic object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillKinematicFromQuery(DVMPObservableKinematic &observableKinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of ObservableKinematic objects from given query.
     * @param observableKinematicList List of ObservableKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillObservableKinematicListFromQuery(
            List<DVMPObservableKinematic> &observableKinematicList,
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* DVMP_OBSERVABLE_KINEMATIC_DAO_H */
