#ifndef COLLINEAR_DISTRIBUTION_SERVICE_H
#define COLLINEAR_DISTRIBUTION_SERVICE_H

/**
 * @file CollinearDistributionService.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date July 18, 2020
 * @version 1.0
 */

#include <string>

#include "../beans/parton_distribution/CollinearDistributionKinematic.h"
#include "../beans/parton_distribution/CollinearDistributionResult.h"
#include "../beans/parton_distribution/CollinearDistributionType.h"
#include "../beans/List.h"
#include "../ServiceObjectTyped.h"

namespace PARTONS {
class CollinearDistributionModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class CollinearDistributionService
 *
 * @brief Class to handle and compute collinear-distribution modules.
 */
class CollinearDistributionService: public ServiceObjectTyped<CollinearDistributionKinematic, CollinearDistributionResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string COLLINEAR_DISTRIBUTION_SERVICE_COMPUTE_SINGLE_KINEMATIC; ///< Name of the XML task used to compute a collinear-distribution at given kinematics.
    static const std::string COLLINEAR_DISTRIBUTION_SERVICE_COMPUTE_MANY_KINEMATIC; ///< Name of the XML task used to compute collinear-distributions for a list of kinematics.
    static const std::string COLLINEAR_DISTRIBUTION_SERVICE_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    /**
     * Default destructor.
     */
    virtual ~CollinearDistributionService();

    virtual void resolveObjectDependencies();
    virtual void computeTask(Task &task);

    /**
     * Computes collinear-distribution model at specific kinematics.
     *
     * @param colldistKinematic collinear-distribution kinematic.
     * @param pCollinearDistributionModule CollinearDistributionModule to use.
     * @param colldistType List of CollinearDistributionType to compute. Default: all the CollinearDistributionTypes available with the CollinearDistributionModule.
     * @return CollinearDistributionResult
     */
    CollinearDistributionResult computeSingleKinematic(const CollinearDistributionKinematic &colldistKinematic,
            CollinearDistributionModule* pCollinearDistributionModule, const List<CollinearDistributionType>& colldistType =
                    List<CollinearDistributionType>()) const;

    /**
     * Computes a collinear-distribution model for a list of kinematics.
     *
     * @param colldistKinematicList List of CollinearDistributionKinematics.
     * @param pCollinearDistributionModule CollinearDistributionModule to use for the computation.
     * @param colldistTypeList List of CollinearDistributionType to compute. Default: all the CollinearDistributionTypes available with the CollinearDistributionModule.
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of CollinearDistributionResults.
     */
    List<CollinearDistributionResult> computeManyKinematic(
            const List<CollinearDistributionKinematic> &colldistKinematicList, CollinearDistributionModule* pCollinearDistributionModule,
            const List<CollinearDistributionType>& colldistTypeList = List<CollinearDistributionType>());

    /**
     * Devises the CollinearDistributionModule from an automation task.
     *
     * @param task Automation task.
     * @return CollinearDistributionModule pointer.
     */
    CollinearDistributionModule* newCollinearDistributionModuleFromTask(const Task &task) const;

    /**
     * Devises the collinear-distribution kinematics from an automation task.
     *
     * @param task Automation task.
     * @return collinear-distribution kinematics.
     */
    CollinearDistributionKinematic newKinematicFromTask(const Task &task) const;

    /**
     * Devises the collinear-distribution kinematics from an automation task.
     *
     * @param task Automation task.
     * @return List of collinear-distribution kinematics.
     */
    List<CollinearDistributionKinematic> newListOfKinematicFromTask(const Task &task) const;

protected:

    /**
     * Default constructor used by the registry.
     */
    CollinearDistributionService(const std::string &className);

private:

    /**
     * Method used in the automated interface to compute collinear-distribution.
     * @param task Automated XML task.
     * @return CollinearDistributionResult object.
     */
    CollinearDistributionResult computeSingleKinematicTask(Task &task);

    /**
     * Method used in the automated interface to compute collinear-distributions for a list of kinematics.
     * @param task Automated XML task.
     * @return List of collinear-distribution results.
     */
    List<CollinearDistributionResult> computeManyKinematicTask(Task &task);

    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task);

    /**
     * Method used to derive an intersection of available collinear-distribution types from the various underlying modules.
     * @param pCollinearDistributionModule CollinearDistributionModule used for the computation.
     * @param colldistTypeList List of desired collinear-distribution types to compute.
     * @return List of collinear-distribution types.
     */
    List<CollinearDistributionType> getFinalCollinearDistributionTypeList(CollinearDistributionModule* pCollinearDistributionModule,
            const List<CollinearDistributionType> &colldistTypeList) const;
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_SERVICE_H */
