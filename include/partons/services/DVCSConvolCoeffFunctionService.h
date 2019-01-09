#ifndef DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H
#define DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file DVCSConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../beans/List.h"
#include "../modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "ConvolCoeffFunctionService.h"

namespace PARTONS {
class Task;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionService
 *
 * @brief Singleton to handle and compute some pre-configured CCF modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class DVCSConvolCoeffFunctionService: public ConvolCoeffFunctionService<
        DVCSConvolCoeffFunctionKinematic, DVCSConvolCoeffFunctionResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionService();

    void resolveObjectDependencies();
    DVCSConvolCoeffFunctionKinematic newKinematicFromTask(
            const Task &task) const;
    List<DVCSConvolCoeffFunctionKinematic> newListOfKinematicFromTask(
            const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionService(const std::string &className);

    virtual ConvolCoeffFunctionModule<DVCSConvolCoeffFunctionKinematic,
            DVCSConvolCoeffFunctionResult>* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H */
