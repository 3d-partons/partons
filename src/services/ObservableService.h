#ifndef OBSERVABLE_SERVICE_H
#define OBSERVABLE_SERVICE_H

/**
 * @file ObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 20 November 2014
 * @version 1.0
 *
 * @class ObservableService
 *
 * @brief \<singleton\> Use for handle and compute some pre-configured CFF modules.
 */

#include <string>
#include <vector>

#include "../beans/observable/ObservableKinematic.h"
#include "../ServiceObject.h"

class DVCSConvolCoeffFunctionModule;
class DVCSModule;
class Observable;
class ObservableKinematic;
class ObservableResultList;

class ObservableService: public ServiceObject {
public:
    static const std::string FUNCTION_NAME_COMPUTE_DVCS_OBSERVABLE;
    static const std::string FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    ObservableService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ObservableService();

    ObservableResultList computeDVCSObservable(DVCSModule* pDVCSModule,
            Observable* pObservable,
            const ObservableKinematic &observableKinematic,
            DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule);

    ObservableResultList computeManyKinematicOneModel(
            std::vector<ObservableKinematic> listOfKinematic,
            DVCSModule* pDVCSModule, Observable* pObservable,
            DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule);

    virtual void computeTask(Task &task);

private:
    void computeDVCSObservableTask(Task &task);
    void computeManyKinematicOneModelTask(Task &task);

};

#endif /* OBSERVABLE_SERVICE_H */
