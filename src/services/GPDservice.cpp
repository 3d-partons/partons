#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../beans/gpd/GPDResult.h"
#include "../beans/gpd/GPDResultList.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/kinematic/GPDKinematic.h"
#include "../beans/Scenario.h"
#include "../modules/GPDModule.h"
#include "../ServiceRegistry.h"
#include "GPDService.h"
#include "ServiceFunctionNames.h"

const std::string GPDService::ID =
        ServiceRegistry::getInstance()->registerNewService(getInstance());

// Global static pointer used to ensure a single instance of the class.
GPDService* GPDService::pInstance = 0;

GPDService::GPDService()
        : Service("GPDService"), m_pGPDKinematic(0), m_pGPDModule(0) {
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

//TODO implement all function
//TODO passer les chaine de caractere en variable final static
void GPDService::computeScenario(Scenario scenario) {
    m_pGPDKinematic = 0;
    m_pGPDModule = 0;

    switch ((unsigned int) scenario.getFunctionName()) {
    case ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL: {
        GPDResult gpdResult = computeGPDModel(scenario);

        std::cout << gpdResult.toString() << std::endl;

        break;
    }

    default: {
        throw std::runtime_error(
                "[GPDService::computeScenario] unknown function name = "
                        + scenario.getFunctionName());
    }
    }
}

GPDResult GPDService::computeGPDModel(Scenario scenario) {
    m_pGPDKinematic =
            static_cast<GPDKinematic*>((scenario.getFunctionArgs().find(
                    "GPDKinematic"))->second);
    m_pGPDModule = static_cast<GPDModule*>((scenario.getFunctionArgs().find(
            "GPDModule"))->second);

    return computeGPDModel(*m_pGPDKinematic, m_pGPDModule);
}

GPDResult GPDService::computeGPDModelRestrictedByGPDType(
        const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        GPDType::Type gpdType) {

    GPDResult gpdResult = pGPDModule->compute(gpdKinematic.getX(),
            gpdKinematic.getXi(), gpdKinematic.getT(), gpdKinematic.getMuF(),
            gpdKinematic.getMuR(), gpdType);
    gpdResult.setGpdKinematic(gpdKinematic);

    gpdResult.setComputedByGpdModuleId(pGPDModule->getClassName());

    return gpdResult;
}

GPDResult GPDService::computeGPDModelWithEvolution(
        const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        EvolQCDModule* pEvolQCDModule, GPDType::Type gpdType) {
    pGPDModule->setEvolQcdModule(pEvolQCDModule);

    GPDResult gpdResult = pGPDModule->computeWithEvolution(gpdKinematic.getX(),
            gpdKinematic.getXi(), gpdKinematic.getT(), gpdKinematic.getMuF(),
            gpdKinematic.getMuR(), gpdType);
    gpdResult.setGpdKinematic(gpdKinematic);

    return gpdResult;
}

GPDResult GPDService::computeGPDModel(const GPDKinematic &gpdKinematic,
        GPDModule* pGPDModule) {
    return computeGPDModelRestrictedByGPDType(gpdKinematic, pGPDModule,
            GPDType::ALL);
}

//ListGPDOutputData GPDService::computeListOfGPDModel(GPDKinematic* pGPDKinematic,
//        std::vector<GPDModule*> &listOfGPDToCompute) {
//    return computeListOfGPDModelRestrictedByGPDType(pGPDKinematic,
//            listOfGPDToCompute, GPDType::ALL);
//}
//
//ListGPDOutputData GPDService::computeListOfGPDModelRestrictedByGPDType(
//        GPDKinematic* pGPDKinematic,
//        std::vector<GPDModule*> &listOfGPDToCompute, GPDType gpdComputeType) {
//    ListGPDOutputData results = ListGPDOutputData();
//
//    // compute inputData for each GPDModule
//    for (size_t i = 0; i != listOfGPDToCompute.size(); i++) {
//        results.add(listOfGPDToCompute[i]->getClassName(),
//                computeGPDModelRestrictedByGPDType(pGPDKinematic,
//                        listOfGPDToCompute[i], gpdComputeType));
//    }
//
//    // return a list of outputData (one outputData by GPDModule computed)
//    return results;
//}

GPDResultList GPDService::computeListOfKinematic(
        std::vector<GPDKinematic> &listOfGPDKinematic, GPDModule* pGPDModule) {
    GPDResultList results = GPDResultList();

// compute GPDModule for each inputData
    for (unsigned int i = 0; i != listOfGPDKinematic.size(); i++) {
        results.add(
                computeGPDModelRestrictedByGPDType(listOfGPDKinematic[i],
                        pGPDModule, GPDType::ALL));
    }

    return results;
}
