#include "../../../include/partons/services/ConvolCoeffFunctionService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <complex>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/beans/system/ResultInfo.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/convol_coeff_function/service/ConvolCoeffFunctionResultDaoService.h"
#include "../../../include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../../../include/partons/modules/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/utils/VectorUtils.h"

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
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }
}

//TODO implement
void ConvolCoeffFunctionService::computeTask(Task &task) {
    ServiceObjectTyped<DVCSConvolCoeffFunctionKinematic,
            DVCSConvolCoeffFunctionResult>::computeTask(task);

    List<DVCSConvolCoeffFunctionResult> resultList;

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
            errorUnknownMethod(task);
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
                throw ElemUtils::CustomException(getClassName(), __func__,
                        ElemUtils::Formatter()
                                << "DVCSConvolCoeffFunctionResultList object : insertion into database failed");
            }
        }
    }
}

DVCSConvolCoeffFunctionResult ConvolCoeffFunctionService::computeForOneCCFModel(
        const DVCSConvolCoeffFunctionKinematic &kinematic,
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
        const List<GPDType> & gpdTypeList) const {

    List<GPDType> restrictedByGPDTypeListFinal = getFinalGPDTypeList(
            pConvolCoeffFunctionModule, gpdTypeList);

    DVCSConvolCoeffFunctionResult result;

    for (unsigned int i = 0; i != restrictedByGPDTypeListFinal.size(); i++) {
        result.add(restrictedByGPDTypeListFinal[i],
                pConvolCoeffFunctionModule->compute(kinematic,
                        restrictedByGPDTypeListFinal[i]));
    }

    result.setKinematic(kinematic);
    result.setComputationModuleName(pConvolCoeffFunctionModule->getClassName());

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
    DVCSConvolCoeffFunctionKinematic kinematic = newKinematicFromTask(task);

//    if (task.isAvailableParameters("DVCSConvolCoeffFunctionKinematic")) {
//        kinematic = DVCSConvolCoeffFunctionKinematic(
//                task.getLastAvailableParameters());
//    } else {
//       throw ElemUtils::CustomException(getClassName(), __func__,
//                ElemUtils::Formatter()
//                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
//                        << task.getFunctionName());
//    }

//    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
//            newConvolCoeffFunctionModuleFromTask(task);

    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
            newConvolCoeffFunctionModuleFromTask(task);

    DVCSConvolCoeffFunctionResult result = computeForOneCCFModel(kinematic,
            pConvolCoeffFunctionModule);

    if (pConvolCoeffFunctionModule) {
        delete pConvolCoeffFunctionModule;
        pConvolCoeffFunctionModule = 0;
    }

    return result;
}

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionService::computeManyKinematicOneModelTask(
        Task& task) {
    List<DVCSConvolCoeffFunctionKinematic> listOfKinematic =
            newListOfKinematicFromTask(task);

    List<GPDType> gpdTypeList = getGPDTypeListFromTask(task);

    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
            newConvolCoeffFunctionModuleFromTask(task);

    return computeForOneCCFModelAndManyKinematics(listOfKinematic,
            pConvolCoeffFunctionModule, gpdTypeList, task.isStoreInDB());
}

List<DVCSConvolCoeffFunctionResult> ConvolCoeffFunctionService::computeForOneCCFModelAndManyKinematics(
        List<DVCSConvolCoeffFunctionKinematic> &kinematics,
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
        const List<GPDType> &gpdTypeList, const bool storeInDB) {
    debug(__func__, "Processing ...");

    info(__func__,
            ElemUtils::Formatter() << kinematics.size()
                    << " CCF kinematic(s) will be computed");

    List<DVCSConvolCoeffFunctionResult> results;

    List<ElemUtils::Packet> listOfPacket;
    List<GPDType> finalGPDTypeList = getFinalGPDTypeList(
            pConvolCoeffFunctionModule, gpdTypeList);

    if (finalGPDTypeList.size() != 0) {

        initComputationalThread(pConvolCoeffFunctionModule);

        info(__func__, "Thread(s) running ...");

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
                packet << kinematic << finalGPDTypeList;
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
                ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;

                info(__func__,
                        ElemUtils::Formatter()
                                << "Results have been stored with computation_id = "
                                << convolCoeffFunctionResultDaoService.insert(
                                        getResultList()));
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

//ConvolCoeffFunctionModule* ConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
//        const Task& task) const {
//
//    DVCSConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;
//
//    if (task.isAvailableParameters("DVCSConvolCoeffFunctionModule")) {
//        pConvolCoeffFunctionModule =
//                m_pModuleObjectFactory->newDVCSConvolCoeffFunctionModule(
//                        task.getLastAvailableParameters().get(
//                                ModuleObject::CLASS_NAME).getString());
//        pConvolCoeffFunctionModule->configure(
//                task.getLastAvailableParameters());
//    }
//
//    GPDModule* pGPDModule = 0;
//
//    try {
//        pGPDModule =
//                Partons::getInstance()->getServiceObjectRegistry()->getGPDService()->newGPDModuleFromTask(
//                        task);
//    } catch (const GPDModuleNullPointerException &e) {
//        // Nothing to do.
//        // An exception is raised if <GPDModule> element cannot be found in the task parameterList, but in this case a DVCSConvolCoeffFunctionModule can be GPDModule independent.
//        // So just catch the exception and continue to run the program.
//    }
//
//    return configureConvolCoeffFunctionModule(pConvolCoeffFunctionModule,
//            pGPDModule);
//}
//
//ConvolCoeffFunctionModule* ConvolCoeffFunctionService::configureConvolCoeffFunctionModule(
//        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
//        GPDModule* pGPDModule) const {
//
//    if (pConvolCoeffFunctionModule == 0) {
//        throw CCFModuleNullPointerException(
//                "You have not provided any ConvolCoeffFunctionModule");
//    }
//
//    if (pConvolCoeffFunctionModule->isGPDModuleDependent()) {
//        if (pGPDModule == 0) {
//           throw ElemUtils::CustomException(getClassName(), __func__,
//                    "This ConvolCoeffFunctionModule is GPDModule dependent but you have not provided any GPDModule");
//        }
//
//        // set gpd module to dvcs convol coeff function module
//        pConvolCoeffFunctionModule->setGPDModule(pGPDModule);
//    }
//
//    return pConvolCoeffFunctionModule;
//}

void ConvolCoeffFunctionService::generatePlotFileTask(Task& task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task, "ccf_plot_2d_view"), ' ');
}

