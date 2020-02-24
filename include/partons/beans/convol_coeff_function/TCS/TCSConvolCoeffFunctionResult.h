#ifndef TCS_CONVOL_COEFF_FUNCTION_RESULT_H
#define TCS_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file TCSConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include "../ConvolCoeffFunctionResult.h"
#include "TCSConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class TCSConvolCoeffFunctionResult
 *
 * @brief Class representing single result of TCS CFF computation.
 *
 * This class is used to store results of a single TCS CCF computation.
 */
class TCSConvolCoeffFunctionResult: public ConvolCoeffFunctionResult<
        TCSConvolCoeffFunctionKinematic> {

public:

    /**
     * Default constructor.
     */
    TCSConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic TCS CCF kinematics to be assigned.
     */
    TCSConvolCoeffFunctionResult(
            const TCSConvolCoeffFunctionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSConvolCoeffFunctionResult(const TCSConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~TCSConvolCoeffFunctionResult();
};

} /* namespace PARTONS */

#endif /* TCS_CONVOL_COEFF_FUNCTION_RESULT_H */
