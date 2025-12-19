#ifndef JET_CONVOL_COEFF_FUNCTION_RESULT_H
#define JET_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file JETConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include "../ConvolCoeffFunctionResult.h"
#include "JETConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class JETConvolCoeffFunctionResult
 *
 * @brief Class representing single result of JET CFF computation.
 *
 * This class is used to store results of a single JET CCF computation.
 */
class JETConvolCoeffFunctionResult: public ConvolCoeffFunctionResult<
        JETConvolCoeffFunctionKinematic> {

public:

    /**
     * Default constructor.
     */
    JETConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic JET CCF kinematics to be assigned.
     */
    JETConvolCoeffFunctionResult(
            const JETConvolCoeffFunctionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETConvolCoeffFunctionResult(const JETConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~JETConvolCoeffFunctionResult();
};

} /* namespace PARTONS */

#endif /* JET_CONVOL_COEFF_FUNCTION_RESULT_H */
