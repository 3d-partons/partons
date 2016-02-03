#include "../../include/partons/ServiceObject.h"

#include <stddef.h>

#include "../../include/partons/Partons.h"
#include "../../include/partons/utils/PropertiesManager.h"
#include "../../include/partons/utils/stringUtils/Formatter.h"
#include "../../include/partons/utils/stringUtils/StringUtils.h"

ServiceObject::ServiceObject(const std::string &className) :
        BaseObject(className), m_pModuleObjectFactory(0) {
}

ServiceObject::~ServiceObject() {
    // Nothing to destroy
    // m_pModuleObjectFactory pointer will be deleted by the Partons class
}

void ServiceObject::addTasks(const List<Packet>& tasks) {
    for (size_t i = 0; i != tasks.size(); i++) {
        m_queueOfTask.push(tasks[i]);
    }
}

bool ServiceObject::isEmptyTaskQueue() {
    return m_queueOfTask.empty();
}

Packet ServiceObject::popTaskFormQueue() {
    return m_queueOfTask.pop();
}

void ServiceObject::initComputationalThread(ModuleObject* pModuleObject) {

    //TODO right cast from string to unsigned int ; currently it's string to int
    unsigned int nbOfThread = StringUtils::fromStringToInt(
            PropertiesManager::getInstance()->getString(
                    "computation.nb.processor"));

    info(__func__, Formatter() << "Creation of thread : " << nbOfThread);

    m_threadManager.newThread(nbOfThread, pModuleObject);
}

void ServiceObject::launchAllThreadAndWaitingFor() {
    m_threadManager.launchAllAndWaitingFor();
}

void ServiceObject::init() {
    BaseObject::init();

    if (!m_pModuleObjectFactory) {
        m_pModuleObjectFactory =
                Partons::getInstance()->getModuleObjectFactory();
    }
}
