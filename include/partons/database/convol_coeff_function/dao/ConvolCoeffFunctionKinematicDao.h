#ifndef CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H
#define CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H

/**
 * @file ConvolCoeffFunctionKinematicDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 */

#include <string>

#include "../../../BaseObject.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionKinematicDao
 *
 * @brief Compton form factor (CFF) kinematics Data Access Object (DAO).
 *
 * This DAO is used to insert, select and delete CFF kinematics from the database. This class in not intended to be used by regular users who should deal with ConvolCoeffFunctionKinematicDaoService instead.
 */
class ConvolCoeffFunctionKinematicDao: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionKinematicDao(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionKinematicDao();
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_H */
