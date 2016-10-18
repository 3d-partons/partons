#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <string>
#include <vector>

#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../include/partons/beans/gpd/GPDResult.h"
#include "../../../include/partons/beans/gpd/GPDType.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/beans/List.h"
#include "../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/gpd/service/GPDResultDaoService.h"
#include "../../../include/partons/modules/evolution/GPDEvolutionModule.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/services/GPDService.h"
#include "../../../include/partons/ServiceObjectTyped.h"
#include "../../../include/partons/utils/exceptions/GPDModuleNullPointerException.h"
#include "../../../include/partons/utils/VectorUtils.h"

const std::string GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL = "computeGPDModel";
const std::string GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION =
        "computeGPDModelWithEvolution";
const std::string GPDService::GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL =
        "computeListOfGPDModel";
const std::string GPDService::GPD_SERVICE_COMPUTE_MANY_KINEMATIC_ONE_MODEL =
        "computeManyKinematicOneModel";
const std::string GPDService::FUNCTION_NAME_GENERATE_PLOT_FILE =
        "generatePlotFile";

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int GPDService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new GPDService("GPDService"));

GPDService::GPDService(const std::string &className) :
        ServiceObjectTyped<GPDKinematic, GPDResult>(className), m_pGPDKinematic(
                0), m_pGPDModule(0) {
}

GPDService::~GPDService() {
}

void GPDService::resolveObjectDependencies() {
    ServiceObject::resolveObjectDependencies();

    try {
        m_batchSize = ElemUtils::GenericType(
                ElemUtils::PropertiesManager::getInstance()->getString(
                        "gpd.service.batch.size")).toUInt();
    } catch (const std::exception &e) {
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }
}

//TODO ParameterList use isAvailable function and throw exception if missing parameter
//TODO implement all function
//TODO passer les chaine de caractere en variable final static
//TODO supprimer les pojnteurs membres de la classe GPDService
//TODO redefinition des tests de selection des cinematiques et autres modules
void GPDService::computeTask(Task &task) {
    debug(__func__, "Processing ...");

    ServiceObjectTyped<GPDKinematic, GPDResult>::computeTask(task);

    List<GPDResult> resultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
        resultList.add(computeManyKinematicOneModelTask(task));
    } else {

        //TODO refactoring scenario handling
//    resultList.getComputation().setScenario(task.getScenario());

        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL)) {
            resultList.add(computeGPDTask(task));
        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                GPDService::GPD_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION)) {

            //create a GPDKinematic and init it with a list of parameters
            GPDKinematic gpdKinematic;

            if (task.isAvailableParameters("GPDKinematic")) {
                gpdKinematic = GPDKinematic(task.getLastAvailableParameters());
            } else {
                ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter()
                                << "Check case or missing object <kineamtics type=\"GPDKinematic\">  for method "
                                << task.getFunctionName());
            }

            GPDModule* pGPDModule = newGPDModuleFromTask(task);

            GPDEvolutionModule* pGPDEvolutionModule = 0;

            if (task.isAvailableParameters("GPDEvolutionModule")) {
                pGPDEvolutionModule =
                        m_pModuleObjectFactory->newGPDEvolutionModule(
                                task.getLastAvailableParameters().get(
                                        ModuleObject::CLASS_NAME).toString());
                pGPDEvolutionModule->configure(
                        task.getLastAvailableParameters());
            } else {
                ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter()
                                << "Missing object : <GPDEvolutionModule> for method "
                                << task.getFunctionName());
            }

            resultList.add(
                    computeGPDModelWithEvolution(gpdKinematic, pGPDModule,
                            pGPDEvolutionModule));

        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                GPDService::GPD_SERVICE_COMPUTE_LIST_OF_GPD_MODEL)) {
            //create a GPDKinematic and init it with a list of parameters
            GPDKinematic gpdKinematic;

            if (task.isAvailableParameters("GPDKinematic")) {
                gpdKinematic = GPDKinematic(task.getLastAvailableParameters());
            } else {
                ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter()
                                << "Check case or missing object <kineamtics type=\"GPDKinematic\">  for method "
                                << task.getFunctionName());
            }

            std::vector<GPDModule*> listOfGPDModule;

            //TODO est-ce que cela fonctionne ?
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
                ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter()
                                << "Missing object : <GPDModule> for method "
                                << task.getFunctionName());
            }

            resultList.add(
                    computeListOfGPDModel(gpdKinematic, listOfGPDModule));
        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                GPDService::FUNCTION_NAME_GENERATE_PLOT_FILE)) {
            generatePlotFileTask(task);
        } else if (!ServiceObjectTyped<GPDKinematic, GPDResult>::computeGeneralTask(
                task)) {
            errorUnknownMethod(task);
        }

        updateResultInfo(resultList, m_resultInfo);

        if (task.isStoreInDB()) {
//        GPDResultDaoService resultService;
//        int computationId = resultService.insert(resultList);
//
//        if (computationId != -1) {
//            info(__func__,
//                    ElemUtils::Formatter()
//                            << "List of GPD result has been stored in database with computation_id = "
//                            << computationId);
//        } else {
//            error(__func__,
//                    ElemUtils::Formatter()
//                            << "Failed to insert List of GPD result into database");
//        }

//TODO refactoring
            GPDResultDaoService gpdResultDaoService;
            gpdResultDaoService.insert(resultList);
        }

    }

    m_resultListBuffer = resultList;
}

