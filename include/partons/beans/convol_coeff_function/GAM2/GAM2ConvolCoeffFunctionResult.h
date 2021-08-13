#ifndef GAM2_CONVOL_COEFF_FUNCTION_RESULT_H
#define GAM2_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file GAM2ConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include "../ConvolCoeffFunctionResult.h"
#include "GAM2ConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class GAM2ConvolCoeffFunctionResult
 *
 * @brief Class representing single result of two photon production CFF computation.
 *
 * This class is used to store results of a single two photon production CCF computation.
 */
class GAM2ConvolCoeffFunctionResult: public ConvolCoeffFunctionResult<
        GAM2ConvolCoeffFunctionKinematic> {

public:

    /**
     * Default constructor.
     */
    GAM2ConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic GAM2 CCF kinematics to be assigned.
     */
    GAM2ConvolCoeffFunctionResult(
            const GAM2ConvolCoeffFunctionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2ConvolCoeffFunctionResult(const GAM2ConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~GAM2ConvolCoeffFunctionResult();
};

} /* namespace PARTONS */

#endif /* GAM2_CONVOL_COEFF_FUNCTION_RESULT_H */
