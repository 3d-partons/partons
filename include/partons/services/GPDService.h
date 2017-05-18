#ifndef GPD_SERVICE_H
#define GPD_SERVICE_H

/**
 * @file GPDService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date Aout 04, 2014
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../modules/GPDModule.h"
#include "../ServiceObjectTyped.h"

class GPDEvolutionModule;

/**
 * @class GPDService
 *
 * @brief \<singleton\> Used to handle and compute some pre-configured GPD models.
 *
 * See [tutorial](@ref services_computation).
 */
class GPDService: public ServiceObjectTyped<GPDKinematic, GPDResult> {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string GPD_SERVICE_COMPUTE_GPD_MODEL; ///< Name of the XML task used to compute a GPD at given kinematics.
    static const std::string GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION; ///< Name of the XML task used to compute a GPD with evolution.
    static const std::string GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL; //TODO What's this?!
    static const std::string GPD_SERVICE_COMPUTE_MANY_KINEMATIC_ONE_MODEL; ///< Name of the XML task used to compute GPDs for a list of kinematics.
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    /**
     * Default constructor used by the registry. Do not use directly!
     */
    GPDService(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GPDService();

    /**
     * See parent class for details.
     */
    virtual void resolveObjectDependencies();

    /**
     * Method used in automation to compute given tasks.
     * @param task Automation task to compute.
     */
    virtual void computeTask(Task &task);

//    GPDResult computeGPDModel_two(const GPDKinematic &gpdKinematic,
//            GPDModule* pGPDModule,
//            std::vector<GPDType::Type> restrictedByGPDTypeList = std::vector<
//                    GPDType::Type>());


    /**
     * Computes GPD model at specific kinematics.
     *
     * @param gpdKinematic GPD kinematic.
     * @param pGPDModule GPDModule to use.
     * @param gpdType List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @return GPDResult
     */
    GPDResult computeGPDModel(const GPDKinematic &gpdKinematic,
            GPDModule* pGPDModule, const List<GPDType> & gpdType =
                    List<GPDType>()) const;

    /**
     * Computes GPD model at specific kinematic with a QCD evolution model.
     *
     * @param gpdKinematic GPD kinematic.
     * @param pGPDModule GPDModule to use.
     * @param pEvolQCDModule Evolution module to use.
     * @param gpdType List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @return GPDResult.
     */
    GPDResult computeGPDModelWithEvolution(const GPDKinematic &gpdKinematic,
            GPDModule* pGPDModule, GPDEvolutionModule* pEvolQCDModule,
            GPDType::Type gpdType = GPDType::ALL);

    /**
     * Computes several GPDModels at specific kinematics.
     *
     * @param gpdKinematic GPD kinematic.
     * @param listOfGPDToCompute List of GPDModules to use.
     * @return List of GPDResults.
     */
    List<GPDResult> computeListOfGPDModel(const GPDKinematic &gpdKinematic,
            std::vector<GPDModule*> &listOfGPDToCompute);

    /**
     * Computes several GPDModels at specific kinematics, restricted by GPDType.
     *
     * @param gpdKinematic GPD kinematic.
     * @param listOfGPDToCompute List of GPDModules to use.
     * @param gpdType GPDType to compute.
     * @return List of GPDResults.
     */
    List<GPDResult> computeListOfGPDModelRestrictedByGPDType(
            const GPDKinematic &gpdKinematic,
            std::vector<GPDModule*> &listOfGPDToCompute, GPDType gpdType);


    /**
     * Computes a GPD model for a list of kinematics.
     *
     * @param gpdKinematicList List of GPDKinematics.
     * @param pGPDModule GPDModule to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of GPDResults.
     */
    List<GPDResult> computeManyKinematicOneModel(
            const List<GPDKinematic> &gpdKinematicList, GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList = List<GPDType>(),
            const bool storeInDB = 0);

//    ComparisonReport compareResultListToDatabase(
//            const std::string &scenarioTestFilePath);

    /**
     * Devises the GPDModule from an automation task.
     *
     * @param task Automation task.
     * @return GPDModule pointer.
     */
    GPDModule* newGPDModuleFromTask(const Task &task) const;

    // GPDModule* configureGPDModule(GPDModule* pGPDModule) const;

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

private:
    GPDKinematic* m_pGPDKinematic; //TODO What's that?!
    GPDModule* m_pGPDModule; ///< Pointer to the currently used GPD module.

    /**
     * Method used in the automated interface to compute GPD.
     * @param task Automated XML task.
     * @return GPDResult object.
     */
    GPDResult computeGPDTask(Task &task);
    /**
     * Method used in the automated interface to compute GPDs for a list of kinematics.
     * @param task Automated XML task.
     * @return List of GPD results.
     */
    List<GPDResult> computeManyKinematicOneModelTask(Task &task);
    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task);

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pGPDModule GPDModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList) const;
};

#endif /* GPD_SERVICE_H */
