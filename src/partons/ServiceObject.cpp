#include "../../include/partons/ServiceObject.h"

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <stddef.h>
#include <iostream>

#include "../../include/partons/beans/automation/BaseObjectData.h"
#include "../../include/partons/beans/automation/Scenario.h"
#include "../../include/partons/beans/automation/Task.h"
#include "../../include/partons/Partons.h"
#include "../../include/partons/services/automation/XMLParserI.h"
#include "../../include/partons/ServiceObjectRegistry.h"

namespace PARTONS {

ServiceObject::ServiceObject(const std::string &className) :
        BaseObject(className), m_pModuleObjectFactory(0), m_pAutomationService(
                0) {
}

ServiceObject::~ServiceObject() {
    // Nothing to destroy
    // m_pModuleObjectFactory pointer will be deleted by the Partons class
}

void ServiceObject::addTasks(const List<ElemUtils::Packet>& tasks) {
    for (size_t i = 0; i != tasks.size(); i++) {
        m_queueOfTask.push(tasks[i]);
    }
}

bool ServiceObject::isEmptyTaskQueue() {
    return m_queueOfTask.empty();
}

ElemUtils::Packet ServiceObject::popTaskFormQueue() {
    return m_queueOfTask.pop();
}

void ServiceObject::initComputationalThread(ModuleObject* pModuleObject) {
    unsigned int nbOfThread = ElemUtils::GenericType(
            ElemUtils::PropertiesManager::getInstance()->getString(
                    "computation.nb.processor")).toUInt();

    info(__func__,
            ElemUtils::Formatter() << "Creation of thread : " << nbOfThread);

    m_threadManager.newThread(nbOfThread, pModuleObject);
}

void ServiceObject::launchAllThreadAndWaitingFor() {
    m_threadManager.launchAllAndWaitingFor();
}

void ServiceObject::clearAllThread() {
    m_threadManager.clearAllThread();
}

void ServiceObject::resolveObjectDependencies() {
    BaseObject::resolveObjectDependencies();

    if (!m_pModuleObjectFactory) {
        m_pModuleObjectFactory =
                Partons::getInstance()->getModuleObjectFactory();
    }

    if (!m_pAutomationService) {
        m_pAutomationService =
                Partons::getInstance()->getServiceObjectRegistry()->getAutomationService();
    }
}

List<GPDType> ServiceObject::getGPDTypeListFromTask(Task& task) const {

    debug(__func__, task.toString());

    List<GPDType> gpdTypeList;

    //TODO replace by static variable
    if (task.getTaskParameters().isAvailableSubModule("GPDType")) {
        try {
            //TODO replace string GPDType by static variable
            gpdTypeList =
                    GPDType::getListOfGPDTypeFromString(
                            task.getTaskParameters().getLastAvailable().getParameters().get(
                                    "value").getString());
        } catch (const std::exception &e) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << e.what() << " for <"
                            << XMLParserI::TASK_PARAM_NODE_NAME
                            << " type=\"GPDType\"> element. Please check your XML file.");
        }
    } else {
        debug(__func__, "No task_param XML element");
    }

    debug(__func__,
            ElemUtils::Formatter() << "GPDTypeList size = "
                    << gpdTypeList.size());

    return gpdTypeList;
}

List<CollinearDistributionType> ServiceObject::getCollinearDistributionTypeListFromTask(Task& task) const {

    debug(__func__, task.toString());

    List<CollinearDistributionType> gpdTypeList;

    //TODO replace by static variable
    if (task.getTaskParameters().isAvailableSubModule("CollinearDistributionType")) {
        try {
            //TODO replace string CollinearDistributionType by static variable
            gpdTypeList =
                    CollinearDistributionType::getListOfCollinearDistributionTypeFromString(
                            task.getTaskParameters().getLastAvailable().getParameters().get(
                                    "value").getString());
        } catch (const std::exception &e) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << e.what() << " for <"
                            << XMLParserI::TASK_PARAM_NODE_NAME
                            << " type=\"CollinearDistributionType\"> element. Please check your XML file.");
        }
    } else {
        debug(__func__, "No task_param XML element");
    }

    debug(__func__,
            ElemUtils::Formatter() << "CollinearDistributionTypeList size = "
                    << gpdTypeList.size());

    return gpdTypeList;
}

void ServiceObject::errorUnknownMethod(const Task& task) const {
    throw ElemUtils::CustomException(getClassName(), __func__,
            ElemUtils::Formatter() << "Unknown method = \""
                    << task.getFunctionName()
                    << "\" ; Please check task number ["
                    << task.getScenarioTaskIndexNumber()
                    << "] in your scenario file = \""
                    << task.getScenario()->getFilePath() << "\"");
}

} /* namespace PARTONS */
