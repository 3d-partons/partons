#ifndef TCS_OBSERVABLE_SERVICE_H
#define TCS_OBSERVABLE_SERVICE_H

/**
 * @file TCSObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/List.h"
#include "../beans/observable/TCS/TCSObservableKinematic.h"
#include "../beans/observable/TCS/TCSObservableResult.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/ProcessModule.h"
#include "ObservableService.h"

namespace PARTONS {

/**
 * @class TCSObservableService
 *
 * @brief Singleton to handle and compute some pre-configured observable modules.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 */
class TCSObservableService: public ObservableService<TCSObservableKinematic,
        TCSObservableResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Destructor.
     */
    virtual ~TCSObservableService();

    virtual void resolveObjectDependencies();
    virtual TCSObservableKinematic newKinematicFromTask(const Task &task) const;
    virtual List<TCSObservableKinematic> newListOfKinematicFromTask(
            const Task &task) const;
    virtual void storeResultListInDatabase(
            const List<TCSObservableResult>& results) const;
    virtual void generatePlotFileTask(Task &task);

protected:

    /**
     * Default constructor.
     */
    TCSObservableService(const std::string &className);

    virtual ProcessModule<TCSObservableKinematic, TCSObservableResult>* newProcessModuleFromTask(
            const Task &task) const;
    virtual Observable<TCSObservableKinematic, TCSObservableResult>* newObservableModuleFromTask(
            const Task &task) const;
};

} /* namespace PARTONS */

#endif /* TCS_OBSERVABLE_SERVICE_H */
