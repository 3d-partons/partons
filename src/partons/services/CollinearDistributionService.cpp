#include "../../../include/partons/services/CollinearDistributionService.h"

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
#include "../../../include/partons/modules/collinear_distribution/CollinearDistributionModule.h"
#include "../../../include/partons/ModuleObjectFactory.h"
#include "../../../include/partons/Partons.h"
#include "../../../include/partons/utils/VectorUtils.h"

namespace PARTONS {

const std::string CollinearDistributionService::COLLINEAR_DISTRIBUTION_SERVICE_COMPUTE_SINGLE_KINEMATIC =
        "computeSingleKinematic";
const std::string CollinearDistributionService::COLLINEAR_DISTRIBUTION_SERVICE_COMPUTE_MANY_KINEMATIC =
        "computeManyKinematic";

const unsigned int CollinearDistributionService::classId =
        Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
                new CollinearDistributionService("CollinearDistributionService"));

CollinearDistributionService::CollinearDistributionService(const std::string &className) :
        ServiceObjectTyped<CollinearDistributionKinematic, CollinearDistributionResult>(className) {
}

CollinearDistributionService::~CollinearDistributionService() {
}

void CollinearDistributionService::resolveObjectDependencies() {

    ServiceObjectTyped<CollinearDistributionKinematic, CollinearDistributionResult>::resolveObjectDependencies();

    try {
        m_batchSize = ElemUtils::GenericType(
                ElemUtils::PropertiesManager::getInstance()->getString(
                        "collinear_distribution.service.batch.size")).toUInt();
    } catch (const std::exception &e) {
        throw ElemUtils::CustomException(getClassName(), __func__, e.what());
    }
}

void CollinearDistributionService::computeTask(Task &task) {

    ServiceObjectTyped<CollinearDistributionKinematic, CollinearDistributionResult>::computeTask(task);

    List<CollinearDistributionResult> resultList;

    if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            CollinearDistributionService::COLLINEAR_DISTRIBUTION_SERVICE_COMPUTE_MANY_KINEMATIC)) {
        resultList.add(computeManyKinematicTask(task));
    }

    else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
            CollinearDistributionService::COLLINEAR_DISTRIBUTION_SERVICE_COMPUTE_SINGLE_KINEMATIC)) {
        resultList.add(computeSingleKinematicTask(task));
    }

    else if (!computeGeneralTask(task)) {
        errorUnknownMethod(task);
    }

    updateResultInfo(resultList, m_resultInfo);

    m_resultListBuffer = resultList;
}

CollinearDistributionResult CollinearDistributionService::computeSingleKinematic(const CollinearDistributionKinematic &colldistKinematic,
        CollinearDistributionModule* pCollinearDistributionModule, const List<CollinearDistributionType>& colldistTypeList) const {

    //get list of collinear-distribution types
    List<CollinearDistributionType> restrictedByCollinearDistributionTypeListFinal = getFinalCollinearDistributionTypeList(pCollinearDistributionModule,
            colldistTypeList);

    //return
    return pCollinearDistributionModule->compute(colldistKinematic, restrictedByCollinearDistributionTypeListFinal);
}

