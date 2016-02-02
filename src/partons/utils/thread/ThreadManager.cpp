#include "../../../../include/partons/utils/thread/ThreadManager.h"

ThreadManager::ThreadManager() {
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

    // start all thread one by one
    for (unsigned int i = 0; i != m_listOfModuleObject.size(); i++) {
        m_listOfModuleObject[i]->launch();
    }

    // waiting end of each thread before continue
    for (unsigned int i = 0; i != m_listOfModuleObject.size(); i++) {
        m_listOfModuleObject[i]->wait();
    }

    // delete previous instantiated thread
    m_listOfModuleObject.clear();
}
