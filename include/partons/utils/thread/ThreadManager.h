#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

/**
 * @file ThreadManager.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @author <contibutor> http://vichargrave.com/java-style-thread-class-in-c/
 * @date 13 January 2016
 */

#include "../../beans/List.h"
#include "../../ModuleObject.h"

namespace PARTONS {
/**
 * @class ThreadManager
 */

class ThreadManager: public BaseObject {
public:
    ThreadManager();
    virtual ~ThreadManager();

    void newThread(const unsigned int numberOfThread,
            ModuleObject *pModuleObject);
    void launchAllAndWaitingFor();

    void clearAllThread();

private:
    List<ModuleObject*> m_listOfModuleObject;
};

} /* namespace PARTONS */

#endif /* THREAD_MANAGER_H */
