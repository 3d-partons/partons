#include <string>
#include <vector>

#include "../beans/automation/Task.h"
#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/gpd/GPDResultList.h"
#include "../beans/gpd/GPDType.h"
#include "../BaseObjectRegistry.h"
#include "../modules/evolution/GPDEvolutionModule.h"
#include "../modules/GPDModule.h"
#include "../ModuleObjectFactory.h"
#include "../utils/GenericType.h"
#include "../utils/ParameterList.h"
#include "../utils/stringUtils/Formatter.h"
#include "../utils/stringUtils/StringUtils.h"
#include "GPDService.h"

const std::string GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL = "computeGPDModel";
const std::string GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION =
        "computeGPDModelWithEvolution";

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int GPDService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDService("GPDService"));

GPDService::GPDService(const std::string &className) :
        ServiceObject(className), m_pGPDKinematic(0), m_pGPDModule(0) {
}

GPDService::~GPDService() {
}

//TODO ParameterList use isAvailable function and throw exception if missing parameter
//TODO implement all function
//TODO passer les chaine de caractere en variable final static
//TODO supprimer les pojnteurs membres de la classe GPDService
void GPDService::computeTask(Task &task) {

    if (StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL)) {

        //create a GPDKinematic and init it with a list of parameters
        GPDKinematic gpdKinematic;

        if (task.isAvailableParameterList("GPDKinematic")) {
            gpdKinematic = GPDKinematic(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDKinematic> for method "
                            << task.getFunctionName());
        }

        GPDModule* pGPDModule = 0;

        if (task.isAvailableParameterList("GPDModule")) {
            pGPDModule = ModuleObjectFactory::newGPDModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDModule->configure(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        GPDResult gpdResult = computeGPDModel(gpdKinematic, pGPDModule);

        info(__func__, Formatter() << gpdResult.toString());

    } else if (StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION)) {

        //create a GPDKinematic and init it with a list of parameters
        GPDKinematic gpdKinematic;

        if (task.isAvailableParameterList("GPDKinematic")) {
            gpdKinematic = GPDKinematic(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDKinematic> for method "
                            << task.getFunctionName());
        }

        GPDModule* pGPDModule = 0;

        if (task.isAvailableParameterList("GPDModule")) {
            pGPDModule = ModuleObjectFactory::newGPDModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDModule->configure(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        GPDEvolutionModule* pGPDEvolutionModule = 0;

        if (task.isAvailableParameterList("GPDEvolutionModule")) {
            pGPDEvolutionModule = ModuleObjectFactory::newGPDEvolutionModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDEvolutionModule->configure(
                    task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter()
                            << "Missing object : <GPDEvolutionModule> for method "
                            << task.getFunctionName());
        }

        GPDResult gpdResult = computeGPDModelWithEvolution(gpdKinematic,
                pGPDModule, pGPDEvolutionModule);

        info(__func__, Formatter() << gpdResult.toString());

    } else {
        throwException(__func__,
                "unknown function name = " + task.getFunctionName());
    }
}

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
        GPDEvolutionModule* pEvolQCDModule, GPDType::Type gpdType) {
    pGPDModule->setEvolQcdModule(pEvolQCDModule);

    GPDResult gpdResult = pGPDModule->compute(gpdKinematic.getX(),
            gpdKinematic.getXi(), gpdKinematic.getT(), gpdKinematic.getMuF2(),
            gpdKinematic.getMuR2(), gpdType);

    gpdResult.setKinematic(gpdKinematic);
    gpdResult.setComputedByGpdModuleId(pGPDModule->getClassName());

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