List<CollinearDistributionResult> CollinearDistributionService::computeManyKinematic(
        const List<CollinearDistributionKinematic> &colldistKinematicList, CollinearDistributionModule* pCollinearDistributionModule,
        const List<CollinearDistributionType>& colldistTypeList) {

    //debug information
    debug(__func__,
            ElemUtils::Formatter() << colldistKinematicList.size()
                    << " collinear-distribution kinematic(s) will be computed with "
                    << pCollinearDistributionModule->getClassName());

    //initialize
    List<CollinearDistributionResult> results;
    List<ElemUtils::Packet> listOfPacket;
    List<CollinearDistributionType> finalCollinearDistributionTypeList = getFinalCollinearDistributionTypeList(pCollinearDistributionModule,
            colldistTypeList);

    //if to be computed
    if (finalCollinearDistributionTypeList.size() != 0) {

        //init thread
        initComputationalThread(pCollinearDistributionModule);

        //print info
        info(__func__, "Thread(s) running ...");

        //batch feature
        unsigned int i = 0;
        unsigned int j = 0;

        //divide to packets
        while (i != colldistKinematicList.size()) {

            listOfPacket.clear();
            j = 0;

            while ((j != m_batchSize) && (i != colldistKinematicList.size())) {
                ElemUtils::Packet packet;
                CollinearDistributionKinematic kinematic;
                kinematic = colldistKinematicList[i];
                packet << kinematic << finalCollinearDistributionTypeList;
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
                "Nothing to compute with your computation configuration ; there is no CollinearDistributionType available");
    }

    return results;
}

CollinearDistributionResult CollinearDistributionService::computeSingleKinematicTask(Task& task) {

    //create a CollinearDistributionKinematic and init it with a list of parameters
    CollinearDistributionKinematic colldistKinematic = newKinematicFromTask(task);

    //get collinear-distribution types
    List<CollinearDistributionType> colldistTypeList = getCollinearDistributionTypeListFromTask(task);

    //get configured observable module
    CollinearDistributionModule* pCollinearDistributionModule = newCollinearDistributionModuleFromTask(task);

    //make computation
    CollinearDistributionResult colldistResult = computeSingleKinematic(colldistKinematic, pCollinearDistributionModule,
            colldistTypeList);

    //remove reference to pCollinearDistributionModule pointer.
    m_pModuleObjectFactory->updateModulePointerReference(pCollinearDistributionModule, 0);
    pCollinearDistributionModule = 0;

    //return
    return colldistResult;
}

List<CollinearDistributionResult> CollinearDistributionService::computeManyKinematicTask(Task& task) {

    //get kinematics
    List<CollinearDistributionKinematic> listOfKinematic = newListOfKinematicFromTask(task);

    //get collinear-distribution types
    List<CollinearDistributionType> colldistTypeList = getCollinearDistributionTypeListFromTask(task);

    //get observable module
    CollinearDistributionModule* pCollinearDistributionModule = newCollinearDistributionModuleFromTask(task);

    //make computation
    List<CollinearDistributionResult> result = computeManyKinematic(listOfKinematic, pCollinearDistributionModule,
            colldistTypeList);

    //remove reference to pCollinearDistributionModule pointer
    m_pModuleObjectFactory->updateModulePointerReference(pCollinearDistributionModule, 0);
    pCollinearDistributionModule = 0;

    //return
    return result;
}

List<CollinearDistributionType> CollinearDistributionService::getFinalCollinearDistributionTypeList(CollinearDistributionModule* pCollinearDistributionModule,
        const List<CollinearDistributionType> &colldistTypeList) const {

    //initialize
    List<CollinearDistributionType> restrictedByCollinearDistributionTypeListFinal = colldistTypeList;

    //get list of collinear-distribution types available
    restrictedByCollinearDistributionTypeListFinal =
            pCollinearDistributionModule->getListOfAvailableCollinearDistributionTypeForComputation();

    //intersection between available CollinearDistributionType and CollinearDistributionType asked
    if (!colldistTypeList.isEmpty()) {
        restrictedByCollinearDistributionTypeListFinal = VectorUtils::intersection(
                restrictedByCollinearDistributionTypeListFinal, colldistTypeList);
    }

    //debug info
    debug(__func__,
            ElemUtils::Formatter() << restrictedByCollinearDistributionTypeListFinal.size()
                    << " CollinearDistributionType will be computed");

    //return
    return restrictedByCollinearDistributionTypeListFinal;
}

CollinearDistributionKinematic CollinearDistributionService::newKinematicFromTask(const Task& task) const {

    //create a kinematic and init it with a list of parameters
    CollinearDistributionKinematic kinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            CollinearDistributionKinematic::COLLINEAR_DISTRIBUTION_KINEMATIC_CLASS_NAME)) {
        kinematic.configure(task.getKinematicsData().getParameters());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <CollinearDistributionKinematic> for method "
                        << task.getFunctionName());
    }

    return kinematic;
}

List<CollinearDistributionKinematic> CollinearDistributionService::newListOfKinematicFromTask(
        const Task& task) const {

    List<CollinearDistributionKinematic> listOfKinematic;

    if (ElemUtils::StringUtils::equals(
            task.getKinematicsData().getModuleClassName(),
            CollinearDistributionKinematic::COLLINEAR_DISTRIBUTION_KINEMATIC_CLASS_NAME)) {

        if (task.getKinematicsData().getParameters().isAvailable("file")) {
            listOfKinematic =
                    KinematicUtils().getCollinearDistributionKinematicFromFile(
                            task.getKinematicsData().getParameters().getLastAvailable().getString());
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "Missing parameter file in object <CollinearDistributionKinematic> for method "
                            << task.getFunctionName());
        }
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Missing object : <CollinearDistributionKinematic> for method "
                        << task.getFunctionName());
    }

    return listOfKinematic;
}

CollinearDistributionModule* CollinearDistributionService::newCollinearDistributionModuleFromTask(const Task& task) const {

    //initialize
    CollinearDistributionModule* pCollinearDistributionModule = 0;

    //check if available
    if (ElemUtils::StringUtils::equals(
            task.getModuleComputationConfiguration().getModuleType(),
            CollinearDistributionModule::COLLINEAR_DISTRIBUTION_MODULE_CLASS_NAME)) {

        //configure
        pCollinearDistributionModule =
                Partons::getInstance()->getModuleObjectFactory()->newCollinearDistributionModule(
                        task.getModuleComputationConfiguration().getModuleClassName());

        pCollinearDistributionModule->configure(
                task.getModuleComputationConfiguration().getParameters());

        pCollinearDistributionModule->prepareSubModules(
                task.getModuleComputationConfiguration().getSubModules());
    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "You have not provided any "
                        << CollinearDistributionModule::COLLINEAR_DISTRIBUTION_MODULE_CLASS_NAME);
    }

    //return
    return pCollinearDistributionModule;
}

} /* namespace PARTONS */
