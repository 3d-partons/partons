#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

/**
 * @file ThreadManager.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @author <contibutor> http://vichargrave.com/java-style-thread-class-in-c/
 * @date 13 January 2016
 *
 * @class ThreadManager
 */

#include "../../beans/List.h"
#include "Thread.h"

class ThreadManager {
public:
    ThreadManager();
    virtual ~ThreadManager();

    void newThread(const unsigned int numberOfThread, Thread *pThread);
    void launchAllAndWaitingFor();

private:

    List<Thread*> m_listOfThreads;
};

#endif /* THREAD_MANAGER_H */
