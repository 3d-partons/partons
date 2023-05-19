#ifndef DDVCS_CONVOL_COEFF_FUNCTION_RESULT_H
#define DDVCS_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file DDVCSConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include "../ConvolCoeffFunctionResult.h"
#include "DDVCSConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionResult
 *
 * @brief Class representing single result of DDVCS CFF computation.
 *
 * This class is used to store results of a single DDVCS CCF computation.
 */
class DDVCSConvolCoeffFunctionResult: public ConvolCoeffFunctionResult<
        DDVCSConvolCoeffFunctionKinematic> {

public:

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic DDVCS CCF kinematics to be assigned.
     */
    DDVCSConvolCoeffFunctionResult(
            const DDVCSConvolCoeffFunctionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSConvolCoeffFunctionResult(const DDVCSConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionResult();
};

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_RESULT_H */
