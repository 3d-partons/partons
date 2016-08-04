#include "../../../include/partons/services/ConvolCoeffFunctionService.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/beans/automation/Scenario.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionResultDaoService.h"
#include "../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/ResourceManager.h"
#include "../../../include/partons/services/GPDService.h"
#include "../../../include/partons/ServiceObjectRegistry.h"
#include "../../../include/partons/utils/exceptions/CCFModuleNullPointerException.h"
#include "../../../include/partons/utils/exceptions/GPDModuleNullPointerException.h"

const std::string ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL =
        "computeWithGPDModel";
const std::string ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL =
        "computeListWithGPDModel";
const std::string ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL =
        "computeManyKinematicOneModel";
const std::string ConvolCoeffFunctionService::FUNCTION_NAME_GENERATE_PLOT_FILE =
        "generatePlotFile";

// Initialise [class]::classId with a unique name.
const unsigned int ConvolCoeffFunctionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new ConvolCoeffFunctionService("ConvolCoeffFunctionService"));

ConvolCoeffFunctionService::ConvolCoeffFunctionService(
        const std::string &className) :
        ServiceObjectTyped<DVCSConvolCoeffFunctionKinematic,
                DVCSConvolCoeffFunctionResult>(className), m_pGPDService(0) {
}

ConvolCoeffFunctionService::~ConvolCoeffFunctionService() {
}

void ConvolCoeffFunctionService::resolveObjectDependencies() {
    ServiceObject::resolveObjectDependencies();

    try {
        m_batchSize = ElemUtils::GenericType(
                ElemUtils::PropertiesManager::getInstance()->getString(
                        "ccf.service.batch.size")).toUInt();
    } catch (const std::exception &e) {
        error(__func__, ElemUtils::Formatter() << e.what());
    }
}

//TODO implement
void ConvolCoeffFunctionService::computeTask(Task &task) {
    List<DVCSConvolCoeffFunctionResult> resultList;

    //TODO Je pense qu'il est possible de supprimer l'étape registerScenario() car par construction il doit toujours exister dans le ResourceManager;
    m_resultInfo = ResultInfo();
    m_resultInfo.setScenarioTaskIndexNumber(task.getScenarioTaskIndexNumber());
    Scenario * tempSenario = ResourceManager::getInstance()->registerScenario(
            task.getScenario());

    if (tempSenario) {
        m_resultInfo.setScenarioHashSum(tempSenario->getHashSum());
    }

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
        resultList = computeManyKinematicOneModelTask(task);
    } else {

        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL)) {
            resultList.add(computeWithGPDModelTask(task));

        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL)) {
            //TODO implement
            // resultList = computeListWithGPDModelTask(task);
        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::FUNCTION_NAME_GENERATE_PLOT_FILE)) {
            generatePlotFileTask(task);
        } else if (!ServiceObjectTyped<DVCSConvolCoeffFunctionKinematic,
                DVCSConvolCoeffFunctionResult>::computeGeneralTask(task)) {
            error(__func__,
                    "unknown function name = " + task.getFunctionName());
        }

        updateResultInfo(resultList, m_resultInfo);

        if (task.isStoreInDB()) {
            ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;
            int computationId = convolCoeffFunctionResultDaoService.insert(
                    resultList);

            if (computationId != -1) {
                info(__func__,
                        ElemUtils::Formatter()
                                << "DVCSConvolCoeffFunctionResultList object has been stored in database with computation_id = "
                                << computationId);
            } else {
                error(__func__,
                        ElemUtils::Formatter()
                                << "DVCSConvolCoeffFunctionResultList object : insertion into database failed");
            }
        }
    }
}

//TODO implementer
DVCSConvolCoeffFunctionResult ConvolCoeffFunctionService::computeWithGPDModel(
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        ConvolCoeffFunctionModule* convolCoeffFunctionModule,
        GPDType::Type gpdType) const {

    DVCSConvolCoeffFunctionResult result = convolCoeffFunctionModule->compute(
            kinematic, gpdType);

    return result;
}

//
//ResultList<DVCSConvolCoeffFunctionResult> DVCSConvolCoeffFunctionService::computeListWithGPDModel(
//        const DVCSConvolCoeffFunctionKinematic& kinematic,
//        std::vector<DVCSConvolCoeffFunctionModule*> listOfDVCSConvolCoeffFunctionModule,
//        GPDType::Type gpdType) const {
//
//    ResultList<DVCSConvolCoeffFunctionResult> results;
//
//    for (unsigned int i = 0; i != listOfDVCSConvolCoeffFunctionModule.size();
//            i++) {
//        results.add(
//                computeWithGPDModel(kinematic,
//                        listOfDVCSConvolCoeffFunctionModule[i], gpdType));
//    }
//
//    return results;
//}

