#ifndef DVMP_CONVOL_COEFF_FUNCTION_RESULT_H
#define DVMP_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file DVMPConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include "../ConvolCoeffFunctionResult.h"
#include "DVMPConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionResult
 *
 * @brief Class representing single result of DVMP CFF computation.
 *
 * This class is used to store results of a single DVMP CCF computation.
 */
class DVMPConvolCoeffFunctionResult: public ConvolCoeffFunctionResult<
        DVMPConvolCoeffFunctionKinematic> {

public:

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic DVMP CCF kinematics to be assigned.
     */
    DVMPConvolCoeffFunctionResult(
            const DVMPConvolCoeffFunctionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPConvolCoeffFunctionResult(const DVMPConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionResult();
};

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_RESULT_H */
