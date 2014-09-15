#include <stddef.h>
#include <vector>

#include "GPDModule.h"
#include "GPDService.h"
#include "ListGPDOutputData.h"
#include "models/GPDComputeType.h"
#include "models/GPDInputData.h"

// Global static pointer used to ensure a single instance of the class.
GPDService* GPDService::pInstance = 0;

GPDService::GPDService() {
}

GPDService* GPDService::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new GPDService();
    }

    return pInstance;
}

GPDService::~GPDService() {
    if (pInstance != 0) {
        delete pInstance;
        pInstance = 0;
    }
}

GPDOutputData* GPDService::compute(GPDInputData* _pGPDInputData,
        GPDModule* _pGPDModule, GPDComputeType::Type _gpdComputeType) {
    return _pGPDModule->compute(_pGPDInputData->getX(), _pGPDInputData->getXi(),
            _pGPDInputData->getT(), _pGPDInputData->getMuF(),
            _pGPDInputData->getMuR(), _gpdComputeType);
}

GPDOutputData* GPDService::compute(GPDInputData* _pGPDInputData,
        GPDModule* _pGPDModule) {
    return compute(_pGPDInputData, _pGPDModule, GPDComputeType::ALL);
}

ListGPDOutputData GPDService::compute(GPDInputData* _pGPDInputData,
        std::vector<GPDModule*> _listOfGPDToCompute) {
    return compute(_pGPDInputData, _listOfGPDToCompute, GPDComputeType::ALL);
}

ListGPDOutputData GPDService::compute(GPDInputData* _pGPDInputData,
        std::vector<GPDModule*> _listOfGPDToCompute,
        GPDComputeType::Type _gpdComputeType) {
    ListGPDOutputData results = ListGPDOutputData();

    // compute inputData for each GPDModule
    for (size_t i = 0; i != _listOfGPDToCompute.size(); i++) {
        results.add(_listOfGPDToCompute[i]->getID(),
                compute(_pGPDInputData, _listOfGPDToCompute[i],
                        _gpdComputeType));
    }

    // return a list of outputData (one outputData by GPDModule computed)
    return results;
}

ListGPDOutputData GPDService::compute(
        std::vector<GPDInputData*> _listOfGPDInputData,
        GPDModule* _pGPDModule) {
    ListGPDOutputData results = ListGPDOutputData();

    // compute GPDModule for each inputData
    for (size_t i = 0; i != _listOfGPDInputData.size(); i++) {
        results.add(_pGPDModule->getID(),
                compute(_listOfGPDInputData[i], _pGPDModule,
                        GPDComputeType::ALL));
    }

    return results;
}
