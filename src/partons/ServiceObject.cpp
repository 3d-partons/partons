#include "../../include/partons/ServiceObject.h"

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <stddef.h>

#include "../../include/partons/Partons.h"
#include "../../include/partons/ServiceObjectRegistry.h"

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