List<GPDType> ConvolCoeffFunctionService::getFinalGPDTypeList(
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
        const List<GPDType> &gpdTypeList) const {

    List<GPDType> availableGPDTypeForCCFModel = gpdTypeList;

    availableGPDTypeForCCFModel =
            pConvolCoeffFunctionModule->getListOfAvailableGPDTypeForComputation();

    // intersection between available GPDType for this CCF model and GPDType asked
    if (!gpdTypeList.isEmpty()) {
        availableGPDTypeForCCFModel = VectorUtils::intersection(
                availableGPDTypeForCCFModel, gpdTypeList);
    }

    // if this CCF model is GPD model dependent we need to perform another intersection with GPDType available for this GPD model
    if (pConvolCoeffFunctionModule->isGPDModuleDependent()) {
        availableGPDTypeForCCFModel =
                VectorUtils::intersection(availableGPDTypeForCCFModel,
                        pConvolCoeffFunctionModule->getGPDModule()->getListOfAvailableGPDTypeForComputation());
    }

    info(__func__,
            ElemUtils::Formatter() << availableGPDTypeForCCFModel.size()
                    << " GPDType will be computed");

    return availableGPDTypeForCCFModel;
}

//ConvolCoeffFunctionModule* ConvolCoeffFunctionService::prepareComputationConfiguration(
//        const List<List<ElemUtils::Parameter> >& moduleNameList,
//        unsigned int level) const {
//
//    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;
//
//    if (moduleNameList.size() != 0) {
//        for (unsigned int i = 0; i != moduleNameList.size(); i++) {
//            if (level < moduleNameList[i].size()) {
//                //TODO remove hardcoded string
//                if (ElemUtils::StringUtils::equals(
//                        moduleNameList.get(i)[level].getName(),
//                        "DVCSConvolCoeffFunctionModule")) {
//
//                    pConvolCoeffFunctionModule =
//                            Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
//                                    moduleNameList.get(i)[level].getString());
//                } else {
//                    throw ElemUtils::CustomException(getClassName(), __func__,
//                            "You have not provided any ConvolCoeffFunctionModule");
//                }
//            }
//
//            pConvolCoeffFunctionModule->prepareComputationConfiguration(
//                    moduleNameList[i], ++level);
//        }
//    } else {
//        throw ElemUtils::CustomException(getClassName(), __func__,
//                "You have not provided any ConvolCoeffFunctionModule");
//    }
//
//    return pConvolCoeffFunctionModule;
//}

ConvolCoeffFunctionModule* ConvolCoeffFunctionService::newConvolCoeffFunctionModuleFromTask(
        const Task &task) const {
    ConvolCoeffFunctionModule* pConvolCoeffFunctionModule = 0;

    //TODO remove hardcoded string
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            "ConvolCoeffFunctionModule")) {
        pConvolCoeffFunctionModule =
                Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pConvolCoeffFunctionModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pConvolCoeffFunctionModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "You have not provided any ConvolCoeffFunctionModule");
    }

    return pConvolCoeffFunctionModule;
}

//TODO remove hardcoded string
DVCSConvolCoeffFunctionKinematic ConvolCoeffFunctionService::newKinematicFromTask(
        const Task& task) const {
    DVCSConvolCoeffFunctionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            "DVCSConvolCoeffFunctionKinematic")) {
        kinematic = DVCSConvolCoeffFunctionKinematic(
                task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

//TODO remove hardcoded string
List<DVCSConvolCoeffFunctionKinematic> ConvolCoeffFunctionService::newListOfKinematicFromTask(
        const Task& task) const {
    List<DVCSConvolCoeffFunctionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            "DVCSConvolCoeffFunctionKinematic")) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getCCFKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <DVCSConvolCoeffFunctionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <DVCSConvolCoeffFunctionKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}
