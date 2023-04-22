#ifndef DDVCS_OBSERVABLE_SERVICE_H
#define DDVCS_OBSERVABLE_SERVICE_H

/**
 * @file DDVCSObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/List.h"
#include "../beans/observable/DDVCS/DDVCSObservableKinematic.h"
#include "../beans/observable/DDVCS/DDVCSObservableResult.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/ProcessModule.h"
#include "ObservableService.h"

namespace PARTONS {
class DDVCSObservable;
class DDVCSProcessModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DDVCSObservableService
 *
 * @brief Singleton to handle and compute some pre-configured observable modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class DDVCSObservableService: public ObservableService<DDVCSObservableKinematic,
        DDVCSObservableResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~DDVCSObservableService();

    virtual void resolveObjectDependencies();
    virtual DDVCSObservableKinematic newKinematicFromTask(
            const Task &task) const;
    virtual List<DDVCSObservableKinematic> newListOfKinematicFromTask(
            const Task &task) const;
    virtual void storeResultListInDatabase(
            const List<DDVCSObservableResult>& results) const;
    virtual void generatePlotFileTask(Task &task);

    /**
     * Uses an automation task (XML file) to configure a DDVCSProcessModule.
     * @param task Automation task.
     * @return Pre-configured DDVCSProcessModule.
     */
    DDVCSProcessModule* newDDVCSProcessModuleFromTask(const Task &task) const;

    /**
     * Uses an automation task (XML file) to configure a DDVCSObservable.
     * @param task Automation task.
     * @return Pre-configured DDVCSObservable.
     */
    DDVCSObservable* newDDVCSObservableModuleFromTask(const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    DDVCSObservableService(const std::string &className);

    virtual ProcessModule<DDVCSObservableKinematic, DDVCSObservableResult>* newProcessModuleFromTask(
            const Task &task) const;
    virtual Observable<DDVCSObservableKinematic, DDVCSObservableResult>* newObservableModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* DDVCS_OBSERVABLE_SERVICE_H */
