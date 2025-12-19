#ifndef JET_CONVOL_COEFF_FUNCTION_SERVICE_H
#define JET_CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file JETConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/convol_coeff_function/JET/JETConvolCoeffFunctionKinematic.h"
#include "../beans/convol_coeff_function/JET/JETConvolCoeffFunctionResult.h"
#include "../beans/List.h"
#include "../modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "ConvolCoeffFunctionService.h"

namespace PARTONS {
class Task;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class JETConvolCoeffFunctionService
 *
 * @brief Singleton to handle and compute some pre-configured CCF modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class JETConvolCoeffFunctionService: public ConvolCoeffFunctionService<
        JETConvolCoeffFunctionKinematic, JETConvolCoeffFunctionResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~JETConvolCoeffFunctionService();

    virtual void resolveObjectDependencies();
    virtual JETConvolCoeffFunctionKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<JETConvolCoeffFunctionKinematic> newListOfKinematicFromTask(
            const Task &task) const;

  /**
     * Uses an automation task (XML file) to configure a JETConvolCoeffFunctionModule.
     * @param task Automation task.
     * @return Pre-configured JETConvolCoeffFunctionModule.
     */
    JETConvolCoeffFunctionModule* newJETConvolCoeffFunctionModuleFromTask(
            const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    JETConvolCoeffFunctionService(const std::string &className);

    virtual ConvolCoeffFunctionModule<JETConvolCoeffFunctionKinematic,
            JETConvolCoeffFunctionResult>* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* JET_CONVOL_COEFF_FUNCTION_SERVICE_H */
