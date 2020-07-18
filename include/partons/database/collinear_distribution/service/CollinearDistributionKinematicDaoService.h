#ifndef COLLINEAR_DISTRIBUTION_KINEMATIC_DAO_SERVICE
#define COLLINEAR_DISTRIBUTION_KINEMATIC_DAO_SERVICE

/**
 * @file CollinearDistributionKinematicDaoService.h
 * @author: Valerio BERTONE (CEA Saclay)
 * @date July 18, 2020
 * @version 1.0
 */

#include <string>

#include "../../../beans/parton_distribution/CollinearDistributionKinematic.h"
#include "../../../beans/List.h"
#include "../dao/CollinearDistributionKinematicDao.h"

namespace PARTONS {

/**
 * @class CollinearDistributionKinematicDaoService
 *
 * @brief collinear distribution kinematics Data Access Object (DAO) service.
 *
 * It deals with CollinearDistributionKinematic C++ object and related tables from the database.
 */
class CollinearDistributionKinematicDaoService: public BaseObject {

public:

    /**
     * Default constructor.
     */
    CollinearDistributionKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionKinematicDaoService();

    /**
     * Insert into database a new CollinearDistributionKinematic object with transactions mechanisms.
     *
     * @param colldistKinematic Reference to CollinearDistributionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const CollinearDistributionKinematic &colldistKinematic) const;

    /**
     * Insert into database a list of CollinearDistributionKinematic objects with transactions mechanisms.
     *
     * @param colldistKinematicList Reference to List of CollinearDistributionKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<CollinearDistributionKinematic> &colldistKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given collinear distribution kinematics.
     *
     * @param colldistKinematic Reference to CollinearDistributionKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with CollinearDistributionKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(const CollinearDistributionKinematic &colldistKinematic) const;

    /**
     * Try to find an entry in the database containing collinear distribution kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return CollinearDistributionKinematic object filled with the retrieved data or a default CollinearDistributionKinematic object if couldn't find it.
     */
    CollinearDistributionKinematic getKinematicById(const int id) const;

    /**
     * Try to find all entries in the database containing collinear distribution kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of CollinearDistributionKinematic objects that matches with computation id.
     */
    List<CollinearDistributionKinematic> getKinematicListByComputationId(
            const int computationId) const;

    /**
     * Try to find unique id of database entry containing collinear distribution kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow CollinearDistributionResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class CollinearDistributionResultDaoService;

    /**
     * CollinearDistributionKinematicDao object to perform database queries.
     */
    CollinearDistributionKinematicDao m_CollinearDistributionKinematicDao;

    /**
     * Insert into the database a new CollinearDistributionKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<CollinearDistributionKinematic> object, because transactions are already performed earlier.
     *
     * @param colldistKinematic Reference to CollinearDistributionKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(const CollinearDistributionKinematic &colldistKinematic) const;
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_KINEMATIC_DAO_SERVICE */
