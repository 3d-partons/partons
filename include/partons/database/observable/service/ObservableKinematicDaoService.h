#ifndef OBSERVABLE_KINEMATIC_DAO_SERVICE_H
#define OBSERVABLE_KINEMATIC_DAO_SERVICE_H

/**
 * @file ObservableKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 *
 * @class ObservableKinematicDaoService
 *
 * @brief Its role is to deal with ObservableKinematic C++ object and related tables from the database.
 * You can insert, select or remove results from database by many kind of stuff.
 * Therefore database services ensure the integrity of the database by using transaction and rollback mechanisms if something wrong append : the database will stay always in a stable state.
 * And they improve querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 */

#include "../../../beans/List.h"
#include "../../../beans/observable/ObservableKinematic.h"
#include "../dao/ObservableKinematicDao.h"

class ObservableKinematicList;

class ObservableKinematicDaoService: public BaseObject {
public:
    /**
     * Default constructor
     */
    ObservableKinematicDaoService();

    /**
     * Default destructor
     */
    virtual ~ObservableKinematicDaoService();

    /**
     * Insert into database a new ObservableKinematic object with transactions mechanisms.
     *
     * @param observableKinematic
     * @return unique id related to the new entry inserted into the database
     */
    int insert(const ObservableKinematic &observableKinematic) const;

    /**
     * Insert into database a list of ObservableKinematic objects with transactions mechanisms.
     *
     * @param observableKinematicList
     * @return unique id related to the last entry inserted into the database
     */
    int insert(const List<ObservableKinematic> &observableKinematicList) const;

    /**
     * Try to find an entry in observable_kinematic table that match with xB, t, Q2, E and phi values.
     *
     * @param observableKinematic
     * @return unique id related to entry that match with ObservableKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const ObservableKinematic &observableKinematic) const;

    /**
     * Return a list of ObservableKinematic objects from the database identified by a specific computation identifier.
     *
     * @param computationId
     * @return list of ObservableKinematic objects.
     */
    List<ObservableKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * * Try to find an entry in observable_kinematic table that match with id value.
     *
     * @param id
     * @return ObservableKinematic object fill with data from database tables or a default ObservableKinematic object if couldn't find it.
     */
    ObservableKinematic getKinematicById(const int kinematicId) const;

private:
    friend class ObservableResultDaoService; ///< allow ObservableResultDaoService to call private member function insertWithoutTransaction(...)

    ObservableKinematicDao m_observableKinematicDao; ///< reference to the right DAO object to perform database queries

    /**
     * Insert into the database a new ObservableKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<ObservableKinematic> object, because transactions are already performed earlier.
     *
     * @param observableKinematic
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(
            const ObservableKinematic &observableKinematic) const;
};

#endif /* OBSERVABLE_KINEMATIC_DAO_SERVICE_H */
