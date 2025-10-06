#ifndef GPD_SERVICE_H
#define GPD_SERVICE_H

/**
 * @file GPDService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date Aout 04, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../ServiceObjectTyped.h"

namespace PARTONS {
class GPDModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class GPDService
 *
 * @brief Class to handle and compute GPD modules.
 */
class GPDService: public ServiceObjectTyped<GPDKinematic, GPDResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string GPD_SERVICE_COMPUTE_SINGLE_KINEMATIC; ///< Name of the XML task used to compute a GPD at given kinematics.
    static const std::string GPD_SERVICE_COMPUTE_MANY_KINEMATIC; ///< Name of the XML task used to compute GPDs for a list of kinematics.

    /**
     * Default destructor.
     */
    virtual ~GPDService();

    virtual void resolveObjectDependencies();
    virtual void computeTask(Task &task);

    /**
     * Computes GPD model at specific kinematics.
     *
     * @param gpdKinematic GPD kinematic.
     * @param pGPDModule GPDModule to use.
     * @param gpdType List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @return GPDResult
     */
    GPDResult computeSingleKinematic(const GPDKinematic &gpdKinematic,
            GPDModule* pGPDModule, const List<GPDType>& gpdType =
                    List<GPDType>()) const;

    /**
     * Computes a GPD model for a list of kinematics.
     *
     * @param gpdKinematicList List of GPDKinematics.
     * @param pGPDModule GPDModule to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of GPDResults.
     */
    List<GPDResult> computeManyKinematic(
            const List<GPDKinematic> &gpdKinematicList, GPDModule* pGPDModule,
            const List<GPDType>& gpdTypeList = List<GPDType>());

    /**
     * Devises the GPDModule from an automation task.
     *
     * @param task Automation task.
     * @return GPDModule pointer.
     */
    GPDModule* newGPDModuleFromTask(const Task &task) const;

    /**
     * Devises the GPD kinematics from an automation task.
     *
     * @param task Automation task.
     * @return GPD kinematics.
     */
    GPDKinematic newKinematicFromTask(const Task &task) const;

    /**
     * Devises the GPD kinematics from an automation task.
     *
     * @param task Automation task.
     * @return List of GPD kinematics.
     */
    List<GPDKinematic> newListOfKinematicFromTask(const Task &task) const;

protected:

    /**
     * Default constructor used by the registry.
     */
    GPDService(const std::string &className);

private:

    /**
     * Method used in the automated interface to compute GPD.
     * @param task Automated XML task.
     * @return GPDResult object.
     */
    GPDResult computeSingleKinematicTask(Task &task);

    /**
     * Method used in the automated interface to compute GPDs for a list of kinematics.
     * @param task Automated XML task.
     * @return List of GPD results.
     */
    List<GPDResult> computeManyKinematicTask(Task &task);

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pGPDModule GPDModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList) const;
};

} /* namespace PARTONS */

#endif /* GPD_SERVICE_H */
