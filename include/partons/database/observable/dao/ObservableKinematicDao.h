#ifndef OBSERVABLE_KINEMATIC_DAO_H
#define OBSERVABLE_KINEMATIC_DAO_H

/**
 * @file ObservableKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 02, 2015
 * @version 1.0
 */

#include <string>

#include "../../../BaseObject.h"

namespace PARTONS {

/**
 * @class ObservableKinematicDao
 *
 * @brief Observable kinematics Data Access Object (DAO).
 */
class ObservableKinematicDao: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ObservableKinematicDao(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ObservableKinematicDao();
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_KINEMATIC_DAO_H */
