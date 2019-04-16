#ifndef CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file ConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 05, 2015
 * @version 1.0
 */

#include <string>

#include "../dao/ConvolCoeffFunctionKinematicDao.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionKinematicDaoService
 *
 * @brief Compton form factor (CFF) kinematics Data Access Object (DAO) service.
 */
class ConvolCoeffFunctionKinematicDaoService: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionKinematicDaoService(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionKinematicDaoService();

    /**
     * Try to find unique id of database entry containing CFF kinematics that matches with given hash sum value.
     * @param hashSum Hash sum value of database entry to be retrieved.
     * @return Unique id of database entry that matches with hash sum value.
     */
    virtual int getKinematicIdByHashSum(const std::string &hashSum) const = 0;

private:

    /**
     * ConvolCoeffFunctionKinematicDao object to perform database queries.
     */
    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao;

};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
