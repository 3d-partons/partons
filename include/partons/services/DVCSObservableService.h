#ifndef DVCS_OBSERVABLE_SERVICE_H
#define DVCS_OBSERVABLE_SERVICE_H

/**
 * @file DVCSObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/List.h"
#include "../beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../beans/observable/DVCS/DVCSObservableResult.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/ProcessModule.h"
#include "ObservableService.h"

namespace PARTONS {
class DVCSObservable;
class DVCSProcessModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVCSObservableService
 *
 * @brief Singleton to handle and compute some pre-configured observable modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class DVCSObservableService: public ObservableService<DVCSObservableKinematic,
        DVCSObservableResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DVCSObservableService();

    virtual void resolveObjectDependencies();
    virtual DVCSObservableKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<DVCSObservableKinematic> newListOfKinematicFromTask(
            const Task &task) const;
    virtual void storeResultListInDatabase(
            const List<DVCSObservableResult>& results) const;
    virtual void generatePlotFileTask(Task &task);

    /**
     * Uses an automation task (XML file) to configure a DVCSProcessModule.
     * @param task Automation task.
     * @return Pre-configured DVCSProcessModule.
     */
    DVCSProcessModule* newDVCSProcessModuleFromTask(const Task &task) const;

    /**
     * Uses an automation task (XML file) to configure a DVCSObservable.
     * @param task Automation task.
     * @return Pre-configured DVCSObservable.
     */
    DVCSObservable* newDVCSObservableModuleFromTask(const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    DVCSObservableService(const std::string &className);

    virtual ProcessModule<DVCSObservableKinematic, DVCSObservableResult>* newProcessModuleFromTask(
            const Task &task) const;
    virtual Observable<DVCSObservableKinematic, DVCSObservableResult>* newObservableModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_SERVICE_H */
