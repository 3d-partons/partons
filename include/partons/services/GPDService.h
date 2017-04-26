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
 * @brief \<singleton\> Used to handle and compute some pre-configured GPD models. See tutorial (TODO: add link to tutorial).
 */
class GPDService: public ServiceObjectTyped<GPDKinematic, GPDResult> {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string GPD_SERVICE_COMPUTE_GPD_MODEL;
    static const std::string GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION;
    static const std::string GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL;
    static const std::string GPD_SERVICE_COMPUTE_MANY_KINEMATIC_ONE_MODEL;
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE;

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
    GPDKinematic* m_pGPDKinematic;
    GPDModule* m_pGPDModule;

    GPDResult computeGPDTask(Task &task);
    List<GPDResult> computeManyKinematicOneModelTask(Task &task);
    void generatePlotFileTask(Task &task);

    List<GPDType> getFinalGPDTypeList(GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList) const;
};

#endif /* GPD_SERVICE_H */
