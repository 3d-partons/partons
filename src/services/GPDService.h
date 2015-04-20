#ifndef GPD_SERVICE_H
#define GPD_SERVICE_H

/**
 * @file GPDService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 04 Aout 2014
 * @version 1.0
 *
 * @class GPDService
 * @brief \<singleton\> Use for handle and compute some pre-configured GPD models.
 */

#include <string>
#include <vector>

#include "../beans/gpd/GPDComputeType.h"
#include "../modules/GPDModule.h"
#include "Service.h"

class GPDKinematic;

class GPDOutputData;
class ListGPDOutputData;

class GPDService: public Service {
public:
    static const std::string COMPUTE_GPD_MODEL;

    static const std::string ID; ///< Unique ID to self-register in the registry

    /**
     * A static function that provides a unique pointer of this class
     *
     * @return A unique pointer of this class
     */
    static GPDService* getInstance();

    /**
     * Default destructor
     */
    virtual ~GPDService();

    virtual void computeScenario(Scenario scenario);

    /**
     * Computes GPD model at specific kinematic
     *
     * @param gpdKinematic
     * @param pGPDModule
     * @return
     */
    GPDOutputData computeGPDModel(GPDKinematic* pGPDKinematic,
            GPDModule* pGPDModule);

    /**
     * Computes the GPD model at specific kinematic, restricted by a GPD's type
     *
     * @param gpdKinematic
     * @param pGPDModule
     * @param gpdComputeType
     * @return
     */
    GPDOutputData computeGPDModelRestrictedByGPDType(
            GPDKinematic* pGPDKinematic, GPDModule* pGPDModule,
            GPDComputeType::Type gpdComputeType);

    /**
     * Computes GPD model at specific kinematic with a QCD evolution model.
     *
     * @param pGPDModule
     * @param pEvolQCDModule
     * @param gpdKinematic
     * @param gpdComputeType
     * @return
     */
    GPDOutputData computeGPDModelWithEvolution(GPDKinematic* pGPDKinematic,
            GPDModule* pGPDModule, EvolQCDModule* pEvolQCDModule,
            GPDComputeType::Type gpdComputeType = GPDComputeType::ALL);

    /**
     * Computes a list of GPD models at specific kinematic
     *
     * @param gpdKinematic
     * @param listOfGPDToCompute
     * @return
     */
    ListGPDOutputData computeListOfGPDModel(GPDKinematic* pGPDKinematic,
            std::vector<GPDModule*> &listOfGPDToCompute);

    /**
     * Computes a list of GPD models at specific kinematic, restricted by a GPD's type
     *
     * @param gpdKinematic
     * @param listOfGPDToCompute
     * @param gpdComputeType
     * @return
     */
    ListGPDOutputData computeListOfGPDModelRestrictedByGPDType(
            GPDKinematic* pGPDKinematic,
            std::vector<GPDModule*> &listOfGPDToCompute,
            GPDComputeType gpdComputeType);

//    /**
//     * Computes GPD model by a kinematics list
//     *
//     * @param listOfGPDKinematic
//     * @param pGPDModule
//     * @return
//     */
//    ListGPDOutputData computeListOfKinematic(
//            std::vector<GPDKinematic> &listOfGPDKinematic,
//            GPDModule* pGPDModule);

private:
    static GPDService* pInstance; ///< Private pointer of this class for a unique instance

    /**
     * Private default constructor for a unique instance of this class
     */
    GPDService();

    GPDKinematic* m_pGPDKinematic;
    GPDModule* m_pGPDModule;

    GPDOutputData computeGPDModel(Scenario scenario);
};

#endif /* GPD_SERVICE_H */
