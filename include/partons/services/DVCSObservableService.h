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
#include "ObservableService.h"

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

    void resolveObjectDependencies();
    DVCSObservableKinematic newKinematicFromTask(const Task &task) const;
    List<DVCSObservableKinematic> newListOfKinematicFromTask(
            const Task &task) const;

protected:

    /**
     * Default constructor.
     */
    DVCSObservableService(const std::string &className);
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_SERVICE_H */
