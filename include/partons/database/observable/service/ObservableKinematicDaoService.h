#ifndef OBSERVABLE_KINEMATIC_DAO_SERVICE_H
#define OBSERVABLE_KINEMATIC_DAO_SERVICE_H

/**
 * @file ObservableKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/ObservableKinematic.h"
#include "../dao/ObservableKinematicDao.h"

/**
 * @class ObservableKinematicDaoService
 *
 * @brief Observable kinematics Data Access Object (DAO) service.
 *
 * It deals with ObservableKinematic C++ object and related tables from the database.
 *
 * With this service you can insert, select or remove observable kinematics from the database. It ensures the integrity of the database by using transaction and rollback mechanisms - if something wrong happened, the database will stay always in a stable state. In addition, it improves querying speed by using transaction and commit mechanisms for a large amount of simultaneous queries.
 *
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //define observable kinematics to be inserted in database
 ObservableKinematic observableKinematicInserted(0.1, 0.2, -0.1, 2., 2.);

 //get ObservableKinematicDaoService
 ObservableKinematicDaoService observableKinematicDaoService;

 //insert kinematics into database
 int id = observableKinematicDaoService.insert(observableKinematicInserted);

 //retrieve kinematics from database
 ObservableKinematic observableKinematicExtracted;
 observableKinematicDaoService.select(id, observableKinematicExtracted);

 //compare
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Inserted: " << observableKinematicInserted.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__,
 ElemUtils::Formatter() << "Extracted: " << observableKinematicExtracted.toString());
 \endcode
 */
class ObservableKinematicDaoService: public BaseObject {
public:

    /**
     * Default constructor.
     */
    ObservableKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~ObservableKinematicDaoService();

    /**
     * Insert into database a new ObservableKinematic object with transactions mechanisms.
     *
     * @param observableKinematic Reference to ObservableKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const ObservableKinematic &observableKinematic) const;

    /**
     * Insert into database a list of ObservableKinematic objects with transactions mechanisms.
     *
     * @param observableKinematicList Reference to List of ObservableKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(const List<ObservableKinematic> &observableKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given observable kinematics.
     *
     * @param observableKinematic Reference to ObservableKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with ObservableKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const ObservableKinematic &observableKinematic) const;

    /**
     * Try to find an entry in the database containing observable kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return  ObservableKinematic object filled with the retrieved data or a default ObservableKinematic object if couldn't find it.
     */
    ObservableKinematic getKinematicById(const int kinematicId) const;

    /**
     * Try to find all entries in the database containing observable kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of ObservableKinematic objects that matches with computation id.
     */
    List<ObservableKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Try to find unique id of database entry containing observable kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow ObservableResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class ObservableResultDaoService;

    /**
     * ObservableKinematicDao object to perform database queries
     */
    ObservableKinematicDao m_observableKinematicDao;

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