GPDResult GPDService::computeGPDModel(const GPDKinematic &gpdKinematic,
        GPDModule* pGPDModule, const List<GPDType> & gpdTypeList) const {
    GPDResult gpdResult;

    List<GPDType> restrictedByGPDTypeListFinal = getFinalGPDTypeList(pGPDModule,
            gpdTypeList);

    for (unsigned int i = 0; i != restrictedByGPDTypeListFinal.size(); i++) {
        gpdResult.addPartonDistribution(restrictedByGPDTypeListFinal[i],
                pGPDModule->compute(gpdKinematic,
                        restrictedByGPDTypeListFinal[i]));
    }

    gpdResult.setKinematic(gpdKinematic);
    gpdResult.setComputationModuleName(pGPDModule->getClassName());

    return gpdResult;
}

GPDResult GPDService::computeGPDModelWithEvolution(
        const GPDKinematic &gpdKinematic, GPDModule* pGPDModule,
        GPDEvolutionModule* pEvolQCDModule, GPDType::Type gpdType) {
    pGPDModule->setEvolQcdModule(pEvolQCDModule);

    GPDResult gpdResult;
    gpdResult.addPartonDistribution(gpdType,
            pGPDModule->compute(gpdKinematic.getX(), gpdKinematic.getXi(),
                    gpdKinematic.getT(), gpdKinematic.getMuF2(),
                    gpdKinematic.getMuR2(), gpdType, true));

    return gpdResult;
}

List<GPDResult> GPDService::computeListOfGPDModel(
        const GPDKinematic &gpdKinematic,
        std::vector<GPDModule*> &listOfGPDToCompute) {
    return computeListOfGPDModelRestrictedByGPDType(gpdKinematic,
            listOfGPDToCompute, GPDType::ALL);
}

List<GPDResult> GPDService::computeListOfGPDModelRestrictedByGPDType(
        const GPDKinematic &gpdKinematic,
        std::vector<GPDModule*> &listOfGPDToCompute, GPDType gpdType) {
    List<GPDResult> results;

    //TODO enable
//    for (size_t i = 0; i != listOfGPDToCompute.size(); i++) {
//        results.add(
//                computeGPDModel(gpdKinematic, listOfGPDToCompute[i], gpdType));
//    }

// return a list of outputData (one outputData by GPDModule computed)
    return results;
}

List<GPDResult> GPDService::computeManyKinematicOneModel(
        const List<GPDKinematic> &gpdKinematicList, GPDModule* pGPDModule,
        const List<GPDType> &gpdTypeList, const bool storeInDB) {
    debug(__func__, "Processing ...");

    info(__func__,
            ElemUtils::Formatter() << gpdKinematicList.size()
                    << " GPD kinematic(s) will be computed");

    List<GPDResult> results;
    List<ElemUtils::Packet> listOfPacket;

    List<GPDType> finalGPDTypeList = getFinalGPDTypeList(pGPDModule,
            gpdTypeList);

    if (finalGPDTypeList.size() != 0) {

        initComputationalThread(pGPDModule);

        info(__func__, "Thread(s) running ...");

        // ##### Batch feature start section #####
        unsigned int i = 0;
        unsigned int j = 0;

        while (i != gpdKinematicList.size()) {
            listOfPacket.clear();
            j = 0;

            while ((j != m_batchSize) && (i != gpdKinematicList.size())) {
                ElemUtils::Packet packet;
                GPDKinematic kineamtic;
                kineamtic = gpdKinematicList[i];
                packet << kineamtic << finalGPDTypeList;
                listOfPacket.add(packet);
                i++;
                j++;
            }

            addTasks(listOfPacket);

            launchAllThreadAndWaitingFor();
            sortResultList();

            info(__func__,
                    ElemUtils::Formatter() << "Kinematic(s) already computed : "
                            << i);

            updateResultInfo(getResultList(), m_resultInfo);

            if (storeInDB) {
                GPDResultDaoService gpdResultDaoService;
                gpdResultDaoService.insert(getResultList());
            } else {
                results.add(getResultList());
            }

            clearResultListBuffer();
        }
        // ##### Batch feature end section #####

        clearAllThread();

    } else {
        info(__func__,
                "Nothing to compute with your computation configuration ; there is no GPDType available");
    }

    return results;
}

