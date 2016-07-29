#ifndef SERVICE_OBJECT_H
#define SERVICE_OBJECT_H

/**
 * @file ServiceObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 01, 2015
 * @version 1.0
 */

#include <ElementaryUtils/thread/Packet.h>
#include <string>

#include "beans/List.h"
#include "utils/thread/ThreadManager.h"
#include "utils/thread/ThreadQueue.h"

class AutomationService;
class ModuleObjectFactory;
class Task;

/**
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

    void addTasks(const List<ElemUtils::Packet> &tasks);

    bool isEmptyTaskQueue();

    ElemUtils::Packet popTaskFormQueue();

    void initComputationalThread(ModuleObject* pModuleObject);

    void launchAllThreadAndWaitingFor();

    void clearAllThread();

    virtual void resolveObjectDependencies();

    std::string generateSQLQueryForPlotFile(const std::string &tableName,
            const ElemUtils::Parameters& selectParams,
            const ElemUtils::Parameters& whereParams) const;

protected:
    ModuleObjectFactory* m_pModuleObjectFactory;
    AutomationService* m_pAutomationService;

    std::string generateSQLQueryForPlotFileTask(Task &task,
            const std::string &tableName) const;

    std::string getOutputFilePathForPlotFileTask(Task &task) const;

private:
    ThreadQueue m_queueOfTask;
    ThreadManager m_threadManager;
};

#endif /* SERVICE_OBJECT_H */
