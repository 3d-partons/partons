#ifndef GPD_KINEMATIC_DAO_SERVICE
#define GPD_KINEMATIC_DAO_SERVICE

/**
 * @file GPDKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date May 05, 2015
 * @version 1.0
 *
 * @class GPDKinematicDaoService
 *
 * @brief Its role is to deal with GPDKinematic C++ object and related tables from the database.
 * You can insert, select or remove results from database by many kind of stuff.
 * Therefore database services ensure the integrity of the database by using transaction and rollback mechanisms if something wrong append : the database will stay always in a stable state.
 * And they improve querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#include <string>

#include "../../../beans/gpd/GPDKinematic.h"
#include "../../../beans/List.h"
#include "../dao/GPDKinematicDao.h"

class GPDKinematic;

class GPDKinematicDaoService: public BaseObject {
public:
    /**
     * Default constructor
     */
    GPDKinematicDaoService();

    /**
     * Default destructor
     */
    virtual ~GPDKinematicDaoService();

    /**
     * Insert into database a new GPDKinematic object with transactions mechanisms.
     *
     * @param gpdKinematic
     * @return unique id related to the new entry inserted into the database
     */
    int insert(const GPDKinematic &gpdKinematic) const;

    /**
     * Insert into database a list of GPDKinematic objects with transactions mechanisms.
     *
     * @param gpdKinematicList
     * @return unique id related to the last entry inserted into the database
     */
    int insert(const List<GPDKinematic> &gpdKinematicList) const;

    /**
     * Try to find an entry in gpd_kinematic table that match with x, xi, t, MuF2 and MuR2 values.
     *
     * @param gpdKinematic
     * @return unique id related to entry that match with GPDKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(const GPDKinematic &gpdKinematic) const;

    /**
     * * Try to find an entry in gpd_kinematic table that match with id value.
     *
     * @param id
     * @return GPDKinematic object fill with data from database tables or a default  GPDKinematic object if couldn't find it.
     */
    GPDKinematic getKinematicById(const int id) const;

    /**
     * Return a list of GPDKinematic objects from the database identified by a specific computation identifier.
     *
     * @param computationId
     * @return list of GPDKinematic objects.
     */
    List<GPDKinematic> getKinematicListByComputationId(
            const int computationId) const;

    int getKinematicIdByHashSum(const std::string &hashSum) const;

    int executeCustomQuery(const double x) const;

private:
    friend class GPDResultDaoService; ///< allow GPDResultDaoService to call private member function insertWithoutTransaction(...)

    GPDKinematicDao m_GPDKinematicDao; ///< reference to the right DAO object to perform database queries

    /**
     * Insert into the database a new GPDKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<GPDKinematic> object, because transactions are already performed earlier.
     *
     * @param gpdKinematic
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(const GPDKinematic &gpdKinematic) const;
};

#endif /* GPD_KINEMATIC_DAO_SERVICE */
