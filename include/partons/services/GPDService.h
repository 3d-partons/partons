#ifndef GPD_SERVICE_H
#define GPD_SERVICE_H

/**
 * @file GPDService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 04 Aout 2014
 * @version 1.0
 *
 * @class GPDService
 *
 * @brief \<singleton\> Use for handle and compute some pre-configured GPD models.
 */

#include <string>
#include <vector>

#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../beans/ResultList.h"
#include "../modules/GPDModule.h"
#include "../ServiceObjectTyped.h"

class GPDEvolutionModule;

class GPDService: public ServiceObjectTyped<GPDKinematic, GPDResult> {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string GPD_SERVICE_COMPUTE_GPD_MODEL;
    static const std::string GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION;
    static const std::string GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL;

    /**
     * Default constructor
     */
    GPDService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~GPDService();

    virtual void computeTask(Task &task);

    /**
     * Computes GPD model at specific kinematic
     *
     * @param gpdKinematic
     * @param pGPDModule
     * @return
     */
    GPDResult computeGPDModel(const GPDKinematic &gpdKinematic,
            GPDModule* pGPDModule);

    /**
     * Computes the GPD model at specific kinematic, restricted by a GPD's type
     *
     * @param gpdKinematic
     * @param pGPDModule
     * @param gpdType
     * @return
     */
    GPDResult computeGPDModelRestrictedByGPDType(
            const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
            GPDType::Type gpdType) const;

    /**
     * Computes GPD model at specific kinematic with a QCD evolution model.
     *
     * @param pGPDModule
     * @param pEvolQCDModule
     * @param gpdKinematic
     * @param gpdType
     * @return
     */
    GPDResult computeGPDModelWithEvolution(const GPDKinematic &gpdKinematic,
            GPDModule* pGPDModule, GPDEvolutionModule* pEvolQCDModule,
            GPDType::Type gpdType = GPDType::ALL);

    /**
     *
     * @param gpdKinematic
     * @param listOfGPDToCompute
     * @return
     */
    ResultList<GPDResult> computeListOfGPDModel(
            const GPDKinematic &gpdKinematic,
            std::vector<GPDModule*> &listOfGPDToCompute);

    /**
     *
     * @param gpdKinematic
     * @param listOfGPDToCompute
     * @param gpdType
     * @return
     */
    ResultList<GPDResult> computeListOfGPDModelRestrictedByGPDType(
            const GPDKinematic &gpdKinematic,
            std::vector<GPDModule*> &listOfGPDToCompute, GPDType gpdType);

    /**
     * Computes GPD model by a kinematics list
     *
     * @param listOfGPDKinematic
     * @param pGPDModule
     * @return
     */
    ResultList<GPDResult> computeManyKinematicOneModel(
            const List<GPDKinematic> &gpdKinematicList, GPDModule* pGPDModule);

    ComparisonReport compareResultListToDatabase(
            const std::string &scenarioTestFilePath);

    GPDModule* newGPDModuleFromTask(const Task &task) const;

private:
    GPDKinematic* m_pGPDKinematic;
    GPDModule* m_pGPDModule;

    // GPDResult computeGPDModel(const Task &task);
};

#endif /* GPD_SERVICE_H */
