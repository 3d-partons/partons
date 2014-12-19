#include <stddef.h>
#include <vector>

#include "../beans/gpd/GPDComputeType.h"
#include "../beans/gpd/GPDOutputData.h"
#include "../beans/gpd/ListGPDOutputData.h"
#include "../beans/kinematic/GPDKinematic.h"
#include "../modules/GPDModule.h"
#include "GPDService.h"

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

GPDOutputData GPDService::computeGPDModelRestrictedByGPDType(
        GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        GPDComputeType::Type gpdComputeType) {
    return pGPDModule->compute(gpdKinematic.getX(), gpdKinematic.getXi(),
            gpdKinematic.getT(), gpdKinematic.getMuF(), gpdKinematic.getMuR(),
            gpdComputeType);
}

GPDOutputData GPDService::computeGPDModelWithEvolution(
        GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        EvolQCDModule* pEvolQCDModule, GPDComputeType::Type gpdComputeType) {
    pGPDModule->setEvolQcdModule(pEvolQCDModule);

    return pGPDModule->computeWithEvolution(gpdKinematic.getX(),
            gpdKinematic.getXi(), gpdKinematic.getT(), gpdKinematic.getMuF(),
            gpdKinematic.getMuR(), gpdComputeType);
}

GPDOutputData GPDService::computeGPDModel(GPDKinematic &gpdKinematic,
        GPDModule* pGPDModule) {
    return computeGPDModelRestrictedByGPDType(gpdKinematic, pGPDModule,
            GPDComputeType::ALL);
}

ListGPDOutputData GPDService::computeListOfGPDModel(GPDKinematic &gpdKinematic,
        std::vector<GPDModule*> &listOfGPDToCompute) {
    return computeListOfGPDModelRestrictedByGPDType(gpdKinematic,
            listOfGPDToCompute, GPDComputeType::ALL);
}

ListGPDOutputData GPDService::computeListOfGPDModelRestrictedByGPDType(
        GPDKinematic &gpdKinematic, std::vector<GPDModule*> &listOfGPDToCompute,
        GPDComputeType gpdComputeType) {
    ListGPDOutputData results = ListGPDOutputData();

    // compute inputData for each GPDModule
    for (size_t i = 0; i != listOfGPDToCompute.size(); i++) {
        results.add(listOfGPDToCompute[i]->getClassName(),
                computeGPDModelRestrictedByGPDType(gpdKinematic,
                        listOfGPDToCompute[i], gpdComputeType));
    }

    // return a list of outputData (one outputData by GPDModule computed)
    return results;
}

ListGPDOutputData GPDService::computeListOfKinematic(
        std::vector<GPDKinematic> &listOfGPDKinematic, GPDModule* pGPDModule) {
    ListGPDOutputData results = ListGPDOutputData();

    // compute GPDModule for each inputData
    for (size_t i = 0; i != listOfGPDKinematic.size(); i++) {
        results.add(pGPDModule->getClassName(),
                computeGPDModelRestrictedByGPDType(listOfGPDKinematic[i],
                        pGPDModule, GPDComputeType::ALL));
    }

    return results;
}
