#ifndef GPD_KINEMATIC_DAO_SERVICE
#define GPD_KINEMATIC_DAO_SERVICE

/**
 * @file GPDKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date May 05, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/gpd/GPDKinematic.h"
#include "../../../beans/List.h"
#include "../dao/GPDKinematicDao.h"

namespace PARTONS {

/**
 * @class GPDKinematicDaoService
 *
 * @brief GPD kinematics Data Access Object (DAO) service.
 *
 * It deals with GPDKinematic C++ object and related tables from the database.
 */
class GPDKinematicDaoService: public BaseObject {

public:

    /**
     * Default constructor.
     */
    GPDKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~GPDKinematicDaoService();

    /**
     * Insert into database a new GPDKinematic object with transactions mechanisms.
     *
     * @param gpdKinematic Reference to GPDKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const GPDKinematic &gpdKinematic) const;

    /**
     * Insert into database a list of GPDKinematic objects with transactions mechanisms.
     *
     * @param gpdKinematicList Reference to List of GPDKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<GPDKinematic> &gpdKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given GPD kinematics.
     *
     * @param gpdKinematic Reference to GPDKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with GPDKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(const GPDKinematic &gpdKinematic) const;

    /**
     * Try to find an entry in the database containing GPD kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return GPDKinematic object filled with the retrieved data or a default GPDKinematic object if couldn't find it.
     */
    GPDKinematic getKinematicById(const int id) const;

    /**
     * Try to find all entries in the database containing GPD kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of GPDKinematic objects that matches with computation id.
     */
    List<GPDKinematic> getKinematicListByComputationId(
            const int computationId) const;

    /**
     * Try to find unique id of database entry containing GPD kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow GPDResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class GPDResultDaoService;

    /**
     * GPDKinematicDao object to perform database queries.
     */
    GPDKinematicDao m_GPDKinematicDao;

    /**
     * Insert into the database a new GPDKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<GPDKinematic> object, because transactions are already performed earlier.
     *
     * @param gpdKinematic Reference to GPDKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insertWithoutTransaction(const GPDKinematic &gpdKinematic) const;
};

} /* namespace PARTONS */

#endif /* GPD_KINEMATIC_DAO_SERVICE */
