#ifndef SERVICE_OBJECT_H
#define SERVICE_OBJECT_H

/**
 * @file ServiceObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 01 July 2015
 * @version 1.0
 *
 * @class ServiceObject
 *
 * @brief Services hide the complexity of low-level functions to provide high-level functionalities to the user.
 *
 * A service is basically a toolbox for the user: the user is being given tools to use the software without knowing the details of its operating.
 * Services combine different modules and datasets to produce results in a transparent way.
 *
 * For example : the GPDService provide several functions that hide the complexity related to repetitive tasks,
 * like evaluating a GPD at a list of kinematic configurations, or evaluating several GPD models at the same input kinematic.
 */

#include <string>

#include "beans/List.h"
#include "utils/thread/Packet.h"
#include "utils/thread/ThreadManager.h"
#include "utils/thread/ThreadQueue.h"

class AutomationService;
class ModuleObjectFactory;
class Task;

class ServiceObject: public BaseObject {
public:
    /**
     * Default constructor
     *
     * @param className
     */
    ServiceObject(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ServiceObject();

    virtual void computeTask(Task &task) = 0;

    void addTasks(const List<Packet> &tasks);

    bool isEmptyTaskQueue();

    Packet popTaskFormQueue();

    void initComputationalThread(ModuleObject* pModuleObject);

    void launchAllThreadAndWaitingFor();

    virtual void init();

protected:
    ModuleObjectFactory* m_pModuleObjectFactory;
    AutomationService* m_pAutomationService;

private:
    ThreadQueue m_queueOfTask;
    ThreadManager m_threadManager;
};

#endif /* SERVICE_OBJECT_H */
