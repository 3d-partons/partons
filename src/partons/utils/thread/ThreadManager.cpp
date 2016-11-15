#include "../../../../include/partons/utils/thread/ThreadManager.h"

#include <ElementaryUtils/logger/CustomException.h>

ThreadManager::ThreadManager() :
        BaseObject("ThreadManager") {
}

ThreadManager::~ThreadManager() {
}

void ThreadManager::newThread(const unsigned int numberOfThread,
        ModuleObject *pModuleObject) {

    for (unsigned int i = 0; i != numberOfThread; i++) {
        m_listOfModuleObject.add(pModuleObject->clone());
    }
}

//TODO gerer lors d'une exception l'arret et la cloture des thread en cours
void ThreadManager::launchAllAndWaitingFor() {

    if (m_listOfModuleObject.size() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "List of instantiated thread(s) is empty");
    }

    // start all thread one by one
    for (unsigned int i = 0; i != m_listOfModuleObject.size(); i++) {
        m_listOfModuleObject[i]->launch();
    }

    // waiting end of each thread before continue
    for (unsigned int i = 0; i != m_listOfModuleObject.size(); i++) {
        m_listOfModuleObject[i]->wait();
    }
}

void ThreadManager::clearAllThread() {
    // delete previous instantiated thread
    m_listOfModuleObject.clear();
}
