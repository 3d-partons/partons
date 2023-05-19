#ifndef DDVCS_OBSERVABLE_KINEMATIC_DAO_SERVICE_H
#define DDVCS_OBSERVABLE_KINEMATIC_DAO_SERVICE_H

/**
 * @file DDVCSObservableKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../beans/List.h"
#include "../../../beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../dao/DDVCSObservableKinematicDao.h"
#include "ObservableKinematicDaoService.h"

namespace PARTONS {

/**
 * @class DDVCSObservableKinematicDaoService
 *
 * @brief DDVCS Observable kinematics Data Access Object (DAO) service.
 */
class DDVCSObservableKinematicDaoService: public ObservableKinematicDaoService {

public:

    /**
     * Default constructor.
     */
    DDVCSObservableKinematicDaoService();

    /**
     * Destructor.
     */
    virtual ~DDVCSObservableKinematicDaoService();

    /**
     * Insert into database a new ObservableKinematic object with transactions mechanisms.
     *
     * @param observableKinematic Reference to ObservableKinematic object to be inserted.
     * @return Unique id related to the new entry inserted into the database.
     */
    int insert(const DDVCSObservableKinematic &observableKinematic) const;

    /**
     * Insert into database a list of ObservableKinematic objects with transactions mechanisms.
     *
     * @param observableKinematicList Reference to List of ObservableKinematic objects to be inserted.
     * @return Unique id related to the last entry inserted into the database.
     */
    int insert(
            const List<DDVCSObservableKinematic> &observableKinematicList) const;

    /**
     * Try to find an entry in the database that matches with given observable kinematics.
     *
     * @param observableKinematic Reference to ObservableKinematic object with kinematics to be found.
     * @return Unique id related to the entry that matches with ObservableKinematic object or -1 if couldn't find it.
     */
    int getIdByKinematicObject(
            const DDVCSObservableKinematic &observableKinematic) const;

    /**
     * Try to find an entry in the database containing observable kinematics that matches with given unique id value.
     *
     * @param id Unique id of database entry to be found.
     * @return  ObservableKinematic object filled with the retrieved data or a default ObservableKinematic object if couldn't find it.
     */
    DDVCSObservableKinematic getKinematicById(const int kinematicId) const;

    /**
     * Try to find all entries in the database containing observable kinematics that are associated with given unique computation id value.
     *
     * @param computationId Computation id of database entries to be found.
     * @return List of ObservableKinematic objects that matches with computation id.
     */
    List<DDVCSObservableKinematic> getKinematicListByComputationId(
            int computationId) const;

    /**
     * Try to find unique id of database entry containing observable kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    virtual int getKinematicIdByHashSum(const std::string &hashSum) const;

private:

    /**
     * Allow DDVCSObservableResultDaoService to call private member function insertWithoutTransaction(...).
     */
    friend class DDVCSObservableResultDaoService;

    /**
     * DDVCSObservableKinematicDao object to perform database queries
     */
    DDVCSObservableKinematicDao m_ddvcsObservableKinematicDao;

    /**
     * Insert into the database a new ObservableKinematic object without using transactions mechanisms.
     * Helpful when dealing with a List<ObservableKinematic> object, because transactions are already performed earlier.
     *
     * @param observableKinematic
     * @return unique id related to the new entry inserted into the database
     */
    int insertWithoutTransaction(
            const DDVCSObservableKinematic &observableKinematic) const;
};

} /* namespace PARTONS */

#endif /* DDVCS_OBSERVABLE_KINEMATIC_DAO_SERVICE_H */
