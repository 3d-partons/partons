#ifndef SERVICE_OBJECT_H
#define SERVICE_OBJECT_H

/**
 * @file ServiceObject.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 01, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/thread/Packet.h>
#include <string>

#include "beans/gpd/GPDType.h"
#include "beans/collinear_distribution/CollinearDistributionType.h"
#include "beans/List.h"
#include "utils/thread/ThreadManager.h"
#include "utils/thread/ThreadQueue.h"

namespace PARTONS {
class AutomationService;
class ModuleObjectFactory;
class Task;
} /* namespace PARTONS */

namespace PARTONS {

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
     * Default constructor.
     *
     * @param className
     */
    ServiceObject(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~ServiceObject();

    virtual void resolveObjectDependencies();

    /**
     * Method used in automation to compute given tasks.
     * Implemented in child classes.
     * @param task Automation task to compute.
     */
    virtual void computeTask(Task &task) = 0;

    /**
     * Add task to queue.
     */
    void addTasks(const List<ElemUtils::Packet> &tasks);

    /**
     * Check if queue is empty.
     */
    bool isEmptyTaskQueue();

    /**
     * Pop task from queue.
     */
    ElemUtils::Packet popTaskFormQueue();

    /**
     * Initialize all threads.
     */
    void initComputationalThread(ModuleObject* pModuleObject);

    /**
     * Lunch all threads.
     */
    void launchAllThreadAndWaitingFor();

    /**
     * Clear all threads.
     */
    void clearAllThread();

    /**
     * Generate SQL query to create a plot file.
     */
    std::string generateSQLQueryForPlotFile(const std::string &tableName,
            const ElemUtils::Parameters& selectParams,
            const ElemUtils::Parameters& whereParams) const;

    /**
     * Generate a plot file.
     */
    void generatePlotFile(const std::string& filePath,
            const std::string &sqlQuery, const char splitChar) const;

protected:

    ModuleObjectFactory* m_pModuleObjectFactory; ///< Pointer to ModuleObjectFactory.
    AutomationService* m_pAutomationService; ///< Pointer to AutomationService.

    /**
     * Generate SQL query from a task.
     */
    std::string generateSQLQueryForPlotFileTask(Task &task,
            const std::string &tableName) const;

    /**
     * Get path to a plot file from a task.
     */
    std::string getOutputFilePathForPlotFileTask(Task &task) const;

    /**
     * Get list of GPD types to be computed from a task.
     */
    List<GPDType> getGPDTypeListFromTask(Task &task) const;

    /**
     * Get list of Collinear Distribution types to be computed from a task.
     */
    List<CollinearDistributionType> getCollinearDistributionTypeListFromTask(Task &task) const;

    /**
     * Throw exception if unknown method.
     */
    void errorUnknownMethod(const Task &task) const;

private:

    ThreadQueue m_queueOfTask; ///< Queue of tasks.
    ThreadManager m_threadManager; ///< Thread manager.
};

} /* namespace PARTONS */

#endif /* SERVICE_OBJECT_H */