DVCSConvolCoeffFunctionResult ConvolCoeffFunctionService::computeWithGPDModelTask(
        Task& task) const {

    //create a kinematic and init it with a list of parameters
    DVCSConvolCoeffFunctionKinematic kinematic;

    if (task.isAvailableParameters("DVCSConvolCoeffFunctionKinematic")) {
        kinematic = DVCSConvolCoeffFunctionKinematic(
                task.getLastAvailableParameters());
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
            newConvolCoeffFunctionModuleFromTask(task);

    DVCSConvolCoeffFunctionResult result = computeWithGPDModel(kinematic,
            pConvolCoeffFunctionModule);

    return result;
}

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionService::computeManyKinematicOneModelTask(
        Task& task) {
    List<DVCSConvolCoeffFunctionKinematic> listOfKinematic;

    if (task.isAvailableParameters("DVCSConvolCoeffFunctionKinematic")) {
        ElemUtils::Parameters parameters = task.getLastAvailableParameters();
        if (parameters.isAvailable("file")) {
            listOfKinematic = KinematicUtils::getCCFKinematicFromFile(
                    parameters.getLastAvailable().toString());
        } else {
            error(__func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <DVCSConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        error(__func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDKinematic> for method "
                        << task.getFunctionName());
    }

    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
            newConvolCoeffFunctionModuleFromTask(task);

    return computeManyKinematicOneModel(listOfKinematic,
            pConvolCoeffFunctionModule, task.isStoreInDB());
}

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionService::computeManyKinematicOneModel(
        List<DVCSConvolCoeffFunctionKinematic> &kinematics,
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
        const bool storeInDB) {

    info(__func__,
            ElemUtils::Formatter() << kinematics.size() << " will be computed");

    List<DVCSConvolCoeffFunctionResult> results;

    List<ElemUtils::Packet> listOfPacket;
    GPDType gpdType(GPDType::ALL);

    initComputationalThread(pConvolCoeffFunctionModule);

    // ##### Batch feature start section #####
    unsigned int i = 0;
    unsigned int j = 0;

    while (i != kinematics.size()) {
        listOfPacket.clear();
        j = 0;

        while ((j != m_batchSize) && (i != kinematics.size())) {
            ElemUtils::Packet packet;
            DVCSConvolCoeffFunctionKinematic kinematic;
            kinematic = kinematics[i];
            packet << kinematic << gpdType;
            listOfPacket.add(packet);
            i++;
            j++;
        }

        addTasks(listOfPacket);
        launchAllThreadAndWaitingFor();
        sortResultList();

        updateResultInfo(getResultList(), m_resultInfo);

        if (storeInDB) {
            ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;
            convolCoeffFunctionResultDaoService.insert(getResultList());
        } else {
            results.add(getResultList());
        }

        clearResultListBuffer();
    }
    // ##### Batch feature end section #####

    clearAllThread();

    return results;
}

ConvolCoeffFunctionModule* ConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
        const Task& task) const {

    DVCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;

    if (task.isAvailableParameters("DVCSConvolCoeffFunctionModule")) {
        pConvolCoeffFunctionModule =
                m_pModuleObjectFactory->newDVCSConvolCoeffFunctionModule(
                        task.getLastAvailableParameters().get(
                                ModuleObject::CLASS_NAME).toString());
        pConvolCoeffFunctionModule->configure(
                task.getLastAvailableParameters());
    }

    GPDModule* pGPDModule = 0;

    try {
        pGPDModule =
                Partons::getInstance()->getServiceObjectRegistry()->getGPDService()->newGPDModuleFromTask(
                        task);
    } catch (const GPDModuleNullPointerException &e) {
        // Nothing to do.
        // An exception is raised if <GPDModule> element cannot be found in the task parameterList, but in this case a DVCSConvolCoeffFunctionModule can be GPDModule independent.
        // So just catch the exception and continue to run the program.
    }

    return configureConvolCoeffFunctionModule(pConvolCoeffFunctionModule,
            pGPDModule);
}

ConvolCoeffFunctionModule* ConvolCoeffFunctionService::configureConvolCoeffFunctionModule(
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
        GPDModule* pGPDModule) const {

    if (pConvolCoeffFunctionModule == 0) {
        throw CCFModuleNullPointerException(
                "You have not provided any ConvolCoeffFunctionModule");
    }

    if (pConvolCoeffFunctionModule->isGPDModuleDependent()) {
        if (pGPDModule == 0) {
            error(__func__,
                    "This ConvolCoeffFunctionModule is GPDModule dependent but you have not provided any GPDModule");
        }

        // set gpd module to dvcs convol coeff function module
        pConvolCoeffFunctionModule->setGPDModule(pGPDModule);
    }

    return pConvolCoeffFunctionModule;
}

void ConvolCoeffFunctionService::generatePlotFileTask(Task& task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task, "ccf_plot_2d_view"), ' ');
}
