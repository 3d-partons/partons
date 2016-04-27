#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
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
#include "../../../include/partons/database/gpd/service/GPDResultDaoService.h"
#include "../../../include/partons/modules/evolution/GPDEvolutionModule.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/GPDService.h"
#include "../../../include/partons/ServiceObjectTyped.h"
#include "../../../include/partons/utils/compare/ComparisonReport.h"

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
        ServiceObjectTyped(className), m_pGPDKinematic(0), m_pGPDModule(0) {
}

GPDService::~GPDService() {
}

//TODO ParameterList use isAvailable function and throw exception if missing parameter
//TODO implement all function
//TODO passer les chaine de caractere en variable final static
//TODO supprimer les pojnteurs membres de la classe GPDService
//TODO redefinition des tests de selection des cinematiques et autres modules
void GPDService::computeTask(Task &task) {

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL)) {

        //create a GPDKinematic and init it with a list of parameters
        GPDKinematic gpdKinematic;

        if (task.isAvailableParameters("GPDKinematic")) {
            gpdKinematic = GPDKinematic(task.getLastAvailableParameters());
        } else if (!m_kinematicListBuffer.isEmpty()) {
            //TODO que faire ?
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Check case or missing object <kineamtics type=\"GPDKinematic\">  for method "
                            << task.getFunctionName());
        }

        GPDModule* pGPDModule = 0;

        if (task.isAvailableParameters("GPDModule")) {
            pGPDModule = m_pModuleObjectFactory->newGPDModule(
                    task.getLastAvailableParameters().get(
                            ModuleObject::CLASS_NAME).toString());
            pGPDModule->configure(task.getLastAvailableParameters());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Check case or missing object <kineamtics type=\"GPDModule\"> for method "
                            << task.getFunctionName());
        }

        GPDResult result = computeGPDModel(gpdKinematic, pGPDModule);

        info(__func__,
                ElemUtils::Formatter() << task.getFunctionName() << "("
                        << pGPDModule->getClassName() << ")" << '\n'
                        << result.toString());

        if (task.isStoreInDB()) {
            GPDResultDaoService resultService;
            int computationId = resultService.insert(result);

            if (computationId != -1) {
                info(__func__,
                        ElemUtils::Formatter()
                                << "GPDResult object has been stored in database with computation_id = "
                                << computationId);
            } else {
                error(__func__,
                        ElemUtils::Formatter()
                                << "GPDResult object : insertion into database failed");
            }
        }

    } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION)) {

        //create a GPDKinematic and init it with a list of parameters
        GPDKinematic gpdKinematic;

        if (task.isAvailableParameters("GPDKinematic")) {
            gpdKinematic = GPDKinematic(task.getLastAvailableParameters());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Check case or missing object <kineamtics type=\"GPDKinematic\">  for method "
                            << task.getFunctionName());
        }

        GPDModule* pGPDModule = 0;

        if (task.isAvailableParameters("GPDModule")) {
            pGPDModule = m_pModuleObjectFactory->newGPDModule(
                    task.getLastAvailableParameters().get(
                            ModuleObject::CLASS_NAME).toString());
            pGPDModule->configure(task.getLastAvailableParameters());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        GPDEvolutionModule* pGPDEvolutionModule = 0;

        if (task.isAvailableParameters("GPDEvolutionModule")) {
            pGPDEvolutionModule = m_pModuleObjectFactory->newGPDEvolutionModule(
                    task.getLastAvailableParameters().get(
                            ModuleObject::CLASS_NAME).toString());
            pGPDEvolutionModule->configure(task.getLastAvailableParameters());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Missing object : <GPDEvolutionModule> for method "
                            << task.getFunctionName());
        }

        add(
                computeGPDModelWithEvolution(gpdKinematic, pGPDModule,
                        pGPDEvolutionModule));

    } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL)) {
        //create a GPDKinematic and init it with a list of parameters
        GPDKinematic gpdKinematic;

        if (task.isAvailableParameters("GPDKinematic")) {
            gpdKinematic = GPDKinematic(task.getLastAvailableParameters());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Check case or missing object <kineamtics type=\"GPDKinematic\">  for method "
                            << task.getFunctionName());
        }

        std::vector<GPDModule*> listOfGPDModule;

        if (task.isAvailableParameters("GPDModule")) {
            std::vector<ElemUtils::Parameters> listOfParameterList =
                    task.getListOfLastAvailableParameters("GPDModule");

            for (unsigned int i = 0; i != listOfParameterList.size(); i++) {
                listOfGPDModule.push_back(
                        m_pModuleObjectFactory->newGPDModule(
                                listOfParameterList[i].get(
                                        ModuleObject::CLASS_NAME).toString()));
                listOfGPDModule[i]->configure(listOfParameterList[i]);
            }

        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Missing object : <GPDModule> for method "
                            << task.getFunctionName());
        }

        add(computeListOfGPDModel(gpdKinematic, listOfGPDModule));
    } else {
        error(__func__, "unknown function name = " + task.getFunctionName());
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

    List<ElemUtils::Packet> listOfPacket;
    GPDType gpdType(GPDType::ALL);

    for (unsigned int i = 0; i != gpdKinematicList.size(); i++) {
        ElemUtils::Packet packet;
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

    results = getResultList();
    clearResultListBuffer();

    return results;
}

ComparisonReport GPDService::compareResultListToDatabase(
        const std::string& scenarioTestFilePath) {
    ComparisonReport comparisonReport;

    Partons::getInstance()->getServiceObjectRegistry();

    return comparisonReport;
}

GPDModule* GPDService::newGPDModuleFromTask(const Task& task) const {

}
