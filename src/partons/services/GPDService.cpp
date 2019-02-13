#include "../../../include/partons/services/GPDService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>

#include "../../../include/partons/beans/automation/BaseObjectData.h"
#include "../../../include/partons/beans/automation/Task.h"
#include "../../../include/partons/beans/KinematicUtils.h"
#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/database/gpd/service/GPDResultDaoService.h"
#include "../../../include/partons/modules/gpd/GPDModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/utils/VectorUtils.h"

namespace PARTONS {

const std::string GPDService::GPD_SERVICE_COMPUTE_SINGLE_KINEMATIC =
        "computeSingleKinematic";
const std::string GPDService::GPD_SERVICE_COMPUTE_MANY_KINEMATIC =
        "computeManyKinematic";
const std::string GPDService::GPD_SERVICE_GENERATE_PLOT_FILE =
        "generatePlotFile";

const unsigned int GPDService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new GPDService("GPDService"));

GPDService::GPDService(const std::string &className) :
        ServiceObjectTyped<GPDKinematic, GPDResult>(className) {
}

GPDService::~GPDService() {
}

void GPDService::resolveObjectDependencies() {

    ServiceObjectTyped<GPDKinematic, GPDResult>::resolveObjectDependencies();

    try {
        m_batchSize = ElemUtils::GenericType(
                ElemUtils::PropertiesManager::getInstance()->getString(
                        "gpd.service.batch.size")).toUInt();
    } catch (const std::exception &e) {
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }
}

void GPDService::computeTask(Task &task) {

    ServiceObjectTyped<GPDKinematic, GPDResult>::computeTask(task);

    List<GPDResult> resultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_MANY_KINEMATIC)) {
        resultList.add(computeManyKinematicTask(task));
    }

    else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_COMPUTE_SINGLE_KINEMATIC)) {
        resultList.add(computeSingleKinematicTask(task));
    }

    else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            GPDService::GPD_SERVICE_GENERATE_PLOT_FILE)) {
        generatePlotFileTask(task);
    }

    else if (!computeGeneralTask(task)) {
        errorUnknownMethod(task);
    }

    updateResultInfo(resultList, m_resultInfo);

    if (task.isStoreInDB()) {

        GPDResultDaoService resultService;

        int computationId = resultService.insert(resultList);

        if (computationId != -1) {
            info(__func__,
                    ElemUtils::Formatter()
                            << "List of GPD result has been stored in database with computation_id = "
                            << computationId);
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Failed to insert List of GPD result into database");
        }

        GPDResultDaoService gpdResultDaoService;
        gpdResultDaoService.insert(resultList);
    }

    m_resultListBuffer = resultList;
}

GPDResult GPDService::computeSingleKinematic(const GPDKinematic &gpdKinematic,
        GPDModule* pGPDModule, const List<GPDType>& gpdTypeList) const {

    //get list of GPD types
    List<GPDType> restrictedByGPDTypeListFinal = getFinalGPDTypeList(pGPDModule,
            gpdTypeList);

    //return
    return pGPDModule->compute(gpdKinematic, restrictedByGPDTypeListFinal);
}

List<GPDResult> GPDService::computeManyKinematic(
        const List<GPDKinematic> &gpdKinematicList, GPDModule* pGPDModule,
        const List<GPDType>& gpdTypeList) {

    //print information
    info(__func__,
            ElemUtils::Formatter() << gpdKinematicList.size()
                    << " GPD kinematic(s) will be computed with "
                    << pGPDModule->getClassName());

    //initialize
    List<GPDResult> results;
    List<ElemUtils::Packet> listOfPacket;
    List<GPDType> finalGPDTypeList = getFinalGPDTypeList(pGPDModule,
            gpdTypeList);

    //if to be computed
    if (finalGPDTypeList.size() != 0) {

        //init thread
        initComputationalThread(pGPDModule);

        //print info
        info(__func__, "Thread(s) running ...");

        //batch feature
        unsigned int i = 0;
        unsigned int j = 0;

        //divide to packets
        while (i != gpdKinematicList.size()) {

            listOfPacket.clear();
            j = 0;

            while ((j != m_batchSize) && (i != gpdKinematicList.size())) {
                ElemUtils::Packet packet;
                GPDKinematic kinematic;
                kinematic = gpdKinematicList[i];
                packet << kinematic << finalGPDTypeList;
                listOfPacket.add(packet);
                i++;
                j++;
            }

            //add, lunch and sort
            addTasks(listOfPacket);
            launchAllThreadAndWaitingFor();
            sortResultList();

            //print info
            info(__func__,
                    ElemUtils::Formatter() << "Kinematic(s) already computed : "
                            << i);

            //update result info
            updateResultInfo(getResultList(), m_resultInfo);

            //add to output
            results.add(getResultList());

            //clear buffer
            clearResultListBuffer();
        }

        //clear threads
        clearAllThread();

    } else {
        info(__func__,
                "Nothing to compute with your computation configuration ; there is no GPDType available");
    }

    return results;
}

