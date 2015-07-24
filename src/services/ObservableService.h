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

#include <map>
#include <string>

#include "../beans/convolCoeffFunction/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../ServiceObject.h"

class DVCSModule;
class Observable;
class ObservableKinematic;

class ObservableService: public ServiceObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    ObservableService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ObservableService();

    std::map<double, double> computeDVCSObservableWithPhiDependency(
            DVCSModule* pDVCSModule, Observable* pObservable,
            ObservableKinematic observableKinematic,
            DVCSConvolCoeffFunctionResult dvcsConvolCoeffFunctionResult);

    virtual void computeTask(const Task &task);

private:

};

#endif /* OBSERVABLE_SERVICE_H */
