#ifndef DDVCS_OBSERVABLE_KINEMATIC_DAO_H
#define DDVCS_OBSERVABLE_KINEMATIC_DAO_H

/**
 * @file DDVCSObservableKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../../../utils/type/PhysicalType.h"
#include "ObservableKinematicDao.h"

namespace PARTONS {

/**
 * @class DDVCSObservableKinematicDao
 *
 * @brief DDVCS Observable kinematics Data Access Object (DAO).
 */
class DDVCSObservableKinematicDao: public ObservableKinematicDao {
public:

    /**
     * Default constructor.
     */
    DDVCSObservableKinematicDao();

    /**
     * Destructor.
     */
    virtual ~DDVCSObservableKinematicDao();

    /**
     * Insert observable kinematics into the database.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param phiL Azimuthal angle of the lepton pair (in radians).
     * @param thetaL Polar angle of the lepton pair (in radians).
     * @param hashSum Hash sum.
     * @return Unique id of inserted row in the database.
     */
    int insert(const PhysicalType<double>& xB,
            const PhysicalType<double>& t, const PhysicalType<double>& Q2,
            const PhysicalType<double>& Q2Prim, const PhysicalType<double>& E,
            const PhysicalType<double>& phi, const PhysicalType<double>& phiL,
            const PhysicalType<double>& thetaL, const std::string& hashSum) const;

    /**
     * Select GPD kinematics from the database.
     * @param xB Bjorken variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param Q2 Virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param Q2Prim Outgoing virtual-photon virtuality (in \f$GeV^{2}\f$).
     * @param E Beam energy (in GeV).
     * @param phi Angle between leptonic and hadronic planes (in radians, Trento convention).
     * @param phiL Azimuthal angle of the lepton pair (in radians).
     * @param thetaL Polar angle of the lepton pair (in radians).
     * @return Unique id of selected row in the database.
     */
    int select(const PhysicalType<double>& xB,
            const PhysicalType<double>& t, const PhysicalType<double>& Q2,
            const PhysicalType<double>& Q2Prim, const PhysicalType<double>& E,
            const PhysicalType<double>& phi, const PhysicalType<double>& phiL,
            const PhysicalType<double>& thetaL) const;

    /**
     * Retrieve observable kinematics from the database by given unique id of row.
     * @param id Unique id of row in the database to be selected.
     * @return ObservableKinematic object containing GPD kinematics retrieved from the database.
     */
    DDVCSObservableKinematic getKinematicById(const int id) const;

    /**
     * Retrieve list of observable kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of ObservableKinematic objects containing GPD kinematics retrieved from the database.
     */
    List<DDVCSObservableKinematic> getKinematicListByComputationId(
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
    void fillKinematicFromQuery(DDVCSObservableKinematic &observableKinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of ObservableKinematic objects from given query.
     * @param observableKinematicList List of ObservableKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillObservableKinematicListFromQuery(
            List<DDVCSObservableKinematic> &observableKinematicList,
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* DDVCS_OBSERVABLE_KINEMATIC_DAO_H */