GPDResult GPDService::computeSingleKinematicTask(Task& task) {

    //create a GPDKinematic and init it with a list of parameters
    GPDKinematic gpdKinematic = newKinematicFromTask(task);

    //get GPD types
    List<GPDType> gpdTypeList = getGPDTypeListFromTask(task);

    //get configured observable module
    GPDModule* pGPDModule = newGPDModuleFromTask(task);

    //make computation
    GPDResult gpdResult = computeSingleKinematic(gpdKinematic, pGPDModule,
            gpdTypeList);

    //remove reference to pGPDModule pointer.
    m_pModuleObjectFactory->updateModulePointerReference(pGPDModule, 0);
    pGPDModule = 0;

    //return
    return gpdResult;
}

List<GPDResult> GPDService::computeManyKinematicTask(Task& task) {

    //get kinematics
    List<GPDKinematic> listOfKinematic = newListOfKinematicFromTask(task);

    //get GPD types
    List<GPDType> gpdTypeList = getGPDTypeListFromTask(task);

    //get observable module
    GPDModule* pGPDModule = newGPDModuleFromTask(task);

    //make computation
    List<GPDResult> result = computeManyKinematic(listOfKinematic, pGPDModule,
            gpdTypeList);

    //remove reference to pGPDModule pointer
    m_pModuleObjectFactory->updateModulePointerReference(pGPDModule, 0);
    pGPDModule = 0;

    //return
    return result;
}

void GPDService::generatePlotFileTask(Task& task) {
    generatePlotFile(getOutputFilePathForPlotFileTask(task),
            generateSQLQueryForPlotFileTask(task, "gpd_plot_2d_view"), ' ');
}

List<GPDType> GPDService::getFinalGPDTypeList(GPDModule* pGPDModule,
        const List<GPDType> &gpdTypeList) const {

    //initialize
    List<GPDType> restrictedByGPDTypeListFinal = gpdTypeList;

    //get list of GPD types available
    restrictedByGPDTypeListFinal =
            pGPDModule->getListOfAvailableGPDTypeForComputation();

    //intersection between available GPDType and GPDType asked
    if (!gpdTypeList.isEmpty()) {
        restrictedByGPDTypeListFinal = VectorUtils::intersection(
                restrictedByGPDTypeListFinal, gpdTypeList);
    }

    //print info
    info(__func__,
            ElemUtils::Formatter() << restrictedByGPDTypeListFinal.size()
                    << " GPDType will be computed");

    //return
    return restrictedByGPDTypeListFinal;
}

GPDKinematic GPDService::newKinematicFromTask(const Task& task) const {

    //create a kinematic and init it with a list of parameters
    GPDKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            GPDKinematic::GPD_KNEMATIC_CLASS_NAME)) {
        kinematic = GPDKinematic(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<GPDKinematic> GPDService::newListOfKinematicFromTask(
        const Task& task) const {

    List<GPDKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            GPDKinematic::GPD_KNEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getGPDKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <GPDKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <GPDKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

GPDModule* GPDService::newGPDModuleFromTask(const Task& task) const {

    //initialize
    GPDModule* pGPDModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            GPDModule::GPD_MODULE_CLASS_NAME)) {

        //configure
        pGPDModule =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pGPDModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pGPDModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "You have not provided any GPDModule");
    }

    //return
    return pGPDModule;
}

} /* namespace PARTONS */
