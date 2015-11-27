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

#include "../beans/List.h"
#include "../beans/observable/ObservableKinematic.h"
//#include "../beans/observable/ObservableKinematicList.h"
#include "../ServiceObject.h"

class DVCSConvolCoeffFunctionModule;
class DVCSModule;
class Observable;
class ObservableResult;
class ObservableResultList;
class ObservableResultListReport;
class Tolerances;

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

    ObservableResult computeDVCSObservable(DVCSModule* pDVCSModule,
            Observable* pObservable,
            const ObservableKinematic &observableKinematic,
            DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) const;

    ObservableResultList computeManyKinematicOneModel(
            const List<ObservableKinematic> & listOfKinematic,
            DVCSModule* pDVCSModule, Observable* pObservable,
            DVCSConvolCoeffFunctionModule* pDVCSConvolCoeffFunctionModule) const;

    ObservableResultListReport compareResultList(
            const ObservableResultList &resultList_01,
            const ObservableResultList &resultList_02,
            const Tolerances &tolerances) const;

    virtual void computeTask(Task &task);

private:
    //TODO improve object copy
    ObservableResult computeDVCSObservableTask(Task &task);
    ObservableResultList computeManyKinematicOneModelTask(Task &task);

};

#endif /* OBSERVABLE_SERVICE_H */
