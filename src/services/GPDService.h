#ifndef GPD_SERVICE_H
#define GPD_SERVICE_H

/**
 * @file GPDService.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 04 Aout 2014
 * @version 1.0
 *
 * Last update : 24 September 2014
 *
 * @class GPDService
 * @brief \<singleton\> Use for handle and compute some pre-configured GPD modules.
 */

#include <vector>

#include "../beans/gpd/GPDComputeType.h"
#include "../beans/gpd/GPDInputData.h"
#include "../modules/GPDModule.h"

class GPDOutputData;
class ListGPDOutputData;

class GPDService {
    /**
     * Private pointer of this class for a unique instance
     */
    static GPDService* pInstance;

    /**
     * Private default constructor for a unique instance
     */
    GPDService();

public:
    /**
     * Share a unique pointer of this class
     */
    static GPDService* getInstance();

    /**
     * Default destructor
     */
    ~GPDService();

    GPDOutputData compute(GPDInputData &_gpdInputData, GPDModule* _pGPDModule,
            GPDComputeType::Type _gpdComputeType);

    GPDOutputData computeWithEvolution(GPDModule* pGPDModule,
            EvolQCDModule* pEvolQCDModule, GPDInputData &gpdInputData,
            GPDComputeType::Type gpdComputeType = GPDComputeType::ALL);

    GPDOutputData compute(GPDInputData &_gpdInputData, GPDModule* _pGPDModule);

    //TODO passer la liste en reference
    ListGPDOutputData compute(GPDInputData &_gpdInputData,
            std::vector<GPDModule*> _listOfGPDToCompute);

    //TODO passer la liste en reference
    ListGPDOutputData compute(GPDInputData &_gpdInputData,
            std::vector<GPDModule*> _listOfGPDToCompute,
            GPDComputeType _gpdComputeType);

    //TODO passer la liste en reference
    ListGPDOutputData compute(std::vector<GPDInputData> _listOfGPDInputData,
            GPDModule* _pGPDModule);

};

#endif /* GPD_SERVICE_H */
