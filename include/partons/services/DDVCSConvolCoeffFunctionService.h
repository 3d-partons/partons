#ifndef DDVCS_CONVOL_COEFF_FUNCTION_SERVICE_H
#define DDVCS_CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file DDVCSConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h"
#include "../beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionResult.h"
#include "../beans/List.h"
#include "../modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "ConvolCoeffFunctionService.h"

namespace PARTONS {
class Task;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DDVCSConvolCoeffFunctionService
 *
 * @brief Singleton to handle and compute some pre-configured CCF modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class DDVCSConvolCoeffFunctionService: public ConvolCoeffFunctionService<
        DDVCSConvolCoeffFunctionKinematic, DDVCSConvolCoeffFunctionResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DDVCSConvolCoeffFunctionService();

    virtual void resolveObjectDependencies();
    virtual DDVCSConvolCoeffFunctionKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<DDVCSConvolCoeffFunctionKinematic> newListOfKinematicFromTask(
            const Task &task) const;
    virtual void storeResultListInDatabase(
            const List<DDVCSConvolCoeffFunctionResult>& results) const;
    virtual void generatePlotFileTask(Task &task);

  /**
     * Uses an automation task (XML file) to configure a DDVCSConvolCoeffFunctionModule.
     * @param task Automation task.
     * @return Pre-configured DDVCSConvolCoeffFunctionModule.
     */
    DDVCSConvolCoeffFunctionModule* newDDVCSConvolCoeffFunctionModuleFromTask(
            const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    DDVCSConvolCoeffFunctionService(const std::string &className);

    virtual ConvolCoeffFunctionModule<DDVCSConvolCoeffFunctionKinematic,
            DDVCSConvolCoeffFunctionResult>* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* DDVCS_CONVOL_COEFF_FUNCTION_SERVICE_H */
