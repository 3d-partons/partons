#ifndef OBSERVABLE_KINEMATIC_DAO_H
#define OBSERVABLE_KINEMATIC_DAO_H

/**
 * @file ObservableKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DVCS/DVCSObservableKinematic.h"

namespace PARTONS {

/**
 * @class ObservableKinematicDao
 *
 * @brief Observable kinematics Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete observable kinematics from the database. This class in not intended to be used by regular users who should deal with ObservableKinematicDaoService instead.
 */
class ObservableKinematicDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ObservableKinematicDao();

    /**
     * Destructor.
     */
    virtual ~ObservableKinematicDao();

    /**
     * Insert observable kinematics into the database.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV^2).
     * @param Q2 Virtual-photon virtuality.
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     * @return Unique id of inserted row in the database.
     */
    int insert(double xB, double t, double Q2, double E, double phi) const;

    /**
     * Select GPD kinematics from the database.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV^2).
     * @param Q2 Virtual-photon virtuality.
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in degrees, Trento convention).
     * @return Unique id of selected row in the database.
     */
    int select(double xB, double t, double Q2, double E, double phi) const;

    /**
     * Retrieve observable kinematics from the database by given unique id of row.
     * @param kinematicId Unique id of row in the database to be selected.
     * @return ObservableKinematic object containing GPD kinematics retrieved from the database.
     */
    DVCSObservableKinematic getKinematicById(const int kinematicId) const;

    /**
     * Retrieve list of observable kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of ObservableKinematic objects containing GPD kinematics retrieved from the database.
     */
    List<DVCSObservableKinematic> getKinematicListByComputationId(
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
    void fillKinematicFromQuery(DVCSObservableKinematic &observableKinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of ObservableKinematic objects from given query.
     * @param observableKinematicList List of ObservableKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillObservableKinematicListFromQuery(
            List<DVCSObservableKinematic> &observableKinematicList,
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_KINEMATIC_DAO_H */
