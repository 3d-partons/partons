#ifndef DVMP_OBSERVABLE_SERVICE_H
#define DVMP_OBSERVABLE_SERVICE_H

/**
 * @file DVMPObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/List.h"
#include "../beans/observable/DVMP/DVMPObservableKinematic.h"
#include "../beans/observable/DVMP/DVMPObservableResult.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/ProcessModule.h"
#include "ObservableService.h"

namespace PARTONS {
class DVMPObservable;
class DVMPProcessModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPObservableService
 *
 * @brief Singleton to handle and compute some pre-configured observable modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class DVMPObservableService: public ObservableService<DVMPObservableKinematic,
        DVMPObservableResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DVMPObservableService();

    virtual void resolveObjectDependencies();
    virtual DVMPObservableKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<DVMPObservableKinematic> newListOfKinematicFromTask(
            const Task &task) const;

    /**
     * Uses an automation task (XML file) to configure a DVMPProcessModule.
     * @param task Automation task.
     * @return Pre-configured DVMPProcessModule.
     */
    DVMPProcessModule* newDVMPProcessModuleFromTask(const Task &task) const;

    /**
     * Uses an automation task (XML file) to configure a DVMPObservable.
     * @param task Automation task.
     * @return Pre-configured DVMPObservable.
     */
    DVMPObservable* newDVMPObservableModuleFromTask(const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    DVMPObservableService(const std::string &className);

    virtual ProcessModule<DVMPObservableKinematic, DVMPObservableResult>* newProcessModuleFromTask(
            const Task &task) const;
    virtual Observable<DVMPObservableKinematic, DVMPObservableResult>* newObservableModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* DVMP_OBSERVABLE_SERVICE_H */
