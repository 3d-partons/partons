#ifndef COLLINEAR_DISTRIBUTION_KINEMATIC_DAO
#define COLLINEAR_DISTRIBUTION_KINEMATIC_DAO

/**
 * @file CollinearDistributionKinematicDao.h
 * @author: Valerio BERTONE (CEA Saclay)
 * @date July 18, 2020
 * @version 1.0
 */

#include <QtSql/qsqlquery.h>
#include <string>

#include "../../../beans/parton_distribution/CollinearDistributionKinematic.h"
#include "../../../beans/List.h"
#include "../../../utils/type/PhysicalType.h"

namespace PARTONS {

/**
 * @class CollinearDistributionKinematicDao
 *
 * @brief collinear distribution kinematics Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete collinear distribution kinematics from the database. This class in not intended to be used by regular users who should deal with CollinearDistributionKinematicDaoService instead.
 */
class CollinearDistributionKinematicDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    CollinearDistributionKinematicDao();

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionKinematicDao();

    /**
     * Insert collinear distribution kinematics into the database.
     * @param x Longitudinal momentum fraction of active parton.
     * @param MuF2 Factorization scale squared.
     * @param MuR2 Renormalization scale squared.
     * @param hashSum Hash sum.
     * @return Unique id of inserted row in the database.
     */
    int insert(const PhysicalType<double>& x, const PhysicalType<double>& MuF2,
            const PhysicalType<double>& MuR2, const std::string& hashSum) const;

    /**
     * Select collinear distribution kinematics from the database.
     * @param x Longitudinal momentum fraction of active parton.
     * @param MuF2 Factorization scale squared.
     * @param MuR2 Renormalization scale squared.
     * @return Unique id of selected row in the database.
     */
    int select(const PhysicalType<double>& x, const PhysicalType<double>& MuF2,
            const PhysicalType<double>& MuR2) const;

    /**
     * Retrieve collinear distribution kinematics from the database by given unique id of row.
     * @param id Unique id of row in the database to be selected.
     * @return CollinearDistributionKinematic object containing collinear distribution kinematics retrieved from the database.
     */
    CollinearDistributionKinematic getKinematicById(const int id) const;

    /**
     * Retrieve list of collinear distribution kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of CollinearDistributionKinematic objects containing collinear distribution kinematics retrieved from the database.
     */
    List<CollinearDistributionKinematic> getKinematicListByComputationId(
            const int computationId) const;

    /**
     * Retrieve unique id of row containing collinear distribution kinematics in the database by given hash sum.
     * @param hashSum Hash sum of collinear distribution kinematics to be selected.
     * @return Unique id of selected row in the database.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Fill CollinearDistributionKinematic object from given query.
     * @param colldistKinematic CollinearDistributionKinematic object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillCollinearDistributionKinematicFromQuery(CollinearDistributionKinematic &colldistKinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of CollinearDistributionKinematic objects from given query.
     * @param colldistKinematicList List of CollinearDistributionKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillCollinearDistributionKinematicListFromQuery(List<CollinearDistributionKinematic> &colldistKinematicList,
            QSqlQuery &query) const;
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_KINEMATIC_DAO */
