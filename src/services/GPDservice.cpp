#include <stddef.h>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../beans/gpd/GPDComputeType.h"
#include "../beans/gpd/GPDOutputData.h"
#include "../beans/gpd/ListGPDOutputData.h"
#include "../beans/kinematic/GPDKinematic.h"
#include "../beans/Scenario.h"
#include "../modules/GPDModule.h"
#include "../ServiceRegistry.h"
#include "../utils/stringUtils/StringUtils.h"
#include "GPDService.h"

const std::string GPDService::COMPUTE_GPD_MODEL = "COMPUTE_GPD_MODEL";

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

    if (StringUtils::equals(scenario.getFunctionName(),
            GPDService::COMPUTE_GPD_MODEL)) {

        GPDOutputData gpdoutputData = computeGPDModel(scenario);

    } else {
        throw std::runtime_error(
                "[GPDService::computeTask] unknown function name = "
                        + scenario.getFunctionName());
    }
}

GPDOutputData GPDService::computeGPDModel(Scenario scenario) {
    m_pGPDKinematic =
            static_cast<GPDKinematic*>((scenario.getFunctionArgs().find(
                    "GPDKinematic"))->second);
    m_pGPDModule = static_cast<GPDModule*>((scenario.getFunctionArgs().find(
            "GPDModule"))->second);

    return computeGPDModel(m_pGPDKinematic, m_pGPDModule);
}

GPDOutputData GPDService::computeGPDModelRestrictedByGPDType(
        GPDKinematic* pGPDKinematic, GPDModule* pGPDModule,
        GPDComputeType::Type gpdComputeType) {
    return pGPDModule->compute(pGPDKinematic->getX(), pGPDKinematic->getXi(),
            pGPDKinematic->getT(), pGPDKinematic->getMuF(),
            pGPDKinematic->getMuR(), gpdComputeType);
}

GPDOutputData GPDService::computeGPDModelWithEvolution(
        GPDKinematic* pGPDKinematic, GPDModule* pGPDModule,
        EvolQCDModule* pEvolQCDModule, GPDComputeType::Type gpdComputeType) {
    pGPDModule->setEvolQcdModule(pEvolQCDModule);

    return pGPDModule->computeWithEvolution(pGPDKinematic->getX(),
            pGPDKinematic->getXi(), pGPDKinematic->getT(),
            pGPDKinematic->getMuF(), pGPDKinematic->getMuR(), gpdComputeType);
}

GPDOutputData GPDService::computeGPDModel(GPDKinematic* pGPDKinematic,
        GPDModule* pGPDModule) {
    return computeGPDModelRestrictedByGPDType(pGPDKinematic, pGPDModule,
            GPDComputeType::ALL);
}

ListGPDOutputData GPDService::computeListOfGPDModel(GPDKinematic* pGPDKinematic,
        std::vector<GPDModule*> &listOfGPDToCompute) {
    return computeListOfGPDModelRestrictedByGPDType(pGPDKinematic,
            listOfGPDToCompute, GPDComputeType::ALL);
}

ListGPDOutputData GPDService::computeListOfGPDModelRestrictedByGPDType(
        GPDKinematic* pGPDKinematic,
        std::vector<GPDModule*> &listOfGPDToCompute,
        GPDComputeType gpdComputeType) {
    ListGPDOutputData results = ListGPDOutputData();

    // compute inputData for each GPDModule
    for (size_t i = 0; i != listOfGPDToCompute.size(); i++) {
        results.add(listOfGPDToCompute[i]->getClassName(),
                computeGPDModelRestrictedByGPDType(pGPDKinematic,
                        listOfGPDToCompute[i], gpdComputeType));
    }

    // return a list of outputData (one outputData by GPDModule computed)
    return results;
}

//ListGPDOutputData GPDService::computeListOfKinematic(
//        std::vector<GPDKinematic> &listOfGPDKinematic, GPDModule* pGPDModule) {
//    ListGPDOutputData results = ListGPDOutputData();
//
//    // compute GPDModule for each inputData
//    for (size_t i = 0; i != listOfGPDKinematic.size(); i++) {
//        results.add(pGPDModule->getClassName(),
//                computeGPDModelRestrictedByGPDType(listOfGPDKinematic[i],
//                        pGPDModule, GPDComputeType::ALL));
//    }
//
//    return results;
//}
