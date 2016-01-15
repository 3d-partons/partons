#include "../../../../include/partons/utils/thread/ThreadManager.h"

#include <pthread.h>

ThreadManager::ThreadManager() {
}

ThreadManager::~ThreadManager() {
}

void ThreadManager::newThread(const unsigned int numberOfThread,
        Thread *pThread) {

    m_listOfThreads.add(pThread);

    for (unsigned int i = 1; i != numberOfThread; i++) {
        m_listOfThreads.add(pThread->clone());
    }
}

//TODO gerer lors d'une exception l'arret et la cloture des thread en cours
void ThreadManager::launchAllAndWaitingFor() {

    // start all thread one by one
    for (unsigned int i = 0; i != m_listOfThreads.size(); i++) {
        m_listOfThreads[i]->start();
    }

    // waiting end of each thread before continue
    for (unsigned int i = 0; i != m_listOfThreads.size(); i++) {
        void* result;
        pthread_join(m_listOfThreads[i]->getThreadId(), &result);
    }

    // delete previous instantiated thread
    m_listOfThreads.clear();
}
