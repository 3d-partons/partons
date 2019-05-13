#ifndef TCS_CONVOL_COEFF_FUNCTION_SERVICE_H
#define TCS_CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file TCSConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h"
#include "../beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h"
#include "../beans/List.h"
#include "../modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "ConvolCoeffFunctionService.h"

namespace PARTONS {
class Task;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class TCSConvolCoeffFunctionService
 *
 * @brief Singleton to handle and compute some pre-configured CCF modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class TCSConvolCoeffFunctionService: public ConvolCoeffFunctionService<
        TCSConvolCoeffFunctionKinematic, TCSConvolCoeffFunctionResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~TCSConvolCoeffFunctionService();

    virtual void resolveObjectDependencies();
    virtual TCSConvolCoeffFunctionKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<TCSConvolCoeffFunctionKinematic> newListOfKinematicFromTask(
            const Task &task) const;
    virtual void storeResultListInDatabase(
            const List<TCSConvolCoeffFunctionResult>& results) const;
    virtual void generatePlotFileTask(Task &task);

protected:

    /**
     * Default constructor.
     */
    TCSConvolCoeffFunctionService(const std::string &className);

    virtual ConvolCoeffFunctionModule<TCSConvolCoeffFunctionKinematic,
            TCSConvolCoeffFunctionResult>* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* TCS_CONVOL_COEFF_FUNCTION_SERVICE_H */
