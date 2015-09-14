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
#include "../beans/gpd/GPDType.h"
#include "../ServiceObject.h"

class Scenario;

class GPDResultList;

class GPDEvolutionModule;
class GPDModule;
class GPDResult;

class GPDService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string GPD_SERVICE_COMPUTE_GPD_MODEL;

    /**
     * Default constructor
     */
    GPDService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~GPDService();

    virtual void computeTask(const Task &task);

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
            GPDType::Type gpdType);

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

//    /**
//     * Computes a list of GPD models at specific kinematic
//     *
//     * @param gpdKinematic
//     * @param listOfGPDToCompute
//     * @return
//     */
//    ListGPDOutputData computeListOfGPDModel(GPDKinematic* pGPDKinematic,
//            std::vector<GPDModule*> &listOfGPDToCompute);
//
//    /**
//     * Computes a list of GPD models at specific kinematic, restricted by a GPD's type
//     *
//     * @param gpdKinematic
//     * @param listOfGPDToCompute
//     * @param gpdType
//     * @return
//     */
//    ListGPDOutputData computeListOfGPDModelRestrictedByGPDType(
//            GPDKinematic* pGPDKinematic,
//            std::vector<GPDModule*> &listOfGPDToCompute, GPDType gpdType);

    /**
     * Computes GPD model by a kinematics list
     *
     * @param listOfGPDKinematic
     * @param pGPDModule
     * @return
     */
    GPDResultList computeListOfKinematic(
            std::vector<GPDKinematic> &listOfGPDKinematic,
            GPDModule* pGPDModule);

private:

    GPDKinematic* m_pGPDKinematic;
    GPDModule* m_pGPDModule;

    // GPDResult computeGPDModel(const Task &task);
};

#endif /* GPD_SERVICE_H */
