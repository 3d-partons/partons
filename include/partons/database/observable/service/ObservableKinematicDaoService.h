#ifndef OBSERVABLE_KINEMATIC_DAO_SERVICE_H
#define OBSERVABLE_KINEMATIC_DAO_SERVICE_H

/**
 * @file ObservableKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../BaseObject.h"

namespace PARTONS {

/**
 * @class ObservableKinematicDaoService
 *
 * @brief Observable kinematics Data Access Object (DAO) service.
 */
class ObservableKinematicDaoService: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ObservableKinematicDaoService(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ObservableKinematicDaoService();

    /**
     * Try to find unique id of database entry containing observable kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    virtual int getKinematicIdByHashSum(const std::string &hashSum) const = 0;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_KINEMATIC_DAO_SERVICE_H */
