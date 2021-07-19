#ifndef DVMP_CONVOL_COEFF_FUNCTION_SERVICE_H
#define DVMP_CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file DVMPConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionKinematic.h"
#include "../beans/convol_coeff_function/DVMP/DVMPConvolCoeffFunctionResult.h"
#include "../beans/List.h"
#include "../modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "ConvolCoeffFunctionService.h"

namespace PARTONS {
class Task;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPConvolCoeffFunctionService
 *
 * @brief Singleton to handle and compute some pre-configured CCF modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class DVMPConvolCoeffFunctionService: public ConvolCoeffFunctionService<
        DVMPConvolCoeffFunctionKinematic, DVMPConvolCoeffFunctionResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DVMPConvolCoeffFunctionService();

    virtual void resolveObjectDependencies();
    virtual DVMPConvolCoeffFunctionKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<DVMPConvolCoeffFunctionKinematic> newListOfKinematicFromTask(
            const Task &task) const;
    virtual void storeResultListInDatabase(
            const List<DVMPConvolCoeffFunctionResult>& results) const;
    virtual void generatePlotFileTask(Task &task);

  /**
     * Uses an automation task (XML file) to configure a DVMPConvolCoeffFunctionModule.
     * @param task Automation task.
     * @return Pre-configured DVMPConvolCoeffFunctionModule.
     */
    DVMPConvolCoeffFunctionModule* newDVMPConvolCoeffFunctionModuleFromTask(
            const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    DVMPConvolCoeffFunctionService(const std::string &className);

    virtual ConvolCoeffFunctionModule<DVMPConvolCoeffFunctionKinematic,
            DVMPConvolCoeffFunctionResult>* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* DVMP_CONVOL_COEFF_FUNCTION_SERVICE_H */
