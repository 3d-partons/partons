#ifndef GPD_SERVICE_H
#define GPD_SERVICE_H

#include <vector>

#include "../gpd/GPDComputeType.h"
#include "../gpd/GPDInputData.h"
#include "../gpd/GPDModule.h"

class ListGPDOutputData;

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

class GPDInputData;
class GPDOutputData;

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

    GPDOutputData compute(GPDInputData &_gpdInputData, GPDModule* _pGPDModule);

    ListGPDOutputData compute(GPDInputData &_gpdInputData,
            std::vector<GPDModule*> _listOfGPDToCompute);

    ListGPDOutputData compute(GPDInputData &_gpdInputData,
            std::vector<GPDModule*> _listOfGPDToCompute,
            GPDComputeType::Type _gpdComputeType);

    ListGPDOutputData compute(std::vector<GPDInputData> _listOfGPDInputData,
            GPDModule* _pGPDModule);

};

#endif /* GPD_SERVICE_H */
