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
 * @brief GPD kinematics Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete GPD kinematics from the database. This class in not intended to be used by regular users who should deal with GPDKinematicDaoService instead.
 */

#include <include/partons/beans/gpd/GPDKinematic.h>
#include <include/partons/beans/List.h>
#include <QtSql/qsqlquery.h>
#include <string>

class GPDKinematicDao: public BaseObject {
public:

    /**
     * Default constructor.
     */
    GPDKinematicDao();

    /**
     * Destructor.
     */
    virtual ~GPDKinematicDao();

    /**
     * Insert GPD kinematics into the database.
     * @param x Longitudinal momentum fraction of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @return Unique id of inserted row in the database.
     */
    int insert(double x, double xi, double t, double MuF2, double MuR2) const;

    /**
     * Select GPD kinematics from the database.
     * @param x Longitudinal momentum fraction of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in GeV<sup>2</sup>).
     * @param MuF2 Factorization scale squared (in GeV<sup>2</sup>).
     * @param MuR2 Renormalization scale squared (in GeV<sup>2</sup>).
     * @return Unique id of selected row in the database.
     */
    int select(double x, double xi, double t, double MuF2, double MuR2) const;

    /**
     * Retrieve GPD kinematics from the database by given unique id of row.
     * @param id Unique id of row in the database to be selected.
     * @return GPDKinematic object containing GPD kinematics retrieved from the database.
     */
    GPDKinematic getKinematicById(const int id) const;

    /**
     * Retrieve list of GPD kinematics from the database by given unique id of computation.
     * @param computationId Unique id of computation to be selected.
     * @return List of GPDKinematic objects containing GPD kinematics retrieved from the database.
     */
    List<GPDKinematic> getKinematicListByComputationId(
            const int computationId) const;

    /**
     * Retrieve unique id of row containing GPD kinematics in the database by given hash sum.
     * @param hashSum Hash sum of GPD kinematics to be selected.
     * @return Unique id of selected row in the database.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Fill GPDKinematic object from given query.
     * @param gpdKinematic GPDKinematic object to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillGPDKinematicFromQuery(GPDKinematic &gpdKinematic,
            QSqlQuery &query) const;

    /**
     * Fill List of GPDKinematic objects from given query.
     * @param gpdKinematicList List of GPDKinematic objects to be filled.
     * @param query Input QSqlQuery query.
     */
    void fillGPDKinematicListFromQuery(List<GPDKinematic> &gpdKinematicList,
            QSqlQuery &query) const;
};

#endif /* GPD_KINEMATIC_DAO */
