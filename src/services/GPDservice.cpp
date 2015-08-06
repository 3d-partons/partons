#include <iostream>
#include <string>
#include <vector>

#include "../beans/automation/Task.h"
#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/gpd/GPDResultList.h"
#include "../beans/gpd/GPDType.h"
#include "../BaseObjectRegistry.h"
#include "../modules/gpd/GK11Model.h"
#include "../ModuleObjectFactory.h"
#include "../utils/stringUtils/StringUtils.h"
#include "GPDService.h"

const std::string GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL = "computeGPDModel";

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int GPDService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDService("GPDService"));

GPDService::GPDService(const std::string &className) :
        ServiceObject(className), m_pGPDKinematic(0), m_pGPDModule(0) {
}

GPDService::~GPDService() {
}

//TODO implement all function
//TODO passer les chaine de caractere en variable final static
//TODO supprimer les pojnteurs membres de la classe GPDService
void GPDService::computeTask(const Task &task) {

    if (StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL)) {
        GPDKinematic gpdKinematic = GPDKinematic(
                task.getParameterList("GPDKinematic"));

        //TODO le construire avec les arguments provenant de ParameterList
        GPDModule* pGPDModule = ModuleObjectFactory::newGPDModule(
                GK11Model::classId);

        GPDResult gpdResult = computeGPDModel(gpdKinematic, pGPDModule);

        std::cout << gpdResult.toString() << std::endl;
    } else {
        throwException(__func__,
                "unknown function name = " + task.getFunctionName());
    }

//    m_pGPDKinematic = 0;
//    m_pGPDModule = 0;
//
//    // TODO supprimer les enums
//    switch ((unsigned int) task.getFunctionName()) {
//    case ServiceFunctionNames::GPD_SERVICE_COMPUTE_GPD_MODEL: {
//
//        GPDKinematic gpdKinematic = GPDKinematic(
//                task.getParameterList("GPDKinematic"));
//
//        GPDResult gpdResult = computeGPDModel(task);
//
//        std::cout << gpdResult.toString() << std::endl;
//
//        break;
//    }
//
//    default: {
//        throwException(__func__,
//                "unknown function name = " + task.getFunctionName());
//
//    }
//    }
}

//GPDResult GPDService::computeGPDModel(const Task &task) {
//    m_pGPDKinematic = BaseObjectFactory::
//    static_cast<GPDKinematic*>((scenario.getFunctionArgs().find("GPDKinematic"))->second);
//    m_pGPDModule = static_cast<GPDModule*>((scenario.getFunctionArgs().find(
//            "GPDModule"))->second);
//
//    return computeGPDModel(*m_pGPDKinematic, m_pGPDModule);
//}

GPDResult GPDService::computeGPDModelRestrictedByGPDType(
        const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        GPDType::Type gpdType) {

    GPDResult gpdResult = pGPDModule->compute(gpdKinematic.getX(),
            gpdKinematic.getXi(), gpdKinematic.getT(), gpdKinematic.getMuF2(),
            gpdKinematic.getMuR2(), gpdType);

    gpdResult.setKinematic(gpdKinematic);
    gpdResult.setComputedByGpdModuleId(pGPDModule->getClassName());

    return gpdResult;
}

GPDResult GPDService::computeGPDModelWithEvolution(
        const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        EvolQCDModule* pEvolQCDModule, GPDType::Type gpdType) {
    pGPDModule->setEvolQcdModule(pEvolQCDModule);

    GPDResult gpdResult = pGPDModule->compute(gpdKinematic.getX(),
            gpdKinematic.getXi(), gpdKinematic.getT(), gpdKinematic.getMuF2(),
            gpdKinematic.getMuR2(), gpdType);

    gpdResult.setKinematic(gpdKinematic);

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
