#include <stddef.h>
#include <string>
#include <vector>

#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../include/partons/beans/gpd/GPDType.h"
#include "../../../include/partons/beans/List.h"
#include "../../../include/partons/beans/ResultList.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/modules/evolution/GPDEvolutionModule.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/GPDService.h"
#include "../../../include/partons/utils/GenericType.h"
#include "../../../include/partons/utils/ParameterList.h"
#include "../../../include/partons/utils/stringUtils/Formatter.h"
#include "../../../include/partons/utils/stringUtils/StringUtils.h"
#include "../../../include/partons/utils/thread/Packet.h"

const std::string GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL = "computeGPDModel";
const std::string GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION =
        "computeGPDModelWithEvolution";
const std::string GPDService::GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL =
        "computeListOfGPDModel";

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int GPDService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
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
            pGPDModule = m_pModuleObjectFactory->newGPDModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDModule->configure(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        GPDResult result = computeGPDModel(gpdKinematic, pGPDModule);

        info(__func__,
                Formatter() << task.getFunctionName() << "("
                        << pGPDModule->getClassName() << ")" << '\n'
                        << result.toString());

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
            pGPDModule = m_pModuleObjectFactory->newGPDModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDModule->configure(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        GPDEvolutionModule* pGPDEvolutionModule = 0;

        if (task.isAvailableParameterList("GPDEvolutionModule")) {
            pGPDEvolutionModule = m_pModuleObjectFactory->newGPDEvolutionModule(
                    task.getLastAvailableParameterList().get("id").toString());
            pGPDEvolutionModule->configure(
                    task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter()
                            << "Missing object : <GPDEvolutionModule> for method "
                            << task.getFunctionName());
        }

        GPDResult result = computeGPDModelWithEvolution(gpdKinematic,
                pGPDModule, pGPDEvolutionModule);

        info(__func__,
                Formatter() << task.getFunctionName() << "("
                        << pGPDModule->getClassName() << " , "
                        << pGPDEvolutionModule->getClassName() << ")" << '\n'
                        << result.toString());

    } else if (StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL)) {
        //create a GPDKinematic and init it with a list of parameters
        GPDKinematic gpdKinematic;

        if (task.isAvailableParameterList("GPDKinematic")) {
            gpdKinematic = GPDKinematic(task.getLastAvailableParameterList());
        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDKinematic> for method "
                            << task.getFunctionName());
        }

        std::vector<GPDModule*> listOfGPDModule;

        if (task.isAvailableParameterList("GPDModule")) {
            std::vector<ParameterList> listOfParameterList =
                    task.getListOfLastAvailableParameterList("GPDModule");

            for (unsigned int i = 0; i != listOfParameterList.size(); i++) {
                listOfGPDModule.push_back(
                        m_pModuleObjectFactory->newGPDModule(
                                listOfParameterList[i].get("id").toString()));
                listOfGPDModule[i]->configure(listOfParameterList[i]);
            }

        } else {
            throwException(__func__,
                    Formatter() << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        ResultList<GPDResult> results = computeListOfGPDModel(gpdKinematic,
                listOfGPDModule);

        info(__func__,
                Formatter() << task.getFunctionName() << "(" << ")" << '\n'
                        << results.toString());
    } else {
        throwException(__func__,
                "unknown function name = " + task.getFunctionName());
    }
}

GPDResult GPDService::computeGPDModelRestrictedByGPDType(
        const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        GPDType::Type gpdType) const {

    GPDResult gpdResult = pGPDModule->compute(gpdKinematic, gpdType);

    return gpdResult;
}

GPDResult GPDService::computeGPDModelWithEvolution(
        const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        GPDEvolutionModule* pEvolQCDModule, GPDType::Type gpdType) {
    pGPDModule->setEvolQcdModule(pEvolQCDModule);

    GPDResult gpdResult = pGPDModule->compute(gpdKinematic.getX(),
            gpdKinematic.getXi(), gpdKinematic.getT(), gpdKinematic.getMuF2(),
            gpdKinematic.getMuR2(), gpdType, true);

    return gpdResult;
}

GPDResult GPDService::computeGPDModel(const GPDKinematic &gpdKinematic,
        GPDModule* pGPDModule) {
    return computeGPDModelRestrictedByGPDType(gpdKinematic, pGPDModule,
            GPDType::ALL);
}

ResultList<GPDResult> GPDService::computeListOfGPDModel(
        const GPDKinematic &gpdKinematic,
        std::vector<GPDModule*> &listOfGPDToCompute) {
    return computeListOfGPDModelRestrictedByGPDType(gpdKinematic,
            listOfGPDToCompute, GPDType::ALL);
}

ResultList<GPDResult> GPDService::computeListOfGPDModelRestrictedByGPDType(
        const GPDKinematic &gpdKinematic,
        std::vector<GPDModule*> &listOfGPDToCompute, GPDType gpdType) {
    ResultList<GPDResult> results;

    for (size_t i = 0; i != listOfGPDToCompute.size(); i++) {
        results.add(
                computeGPDModelRestrictedByGPDType(gpdKinematic,
                        listOfGPDToCompute[i], gpdType));
    }

    // return a list of outputData (one outputData by GPDModule computed)
    return results;
}

ResultList<GPDResult> GPDService::computeManyKinematicOneModel(
        const List<GPDKinematic> &gpdKinematicList, GPDModule* pGPDModule) {
    ResultList<GPDResult> results;

    List<Packet> listOfPacket;
    GPDType gpdType(GPDType::ALL);

    for (unsigned int i = 0; i != gpdKinematicList.size(); i++) {
        Packet packet;
        GPDKinematic obsK;
        obsK = gpdKinematicList[i];
        packet << obsK << gpdType;
        listOfPacket.add(packet);
    }

    addTasks(listOfPacket);

    initComputationalThread(pGPDModule);
    launchAllThreadAndWaitingFor();
//
//// compute GPDModule for each inputData
//    for (unsigned int i = 0; i != gpdKinematicList.size(); i++) {
//        results.add(
//                computeGPDModelRestrictedByGPDType(gpdKinematicList[i],
//                        pGPDModule, GPDType::ALL));
//    }

    return results;
}