//ComparisonReport GPDService::compareResultListToDatabase(
//        const std::string& scenarioTestFilePath) {
//    ComparisonReport comparisonReport;
//
//    Partons::getInstance()->getServiceObjectRegistry();
//
//    return comparisonReport;
//}

GPDModule* GPDService::newGPDModuleFromTask(const Task& task) const {
    GPDModule* pGPDModule = 0;

    if (task.isAvailableParameters("GPDModule")) {
        pGPDModule =
                m_pModuleObjectFactory->newGPDModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pGPDModule->configure(task.getLastAvailableParameters());
    }

//TODO how to deal with evolution module ?

//TODO how to handle many GPD module ?

    return configureGPDModule(pGPDModule);
}

GPDModule* GPDService::configureGPDModule(GPDModule* pGPDModule) const {

    if (pGPDModule == 0) {
        throw GPDModuleNullPointerException(
                "You have not provided any GPDModule");
        // throwException(__func__, "You have not provided any GPDModule");
    }

    return pGPDModule;
}

GPDResult GPDService::computeGPDTask(Task& task) {
//create a GPDKinematic and init it with a list of parameters
    GPDKinematic gpdKinematic;

    if (task.isAvailableParameters("GPDKinematic")) {
        gpdKinematic = GPDKinematic(task.getLastAvailableParameters());
    } else if (!m_kinematicListBuffer.isEmpty()) {
        //TODO que faire ?
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Check case or missing object <kineamtics type=\"GPDKinematic\">  for method "
                        << task.getFunctionName());
    }

    GPDModule* pGPDModule = newGPDModuleFromTask(task);

    return computeGPDModel(gpdKinematic, pGPDModule);
}

List<GPDResult> GPDService::computeManyKinematicOneModelTask(Task& task) {
    List<GPDKinematic> listOfKinematic;

    if (task.isAvailableParameters("GPDKinematic")) {
        ElemUtils::Parameters parameters = task.getLastAvailableParameters();
        if (parameters.isAvailable("file")) {
            listOfKinematic = KinematicUtils().getGPDKinematicFromFile(
                    parameters.getLastAvailable().toString());
        } else {
            ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <GPDKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDKinematic> for method "
                        << task.getFunctionName());
    }

    List<GPDType> gpdTypeList = getGPDTypeListFromTask(task);

    GPDModule* pGPDModule = newGPDModuleFromTask(task);

    List<GPDResult> result = computeManyKinematicOneModel(listOfKinematic,
            pGPDModule, gpdTypeList, task.isStoreInDB());

//       info(__func__,
//               ElemUtils::Formatter() << task.getFunctionName() << "("
//                       << pGPDModule->getClassName() << ")" << '\n'
//                       << result.toString());

    return result;
}

void GPDService::generatePlotFileTask(Task& task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task, "gpd_plot_2d_view"), ' ');
}

List<GPDType> GPDService::getFinalGPDTypeList(GPDModule* pGPDModule,
        const List<GPDType> &gpdTypeList) const {

    List<GPDType> restrictedByGPDTypeListFinal = gpdTypeList;

    restrictedByGPDTypeListFinal =
            pGPDModule->getListOfAvailableGPDTypeForComputation();

    if (!gpdTypeList.isEmpty()) {
        restrictedByGPDTypeListFinal = VectorUtils::intersection(
                restrictedByGPDTypeListFinal, gpdTypeList);
    }

    info(__func__,
            ElemUtils::Formatter() << restrictedByGPDTypeListFinal.size()
                    << " GPDType will be computed");

    return restrictedByGPDTypeListFinal;
}
