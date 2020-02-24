#ifndef DVCS_CONVOL_COEFF_FUNCTION_RESULT_H
#define DVCS_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file DVCSConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include "../ConvolCoeffFunctionResult.h"
#include "DVCSConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionResult
 *
 * @brief Class representing single result of DVCS CFF computation.
 *
 * This class is used to store results of a single DVCS CCF computation.
 */
class DVCSConvolCoeffFunctionResult: public ConvolCoeffFunctionResult<
        DVCSConvolCoeffFunctionKinematic> {

public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic DVCS CCF kinematics to be assigned.
     */
    DVCSConvolCoeffFunctionResult(
            const DVCSConvolCoeffFunctionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSConvolCoeffFunctionResult(const DVCSConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionResult();
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_H */
