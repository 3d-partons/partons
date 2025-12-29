#ifndef JET_OBSERVABLE_SERVICE_H
#define JET_OBSERVABLE_SERVICE_H

/**
 * @file JETObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/List.h"
#include "../beans/observable/JET/JETObservableKinematic.h"
#include "../beans/observable/JET/JETObservableResult.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/ProcessModule.h"
#include "ObservableService.h"

namespace PARTONS {
class JETObservable;
class JETProcessModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class JETObservableService
 *
 * @brief Singleton to handle and compute some pre-configured observable modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class JETObservableService: public ObservableService<JETObservableKinematic,
        JETObservableResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~JETObservableService();

    virtual void resolveObjectDependencies();
    virtual JETObservableKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<JETObservableKinematic> newListOfKinematicFromTask(
            const Task &task) const;

    /**
     * Uses an automation task (XML file) to configure a JETProcessModule.
     * @param task Automation task.
     * @return Pre-configured JETProcessModule.
     */
    JETProcessModule* newJETProcessModuleFromTask(const Task &task) const;

    /**
     * Uses an automation task (XML file) to configure a JETObservable.
     * @param task Automation task.
     * @return Pre-configured JETObservable.
     */
    JETObservable* newJETObservableModuleFromTask(const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    JETObservableService(const std::string &className);

    virtual ProcessModule<JETObservableKinematic, JETObservableResult>* newProcessModuleFromTask(
            const Task &task) const;
    virtual Observable<JETObservableKinematic, JETObservableResult>* newObservableModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* JET_OBSERVABLE_SERVICE_H */
