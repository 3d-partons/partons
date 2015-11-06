#ifndef CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H
#define CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H

/**
 * @file ConvolCoeffFunctionKinematicDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 November 2015
 * @version 1.0
 *
 * @class ConvolCoeffFunctionKinematicDaoService
 *
 * @brief
 */

#include "../../../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../dao/ConvolCoeffFunctionKinematicDao.h"

class ConvolCoeffFunctionKinematicDaoService: public BaseObject {
public:
    ConvolCoeffFunctionKinematicDaoService();
    virtual ~ConvolCoeffFunctionKinematicDaoService();

    int insert(const DVCSConvolCoeffFunctionKinematic &kinematic) const;
    int getKinematicId(const DVCSConvolCoeffFunctionKinematic &kinematic) const;

private:
    ConvolCoeffFunctionKinematicDao m_convolCoeffFunctionKinematicDao;
};

#endif /* CONVOL_COEFF_FUNCTION_KINEMATIC_DAO_SERVICE_H */
